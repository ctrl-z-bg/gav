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

#ifndef __CONTROLSARRAY_H__
#define __CONTROLSARRAY_H__

#include <string.h>
#include "InputState.h"
#include "globals.h"

typedef struct {
  Uint8 left;
  Uint8 right;
  Uint8 jump;
} triple_t;

class ControlsArray {
  triple_t _inputs[MAX_PLAYERS];
  Uint8 _f[12];
  bool _isArtificial[MAX_PLAYERS];

public:
  ControlsArray() {
    bzero(_inputs, MAX_PLAYERS * sizeof(triple_t));
    bzero(_f, 12);
    for ( int i = 0; i < MAX_PLAYERS; _isArtificial[i++] = false );
  }

  //  void addChannel(){}
  
  void setControls(InputState *is);
  inline triple_t getCommands(int idx) { return _inputs[idx];}

  void action(int plId, int movx, int movy);
  inline void isArtificial(int plId) {
    _isArtificial[plId] = true;
  }
  inline void isHuman(int plId) {
    _isArtificial[plId] = false;
  }
};

#endif // __CONROLSARRAY_H__

