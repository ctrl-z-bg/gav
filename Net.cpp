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

#include "Net.h"

int Net::StartServer(int port) {
  /* open the socket */
  mySock = SDLNet_UDP_Open(port);
  if(!mySock) {
    fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
    return -1;
  }

  return 0;
}

int Net::KillServer() {
  SDLNet_UDP_Close(mySock);
  return 0;
}

int Net::WaitClients(int nclients) {
  IPaddress * ipa;
  int connected = 0;

  while (connected != nclients) {
    if (SDLNet_UDP_Recv(mySock, packetCmd) != 0) {
      ipa = (IPaddress*)malloc(sizeof(IPaddress));
      memcpy(ipa, &(packetCmd->address), sizeof(IPaddress));
      clientIP.push_back(ipa);
      connected++;
    }
  }

  return 0;
}

int Net::SendSnapshot(net_game_snapshot_t * snap) {
  unsigned int i;

  memcpy(packetSnap->data, snap, sizeof(net_game_snapshot_t));
  /* send the snapshot to all clients */
  for (i = 0; i < clientIP.size(); i++) {
    packetSnap->address = *(clientIP[i]);
    SDLNet_UDP_Send(mySock, -1, packetSnap);
  }
  return 0;
}

int Net::ReceiveCommand() {
  return 0;
}

int Net::ConnectToServer(char * hostname, int port) {
  /* open the socket */
  mySock = SDLNet_UDP_Open(0);
  /* resolve the server name */
  if (SDLNet_ResolveHost(&ipaddress, hostname, port) == -1) {
    fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
    return -1;
  }
  /* bind */
  channel=SDLNet_UDP_Bind(mySock, -1, &ipaddress);
  if(channel==-1) {
    fprintf(stderr, "SDLNet_UDP_Bind: %s\n", SDLNet_GetError());
    return -1;
  }

  packetCmd->address = packetSnap->address = ipaddress;
  
  return 0;
}

int Net::KillClient() {
  SDLNet_UDP_Unbind(mySock, channel);
  SDLNet_UDP_Close(mySock);
  return 0;
}

int Net::ReceiveSnapshot(net_game_snapshot_t * snap) {
  if (SDLNet_UDP_Recv(mySock, packetSnap) != 0) {
    memcpy(snap, packetSnap->data, sizeof(net_game_snapshot_t));
    return 0;
  }
  return -1;
}

int Net::SendCommand() {
  net_command_t * data = (net_command_t*)(packetCmd->data);
  int ret;

  data->command = 123;

  ret = SDLNet_UDP_Send(mySock, -1, packetCmd);
  return ret?0:-1;
}

