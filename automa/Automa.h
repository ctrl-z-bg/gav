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

#ifndef _AUTOMA_H_
#define _AUTOMA_H_

#include <vector>
#include "State.h"

#define NO_TRANSITION (0)

class Automa {
protected:
  std::vector<State*> _states;
  int _curr; // current state index
  int _prev;

public:
  Automa(std::vector<State*> states): _states(states) {};
  Automa() {};
  virtual int addState(State *state) {
    _states.push_back(state);
    return(_states.size());
  }
  virtual int start() { return(0); }
  virtual ~Automa() {
    for ( std::vector<State*>::iterator it = _states.begin();
	  it != _states.end(); it++ )
      delete(*it);
  }
};

#endif // _AUTOMA_H_
