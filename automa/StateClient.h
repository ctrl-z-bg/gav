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

#ifndef _STATECLIENT_H_
#define _STATECLIENT_H_

#include "State.h"
#include "StateWithInput.h"
#include "InputState.h"
#include "Team.h"
#include "Ball.h"
#include "AI.h"

class StateClient : public State, public StateWithInput {
private:
  Team *tl, *tr; // team left and team right
  Ball *b;
  Agent *agentR[MAX_PLAYERS/2], *agentL[MAX_PLAYERS/2];
  unsigned int prevDrawn;
  int _lp, _rp;

public:
  StateClient() : prevDrawn(0) {}

  virtual int execute(InputState *is, unsigned int ticks,
		      unsigned int prevTicks, int firstTime);
  
private:
  int setupConnection(InputState *is);
};

#endif // _STATEPLAYING_H_
