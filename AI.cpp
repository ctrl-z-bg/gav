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

#define HITDELTA        (25)
#define JUMP_LIMIT       ( 200 )
#define EPSILON (5)
#define MAX(a,b)        ((a>b)?a:b)
#define MIN(a,b)        ((a<b)?a:b)

void Agent::update() {
  int jmp = 0;
  //int i;
  int side = (_p->team())->side();
  int nplrs = (_p->team())->nplayers();
  int px, bx, fs, mp, bsx, net, wall; /* Normalized values */
  int mypos, minslot, maxslot;
  float slotsize;
  int move, pw;

  net = fs = abs(_p->maxX() - _p->minX());
  wall = 0;
  mp = fs/2;

  /* Normalize player x position, ball x position and ball x speed:
     Reasoning as it plays on the left, net=fs, wall=0 */
  px = (side>0)?(_p->maxX()-_p->x()-_p->width()/2):
      (_p->x()+_p->width()/2-_p->minX()); 
  pw = _p->width();
  bx = (side>0)?(_p->maxX()-(_b->radius()+_b->x())):
      (_b->radius()+_b->x()-_p->minX()); 
  bsx = -side*_b->spdx();

  slotsize = net/nplrs;
  
  if (nplrs > 1) {
      std::vector<Player *> plv = (_p->team())->players();
      /* Look for my id */
      std::vector<Player *>::iterator it; 
      int myidx = _p->orderInField(), i;

      /* If nobody set the Order In Field I will do for all the team */
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
  
  //  int hd = 3*_b->radius()/4;
  int hd = (_b->radius()+_p->width())/2;
  int minhd = 0;
  if ( !_b->gravity() ) { 
      if (hd/2 >= 12) 
	  minhd = rand()%(hd/2-10)+10; 
      hd = rand()%(HITDELTA-minhd)+minhd;
  }
  /* I care whether I'm the closer only when it's "service time" */
  int closest = 1;
  if ( !_b->gravity() ) {
      int opx;
      std::vector<Player *> plv = (_p->team())->players();
      /* Look for my id */
      std::vector<Player *>::iterator it; 
      for ( it = plv.begin();
	    it != plv.end(); 
	    it++ ) {
	  opx = (side>0)?((*it)->maxX()-(*it)->x()-(*it)->width()/2):
	      ((*it)->x()+(*it)->width()/2-(*it)->minX()); 	  
	  if ( abs(px-bx) > abs(opx-bx) )
	      closest = 0;
      }      
  } 

//      printf("%d\n", _b->spdy());
//      if (abs(bsx) < 160) hd += 7;
  if ( _b->gravity() ) { 
      if ( ( (abs(px - bx)) <= hd ) && // is the ball close?
	   //(abs(_b->spdy()) > 10) && // is the ball going down too slow?
	   //( abs(px-bx) > 5 ) &&     // am I to close to the ball center?
	   // Can I reach the ball jumping, or am I missing the ball? 
	   (_b->y() > ((_b->spdy() > 20)?JUMP_LIMIT:(JUMP_LIMIT+20)))
	  ) {
	  jmp = 1;
      }
  } else { // I serve only forward
      jmp = ( (minhd < (bx-px) && ((bx - px) <= hd) ) );
  }
//  printf("hd: %d - %d (%d - %d)\n",hd, abs(bx-px), _b->spdy(), _b->y()); 
  // if I'm the closest player I serve (when it's "service time").
  // if the ball is (too) outside my (slot) size it does not go for it.
  double concern = (nplrs>1)?(slotsize*1.05):slotsize;
  if ( (abs(bx-mypos) < concern) && closest) { 
      if ( _b->gravity() ) { 
	  move = bx-hd-px;
      } else { // I serve only forward
	  move = bx-hd-px;
      }
  } else {
      move = mypos-px;
  }

  move=-side*(move);

  _ca->action(_p->id(), move, jmp);
}
