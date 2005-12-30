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

#include <math.h>
#include "GameRenderer.h"

using namespace std;

GameRenderer * gameRenderer = NULL;

void GameRenderer::display(SDL_Surface *dest, SDL_Rect *rect,
			   FrameSeq *what, int frame) {
  // Compute the actual coordinates in the display realm
  SDL_Rect r;

  r.x = (int) round(rect->x * _ratioX);
  r.y = (int) round(rect->y * _ratioY);
  r.w = (int) round(rect->w * _ratioX);
  r.h = (int) round(rect->h * _ratioY);

  what->blit(frame, dest, &r);
}
