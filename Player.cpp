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

#include "Player.h"
#include "ControlsArray.h"
#include <math.h>

int Player::maxX() { return _team->xmax(); }
int Player::minX() { return _team->xmin(); }
int Player::minY() { return _team->ymin(); }

int Player::speedX()  {
  if ( (_speedX > 0) &&
       (_x < (_team->xmax() - _frames->width())) )
    return(_speedX);
  if ( (_speedX < 0) &&
       (_x > _team->xmin()) )
    return(_speedX);
  return(0);
}


void Player::update(int ticks, ControlsArray *ca) {
  triple_t input = ca->getCommands(_plId);

  int dx = 0;

  if ( input.left ) 
    dx--;

  if ( input.right )
    dx++;

  _speedX = dx?(dx * _speed):0;

  _x += dx * (_speed * ticks / 1000);

  if ( _x > (_team->xmax() - _frames->width()) )
    _x = (_team->xmax() - _frames->width());
  else if ( _x < _team->xmin() )
    _x = _team->xmin();

  if ( _y == GROUND_LEVEL && input.jump ) {
    _speedY = -SPEEDY;
  }

  if ( _y > GROUND_LEVEL ) {
    _y = GROUND_LEVEL;
    _speedY = 0;
  }

  _y += (int) (_speedY * 5 * ((float) ticks / 1000));

  if ( _y < GROUND_LEVEL )
    _speedY += (SPEEDY * 5 * ticks) / 1000;

  /* What frame have I to draw? */
  if ( _y < GROUND_LEVEL ) {    // I'm jumping!
    _state  = PL_STATE_JUMP;
    _lastXWalkDrawChanged = RESET_WALK_SEQUENCE;
  } else {
    if (!dx) {                  // I'm still
      _state  = PL_STATE_STILL;
      _lastXWalkDrawChanged = RESET_WALK_SEQUENCE;
    } else                      // I'm walking
      if (abs(_x - _lastXWalkDrawChanged) > _walkDrawSpeed) {
	_lastXWalkDrawChanged = _x;
	if (_state == PL_STATE_WALK2)
	  _state = PL_STATE_WALK1;
	else
	  _state = PL_STATE_WALK2;
      }
  }

}

void Player::draw(SDL_Surface * screen) {
  SDL_Rect rect;
  rect.x = _x;
  rect.y = _y;

  _frames->blit(_state, screen, &rect);
}

bool
Player::collidesWith(FrameSeq *fs, int idx, SDL_Rect *rect)
{
  SDL_Rect myRect;
  myRect.x = _x;
  myRect.y = _y;

  return(_frames->collidesWith(fs, _state, idx, &myRect, rect));
}
