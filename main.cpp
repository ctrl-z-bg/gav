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
#include <sys/types.h>
#include <dirent.h>
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
#include "MenuItemPlayer.h"
#include "MenuItemNotImplemented.h"
#include "MenuKeys.h"
#include "MenuItemBack.h"
#include "MenuItemTheme.h"
#include "MenuItemFullScreen.h"

#include "Theme.h"


#define BPP 16

using namespace std;

void
init()
{
  SDL_Init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);

  setThemeDir(TH_DIR);
  videoinfo = SDL_GetVideoInfo();
  CurrentTheme = new Theme(TH_DEFAULT);
}

int main(int argc, char *argv[]) {
  init();

   /* initialize menus */
  Menu m;
  MenuItemPlay miplay;
  MenuItemExit miexit;
  Menu *menuExtra = new Menu();
  Menu *menuThemes = new Menu();
  MenuItemBack *mib = new MenuItemBack("back");
  DIR *dir;
  if ((dir = opendir(ThemeDir.c_str())) == NULL) {
    std::cerr << "Cannot find themes directory\n";
    exit(0);
  }
  struct dirent *themeDir;
  do {
    themeDir = readdir(dir);
    if ( themeDir && (themeDir->d_name[0] != '.') )
      menuThemes->add(new MenuItemTheme(string(themeDir->d_name)));
  } while (themeDir);
  closedir(dir);
  menuThemes->add(mib);

  menuExtra->add(new MenuItemSubMenu(menuThemes, string("Theme")));
  menuExtra->add(new MenuItemPlayer(TEAM_LEFT, 1));
  menuExtra->add(new MenuItemPlayer(TEAM_RIGHT, 1));
  menuExtra->add(new MenuItemSubMenu(new MenuKeys(1),
				     string("Define Keys")));
  menuExtra->add(new MenuItemFullScreen());
  menuExtra->add(mib);

  m.add(&miplay);
  m.add(new MenuItemPlayer(TEAM_LEFT, 0));
  m.add(new MenuItemPlayer(TEAM_RIGHT, 0));
  m.add(new MenuItemNotImplemented(string("Sound Off")));
  m.add(new MenuItemSubMenu(new MenuKeys(0),
			    string("Define Keys")));
  m.add(new MenuItemNotImplemented(string("Set Joystick")));
  m.add(new MenuItemSubMenu(menuExtra,
			    string("Extra")));
  m.add(&miexit);
  mroot = new MenuRoot();
  mroot->add(&m);

  AutomaMainLoop *a = new AutomaMainLoop();
  a->start();

  return 0;
}
