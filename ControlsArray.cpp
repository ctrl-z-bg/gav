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

void ControlsArray::action(int plId, int movx, int movy) {
  _inputs[plId].left = _inputs[plId].right = _inputs[plId].jump = 0;
  
  if ( movx < 0 )
    _inputs[plId].left = 1;
  if ( movx > 0 )
    _inputs[plId].right = 1;
  if ( movy )
    _inputs[plId].jump = 1;
}

void ControlsArray::setControls(InputState *is) {
  Uint8 *keystate = is->getKeyState();
  
  if ( !_isArtificial[0] ) {
    _inputs[0].left = keystate[SDLK_z];
    _inputs[0].right = keystate[SDLK_c];
    _inputs[0].jump = keystate[SDLK_LSHIFT];
  }    

  if ( !_isArtificial[1] ) {
    _inputs[1].left = keystate[SDLK_LEFT];
    _inputs[1].right = keystate[SDLK_RIGHT];
    _inputs[1].jump = keystate[SDLK_UP];
  }    

  if ( !_isArtificial[2] ) {
    _inputs[2].left = keystate[SDLK_h];
    _inputs[2].right = keystate[SDLK_k];
    _inputs[2].jump = keystate[SDLK_u];
  }

  if ( !_isArtificial[3] ) { 
    _inputs[3].left = keystate[SDLK_s];
    _inputs[3].right = keystate[SDLK_f];
    _inputs[3].jump = keystate[SDLK_e];
  }
}


