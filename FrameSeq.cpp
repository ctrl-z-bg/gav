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

#include "FrameSeq.h"

#define max(x, y)  ((x)>(y)?(x):(y))

#define min(x, y)  ((x)<(y)?(x):(y))

void FrameSeq::blit(int idx, SDL_Surface * dest, SDL_Rect * rect) {
  SDL_Rect r;

  r.x = (idx % _nframes) * _width;
  r.y = 0;
  r.w = _width;
  r.h = _surface->h;
  SDL_BlitSurface(_surface, &r, dest, rect);
}

Uint16 CreateHicolorPixel(SDL_PixelFormat *fmt, Uint8 red,
			  Uint8 green, Uint8 blue) {
  return( ((red >> fmt->Rloss) << fmt->Rshift) +
	  ((green >> fmt->Gloss) << fmt->Gshift) +
	  ((blue >> fmt->Bloss) << fmt->Bshift) );
}

bool
FrameSeq::collidesWith(FrameSeq *fs, int idx1, int idx2, SDL_Rect * rect1,
		       SDL_Rect * rect2) {
  int xmin = max(rect1->x, rect2->x);
  int xmax = min(rect1->x + _width, rect2->x + fs->_width);
  int ymin = max(rect1->y, rect2->y);
  int ymax = min(rect1->y + _height, rect2->y + fs->_height);
  
  if ( (xmin > xmax) || (ymin > ymax) ) return(false);

  Uint16 *pix1 = (Uint16 *) _surface->pixels;
  Uint16 *pix2 = (Uint16 *) fs->_surface->pixels;

  Uint16 empty_pixel = CreateHicolorPixel(screen->format, 0, 0, 0);

  int sp1 = (_surface->pitch / 2);
  int sp2 = (fs->_surface->pitch / 2);
  int xdisp1 = ((idx1 % _nframes) * _width);
  int xdisp2 = ((idx2 % fs->_nframes) * fs->_width);
		
  while ( ymin <= ymax ) {
    int xrun = xmin;
    while ( xrun <= xmax ) {
      int p1off = sp1 * (ymin - rect1->y) + xdisp1 + (xrun - rect1->x);
      int p2off = sp2 * (ymin - rect2->y) + xdisp2 + (xrun - rect2->x);
      if ( ( pix1[p1off] != empty_pixel ) &&
	   ( pix2[p2off] != empty_pixel) )
	return(true);
      xrun++;
    }
    ymin++;
  }

  return(false);
}


