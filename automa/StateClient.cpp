/* -*- C++ -*- */
/*
  GAV - Gpl Arcade Volleyball
  
  Copyright (C) 2002
  GAV team (http://sourceforge.net/projects/gav/)

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <SDL.h>
#include "StateClient.h"
#include "AutomaMainLoop.h"
#ifndef NONET
#include "NetClient.h"
#endif //NONET

using namespace std;


int StateClient::setupConnection(InputState *is) {
#ifndef NONET
  bool configured = false;
  string saddress = "";
  char ch;
  string ports = "";
  int port;
  char ti;

  while ( !configured ) {
    /* first, delete the screen... */
    SDL_Rect r;
    r.x = r.y = 0;
    r.h = background->h;
    r.w = background->w;
    SDL_BlitSurface(background, &r, screen, &r);
    SDL_Flip(screen);
    
    /* now, ask for server address, port and team side */
    cga->printRow(screen, 0, "Please type server address");
    SDL_Flip(screen);
    while ( (ch = getKeyPressed(is)) != 0 ) {
      if ( ch < 0 ) {
	saddress = deleteOneChar(saddress); // should be backspace...
	cga->printRow(screen, 1, "                       ", background);
      } else {
	char w[2];
	w[0] = ch;
	w[1] = 0;
	saddress = saddress + w;
      }
      cga->printRow(screen, 1, saddress.c_str(), background);
      SDL_Flip(screen);
    }
    char msg[100];
    sprintf(msg, "Please type port number [%d]", SERVER_PORT);
    cga->printRow(screen, 2, msg);
    SDL_Flip(screen);
    while ( (ch = getKeyPressed(is)) != 0 ) {
      if ( ch < 0 ) {
	ports = deleteOneChar(ports); // should be backspace...
	cga->printRow(screen, 3, "                       ", background);
      } else {
	char w[2];
	w[0] = ch;
	w[1] = 0;
	ports = ports + w;
      }
      cga->printRow(screen, 3, ports.c_str(), background);
      SDL_Flip(screen);
    }
    port = atoi(ports.c_str());
    if ( !port )
      port = SERVER_PORT;
    string team = "";
    cga->printRow(screen, 4, "Left or right team? (l/r)");
    SDL_Flip(screen);
    while ( (ch = getKeyPressed(is)) != 0 ) {
      if ( ch < 0 ) {
	team = deleteOneChar(team); // should be backspace...
	cga->printRow(screen, 5, "                       ", background);
      } else {
	char w[2];
	w[0] = ch;
	w[1] = 0;
	team = team + w;
      }
      cga->printRow(screen, 5, team.c_str(), background);
      SDL_Flip(screen);
    }
    ti = (*(team.c_str())=='l')?NET_TEAM_LEFT:NET_TEAM_RIGHT;

    configured = true;
  }

  netc = new NetClient();
  cga->printRow(screen, 6, "connecting...");
  SDL_Flip(screen);
  if ( netc->ConnectToServer(&_lp, &_rp, ti, saddress.c_str(),
			     port) == -1 ) {
    delete(netc);
    cga->printRow(screen, 7, "host unreachable");
    SDL_Flip(screen);
    SDL_Delay(1000);
    nets = NULL;
    return(STATE_MENU);
  }
  cga->printRow(screen, 7, "connected. Waiting for other clients...");
  SDL_Flip(screen);

  /* ok, I'm connected and I'm waiting for the game start */
  netc->WaitGameStart();

  return(0);
#endif //!NONET
}

// executes one step of the game's main loop for a network client.
// before the game loop actually begins, connection must be set up
// Returns NO_TRANSITION if the game continues, the next state otherwise
int StateClient::execute(InputState *is, unsigned int ticks,
			  unsigned int prevTicks, int firstTime)
{
#ifndef NONET
  if ( firstTime ) {
    int ret = 0;
    if ( (ret = setupConnection(is)) )
      return(ret);

   /* 
       First time we change to execute state: we should
       probably create players here instead of in the constructor,
       and think of a clever way to destroy them once we're done.
    */

    prevDrawn = ticks;
    tl = new Team(-1);
    tr = new Team(1);
    b = new Ball(BALL_ORIG);

    for ( int i = 0; i < MAX_PLAYERS/2; i++ ) {
      agentL[i] = NULL;
      agentR[i] = NULL;
    }

    for ( int j = 0; j < _lp; j++ ) {
      string name = "Pippo-" + j;
      tl->addPlayer(name.c_str(), PL_TYPE_MALE_LEFT);
    }

    for ( int j = 0; j < _rp; j++ ) {
      string name = "Pluto-" + j;
      tr->addPlayer(name.c_str(), PL_TYPE_MALE_RIGHT);
    }

    tl->setScore(0);
    tr->setScore(0);
    b->resetPos((int) (SCREEN_WIDTH() * 0.25),
		(int) (SCREEN_HEIGHT() * 0.66));
    
    controlsArray->setHuman(0);
  }
  
  if ( is->getKeyState()[SDLK_ESCAPE] ) {
    delete(tl);
    delete(tr);
    delete(b);
    delete(netc);
    netc = NULL;
    return(STATE_MENU);
  }  

  controlsArray->setControlsState(is);
  triple_t input = controlsArray->getCommands(0);
  netc->SendCommand((input.left?CNTRL_LEFT:0)|
		    (input.right?CNTRL_RIGHT:0)|
		    (input.jump?CNTRL_JUMP:0));

  while ( netc->ReceiveSnapshot(tl, tr, b) != -1 );
  if ( (ticks - prevDrawn) >
       (unsigned int) (FPS - (FPS / (configuration.frame_skip + 1)) ) ) {
    SDL_Rect r;
    r.x = r.y = 0;
    r.h = background->h;
    r.w = background->w;
    SDL_BlitSurface(background, &r, screen, &r);
    
    tl->draw();
    tr->draw();
    b->draw();
    SDL_Flip(screen);
    prevDrawn = ticks;
  }

  if ( ((tl->getScore() >= configuration.winning_score) &&
	(tl->getScore() > (tr->getScore()+1))) ||
       ((tr->getScore() >= configuration.winning_score) &&
	(tr->getScore() > (tl->getScore()+1))) ) {
    /* Deallocate teams, ball and players */
    delete(tl);
    delete(tr);
    delete(b);
    delete(netc);
    netc = NULL;
    return(STATE_MENU); // end of the game
  }

  return(NO_TRANSITION);
#endif // !NONET
}
