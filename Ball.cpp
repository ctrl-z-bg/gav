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

#include "Ball.h"

#define PI 3.14158

#define GRAVITY 250
#define MAX_TOUCHES 3

#define MIN(a,b)        ((a<b)?a:b)
#define MAX(a,b)        ((a>b)?a:b)

using namespace std;

bool Ball::approaching(int spdx, int spdy) {
  int sgnx = (spdx>0)?1:(spdx<0)?-1:0;
  int sgny = (spdy>0)?1:(spdy<0)?-1:0;
    
  if ( ((spdx - _spdx) * sgnx) >= 0 ) return(true);
  if ( ((spdy - _spdy) * sgny) >= 0 ) return(true);

  return(false);
}


// evaluates a collision ("sprite"-wise)
bool Ball::spriteCollide(Player *p) {
  SDL_Rect r;
  r.x = _x;
  r.y = _y;

  return(p->collidesWith(_frames, _frameIdx, &r));
}


// evaluate a collision
bool Ball::collide(Player *p) {
  return( approaching(p->speedX(), - p->speedY()) &&
	  spriteCollide(p) );
}


void Ball::update_internal(Player * pl) {
#define AMPLIFY 5
  
  int cplx = pl->x() + pl->width() / 2;
  int cply = pl->y() + pl->height() / 2;
  int cbx = _x + _frames->width() / 2;
  int cby = _y + _frames->height() / 2;

  int dx = (cplx - cbx);
  int dy = (cply - cby);

  float beta = dx?atan((float) dy/ (float) dx):(PI/2);

  float sinb = sin(-beta);
  float cosb = cos(-beta);

  float x1, y1;

  // x1 = _spdx * cosb - _spdy * sinb;
  y1 = _spdx * sinb + (- _spdy) * cosb;
  x1 = AMPLIFY * (pl->speedX() * cosb - pl->speedY() * sinb);
  
  // x1 = x2;

  int oldspdy = _spdy;

  _spdx = (int) (x1 * cos(beta) - y1 * sin(beta));
  _spdy = (int) (- (x1 * sin(beta) + y1 * cos(beta)));

  if ( (pl->speedX() - _spdx) * dx < 0)
    _spdx = pl->speedX();

  if ( (pl->speedY() - _spdy) * dy > 0)
    _spdy = pl->speedY();


#define MIN_POS_SPD 100

  if ( (_spdy < MIN_POS_SPD) && (cby < cply) )
    _spdy = (int) ((1.1 * ELASTIC_SMOOTH) * abs(oldspdy));

}



void Ball::assignPoint(int side, Team *t) {
  if ( _side == side )
    t->score();
  _side = side;
  _x = (SCREEN_WIDTH() / 2) + ((SCREEN_WIDTH() * _side) / 4);
  _y = (SCREEN_HEIGHT() * 2) / 3;
  _spdx = _spdy = _accelY = 0;
  _scorerSide = 0;
  resetCollisionCount();
}

void Ball::resetCollisionCount() {
  for (map<Team *, int>::iterator it = _collisionCount.begin();
       it != _collisionCount.end(); it++ ) {
    it->second = 0;
  }
}

void Ball::resetPos(int x, int y) {
  _side = -1;
  _scorerSide = 0;
  _x = x; _y = y; _spdx = _spdy = 0; _accelY = 0;
  resetCollisionCount();
}

void Ball::draw(SDL_Surface *scr) {
  SDL_Rect rect;
  rect.x = _x;
  rect.y = _y;

  _frames->blit(_frameIdx, scr, &rect);
}

float
Ball::distance(int x, int y)
{
  int bx = _x + _frames->width()/2; // center of the ball
  int by = _y + _frames->height()/2;
  return(sqrt((double) (x - bx)*(x - bx) + (y - by) * (y - by))); 
}

bool
Ball::netPartialCollision(int x, int y)
{
  int xmin, xmax, ymin, ymax;

  xmin = MIN(x, _oldx);
  xmax = MAX(x, _oldx);
  ymin = MIN(y, _oldy);
  ymax = MAX(y, _oldy);

  if ((xmin < NET_X) && (xmax > NET_X)) {
    int collisionY = (NET_X - xmin)*(ymax - ymin)/(xmax - xmin);
    if ( (collisionY < NET_Y ) && (collisionY > NET_Y - _frames->height()/2) )
      return true;
  }

  return((x + _frames->width() > NET_X) && (x < NET_X) &&
	 ( distance(NET_X, NET_Y) < (_frames->width()/2)) );
	 /* (y + _frames->height()/2 < NET_Y) &&
	    (y + _frames->height() > NET_Y)); */
}

bool
Ball::netFullCollision(int x, int y)
{
  int xmin, xmax, ymin, ymax;

  xmin = MIN(x, _oldx);
  xmax = MAX(x, _oldx);
  ymin = MIN(y, _oldy);
  ymax = MAX(y, _oldy);

  if ((xmin < NET_X) && (xmax > NET_X)) {
    int collisionY = (NET_X - xmin)*(ymax - ymin)/(xmax - xmin);
    if ( collisionY > NET_Y )
      return true;
  }

  return((x + _frames->width() > NET_X) && (x < NET_X) &&
	 (y + _frames->height()/2 >= NET_Y));
}

