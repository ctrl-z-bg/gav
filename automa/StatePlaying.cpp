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
#include "StatePlaying.h"
#include "Automa.h"
#include "AutomaMainLoop.h"
#ifndef NONET
#include "NetServer.h"
#endif
#include "StateWithInput.h"

using namespace std;


void StatePlaying::setupConnection(InputState *is) {
#ifndef NONET
  std::string clinumb = "";
  int nclients = 0;
  signed char ch;
  
  nets = new NetServer();
  /* first, delete the screen... */
  SDL_Rect r;
  r.x = r.y = 0;
  r.h = background->h;
  r.w = background->w;
  SDL_BlitSurface(background, &r, screen, &r);
  SDL_Flip(screen);
  /* now, ask for the port to listen on */
  char msg[100];
  string ports = "";
  int port;
  sprintf(msg, "What port to listen on? [%d]", SERVER_PORT);
  cga->printRow(screen, 0, msg);
  SDL_Flip(screen);
  while ( (ch = getKeyPressed(is)) != 0 ) {
    if ( ch < 0 ) {
      ports = deleteOneChar(ports); // should be backspace...
      cga->printRow(screen, 1, "                 ", background);
    } else {
      char w[2];
      w[0] = (char)ch;
      w[1] = 0;
      ports = ports + w;
    }
    cga->printRow(screen, 1, ports.c_str(), background);
    SDL_Flip(screen);
  }
  port = atoi(ports.c_str());
  if ( !port )
    port = SERVER_PORT;

  cga->printRow(screen, 2, "How many clients to wait? [1]");
  SDL_Flip(screen);
  while ( (ch = getKeyPressed(is)) != 0 ) {
    if ( ch < 0 ) {
      clinumb = deleteOneChar(clinumb); // should be backspace...
      cga->printRow(screen, 3, "                 ", background);
    } else {
      char w[2];
      w[0] = (char)ch;
      w[1] = 0;
      clinumb = clinumb + w;
    }
    cga->printRow(screen, 3, clinumb.c_str(), background);
    SDL_Flip(screen);
  }
  nclients = atoi(clinumb.c_str());
  if ( !nclients )
    nclients = 1;
  
  nets->StartServer(port);
  cga->printRow(screen, 4, "Server started. Waiting for clients...",
		background);

  SDL_Flip(screen);  
  int remaining = nclients;
  do {
    remaining = nets->WaitClients(remaining);
    char msg[100];
    sprintf(msg, "  %d connection(s) to go  ", remaining);
    cga->printRow(screen, 5, msg, background);
    SDL_Flip(screen);  
  } while ( remaining > 0 );
#endif //!NONET
}

// executes one step of the game's main loop
// Returns NO_TRANSITION if the game continues, the next state otherwise
int StatePlaying::execute(InputState *is, unsigned int ticks,
			  unsigned int prevTicks, int firstTime)
{
  if ( firstTime ) {
#ifndef NONET
    if ( nets ) {
      setupConnection(is);
    }
#endif
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

    for ( int i = 0, j = 0; i < configuration.left_nplayers; j++ ) {
      if ( configuration.left_players[j] == PLAYER_NONE ) {
	continue;
      }
      string name = "Pippo-" + j;
      Player *automL = tl->addPlayer(name.c_str(), PL_TYPE_MALE_LEFT);
      agentL[j] = new Agent(b, automL, controlsArray);
      if ( configuration.left_players[j] == PLAYER_HUMAN )
	controlsArray->setHuman(j*2);
#ifdef NONET 
      else 
	controlsArray->setArtificial(j*2);
#else
      else if (!nets || nets->isRemote(j*2))
	controlsArray->setArtificial(j*2);
      else
	controlsArray->setRemote(j*2);
#endif
      i++;
    }

    for ( int i = 0, j = 0; i < configuration.right_nplayers; j++ ) {
      if ( configuration.right_players[j] == PLAYER_NONE ) {
	continue;
      }
      string name = "Pluto-" + j;
      Player *automR = tr->addPlayer(name.c_str(), PL_TYPE_MALE_RIGHT);
      agentR[j] = new Agent(b, automR, controlsArray);
      if ( configuration.right_players[j] == PLAYER_HUMAN )
	controlsArray->setHuman(j*2 + 1);
#ifdef NONET 
      else 
	controlsArray->setArtificial(j*2 + 1);
#else 
      else if (!nets || nets->isRemote(j*2+1))
	controlsArray->setArtificial(j*2 + 1);
      else
	controlsArray->setRemote(j*2+1);
#endif
      i++;
    }

    tl->setScore(0);
    tr->setScore(0);
    b->resetPos((int) (SCREEN_WIDTH() * 0.25),
		(int) (SCREEN_HEIGHT() * 0.66));
  }
  
  controlsArray->setControlsState(is);
  
  if ( is->getKeyState()[SDLK_ESCAPE] ) {
    delete(tl);
    delete(tr);
    delete(b);
#ifndef NONET
    if ( nets ) {
      delete(nets);
      nets = NULL;
    } 
#endif
    for ( int i = 0; i < MAX_PLAYERS/2; i++ ) {
      if ( agentL[i] ) delete(agentL[i]);
      if ( agentL[i] ) delete(agentR[i]);
    }
    return(STATE_MENU);
  }  

  /* update AI agents */
  for ( int i = 0, j = 0; i < configuration.left_nplayers; j++ ) {
    if ( configuration.left_players[j] == PLAYER_COMPUTER )
#ifdef NONET
	agentL[i]->update();
#else 
      if ( !nets || !(nets->isRemote(2*i)) )
	agentL[i]->update();
#endif

    if ( configuration.left_players[j] != PLAYER_NONE )
      i++;
  }
  for ( int i = 0, j = 0; i < configuration.right_nplayers; j++ ) {
    if ( configuration.right_players[j] == PLAYER_COMPUTER )
#ifdef NONET
	agentR[j]->update();
#else
      if ( !nets || !(nets->isRemote(2*i+1)) )
	agentR[j]->update();
#endif
    if ( configuration.right_players[j] != PLAYER_NONE )
      i++;
  }

  tl->update(ticks - prevTicks, controlsArray);
  tr->update(ticks - prevTicks, controlsArray);
  
  b->update(ticks - prevTicks, tl, tr);
  
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
#ifndef NONET
    if (nets)
      nets->SendSnapshot(tl, tr, b);
#endif
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
#ifndef NONET
    if ( nets ) {
      delete(nets);
      nets = NULL;
    }
#endif
    for ( int i = 0; i < MAX_PLAYERS/2; i++ ) {
      if ( agentL[i] ) delete(agentL[i]);
      if ( agentL[i] ) delete(agentR[i]);
    }
    return(STATE_MENU); // end of game
  }

  return(NO_TRANSITION);
}
