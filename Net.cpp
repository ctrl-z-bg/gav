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
#include "Player.h"

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
  char nleft = 0;
  char nright = 0;
  char * id;

  while ((nright + nleft) != nclients) {
    if (SDLNet_UDP_Recv(mySock, packetCmd) != 0) {
      ipa = (IPaddress*)malloc(sizeof(IPaddress));
      memcpy(ipa, &(packetCmd->address), sizeof(IPaddress));
      clientIP.push_back(ipa);
      id = &(((net_command_t*)(packetCmd->data))->id);
      if (*id & TEAM_LEFT) {
	*id |= nleft;
	nleft++;
      } else if (*id & TEAM_RIGHT) {
	*id |= nright;
	nright++;
      }
      /* send the ID back to client */
      SDLNet_UDP_Send(mySock, -1, packetCmd);
    }
  }

  return 0;
}

int Net::SendSnapshot(Team *tleft, Team *tright, Ball * ball) {
  unsigned int i;
  net_game_snapshot_t * snap = (net_game_snapshot_t *)(packetSnap->data);
  std::vector<Player *> plv;

  /* fill the left team informations */
  plv = tleft->players();
  for (i = 0; i < plv.size(); i++) {
    (snap->teaml)[i].x = plv[i]->x();
    (snap->teaml)[i].y = plv[i]->y();
    (snap->teaml)[i].frame = plv[i]->state();
  }
  /* fill the right team informations */
  plv = tright->players();
  for (i = 0; i < plv.size(); i++) {
    (snap->teamr)[i].x = plv[i]->x();
    (snap->teamr)[i].y = plv[i]->y();
    (snap->teamr)[i].frame = plv[i]->state();
  }
  /* fill the ball informations */
  (snap->ball).x = ball->x();
  (snap->ball).y = ball->y();
  (snap->ball).frame = ball->frame();

  /* send the snapshot to all clients */
  for (i = 0; i < clientIP.size(); i++) {
    packetSnap->address = *(clientIP[i]);
    SDLNet_UDP_Send(mySock, -1, packetSnap);
  }
  return 0;
}

int Net::ReceiveCommand(char * team, char * player, cntrl_t * cmd) {
  if (SDLNet_UDP_Recv(mySock, packetCmd) != 0) {
    net_command_t * c = (net_command_t*)(packetCmd->data);
    *team = (c->id) & 0xC0;
    *player = (c->id) & 0x3F;
    *cmd = c->command;
    return 0;
  }

  return -1;
}

int Net::ConnectToServer(char team, char * hostname, int port) {
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
  ((net_command_t*)(packetCmd->data))->id = team;

  SDLNet_UDP_Send(mySock, -1, packetCmd);
  while (!SDLNet_UDP_Recv(mySock, packetCmd));
  _id = ((net_command_t*)(packetCmd->data))->id;

  return 0;
}

int Net::KillClient() {
  SDLNet_UDP_Unbind(mySock, channel);
  SDLNet_UDP_Close(mySock);
  return 0;
}

int Net::ReceiveSnapshot(Team *tleft, Team *tright, Ball * ball) {
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
    return 0;
  }
  return -1;
}

int Net::SendCommand(cntrl_t cmd) {
  net_command_t * command = (net_command_t *)(packetCmd->data);
  command->id = _id;
  command->command = cmd;
  return SDLNet_UDP_Send(mySock, -1, packetCmd)?0:-1;
}

