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

#include <stdlib.h>
#include <SDL.h>
#include <sys/types.h>
#include <dirent.h>
#include "globals.h"
#include "ScreenFont.h"
#include "AutomaMainLoop.h"
#include "StatePlaying.h"
#include "StateMenu.h"

#include "MenuRoot.h"
#include "Menu.h"
#include "MenuItem.h"
#include "MenuItemSubMenu.h"
#include "MenuItemPlay.h"
#include "MenuItemExit.h"
#include "MenuItemPlayer.h"
#include "MenuItemNotImplemented.h"
#include "MenuKeys.h"
#include "MenuItemBack.h"
#include "MenuItemTheme.h"
#include "MenuItemFullScreen.h"
#include "MenuItemFrameSkip.h"
#include "MenuItemFPS.h"
#include "MenuItemScore.h"
#include "MenuItemMonitor.h"
#include "MenuItemBigBackground.h"
#include "MenuItemClient.h"
#include "MenuItemServer.h"

#include "Theme.h"

#include "Sound.h"

#include "NetClient.h"
#include "NetServer.h"

#define BPP 16

using namespace std;

#ifdef AUDIO
SDL_AudioSpec desired,obtained;

playing_t playing[MAX_PLAYING_SOUNDS];

void AudioCallBack(void *user_data,Uint8 *audio,int length)
{
  int i;

  memset(audio,0 , length);

  for(i=0; i <MAX_PLAYING_SOUNDS;i++)
    {
      if(playing[i].active) {
	Uint8 *sound_buf;
	Uint32 sound_len;
	sound_buf = playing[i].sound->samples;
	sound_buf += playing[i].position;

	if((playing[i].position +length)>playing[i].sound->length){
	  sound_len = playing[i].sound->length - playing[i].position;
	} else {
	  sound_len=length;
	}

	SDL_MixAudio(audio,sound_buf,sound_len,VOLUME_PER_SOUND);

	playing[i].position+=length;

	if(playing[i].position>=playing[i].sound->length){
	  playing[i].active=0;
	}
      }
    }
}


void ClearPlayingSounds(void)
{
  int i;

  for(i=0;i<MAX_PLAYING_SOUNDS;i++)
    {
      playing[i].active=0;
    }
}
#endif

void
init()
{
  if ( SDL_Init(SDL_INIT_VIDEO) ) {
    cerr << "Cannot initialize SDL, exiting." << endl;
    exit(1);
  }
  atexit(SDL_Quit);

#ifdef AUDIO
  atexit(SDL_CloseAudio);

  desired.freq=22050;
  desired.format = AUDIO_S16;
  desired.samples=4096;
  desired.channels=2;
  desired.callback=AudioCallBack;
  desired.userdata=NULL;

  
  if(SDL_OpenAudio(&desired,&obtained)<0){
    printf("Cannot open the audio device\n");
  }


  ClearPlayingSounds();

  SDL_PauseAudio(0);
#endif

  setThemeDir(TH_DIR);
  videoinfo = SDL_GetVideoInfo();
  CurrentTheme = new Theme(TH_DEFAULT);
}

#ifdef AUDIO
Sound * Prova;
#endif
#include <unistd.h>
int main(int argc, char *argv[]) {
#if 0
  NetClient * client;
  //NetServer * server;

  if (fork()) {
    nets = new NetServer();
    //delete(server); while(1);
    nets->StartServer();
    sleep(1);
    nets->WaitClients();
#if 0
    b.setFrame(33);
    while(1) {
      nets->SendSnapshot(&t1, &t2, &b);
      {
	char team, player;
	cntrl_t cmd;
	if (nets->ReceiveCommand(&team, &player, &cmd) != -1)
	  printf ("     +++++ %p %d %d\n", team, (int)player, (int)cmd);
      }
      usleep(1000/60);
      b.setFrame((b.frame() + 1) % 100);
      //printf("   +++++ %d\n",(int)b.frame());
    }
#endif
  } else {
    int pl, pr;
    init();
    Team t1, t2;
    Ball b(BALL_ORIG);
    client = new NetClient();
    //delete(client); while(1);
    usleep(100);
    client->ConnectToServer(&pl, &pr, NET_TEAM_RIGHT, "localhost");
    while(1) {
#if 1
      if (client->ReceiveSnapshot(&t1, &t2, &b) != -1)
	printf("client %p: %d\n", (int*)client->id(), b.x());
#endif
      client->SendCommand(CNTRL_JUMP);
      usleep(1000/60);
    }
  }
#endif // NETWORK TEST

  init();
  
#ifdef AUDIO
  Prova = new Sound("rocket.wav");
#endif

   /* initialize menus */
  Menu m;
  MenuItemPlay miplay;
  MenuItemExit miexit;
  Menu *menuExtra = new Menu();
  Menu *menuThemes = new Menu();
  Menu *menuNetwork = new Menu();
  MenuItemBack *mib = new MenuItemBack("back");
  DIR *dir;
  if ((dir = opendir(ThemeDir.c_str())) == NULL) {
    std::cerr << "Cannot find themes directory\n";
    exit(0);
  }
  struct dirent *themeDir;
  do {
    themeDir = readdir(dir);
    if ( themeDir && (themeDir->d_name[0] != '.') )
      menuThemes->add(new MenuItemTheme(string(themeDir->d_name)));
  } while (themeDir);
  closedir(dir);
  menuThemes->add(mib);

  menuNetwork->add(new MenuItemServer());
  menuNetwork->add(new MenuItemClient());
  menuNetwork->add(mib);

  menuExtra->add(new MenuItemSubMenu(menuThemes, string("Theme")));
  menuExtra->add(new MenuItemSubMenu(menuNetwork, string("Network game")));
  menuExtra->add(new MenuItemPlayer(TEAM_LEFT, 1));
  menuExtra->add(new MenuItemPlayer(TEAM_RIGHT, 1));
  menuExtra->add(new MenuItemSubMenu(new MenuKeys(1),
				     string("Define Keys")));
  menuExtra->add(new MenuItemFPS());
  menuExtra->add(new MenuItemFrameSkip());
  menuExtra->add(new MenuItemScore());
  menuExtra->add(new MenuItemMonitor());
  menuExtra->add(new MenuItemBigBackground());
  menuExtra->add(new MenuItemFullScreen());
  menuExtra->add(mib);

  m.add(&miplay);
  m.add(new MenuItemPlayer(TEAM_LEFT, 0));
  m.add(new MenuItemPlayer(TEAM_RIGHT, 0));
  m.add(new MenuItemNotImplemented(string("Sound Off")));
  m.add(new MenuItemSubMenu(new MenuKeys(0),
			    string("Define Keys")));
  m.add(new MenuItemNotImplemented(string("Set Joystick")));
  m.add(new MenuItemSubMenu(menuExtra,
			    string("Extra")));
  m.add(&miexit);
  mroot = new MenuRoot();
  mroot->add(&m);

  AutomaMainLoop *a = new AutomaMainLoop();
  a->start();

  return 0;
}
