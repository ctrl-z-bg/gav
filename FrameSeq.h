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

#ifndef __FRAMESEQ_H__
#define __FRAMESEQ_H__

#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

extern SDL_Surface *screen;

class FrameSeq {
  SDL_Surface * _surface;
  int _nframes;
  int _width;
  int _height;

public:

  FrameSeq (const char * filename, int nframes) {
    SDL_Surface * temp;

    _nframes = nframes;
    if ((temp = IMG_Load(filename)) == NULL) {
      fprintf(stderr, "FrameSeq: cannot load %s\n", filename);
      exit(-1);
    }
    SDL_SetColorKey(temp, SDL_SRCCOLORKEY,
		    (Uint16) SDL_MapRGB(screen->format, 0, 0, 0));
    _surface = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);
    _width = _surface->w / _nframes;
    _height = _surface->h;
  }

  ~FrameSeq() {
    SDL_FreeSurface(_surface);
  }

  inline int width() { return _width; }
  inline int height() { return _height; }

  void blit(int idx, SDL_Surface * dest, SDL_Rect * rect);
  bool collidesWith(FrameSeq *fs, int idx1, int idx2, SDL_Rect * rect1,
		    SDL_Rect * rect2);

};

#endif
