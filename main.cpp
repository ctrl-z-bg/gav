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

#include <stdlib.h>
#include <SDL.h>
#include "globals.h"
#include "ScreenFont.h"
#include "AutomaMainLoop.h"
#include "StatePlaying.h"
#include "StateMenu.h"

#include "MenuRoot.h"
#include "Menu.h"
#include "MenuItem.h"
#include "MenuItemSubMenu.h"
#include "MenuItemPlay.h"
#include "MenuItemExit.h"
#include "MenuItemVar.h"
#include "MenuItemNotImplemented.h"

#include "Theme.h"


#define BPP 16

void
init()
{
  SDL_Init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);

  setThemeDir(TH_DIR);
  CurrentTheme = new Theme(TH_DEFAULT);
  
  background = IMG_Load(CurrentTheme->background());
  //if ( CurrentTheme->hasnet() ) IMG_Load(CurrentTheme->net());
  videoinfo = SDL_GetVideoInfo();

  //screenFlags = SDL_DOUBLEBUF|SDL_HWSURFACE;
  screenFlags = SDL_DOUBLEBUF;
  screen = SDL_SetVideoMode(SCREEN_WIDTH(),
			    //SCREEN_HEIGHT(), BPP, SDL_DOUBLEBUF);
			    SCREEN_HEIGHT(), videoinfo->vfmt->BitsPerPixel,
			    screenFlags);


  SDL_Surface *temp = background;
  background = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);  

  cga = new ScreenFont(CurrentTheme->font(), FONT_FIRST_CHAR, FONT_NUMBER);
  cgaInv = new ScreenFont(CurrentTheme->fontinv(), FONT_FIRST_CHAR, FONT_NUMBER);
}

int main() {
  init();

   /* initialize menus */
  Menu m(cga, cgaInv);
  MenuItemPlay miplay;
  MenuItemExit miexit;

  m.add(&miplay);
  m.add(new MenuItemPL1(string("PL1")));
  m.add(new MenuItemPL2(string("PL2")));
  m.add(new MenuItemNotImplemented((string)"Sound Off"));
  m.add(new MenuItemNotImplemented((string)"Define Keys"));
  m.add(new MenuItemNotImplemented((string)"Set Joystick"));
  m.add(&miexit);
  mroot = new MenuRoot();
  mroot->add(&m);

  AutomaMainLoop *a = new AutomaMainLoop();
  a->start();
}
