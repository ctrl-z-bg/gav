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

#ifndef __MENUITEMVAR_H__
#define __MENUITEMVAR_H__

#include <SDL.h>
#include "MenuItem.h"
#include "globals.h"

class MenuItemPL2: public MenuItem {
 private:
  std::string prefix;

public:
  MenuItemPL2(std::string l) {
    prefix = l;
    setLabel();
  }
  void setLabel() {
    label = prefix + std::string(configuration.right_human?" Keyboard":" Computer");
  }
  int execute(std::stack<Menu *> &s) {
    if ( configuration.right_human )
      configuration.right_human = 0;
    else
      configuration.right_human = 1;
    setLabel();
    return(0);
  }
};

class MenuItemPL1: public MenuItem {
 private:
  std::string prefix;

public:
  MenuItemPL1(std::string l) {
    prefix = l;
    setLabel();
  }
  void setLabel() {
    label = prefix + std::string(configuration.left_human?" Keyboard":" Computer");
  }
  int execute(std::stack<Menu *> &s) {
    if ( configuration.left_human )
      configuration.left_human = 0;
    else
      configuration.left_human = 1;
    setLabel();
    return(0);
  }
};

#endif
