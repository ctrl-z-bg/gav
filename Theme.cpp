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

#include <unistd.h>
#include <string>
#include "Theme.h"

Theme *CurrentTheme;
string ThemeDir;

void errorOn(string file) {
    cerr << "Error accessing file " << file;
    exit(-1);
}

bool Theme::_checkTheme() {
    bool r;
    cerr << "Verifying Theme `" << _name << "' [" << ThemeDir << "/" << _name << "/]:\n";
    if ( access(_CCS(_background), R_OK) )  errorOn(TH_BACKGROUND);
    if ( access(_CCS(_font), R_OK) )        errorOn(TH_FONT);
    if ( access(_CCS(_fontinv), R_OK) )     errorOn(TH_FONTINV);
    if ( access(_CCS(_leftmale), R_OK) )    errorOn(TH_LEFTMALE);
    if ( access(_CCS(_rightmale), R_OK) )   errorOn(TH_RIGHTMALE);
    if ( access(_CCS(_leftfemale), R_OK) )  errorOn(TH_LEFTFEMALE);
    if ( access(_CCS(_rightfemale), R_OK) ) errorOn(TH_RIGHTFEMALE);
    if ( access(_CCS(_ball), R_OK) )        errorOn(TH_BALL);

    r = (access(_CCS(_net), R_OK) == 0);
    
    if ( !r ) cerr << "Warning: No net for this theme!\n";

    cerr << "OK!\n";

    return(r);
}
