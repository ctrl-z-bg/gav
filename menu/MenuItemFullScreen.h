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

#ifndef __MENUITEMFULLSCREEN_H__
#define __MENUITEMFULLSCREEN_H__

#include <SDL.h>
#include "MenuItem.h"
#include "globals.h"

class MenuItemFullScreen: public MenuItem {
public:
  MenuItemFullScreen() {
    label = std::string("Fullscreen: ");
    label += (configuration.fullscreen?"Yes":"No");
  }

  void apply() {
    if ( configuration.fullscreen )
      screenFlags |= SDL_FULLSCREEN;
    else
      screenFlags = (screenFlags & ~SDL_FULLSCREEN);
  }

  int execute(std::stack<Menu *> &s) {
    SDL_FreeSurface(screen);
    configuration.fullscreen = !configuration.fullscreen;
    label = configuration.fullscreen?"Fullscreen: Yes":"FullScreen: No";
    apply();
    screen = SDL_SetVideoMode(SCREEN_WIDTH(),
			      //	SCREEN_HEIGHT(), BPP, screenFlags);
			      SCREEN_HEIGHT(),
			      videoinfo->vfmt->BitsPerPixel, 
			      screenFlags);
    
    return(0);
  }
};

#endif
