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

#include "Menu.h"
#include <string.h>
#include "SoundMgr.h"

using namespace std;

void Menu::add(MenuItem * mi) {
  if (currentItem < 0)
    currentItem = 0;
  if ((mi->getLabel()).length() > maxLabelLength)
    maxLabelLength = (mi->getLabel()).length();
  items.push_back(mi);
}

int Menu::execute(InputState *is, std::stack<Menu *> &s) {
  SDL_Rect rect;
  const char * label;
  ScreenFont * font;
  static bool spacePressed = false;

  if ( is->getKeyState()[SDLK_UP] && (currentItem > 0) ) {
    currentItem--;
#ifdef AUDIO
    soundMgr->playSound(SND_MENU_SELECT);
#endif // AUDIO
  }
  if ( is->getKeyState()[SDLK_DOWN] &&
       (currentItem < (int) (items.size() - 1)) ) {
    currentItem++;
#ifdef AUDIO
    soundMgr->playSound(SND_MENU_SELECT);
#endif // AUDIO
  }
  /* draw menu items labels */
  rect.y = 30;
  for ( unsigned int it = 0; it < items.size(); it++ ) {
    label = items[it]->getLabel().c_str();
    rect.x = (screen->w / 2) - strlen(label)*(cga->charWidth())/2;
    if (it == (unsigned int) currentItem)
      font = cgaInv;
    else
      font = cga;
    font->printXY(screen, &rect, items[it]->getLabel().c_str());
    rect.y += font->charHeight();
  }

  /* call the execute method of the current item, if an event occurred */
  if ( (is->getKeyState()[SDLK_SPACE]) ||
       (is->getKeyState()[SDLK_RETURN]) ) {
    spacePressed = true;
  }
  if ( spacePressed && !(is->getKeyState()[SDLK_SPACE]) &&
       !(is->getKeyState()[SDLK_RETURN]) ) {
    spacePressed = false;
#ifdef AUDIO
    soundMgr->playSound(SND_MENU_ACTIVATE);
#endif // AUDIO
    return items[currentItem]->execute(s);
  }
  return 0;
}

