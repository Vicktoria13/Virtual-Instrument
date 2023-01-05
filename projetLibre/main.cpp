/**
 * @file main.cpp
 * @author Victoria et Matthieu
 * @brief 
 * @version 0.1
 * @date 2022-06-08
 * 
 * 
 */


#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include <SDL2/SDL.h>
#include <cstdlib>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>

#include "play.h"

using namespace cv;
using namespace std;





int main(){
  // INIT

  /**
   * @brief Fonction principale du jeu : permet de lancer le menu et le jeu en fonction du choix de l'utilisateur
   * 
   */


    if (SDL_Init(SDL_INIT_VIDEO) < 0)     
    {         
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());         
        return EXIT_FAILURE;     
    }    


    /*******************************SDL AUDIO*************************************************************************************************/

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return -1;

    if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur initialisation SDL_mixer : %s", Mix_GetError());
        SDL_Quit();
        return -1;
    }



    /******************************FENETRE + RENDERER SDL************************************************************************************************/

        SDL_Window* pWindow{ nullptr };     
        SDL_Renderer* pRenderer{ nullptr };  


    pWindow = SDL_CreateWindow("SDL ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 100, 75, SDL_WINDOW_SHOWN);   
    
    
    if (pWindow == nullptr)     
    {         
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());         
        SDL_Quit();         
        return EXIT_FAILURE;     
    }            

    pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);       


     if (SDL_CreateWindowAndRenderer(200, 150, SDL_WINDOW_SHOWN, &pWindow, &pRenderer) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur creation fenetre et rendue SDL : %s", SDL_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return -1;
    }
    
    /******************************************************************************************************************************/
     
   


    Mix_Chunk* gameO = Mix_LoadWAV("/home/victoria/Bureau/ProjetROB3/ProjetC_NGUYEN_SYSOUVANH/projetLibre/samples piano/game over.wav");
    Mix_Chunk* begin = Mix_LoadWAV("/home/victoria/Bureau/ProjetROB3/ProjetC_NGUYEN_SYSOUVANH/projetLibre/samples piano/start jeu.wav");



      Mix_AllocateChannels(2);

     // Mets le son a 100% en volume pour les premier cannaux
    
      Mix_Volume(0, MIX_MAX_VOLUME );  
      Mix_Volume(1, MIX_MAX_VOLUME );  


	while(1){

        
		int a = startmenu();//on retourne le mode de jeu sélectionné
        Mix_PlayChannel(0, begin, 0);
		if(a==1){//mode freestyle
        int b =  jeu();
		}
		int jeu=0;
		if(a==2){//mode jeu
			jeu = piano_tiles();
		}
		if(jeu==1){//on a perdu le jeu
             Mix_PlayChannel(0, gameO, 0);
			int b=gameover();//lancer ecran game over
			if(b==2){//le joueur a choisi de quitter le programme
				break;
			}
			jeu==0;
		}
		if(a==3){//on a appuyé sur escape dans le menu
			break;
		}
	}




  Mix_FreeChunk(gameO);
  Mix_FreeChunk(begin);
	return 0;
}
