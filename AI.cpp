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
  int jmp = 0;
  //int i;
  int side = (_p->team())->side();
  int nplrs = (_p->team())->nplayers();
  int px, bx, fs, mp, bsx, net, wall; /* Normalized values */
  int mypos, minslot, maxslot;
  float slotsize;
  //float c2n, c2w, mid; /* Fuzzy predicates */
  //float p;
  int move, pw;
  //int moves[10];
  //float prec[10];
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

  slotsize = net/nplrs;
  
  if (nplrs > 1) {
      vector<Player *> plv = (_p->team())->players();
      /* Look for my id */
      vector<Player *>::const_iterator it; 
      int myidx = _p->orderInField(), i;

      if (_p->orderInField() < 0) {
	  for ( it = plv.begin(), i=0;
		it != plv.end();
		it++,i++ ) 
	      (*it)->setOIF(i);	      
	  
	  myidx = _p->orderInField();

      }
      mypos = (int)(slotsize*(myidx+0.5));
      Player *closerp;
      int minxsearching = net, minopx, opx, closer;
      for ( it = plv.begin();
	    it != plv.end(); 
	    it++ ) {
	  opx = (side>0)?((*it)->maxX()-(*it)->x()-(*it)->width()/2):
	      ((*it)->x()+(*it)->width()/2-(*it)->minX()); 	  
	  if ( ( (*it)->id() != _p->id() ) &&
	       ( abs(opx-mypos) < minxsearching ) ){
	      closer=(*it)->id();
	      closerp=(*it);
	      minxsearching=abs(opx-mypos);
	      minopx=opx;
	  }
			     
      }
      
      /* Someone is inside my slot over the 15% the slot size */
      if (minxsearching < (slotsize*0.35)) {
	  myidx = closerp->orderInField();
	  closerp->setOIF(_p->orderInField());
	  _p->setOIF(myidx);
      }
      mypos = (int )(slotsize*(myidx+0.5));
      minslot = (int )(slotsize*myidx);
      maxslot = (int )(slotsize*(myidx+1));
  } else {
      minslot = wall;
      mypos = mp-50; // behind the middle point
      maxslot = net;
  }

  /* My rest position has been chosen, and the slot determined */
  
  /* Predicates for fuzzy control, not used 
     c2n = (bx < mp)?0:(1-(net-bx)/(net-mp));
     c2w = (bx > mp)?0:(1-(bx-wall)/(mp-wall));
     mid = ((bx > (net-mp/2))||(bx < mp/2))?0:(1-abs(bx-mp)/mp);
     bms = bx < net;
  */
  if (_b->spdy() < 20) jumped=0;
  int hd = HITDELTA;
//      printf("%d\n", _b->spdy());
//      if (abs(bsx) < 160) hd += 7;
  if ( //(abs(_b->spdy()) > 10) &&
//	  !jumped &&
      (bx > (minslot*0.95)) && (bx < (maxslot*1.05)) && 
      // its on my side (in my slot)
      ( (abs(px - bx)) <= hd ) && 
//	    ( bsx?HITDELTA : EPSILON) ) &&
      ( abs(px-bx) > 5 ) &&
      (_b->y() > ((_b->spdy() > 40)?JUMP_LIMIT:(JUMP_LIMIT+20)) ) ) {
//	  (_b->y() > (JUMP_LIMIT) )  ){
//	  printf ("%d %d\n", _b->y(), _b->spdy());
      jmp = 1;
      jumped = 1;
  }
  if ((bx < minslot) || ( bx > net )) { // the ball is outside my slot /my side
      move = mypos-px;
  } else {
      move = bx-hd-px;
  }
  
  move=-side*(move);

  _ca->action(_p->id(), move, jmp);
}
