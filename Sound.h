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

#ifndef __SOUND_H__
#define __SOUND_H__

#include <SDL/SDL.h>
#include "globals.h"

#ifdef AUDIO
class Sound {
 private:
 
  sound_t  this_sound;
  int loadAndConvertSound(char *filename, SDL_AudioSpec *spec,sound_p sound);
 public:
  Sound(char *filename) {
    loadAndConvertSound(filename,&obtained,&this_sound);
  }
  int playSound();

};
#endif

#endif //_SOUND_H_
