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

#include <stdlib.h>
#include <SDL.h>
#include "InputState.h"
#include "ControlsArray.h"
#include "vector"
#include "Player.h"
#include "Team.h"
#include "PlayerAI.h"
#ifndef NONET
#include "NetServer.h"
#endif

void ControlsArray::initializeControls() {
  _keyMapping[0].left_key = SDLK_z;
  _keyMapping[0].right_key = SDLK_c;
  _keyMapping[0].jump_key = SDLK_LSHIFT;
  
  _keyMapping[1].left_key = SDLK_LEFT;
  _keyMapping[1].right_key = SDLK_RIGHT;
  _keyMapping[1].jump_key = SDLK_UP;
  
  _keyMapping[2].left_key = SDLK_h;
  _keyMapping[2].right_key = SDLK_k;
  _keyMapping[2].jump_key = SDLK_u;

  _keyMapping[3].left_key = SDLK_s;
  _keyMapping[3].right_key = SDLK_f;
  _keyMapping[3].jump_key = SDLK_e;
}

void ControlsArray::setControlsState(InputState *is, Team * tl, Team * tr) {
  Uint8 *keystate = is->getKeyState();
  int i = 0;
  std::vector<Player *> players;

#ifndef NONET
  int player;
  char cmd;
  if (nets)
    while ( nets->ReceiveCommand(&player, &cmd) != -1 ) {
      _inputs[player].left = cmd & CNTRL_LEFT;
      _inputs[player].right = cmd & CNTRL_RIGHT;
      _inputs[player].jump = cmd & CNTRL_JUMP;
    }
#endif

  players = tl->players();

  for (i = 0; i < 2; i++) {
    for ( std::vector<Player *>::const_iterator it = players.begin();
	  it != players.end(); it++ ) {
      if ( (*it)->getCtrl() == PL_CTRL_HUMAN ) {
	_inputs[(*it)->id()].left = keystate[_keyMapping[(*it)->id()].left_key];
	_inputs[(*it)->id()].right = keystate[_keyMapping[(*it)->id()].right_key];
	_inputs[(*it)->id()].jump = keystate[_keyMapping[(*it)->id()].jump_key];
      } else if ( (*it)->getCtrl() == PL_CTRL_AI ) {
	_inputs[(*it)->id()] = ((PlayerAI *) (*it))->planAction();
      }
    }
    players = tr->players();
  }
}

