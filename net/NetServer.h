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

#ifndef __NETSERVER_H__
#define __NETSERVER_H__

#include "Net.h"

class NetServer: public Net {
  std::vector<IPaddress*> clientIP;

public:
  NetServer() {
  }

  ~NetServer() {
    unsigned int i;
    /* deallocation of clientIP elements */
    for (i = 0; i < clientIP.size(); i++)
      free(clientIP[i]);
  }

  /* server methods */
  int StartServer(int port = SERVER_PORT);
  int KillServer();
  int WaitClients(int nclients = 1);
  int SendSnapshot(Team *tleft, Team *tright, Ball * ball);
  int ReceiveCommand(char * team, char * player, cntrl_t * cmd);

};

#endif
