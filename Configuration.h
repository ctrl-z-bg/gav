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

/* Configuration options */

#define MAX_PLAYERS (12)
#define FPS (50)

enum { PLAYER_NONE, PLAYER_HUMAN, PLAYER_COMPUTER};

class Configuration {
public:
  int left_nplayers;
  int right_nplayers;
  int left_players[MAX_PLAYERS/2];
  int right_players[MAX_PLAYERS/2];  

  /* To add: something meaningful to record the controls... */

  int sound;
  int winning_score;

  unsigned int frame_skip;     // one every frame_skip + 1 are actually drawn
  unsigned int fps;            // fps of the update (not graphical)
  unsigned int mill_per_frame; // caches the # of msecs per frame (1000/fps)

  Configuration() : left_nplayers(1), right_nplayers(1),
		    sound(0), winning_score(15) {
    frame_skip = 0;
    fps = FPS;
    mill_per_frame = 1000 / fps;
    left_players[0] = PLAYER_HUMAN;
    right_players[0] = PLAYER_COMPUTER;
    for ( int i = 1; i < MAX_PLAYERS/2; i++ ) {
      left_players[i] = PLAYER_NONE;
      right_players[i] = PLAYER_NONE;
    }
  }

  inline void setFps(int val) {
    fps = val;
    mill_per_frame = 1000 / val;
  }
};

