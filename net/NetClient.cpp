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

#include "NetClient.h"
#include "MenuItemBigBackground.h"

using namespace std;

int NetClient::ConnectToServer(int * pl, int * pr, char team, 
			       char * hostname, int port) {
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

  packetRegister->address = packetSnap->address = 
    packetCmd->address = ipaddress;

  ((net_register_t*)(packetRegister->data))->id = team;

  SDLNet_UDP_Send(mySock, -1, packetRegister);
  while (!SDLNet_UDP_Recv(mySock, packetRegister));
  _id = ((net_register_t*)(packetRegister->data))->id;
  _nplayers_l = ((net_register_t*)(packetRegister->data))->nplayers_l;
  _nplayers_r = ((net_register_t*)(packetRegister->data))->nplayers_r;
  if (((net_register_t*)(packetRegister->data))->bgBig &&
      !configuration.bgBig) {
    MenuItemBigBackground menuBG;
    std::stack<Menu *> st;
    menuBG.execute(st);
  }
  
  *pl = (int)_nplayers_l;
  *pr = (int)_nplayers_r;

  return 0;
}

int NetClient::KillClient() {
  SDLNet_UDP_Unbind(mySock, channel);
  SDLNet_UDP_Close(mySock);
  return 0;
}

int NetClient::ReceiveSnapshot(Team *tleft, Team *tright, Ball * ball) {
  net_game_snapshot_t * snap;
  std::vector<Player *> plv;
  unsigned int i;

  if (SDLNet_UDP_Recv(mySock, packetSnap) != 0) {
    snap = (net_game_snapshot_t*)packetSnap->data;
    /* fill the left team informations */
    plv = tleft->players();
    for (i = 0; i < plv.size(); i++) {
      plv[i]->setX((int)(snap->teaml)[i].x);
      plv[i]->setY((int)(snap->teaml)[i].y);
      plv[i]->setState((pl_state_t)(snap->teaml)[i].frame);
    }
    /* fill the right team informations */
    plv = tright->players();
    for (i = 0; i < plv.size(); i++) {
      plv[i]->setX((int)(snap->teamr)[i].x);
      plv[i]->setY((int)(snap->teamr)[i].y);
      plv[i]->setState((pl_state_t)(snap->teamr)[i].frame);
    }
    /* fill the ball informations */
    ball->setX((int)(snap->ball).x);
    ball->setY((int)(snap->ball).y);
    ball->setFrame((int)(snap->ball).frame);

    /* fill the score information */
    tleft->setScore(snap->scorel);
    tright->setScore(snap->scorer);

    return 0;
  }
  return -1;
}

int NetClient::SendCommand(char cmd) {
  net_command_t * command = (net_command_t *)(packetCmd->data);
  command->id = _id;
  command->command = cmd;
  return SDLNet_UDP_Send(mySock, -1, packetCmd)?0:-1;
}

