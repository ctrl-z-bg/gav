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

#include "AI.h"

#define HIT_POINT(a)  ( (a) + 20 )
#define EPSILON (5)

void Agent::update() {
  int jmp = 0;

  if ( /*(abs(_b->spdy()) < 10) &&*/ 
      (abs(_p->x() - HIT_POINT(_b->x())) <= 
       ( (_b->spdx()) ? (_p->width() / 2) : EPSILON)) &&
      (_b->y() > 200) )
    jmp = 1;

  int move;

  if ( _b->x() < _p->minX() ) {
    move = (_p->minX() + _p->maxX())/2 - _p->x();
  } else {
    move = HIT_POINT(_b->x()) - _p->x();
  }
  _ca->action(_p->id(), move, jmp);
}
