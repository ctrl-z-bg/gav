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

#include <string.h>
#include "ScreenFont.h"

void ScreenFont::printXY(SDL_Surface *dest, SDL_Rect *rect, const char * str,
			 bool wrapAround)
{
  SDL_Rect r;
  
  int _charWid = _frames->width();
  r.x = rect->x;
  r.y = rect->y;

  const char *run = str;
  while ( *run ) {

    if ( wrapAround )
      r.x = (r.x + dest->w) % dest->w;
    
    if ( ((*run) >= _fst) && ((*run) < (_fst + _nchars)) ) {
      _frames->blit((int) ((*run) - _fst), dest, &r);
    }
    run++;
    r.x += _charWid;
  }
}


void ScreenFont::printRow(SDL_Surface *dest, int row, const char *str,
			  SDL_Surface *bg)
{
  SDL_Rect rect;
  /* draw menu items labels */
  rect.y = 30 + row * charHeight();

  rect.x = (screen->w / 2) - strlen(str)*(charWidth())/2;
  if ( bg )
    SDL_BlitSurface(bg, &rect, dest, &rect);
  printXY(dest, &rect, str, false);
}
