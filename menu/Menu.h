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

#ifndef __MENU_H__
#define __MENU_H__

#include <SDL.h>
#include <vector>
#include "MenuItem.h"
#include "ScreenFont.h"
#include "InputState.h"

class Menu {
  std::vector<MenuItem *> items;
  ScreenFont * normal_font;
  ScreenFont * selected_font;
  int currentItem;
  unsigned int maxLabelLength;

public:
  Menu(ScreenFont * nf, ScreenFont * rf): 
    normal_font(nf), selected_font(rf), currentItem(-1), maxLabelLength(0) {}

  void add(MenuItem * mi);
  int execute(InputState *is, std::stack<Menu *> &s);
  ~Menu() {}
};

#endif