// updates the ball position, knowing 'passed' milliseconds went
// away
void Ball::update(int passed, Team *tleft, Team *tright) {
  int dx, dy;
  static int overallPassed = 0;
    
  overallPassed += passed;
  if ( (overallPassed > FRAME_RATE) ) {
    overallPassed = 0;
    _frameIdx = (_frameIdx + 1) % BALL_FRAMES;
  }
    
  if ( _scorerSide ) {
    _scoredTime += passed;
    if ( _scoredTime > 650 ) {
      assignPoint(_scorerSide, (_scorerSide<0)?tleft:tright);
      return;
    }
  }

  if ( !netPartialCollision(_x, _y) ) // if it's not network supported
    update_direction(passed);

  dx = (int) (_spdx * ((float) passed / 1000.0));
  dy = (int) (_spdy * ((float) passed / 1000.0));

  _oldx = _x;
  _oldy = _y;
    

  _x += dx;
  _y -= dy; // usual problem with y
  
  //ball hits upper wall
  if ( _y < 24 ) {
    _y = 24;
    _spdy = - (int) (_spdy * ELASTIC_SMOOTH);
  }
    
  //ball hits left wall
  if ( _x < 7  ) {
    _x = 7;
    _spdx = - (int) (_spdx * ELASTIC_SMOOTH);
    if ( _collisionCount[tright] )
      resetCollisionCount();
  }

  
  //ball hits right wall
  if ( _x > (SCREEN_WIDTH() - _frames->width() -16) ) {
    _x = (SCREEN_WIDTH() - _frames->width()-16);
    _spdx = - (int) (_spdx * ELASTIC_SMOOTH);
    if ( _collisionCount[tleft] )
      resetCollisionCount();
  }

  // net collision
  if ( netPartialCollision(_x, _y) ) {
    if ( !netPartialCollision(_oldx, NET_Y - 3*_frames->height()/4) ) {
      if ( _oldy < _y ) { // hits from the top
	if ( (_x + _frames->width()/8 < NET_X) &&
	     (_x + 7*_frames->width()/8 > NET_X) )
	  _spdx = - _spdx;
      }
      _spdx = (int) (_spdx * ELASTIC_SMOOTH * ELASTIC_SMOOTH * ELASTIC_SMOOTH);
    }
    _y -= _frames->height()/4; //(int) (_frames->height() -
    //(4*distance(NET_X, NET_Y) / _frames->height()));
    _spdy = (int) fabs(_spdy * ELASTIC_SMOOTH * ELASTIC_SMOOTH);
  } else if ( netFullCollision(_x, _y) && !netFullCollision(_oldx, _oldy)) {
    _spdx = (int) ((- _spdx) * ELASTIC_SMOOTH);
    if ( _oldx > _x )
      _x = 2 * NET_X - _x; // moves ball out of the net by the right amount
    else
      _x =  2* NET_X - 2*_frames->width() - _x;
  }

  //ball hits floor
  if ( _y > (SCREEN_HEIGHT() - _frames->height()-2) ) {
    _y = (SCREEN_HEIGHT() - _frames->height()-2);
    _spdy = - (int) (_spdy * ELASTIC_SMOOTH);
    if ( !_scorerSide ) {
      // oldx, so we're safe from collisions against
      // the net
      _scorerSide = (_oldx < (SCREEN_WIDTH() / 2))?1:-1;
      _scoredTime = 0;
    }
  }

  // collisions with the players
  for ( int teami = 0; !_scorerSide && (teami < 2); teami ++ ) {
    Team *team = (teami?tright:tleft);
    vector<Player *> plv = team->players();
    for ( vector<Player *>::const_iterator it = plv.begin();
	  it != plv.end(); it++ ) {
      // cout << "xy" << (*it)->x() << " " << (*it)->y() << endl;
      if ( collide(*it) ) {
	if ( _inCollisionWith != *it ) {
	  _inCollisionWith = *it;
	  // cerr << "collide " << passed << "\n";
	  if ( !_collisionCount[team] )
	    resetCollisionCount();
	  _collisionCount[team]++;
	  if ( _collisionCount[team] > MAX_TOUCHES ) {
	    _scorerSide = (team == tleft)?1:-1;
	    _scoredTime = 0;
	  }
	  // cerr << "CollisionCount " << string(teami?"R: ":"L: ") <<
	  //_collisionCount[team] << endl;
	  update_internal(*it);
	  while (collide(*it)) {
	    _x += (_spdx>0)?1:-1;
	    _y -= (_spdy>0)?1:-1; // usual problem with y
	  }
	  //_inCollisionWith = NULL;
	} else {
	  while (collide(*it)) {
	    _x += (_spdx>0)?1:-1;
	    _y -= (_spdy>0)?1:-1; // usual problem with y
	  }
	}

      } else if ( _inCollisionWith == *it )
	_inCollisionWith = NULL;
    }
  }
  
  // to activate 'gravity'
  if ( !_accelY && (abs(_spdx) + abs(_spdy)) )
    _accelY = GRAVITY;

}

