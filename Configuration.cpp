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

#include <string>
#include <iostream>
#include "Configuration.h"

using namespace std;

Configuration configuration;

Aargh aargh;

int Configuration::loadConfiguration(const char *fname) {
  if ( !aargh.loadConf(fname) )
    return(-1);
  string value;
  if ( aargh.getArg("left_nplayers", value) )
    configuration.left_nplayers = atoi(value.c_str());
  if ( aargh.getArg("right_nplayers", value) )
    configuration.right_nplayers = atoi(value.c_str());
  if ( aargh.getArg("monitor_type", value) ) {
    if ( value == "normal" )
      configuration.monitor_type = MONITOR_NORMAL;
    else if ( value == "old" )
      configuration.monitor_type = MONITOR_OLD;
    else if ( value == "very_old")
      configuration.monitor_type = MONITOR_VERYOLD;
    else if ( value == "very_very_old")
      configuration.monitor_type = MONITOR_VERYVERYOLD;
    else
      cerr << "Unrecognized monitor type \"" << value << "\"\n";
  }
  if ( aargh.getArg("frame_skip", value) )
    configuration.frame_skip = atoi(value.c_str());
  if ( aargh.getArg("fps", value) )
    configuration.setFps(atoi(value.c_str()));
  for ( int i = 0; i < left_nplayers; i++ ) {
    if ( aargh.getArg(("left_player" + toString(i)), value) )
      if ( value ==  "human")
	configuration.left_players[i] = PLAYER_HUMAN;
      else if ( value == "computer")
	configuration.left_players[i] = PLAYER_COMPUTER;
      else
	cerr << "Unrecognized player type \"" << value << "\"\n";
  }
  
  for ( int i = 0; i < right_nplayers; i++ ) {
    if ( aargh.getArg(("right_player" + toString(i)), value) )
      if ( value == "human" )
	configuration.right_players[i] = PLAYER_HUMAN;
      else if ( value == "computer")
	configuration.right_players[i] = PLAYER_COMPUTER;
      else
	cerr << "Unrecognized player type \"" << value << "\"\n";
  }
  if ( aargh.getArg("big_background", value) )
    configuration.bgBig = (value=="yes");
  if ( aargh.getArg("fullscreen", value) )
    configuration.fullscreen = (value=="yes");
  if ( aargh.getArg("ball_speed", value) )
    if ( value == "normal" )
      configuration.ballAmplify = DEFAULT_BALL_AMPLIFY;
    else if ( value == "fast" )
      configuration.ballAmplify = DEFAULT_BALL_AMPLIFY + BALL_SPEED_INC;
    else if ( value == "very_fast" )
      configuration.ballAmplify = DEFAULT_BALL_AMPLIFY + 2*BALL_SPEED_INC;
    else
      cerr << "Unrecognized ball speed \"" << value << "\"\n";
  if ( aargh.getArg("theme", value) )
    configuration.currentTheme = value;

  if ( aargh.getArg("sound", value) )
    configuration.sound = (value == "yes");

  if ( aargh.getArg("winning_score", value) )
    configuration.winning_score = atoi(value.c_str());

  return 0;
}

/* we'll use aargh's dump feature! Yiipeee!! */
int Configuration::saveConfiguration(const char *fname) {
  string tosave;
  
  aargh.dump(tosave);
  
  FILE *fp;
  if ( (fp = fopen(fname, "w")) == NULL )
    return(-1);
  
  fputs(tosave.c_str(), fp);
  fclose(fp);
  return 0;
}

/* unfortunately, I *HAVE* to go through all the settings...
   This function puts configuration parameters inside aargh, and then
   dumps it. */
int Configuration::createConfigurationFile(const char *fname) {
  Configuration c = configuration; /* for short :) */

  aargh.setArg("left_nplayers", c.left_nplayers);
  aargh.setArg("right_nplayers", c.right_nplayers);
  switch ( c.monitor_type ) {
  case MONITOR_NORMAL:
    aargh.setArg("monitor_type", "normal"); break;
  case MONITOR_OLD:
    aargh.setArg("monitor_type", "old"); break;
  case MONITOR_VERYOLD:
    aargh.setArg("monitor_type", "very_old"); break;
  case MONITOR_VERYVERYOLD:
    aargh.setArg("monitor_type", "very_very_old"); break;
  }
  aargh.setArg("frame_skip", c.frame_skip);
  aargh.setArg("fps", c.fps);
  for ( int i = 0; i < left_nplayers; i++ ) {
    switch ( c.left_players[i] ) {
    case PLAYER_HUMAN:
      aargh.setArg("left_player" + toString(i), "human"); break;
    case PLAYER_COMPUTER:
      aargh.setArg("left_player" + toString(i), "computer"); break;
    } 
  }
  for ( int i = 0; i < right_nplayers; i++ ) {
    switch ( c.right_players[i] ) {
    case PLAYER_HUMAN:
      aargh.setArg("right_player" + toString(i), "human"); break;
    case PLAYER_COMPUTER:
      aargh.setArg("right_player" + toString(i), "computer"); break;
    }
  }
  aargh.setArg("big_background", "yes");
  aargh.setArg("fullscreen", (c.fullscreen?"yes":"no"));
  if ( c.ballAmplify == DEFAULT_BALL_AMPLIFY + BALL_SPEED_INC )
    aargh.setArg("ball_speed", "fast");
  else if ( c.ballAmplify == DEFAULT_BALL_AMPLIFY + 2*BALL_SPEED_INC )
    aargh.setArg("ball_speed", "very_fast");
  aargh.setArg("theme", c.currentTheme);
  aargh.setArg("sound", c.sound?"yes":"no");
  aargh.setArg("winning_score", c.winning_score);

  return(saveConfiguration(fname));
}
