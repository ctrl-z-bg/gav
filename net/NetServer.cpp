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

#include "NetServer.h"
#include "Player.h"

using namespace std;

int NetServer::StartServer(int port) {
  /* open the socket */
  mySock = SDLNet_UDP_Open(port);
  if(!mySock) {
    fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
    return -1;
  }

  return 0;
}

int NetServer::KillServer() {
  SDLNet_UDP_Close(mySock);
  return 0;
}

int NetServer::ComputePlayerID(char id) {
  char tm ,pl;
  tm = (id & NET_TEAM_LEFT)?0:1;
  pl = id & 0x3F;
  return  (2 * pl + tm);
}

int NetServer::WaitClients(int nclients) {
  IPaddress * ipa;
  char nleft = 0;
  char nright = 0;
  char * id;
  int i;
  bool inserted = false;

  memset(_players, 0, MAX_PLAYERS * sizeof(int));

  _nclients = nclients;

  while ((nright + nleft) != nclients) {
    inserted = false;
    if (SDLNet_UDP_Recv(mySock, packetRegister) != 0) {
  printf("            --- pacchetto ricevuto\n");
      ipa = (IPaddress*)malloc(sizeof(IPaddress));
      memcpy(ipa, &(packetRegister->address), sizeof(IPaddress));
      id = &(((net_register_t*)(packetRegister->data))->id);
      if (*id & NET_TEAM_LEFT) {
	for (i = 0; (i < configuration.left_nplayers) && !inserted; i++)
	  if ((configuration.left_players[i] == PLAYER_COMPUTER) &&
	      !_players[2*i])
	    inserted = true;
	if (inserted) {
	  nleft++;
	  *id |= nleft;
	} else
	  continue;
      } else if (*id & NET_TEAM_RIGHT) {
	for (i = 0; (i < configuration.right_nplayers) && !inserted; i++)
	  if ((configuration.right_players[i] == PLAYER_COMPUTER) &&
	      !_players[2*i+1])
	    inserted = true;
	if (inserted) {
	  *id |= nright;
	  nright++;
	} else
	  continue;
      }
      _players[ComputePlayerID(*id)] = 1;
      clientIP.push_back(ipa);
      /* send the ID back to client */
      ((net_register_t*)(packetRegister->data))->nplayers_l = 
	configuration.left_nplayers;
      ((net_register_t*)(packetRegister->data))->nplayers_r = 
	configuration.right_nplayers;
      ((net_register_t*)(packetRegister->data))->bgBig =
	configuration.bgBig;
      SDLNet_UDP_Send(mySock, -1, packetRegister);
    }
  }

  return 0;
}

int NetServer::SendSnapshot(Team *tleft, Team *tright, Ball * ball) {
  unsigned int i;
  net_game_snapshot_t * snap = (net_game_snapshot_t *)(packetSnap->data);
  std::vector<Player *> plv;

  snap->scorel = tleft->getScore();
  snap->scorer = tright->getScore();
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

int NetServer::ReceiveCommand(int * player, char * cmd) {
  if (SDLNet_UDP_Recv(mySock, packetCmd) != 0) {
    net_command_t * c = (net_command_t*)(packetCmd->data);
    *player = ComputePlayerID(c->id);
    *cmd = c->command;
    return 0;
  }

  return -1;
}

int NetServer::isRemote(int pl) {
  return _players[pl];
}
