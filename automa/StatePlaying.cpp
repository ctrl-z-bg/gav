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

using namespace std;

#define WINNING_SCORE 15

// executes one step of the game's main loop
// Returns NO_TRANSITION if the game continues, anything else otherwise
int StatePlaying::execute(InputState *is, unsigned int ticks,
			  unsigned int prevTicks, int firstTime)
{
  if ( firstTime ) {
    /* 
       First time we change to execute state: we should
       probably create players here instead of in the constructor,
       and think of a clever way to destroy them once we're done.
    */

    tl = new Team(-1);
    tr = new Team(1);
    b = new Ball(BALL_ORIG);

    for ( int i = 0, j = 0; i < configuration.left_nplayers; j++ ) {
      if ( configuration.left_players[j] == PLAYER_NONE ) {
	continue;
      }
      string name = "Pippo-" + j;
      Player *automL = tl->addPlayer(name.c_str(), PL_TYPE_MALE_LEFT);
      agentL[j] = new Agent(b, automL, controlsArray);
      if ( configuration.left_players[j] == PLAYER_HUMAN )
	controlsArray->isHuman(j*2);
      else
	controlsArray->isArtificial(j*2);
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
	controlsArray->isHuman(j*2 + 1);
      else
	controlsArray->isArtificial(j*2 + 1);
      i++;
    }

    tl->setScore(0);
    tr->setScore(0);
    b->resetPos((int) (SCREEN_WIDTH() * 0.25),
		(int) (SCREEN_HEIGHT() * 0.66));
  }
  
  controlsArray->setControlsState(is);
  
  if ( is->getKeyState()[SDLK_ESCAPE] )
    return(NO_TRANSITION + 1); // everything but NO_TRANSITION
  
  /* update AI agents */
  for ( int i = 0, j = 0; i < configuration.left_nplayers; j++ ) {
    if ( configuration.left_players[j] == PLAYER_COMPUTER )
      agentL[i]->update();
    if ( configuration.left_players[j] != PLAYER_NONE )
      i++;
  }
  for ( int i = 0, j = 0; i < configuration.right_nplayers; j++ ) {
    if ( configuration.right_players[j] == PLAYER_COMPUTER )
      agentR[j]->update();
    if ( configuration.right_players[j] != PLAYER_NONE )
      i++;
  }

  tl->update(ticks - prevTicks, controlsArray);
  tr->update(ticks - prevTicks, controlsArray);
  
  b->update(ticks - prevTicks, tl, tr);
  
  if ( 1 ) { // FIXME!!! (ticks - prevDrawn) > 30 ) {
    SDL_Rect r;
    r.x = r.y = 0;
    r.h = background->h;
    r.w = background->w;
    SDL_BlitSurface(background, &r, screen, &r);
    
    tl->draw();
    tr->draw();
    b->draw();
    SDL_Flip(screen);
    // prevDrawn = ticks;
  }
  
  if ( ((tl->getScore() >= WINNING_SCORE) &&
	(tl->getScore() > (tr->getScore()+1))) ||
       ((tr->getScore() >= WINNING_SCORE) &&
	(tr->getScore() > (tl->getScore()+1))) )
    return(NO_TRANSITION + 1); // everything but NO_TRANSITION

  return(NO_TRANSITION);
}
