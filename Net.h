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

#ifndef __NET_H__
#define __NET_H__

#include "Configuration.h"
#include <SDL_net.h>
#include <vector>

#define PLAYER_FOR_TEAM_IN_NET_GAME 2
#define SERVER_PORT 7145

typedef struct {
  short x;
  short y;
  char  frame;
} net_object_snapshot_t;

typedef struct {
  net_object_snapshot_t team1[PLAYER_FOR_TEAM_IN_NET_GAME];
  net_object_snapshot_t team2[PLAYER_FOR_TEAM_IN_NET_GAME];
  net_object_snapshot_t ball;
} net_game_snapshot_t;

typedef struct {
  char id;       // the client ID
  char command;
} net_command_t;

class Net {
  std::vector<IPaddress*> clientIP;
  UDPsocket mySock;
  IPaddress ipaddress;
  UDPpacket * packetCmd;
  UDPpacket * packetSnap;
  int channel;   // the channel assigned to client

public:
  Net() {
    packetSnap = SDLNet_AllocPacket(sizeof(net_game_snapshot_t));
    packetSnap->len = packetSnap->maxlen;
    packetCmd = SDLNet_AllocPacket(sizeof(net_command_t));
    packetCmd->len = packetCmd->maxlen;
  }

  ~Net() {
    unsigned int i;
    SDLNet_FreePacket(packetSnap);
    SDLNet_FreePacket(packetCmd);
    /* TODO: deallocation of clientIP elements */
    for (i = 0; i < clientIP.size(); i++)
      free(clientIP[i]);
  }

  /* server methods */
  int StartServer(int port = 7145);
  int KillServer();
  int WaitClients(int nclients = 1);
  int SendSnapshot(net_game_snapshot_t * snap);
  int ReceiveCommand();

  /* client methods */
  int ConnectToServer(char * hostname, int port = 7145);
  int KillClient();
  int ReceiveSnapshot(net_game_snapshot_t * snap);
  int SendCommand();
};

#endif
