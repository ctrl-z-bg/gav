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
#define HITDELTA        (20)
#define HITDELTA2       (25)
#define JUMP_LIMIT       ( 200 )
#define EPSILON (5)
#define MAX(a,b)        ((a>b)?a:b)
#define MIN(a,b)        ((a<b)?a:b)
#define F_AND(a,b)      MIN(a)
#define F_OR(a,b)       MAX(a) 
#define F_NOT(a)      ( 1.0 - (a) )

void Agent::update() {
  int jmp = 0, i;
  int side = (_p->team())->side();
  int px, bx, fs, mp, bms, bsx, net, wall; /* Normalized values */
  float p, c2n, c2w, mid; /* Fuzzy predicates */
  int move, pw;
  int moves[10];
  float prec[10];
  static int jumped = 0;

  net = fs = abs(_p->maxX() - _p->minX());
  wall = 0;
  mp = fs/2;

  /* Normalize player x position, ball x position and ball x speed:
     Reasoning as it plays on the left, net=fs, wall=0 */
  px = (side>0)?(_p->maxX()-_p->x()-_p->width()/2):(_p->x()+_p->width()/2-_p->minX()); 
  pw = _p->width();
  bx = (side>0)?(_p->maxX()-(_b->radius()+_b->x())):(_b->radius()+_b->x()-_p->minX()); 
  bsx = -side*_b->spdx();

  c2n = (bx < mp)?0:(1-(net-bx)/(net-mp));
  c2w = (bx > mp)?0:(1-(bx-wall)/(mp-wall));
  mid = ((bx > (net-mp/2))||(bx < mp/2))?0:(1-abs(bx-mp)/mp);
  bms = bx < net;
  if (_b->spdy() < 20) jumped=0;
#ifdef OLDAI
  if (side > 0) {
      if ( /*(abs(_b->spdy()) < 10) &&*/ 
	  (abs(_p->x() - HIT_POINT(_b->x())) <= 
	   ( (_b->spdx()) ? (_p->width() / 2) : EPSILON)) &&
	  (_b->y() > JUMP_LIMIT) ) {
	  jmp = 1;
	  jumped = 1;
      }
      
      if ( _b->x() < _p->minX() ) {
	  move = (_p->minX() + _p->maxX())/2 - _p->x();
      } else {
	  move = HIT_POINT(_b->x()) - _p->x();
      }
  } else {
#endif
      int hd = HITDELTA;
//      printf("%d\n", _b->spdy());
//      if (abs(bsx) < 160) hd += 7;
      if ( //(abs(_b->spdy()) > 10) &&
//	  !jumped &&
	  (bx < net) &&
	  ( (abs(px - bx)) <= hd ) &&
//	    ( bsx?HITDELTA : EPSILON) ) &&
	  ( abs(px-bx) > 5 ) &&
	  (_b->y() > ((_b->spdy() > 40)?JUMP_LIMIT:(JUMP_LIMIT+20)) ) ) {
//	  (_b->y() > (JUMP_LIMIT) )  ){
//	  printf ("%d %d\n", _b->y(), _b->spdy());
	  jmp = 1;
	  jumped = 1;
      }
      if ( bx > net ) {
	  move = mp-px-50;
      } else {
	  move = bx-hd-px;
      }
      
      move=-side*(move);
#ifdef OLDAI
   }
#endif
  _ca->action(_p->id(), move, jmp);
}
