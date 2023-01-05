/**
 * @file play.cpp
 * @author Victoria Nguyen et Matthieu Sysouvanh ROB3
 * @brief fichier cpp contenant les fonctions appelées dans le programme principal. Permet de jouer des instruments virtuellement via 
 * un objet tracké via sa couleur.
 * @version 0.1
 * @date 2022-06-08
 * 
 *\details Fichier regroupant toutes les fonctions nécessaires pour chaque mode.
 * ATTENTION : 
 * IMPORTANT : 
 * 
 * - Il faut absolument modifier la LIGNE 91 avec le chemin absolue depuis la racine qui mene au dossier sampples piano situé dans projetLibre
 * 
 * 
 * - Si le chemin d'ecriture lors de l'ouverture du fichier port série ne fonctionne pas, il y aura une erreur :
 * le chemin actuel est /dev/ttyACM0. Changer la variable path pour que le port ou est branché la carte soit le bon ! 
 * 
 * LIGNE 91
 * 
 * 
 * *Si les servos ne sont pas branchés/ relié a l'ordinateur, le programme ne fonctionne pas
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

#include <iostream>
#include <time.h>
#include <string.h>


//VARIABLES GLOBALES 
#define TAILLEX 640//taille de l'image de la camera
#define TAILLEY 480
#define ID 0//choix du port de la camera
#define TOLERANCE 30 //tolérance sur le filtre de couleur
#define VIES 2//!nombre de vies en mode jeu 
#define TEMPO 50000


// tracé du piano
#define noir_longueur 150 // en pixels
#define noir_largeur 40
#define touche 14 // 12 touches piano + slow drum + saxo


#define point_debut_x  90
#define point_debut_y  450

#define x_LeftUP point_debut_x
#define y_LeftUp point_debut_y-noir_longueur

#define epsilon 25
#define alpha 0.7

#define largeur_drum 80
#define longueur_drum 80


// Pour le jeu

#define largeur_touche 50
#define longueur_touche 50

#define DEADZONE_X 610
#define DEADZONE_Y 450

/***********************************************************************************************************************/

using namespace cv;
using namespace std;


/**************************Variables Globales*********************************************************************************************/

/************A MODIFIER ***********************************************************************************/
/************A MODIFIER ***********************************************************************************/
// Remplacer par le chemin absolue de l'utilisateur menant au dossier samples piano

 char chemin_acces[200]="/home/victoria/Bureau/ProjetROB3/ProjetC_NGUYEN_SYSOUVANH/projetLibre/samples piano/";

/************A MODIFIER ***********************************************************************************/
/************A MODIFIER ***********************************************************************************/

FILE* pFile; 




const char*  path ="/dev/ttyACM0";


char window_name[130] = "Choisissez la couleur a tracker sur cette fenetre";

Mat src;

int flag_servo; // flag permettant de savoir si les servos sont bien en état de recevoir des données

Mat frame,hsv,mask,img_transparent;  

int blurSize = 5;

int elementSize = 5;






//GESTION DES MENUS ET BOUTONS

Rect bouton1;
Rect bouton2;
Mat fond;
string menu="menu";
string game_over="game over";


/***********************************************************************************************************************/



struct pixel {
  /**
   * 
   * \struct pixel
 * @brief  Structure permettant de regrouper le BGR d'un pixel
 * 
 */

int bl;
int gre;
int re;
 
 } pixel_frame;



struct HsV {
/**
   * 
   * \struct pixel
 * @brief  Structure permettant de regrouper le HSV d'un pixel
 * 
 */
int hue;
int sat;
int val;
 
 } HsV_frame;




void onMouse( int event, int x, int y, int f, void* param){
  /**
     * @brief Fonction callback : appelée a chaque evenemet de la souris. Place les valeurs GRB et HSV du pixel a la position
     * (x,y) dans des variables globales, declarées dans des structures.
     * 
     * \param x coordonnées x souris
     * \param y coordonnées y souris
     * \param event evenement declencheur de la fonction
     * \param param parametre modifié par la fonction
     * 
     * \return Aucun 
     * 
     */


     if  ( event == EVENT_LBUTTONDOWN ){ // si on detecte un clic gauche : selection d'une couleur depuis l'ecran

     Mat* snapshot = (Mat*)param; // On cree une matrice pointeur sur la source (cam)
     Vec3b pixel = snapshot->at<Vec3b>(Point (x, y)); // On recupere les pixels RGB

     pixel_frame.bl=pixel.val[0]; // stocké dans la structure Pixel_frame
     pixel_frame.gre=pixel.val[1];
     pixel_frame.re=pixel.val[2];

     Mat HSV;
     cvtColor(*snapshot, HSV,COLOR_BGR2HSV); // converti en HSV pour recuperer le HSV du pixel


    Vec3b hsv=HSV.at<Vec3b>(Point (x, y));
    HsV_frame.hue=hsv.val[0]; // stocké dans la structure HSV_frame
    HsV_frame.sat=hsv.val[1];
    HsV_frame.val=hsv.val[2];
    
     // affichage du HSV
     imshow( window_name, *snapshot );

    
     }
}


void capture_couleur( int event, int x, int y, int f, void* param){

	/**
     * @brief Fonction callback : appelée a chaque evenemet de la souris. Place les valeurs GRB et HSV du pixel a la position
     * (x,y) dans des variables globales, declarées dans des structures.
     * 
     * \param x coordonnées x souris
     * \param y coordonnées y souris
     * \param event evenement declencheur de la fonction
     * \param param parametre modifié par la fonction
     * 
     * \return Aucun 
     * 
     */

	if  ( event == EVENT_LBUTTONDOWN ){ // si on detecte un clic gauche : selection d'une couleur depuis l'ecran
    Mat* snapshot = (Mat*)param; // On cree une matrice pointeur sur la source (cam)
    Vec3b pixel = snapshot->at<Vec3b>(Point (x, y)); // On recupere les pixels RGB

    pixel_frame.bl=pixel.val[0]; // stocké dans la structure Pixel_frame
    pixel_frame.gre=pixel.val[1];
    pixel_frame.re=pixel.val[2];

    Mat HSV;
    cvtColor(*snapshot, HSV,COLOR_BGR2HSV); // converti en HSV pour recuperer le HSV du pixel

    Vec3b hsv=HSV.at<Vec3b>(Point (x, y));
    HsV_frame.hue=hsv.val[0]; // stocké dans la structure HSV_frame
    HsV_frame.sat=hsv.val[1];
    HsV_frame.val=hsv.val[2];

    //cout<< "(B,G,R)=" << pixel_frame.bl <<", "<< pixel_frame.gre <<", "<< pixel_frame.re <<endl;   //affichage des valeurs possible
    //cout<< "(H,S,V)=" << HsV_frame.hue<<", "<< HsV_frame.sat<<", "<< HsV_frame.val <<endl;
    }
}





void init_flags(int* tab,int n){
  /**
   * @brief Met tous les elements de tab a 1
   * 
   * \param tab pointeur vers le tableau
   * \param tab tableau des flags de chaque touche du piano
   * \param n taille du tableau
   * \return  Aucun 
   * 
   */
 for (int i=0;i<n;i++){
   tab[i]=1; // tous les flags sont initalisé a 1
 }
 

 
}
void gestion_flags(int* tab, int taille, int position_set0){
 // met a 0 le flag qui est en position tab[position_set0]; et 1 le reste
 // attention : position_set0 est entre 1 et n

 /**
   * @brief  Fonction permettant la maintenance d'un système de flags. Appelée lors de la fonction principale, de sorte que si
   * l'on passe dans un rectangle (=touche=), alors il faut passer dans un autre rectangle afin de pouvoir retoucher le rectangle.
   * 
   * \param tab tableau des flags dans l'ordre 
   * \param n taille du tableau
   * 
   * \return Aucun
   * 
   */
 init_flags(tab,taille);
 tab[position_set0-1] = 0; //
 }
 






int jeu()
{   // Partie Libre 1.1
 
  /*!
  * \brief Mode instrument virtuels
  * Fonction permettant de jouer une musique virtuelle via des samples. Un piano virtuel + des cases d'instruments.
  * Le programme se base sur la detection de couleur comme le projet imposé.
  * 
  * \return un entier : -1 si une des initialisations n'a pas marché, 0 si le programme a été arreté par l'utilisateur.
  * 
  * 
  */
  /********************************* Inititialisation Camera *******************************************************************/
  VideoCapture cap(ID);

    if (cap.isOpened() == false)  
        {
            cout << "Cannot open the video camera" << endl;
            cin.get(); //wait for any key press
            return -1;
        } 
  namedWindow("well");


pixel_frame.bl=0;
pixel_frame.gre=0;
pixel_frame.re=0;

HsV_frame.hue=0;
HsV_frame.sat=0;
HsV_frame.val=0;



 /****************flags***************************************************************************/
    
int tab_flags[touche];
init_flags(tab_flags,touche); // tous les flags sont initialisé a 1


 /*******************************SDL_ AUDIO et VIDEO*************************************************************************************************/


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


    pWindow = SDL_CreateWindow("SDL martigues", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 100, 75, SDL_WINDOW_SHOWN);   
    
    
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
    
    /*******************************COnverti Mat en SDL*************************************************************************************************/
    
    SDL_Texture* tex = SDL_CreateTexture(
    pRenderer, SDL_PIXELFORMAT_BGR24, SDL_TEXTUREACCESS_STREAMING, img_transparent.cols,
    img_transparent.rows);


    
    /*****************************Allocation des canaux pour le son*************************************************************************************************/
    // On alloue autant de canaux que de sons que l'on veut jouer.
     Mix_AllocateChannels(touche); // On alloue 12 sons (piano) + son fond + SlowDrum !!  de 0 a touche-1 !!

     // Mets le son a 100% en volume pour les premier cannaux
    
      for (int i=0;i<=13;i++){
            Mix_Volume(i, MIX_MAX_VOLUME );  
            }// Mets le son a 100% pour les 12 touches de piano + le saxo

      //Mix_Volume(touche, MIX_MAX_VOLUME/20); // Chanson de fond a volume reduit : slow drum
     
  

    /***********************************SERVO MOTEURS ******************************************************************************************/
    FILE* pFile; 
     pFile = fopen(path,"w");//on définit le fichier texte servant au transfert de données par le port série		
	  if(pFile==NULL){
          printf("erreur de port : les servomoteurs ne marcheront pas");
          flag_servo=0;
     }
     else {
       fclose(pFile);
       flag_servo=1;
     }
     
    /***************************Chargement des pistes************************************************************************************************/
    // A modifier selon le path
    char not1[20]="13-C3.wav";
    char not2[20]="15-D3.wav";
    char not3[20]="16-D3#.wav";
    char not4[20]="17-E3.wav";
    char not5[20]="18-F3.wav";
    char not6[20]="19-F3#.wav";
    char not7[20]="20-G3.wav";
    char not8[20]="21-G3#.wav";
    char not9[20]="22-A4.wav";
    char not10[20]="23-A4#.wav";
    char not11[20]="24-B4.wav";
    char not12[20]="22-A4.wav";

    char tromp[20]="trumpet.wav";
    char beat[20]="SlowDrum.wav";
    char saxx[20]="saxo.wav";
    

    char chemin[200];


    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, not1); // Fusionne chemin et not1 et le place dans chemin
    Mix_Chunk* note_blanc1 = Mix_LoadWAV(chemin);

    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, not2); // Fusionne chemin et not1 et le place dans chemin
    Mix_Chunk* note_noir2 = Mix_LoadWAV(chemin);

    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, not3);
    Mix_Chunk* note_blanc3 = Mix_LoadWAV(chemin);
    
    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, not4);
    Mix_Chunk* note_noir4 = Mix_LoadWAV(chemin);

    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, not5);
    Mix_Chunk* note_blanc5 = Mix_LoadWAV(chemin);

    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, not6);
    Mix_Chunk* note_blanc6= Mix_LoadWAV(chemin);

    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, not7);
    Mix_Chunk* note_noir7 = Mix_LoadWAV(chemin);

    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, not8);
    Mix_Chunk* note_blanc8 = Mix_LoadWAV(chemin);

    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, not9);
    Mix_Chunk* note_noir9 = Mix_LoadWAV(chemin);

    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, not10);
    Mix_Chunk* note_blanc10 = Mix_LoadWAV(chemin);

    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, not11);
    Mix_Chunk* note_noir11 = Mix_LoadWAV(chemin);

    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, not12);
    Mix_Chunk* note_blanc12 = Mix_LoadWAV(chemin);

     strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, tromp);
    Mix_Chunk* trompette = Mix_LoadWAV(chemin);

    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, beat);
    Mix_Chunk* slowBeat = Mix_LoadWAV(chemin);

    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, saxx);
    Mix_Chunk* Saxo = Mix_LoadWAV(chemin);
    

    /*******************************************************************************************************************************/

    SDL_Event events;
    bool close = false;

    Rect Deadzone(320-DEADZONE_X/2,240-DEADZONE_Y/2,DEADZONE_X,DEADZONE_Y);


  while (1)
  {


      /********************Creation de la fenetre de detection *********************************************************/
      cap.read(frame);
      flip(frame,frame,1);  
      img_transparent=frame.clone();

      cvtColor(frame, hsv, COLOR_BGR2HSV);
      medianBlur(hsv, hsv, blurSize);

      namedWindow("Choisissez la couleur a tracker sur cette fenetre",1);
        
      
    /*******************************Boucle principale**********************************************************************************************/
      /***********************TRACE DU PIANO ****************************************************************************/

            Rect blanc1(x_LeftUP,y_LeftUp,noir_largeur,noir_longueur);
            Rect noir2(x_LeftUP+noir_largeur,y_LeftUp,noir_largeur,noir_longueur/2);
            Rect blanc3(2*noir_largeur+x_LeftUP,y_LeftUp,noir_largeur,noir_longueur);
            Rect noir4(x_LeftUP+3*noir_largeur,y_LeftUp,noir_largeur,noir_longueur/2);
             
            Rect blanc5(x_LeftUP+5*noir_largeur,y_LeftUp,noir_largeur,noir_longueur);
            Rect blanc6(4*noir_largeur+x_LeftUP,y_LeftUp,noir_largeur,noir_longueur);

            Rect noir7(6*noir_largeur+x_LeftUP,y_LeftUp,noir_largeur,noir_longueur/2);
            Rect blanc8(7*noir_largeur+x_LeftUP,y_LeftUp,noir_largeur,noir_longueur);

            Rect noir9(8*noir_largeur+x_LeftUP,y_LeftUp,noir_largeur,noir_longueur/2);
            Rect blanc10(9*noir_largeur+x_LeftUP,y_LeftUp,noir_largeur,noir_longueur);

            Rect noir11(10*noir_largeur+x_LeftUP,y_LeftUp,noir_largeur,noir_longueur/2);
            Rect blanc12(11*noir_largeur+x_LeftUP,y_LeftUp,noir_largeur,noir_longueur);

           
            rectangle(img_transparent,blanc1,Scalar(255,255,255),FILLED);
            rectangle(img_transparent,blanc1,Scalar(0,0,0),1);

            rectangle(img_transparent,noir2,Scalar(0,0,0),FILLED);
            rectangle(img_transparent,noir2,Scalar(0,0,0),1);

            rectangle(img_transparent,blanc3,Scalar(255,255,255),FILLED);
            rectangle(img_transparent,blanc3,Scalar(0,0,0),1);

            rectangle(img_transparent,noir4,Scalar(0,0,0),FILLED);
            rectangle(img_transparent,noir4,Scalar(0,0,0),1);

            rectangle(img_transparent,blanc5,Scalar(255,255,255),FILLED);
            rectangle(img_transparent,blanc5,Scalar(0,0,0),1);

            rectangle(img_transparent,blanc6,Scalar(255,255,255),FILLED);
            rectangle(img_transparent,blanc6,Scalar(0,0,0),1);

            rectangle(img_transparent,noir7,Scalar(0,0,0),FILLED);
            rectangle(img_transparent,noir7,Scalar(0,0,0),1);

             rectangle(img_transparent,blanc8,Scalar(255,255,255),FILLED);
            rectangle(img_transparent,blanc8,Scalar(0,0,0),1);

            rectangle(img_transparent,noir9,Scalar(0,0,0),FILLED);
            rectangle(img_transparent,noir9,Scalar(0,0,0),1);

            rectangle(img_transparent,blanc10,Scalar(255,255,255),FILLED);
            rectangle(img_transparent,blanc10,Scalar(0,0,0),1);

            rectangle(img_transparent,noir11,Scalar(0,0,0),FILLED);
            rectangle(img_transparent,noir11,Scalar(0,0,0),1);

            rectangle(img_transparent,blanc12,Scalar(255,255,255),FILLED);
            rectangle(img_transparent,blanc12,Scalar(0,0,0),1);

            Rect contour(x_LeftUP,y_LeftUp,12*noir_largeur,noir_longueur);
            rectangle(img_transparent,contour,Scalar(20,20,20),3);

           

           // INSTRUMENT ANNEXES

           // SLOW BATTERY : 
           Rect slowDrum( 20, 20, longueur_drum,largeur_drum);
           Rect ContourDrum(15,15,longueur_drum+10,largeur_drum+10);
           rectangle(img_transparent,slowDrum,Scalar(255,0,0),FILLED);
           rectangle(img_transparent,ContourDrum,Scalar(0,255,0),3);

           // Saxo
           Rect saxo(540,20,longueur_drum,largeur_drum);
           Rect contourSaxo(535,15,longueur_drum+10,largeur_drum+10);
           rectangle(img_transparent,saxo,Scalar(255,0,0),FILLED);
           rectangle(img_transparent,contourSaxo,Scalar(0,255,0),3);

           // autre
           Rect trump(280,20,longueur_drum,largeur_drum);
           Rect contourTrump(275,15,longueur_drum+10,largeur_drum+10);
           rectangle(img_transparent,trump,Scalar(255,0,0),FILLED);
           rectangle(img_transparent,contourTrump,Scalar(0,255,0),3);

         
           





      /*************Mise a jour via la souris du pixel tracké****************************************************************/      
      setMouseCallback( "Choisissez la couleur a tracker sur cette fenetre", onMouse, &frame );

      Scalar lower(HsV_frame.hue-epsilon ,HsV_frame.sat-2*epsilon,HsV_frame.val-epsilon);
      Scalar upper(HsV_frame.hue +epsilon ,HsV_frame.sat+2*epsilon,HsV_frame.val+epsilon);

      // on fait le tri

      inRange(hsv,lower,upper,mask);

      /************************************traitement de l'image detectée**************************************/

      erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	    dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	    dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	    erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

      /*********************** DETECTE les CONTOURS ET SUPPRIME LE BRUIT ***************************************/

      vector<vector<Point> > contours;
      vector<Vec4i> hierarchy;
      findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
      size_t largestContour = 0;

      for (size_t i = 1; i < contours.size(); i++)
      {
          if (contourArea(contours[i]) > contourArea(contours[largestContour]))
              largestContour = i;
      }
      
      drawContours(img_transparent, contours, largestContour, Scalar(0, 0, 255), 1); // dessine les contours sur l'image piano retour


    if (!contours.empty())
      {
          vector<vector<Point> > hull(1);
          convexHull(Mat(contours[largestContour]), hull[0], false);
          drawContours(frame, hull, 0, Scalar(0, 255, 0), 3);
          drawContours(img_transparent, hull, 0, Scalar(0, 255, 0), 3);
          if (hull[0].size() > 2)
          {
             vector<int> hullIndexes;
              cv::convexHull(cv::Mat(contours[largestContour]), hullIndexes, true);
              vector<cv::Vec4i> convexityDefects;
              cv::convexityDefects(cv::Mat(contours[largestContour]), hullIndexes, convexityDefects);
              Rect boundingBox = boundingRect(hull[0]);
              
 

/* ******** ****************************************/
              Point center = Point(boundingBox.x + boundingBox.width / 2, boundingBox.y + boundingBox.height / 2); //centre du rectangle permettant le piano
              circle( img_transparent,center, 5,Scalar( 0, 0, 255 ),FILLED,LINE_8 );
/*************************************************/
               if (blanc1.contains(center)  && tab_flags[0] )
                    {
                       rectangle(img_transparent,blanc1,Scalar(50,50,50,10.4),FILLED);    
                       Mix_PlayChannel(0, note_blanc1, 0); // Joue noir1 1 fois sur le canal 1 
                       gestion_flags(tab_flags,touche,1);  
                   }

               else if (noir2.contains(center) && tab_flags[1]){
                  rectangle(img_transparent,noir2,Scalar(100,100,100,10.4),FILLED);
                  Mix_PlayChannel(1, note_noir2, 0);
                  gestion_flags(tab_flags,touche,2);
               
               }
              else if (blanc3.contains(center) && tab_flags[2]){
                  rectangle(img_transparent,blanc3,Scalar(100,100,100,10.4),FILLED);
                  Mix_PlayChannel(2, note_blanc3, 0);
                  gestion_flags(tab_flags,touche,3);
               
             }
              else if (noir4.contains(center) && tab_flags[3]){
                   rectangle(img_transparent,noir4,Scalar(100,100,100,10.4),FILLED);
                  Mix_PlayChannel(3, note_noir4, 0);
                  gestion_flags(tab_flags,touche,4);
             
            }

            else if (blanc5.contains(center) && tab_flags[4]){
              rectangle(img_transparent,blanc5,Scalar(100,100,100,10.4),FILLED);
              Mix_PlayChannel(4, note_blanc5, 0);
              gestion_flags(tab_flags,touche,5);
           
               
               
            }
            else if (blanc6.contains(center) && tab_flags[5]){
              rectangle(img_transparent,blanc6,Scalar(100,100,100,10.4),FILLED);
              Mix_PlayChannel(5, note_blanc6, 0);
              gestion_flags(tab_flags,touche,6);         
               
            }

            else if (noir7.contains(center) && tab_flags[6]){
              rectangle(img_transparent,noir7,Scalar(100,100,100,10.4),FILLED);
              Mix_PlayChannel(6, note_noir7, 0);
              gestion_flags(tab_flags,touche,7);
            }
            else if (blanc8.contains(center) && tab_flags[7]){
              rectangle(img_transparent,blanc8,Scalar(100,100,100,10.4),FILLED);
              Mix_PlayChannel(7, note_blanc8, 0);
              gestion_flags(tab_flags,touche,8);
             
            }
            else if (noir9.contains(center) && tab_flags[8]){
              rectangle(img_transparent,noir9,Scalar(100,100,100,10.4),FILLED);
              Mix_PlayChannel(8, note_noir9, 0);
              gestion_flags(tab_flags,touche,9);
            }
            else if (blanc10.contains(center) && tab_flags[9]){
              rectangle(img_transparent,blanc10,Scalar(225,225,225,10.4),FILLED);
              Mix_PlayChannel(9, note_blanc10, 0);
              gestion_flags(tab_flags,touche,10);
            }
            else if (noir11.contains(center) && tab_flags[10]){
              rectangle(img_transparent,noir11,Scalar(225,225,225,10.4),FILLED);
              Mix_PlayChannel(10, note_noir11, 0);
              gestion_flags(tab_flags,touche,11);
            }
            else if (blanc12.contains(center) && tab_flags[11]){
              rectangle(img_transparent,blanc12,Scalar(225,225,225,10.4),FILLED);
              Mix_PlayChannel(11, note_blanc12, 0);
              gestion_flags(tab_flags,touche,12);
            }

            else if (slowDrum.contains(center)){
                rectangle(img_transparent,slowDrum,Scalar(150,150,150,10.4),FILLED);
                Mix_PlayChannel(12, slowBeat, 0);
            }
            else if (saxo.contains(center)&& tab_flags[12]){
                rectangle(img_transparent,saxo,Scalar(150,150,150,10.4),FILLED);
                Mix_PlayChannel(13, Saxo, 0);
                gestion_flags(tab_flags,touche,13);
            }

             else if (trump.contains(center)){
                rectangle(img_transparent,trump,Scalar(150,150,150,10.4),FILLED);
                Mix_PlayChannel(13, trompette, 0);

            }

            // Instructions pour la sortie de la deadzone : servomoteurs

          	if ((center.x <= 320-DEADZONE_X/2) && flag_servo){//dépassement par la gauche

				      pFile = fopen(path,"w");
              	fprintf(pFile,"%c",'g');
	              fclose (pFile);
				        cout << "/depassement droite" << endl;
            }
           
		
            else if ((center.x>DEADZONE_X) && flag_servo){//dépassement par la droite

			        	

                pFile = fopen(path,"w");
            	fprintf(pFile,"%c",'d'); //commande de suivi de caméra 
	            fclose (pFile);
				      cout << "/depassement gauche" << endl;
            }  
            
          }

      }   


      addWeighted(frame,alpha, img_transparent, 1.0 - alpha , 0.0, img_transparent);  // PERMET LA TRANSPARENCE

      // DEADZONE

      rectangle(img_transparent,Deadzone,Scalar(0,0,200),2);

      putText(img_transparent,"Slow",Point(30,40),FONT_HERSHEY_COMPLEX_SMALL,0.8,Scalar(0,0,0),2);
      putText(img_transparent,"Drum",Point(30,68),FONT_HERSHEY_COMPLEX_SMALL,0.8,Scalar(0,0,0),2);
      putText(img_transparent,"Saxo",Point(550,45),FONT_HERSHEY_COMPLEX_SMALL,0.8,Scalar(0,0,0),2);
      putText(img_transparent,"trumpet",Point(300,40),FONT_HERSHEY_COMPLEX_SMALL,0.8,Scalar(0,0,0),2);

      

      imshow("Choisissez la couleur a tracker sur cette fenetre", frame); // permet de cliquer
      imshow("clone", img_transparent); // permet d'afficher
      
      if (waitKey(1) == 27) break;
  } // Fin du xhile


  /* ------------------ LIBERATION DE LA MEMOIRE ************************************/ 

  Mix_FreeChunk(note_blanc1);
  Mix_FreeChunk(note_noir2);
  Mix_FreeChunk(note_blanc3);
  Mix_FreeChunk(note_noir4);
  Mix_FreeChunk(note_blanc5);
  Mix_FreeChunk(note_blanc6);
  Mix_FreeChunk(note_noir7);
  Mix_FreeChunk(note_blanc8);
  Mix_FreeChunk(note_noir9);
  Mix_FreeChunk(note_blanc10);
  Mix_FreeChunk(note_noir11);
  Mix_FreeChunk(note_blanc12);
  Mix_FreeChunk(Saxo);
  Mix_FreeChunk(slowBeat);
   Mix_FreeChunk(trompette);


  return 0;
}










/********************JEU PARTIE JEU VIDEO ****************************************************************************/





void choix_menu(int event, int x, int y, int f, void* param){
	/*!
	* @brief Fonction codant les boutons cliquables dans le menu du programme
      * \param x coordonnées x souris
     * \param y coordonnées y souris
     * \param event evenement declencheur de la fonction
     * \param param parametre modifié par la fonction
  *
  *\return aucun
	*/

	if (event == EVENT_LBUTTONDOWN){//si un clic a eu lieu sur la fenêtre
		if(bouton1.contains(Point(x,y))){//on a cliqué sur le mode libre
			cout<<"MODE FREESTYLE"<<endl;//affichage
			*(int*)param=1;//on modifie le contenu du pointeur de retour
		}
		if(bouton2.contains(Point(x,y))){//on a cliqué sur le bouton jeu
			cout<<"MODE JEU"<<endl;
			*(int*)param=2;
		}
	}
	imshow(menu,fond);
}







void choix_game_over(int event, int x, int y, int f, void* param){
	/*!
	* @brief Fonction codant les boutons cliquables dans l'écran de game over du mode jeu
  *   \param x coordonnées x souris
     * \param y coordonnées y souris
     * \param event evenement declencheur de la fonction
     * \param param parametre modifié par la fonction
	*/

	if (event == EVENT_LBUTTONDOWN){//si un clic a eu lieu sur la fenêtre
		if(bouton1.contains(Point(x,y))){//on a cliqué sur le retour au menu
			cout<<"MENU"<<endl;//affichage
			*(int*)param=1;//on modifie le contenu du pointeur de retour
		}
		if(bouton2.contains(Point(x,y))){//on a cliqué sur quitter
			cout<<"QUITTER"<<endl;
			*(int*)param=2;
		}
	}
	imshow(game_over,fond);
}

	








int startmenu(){
	/*!
	* @brief Fonction codant le menu de démarrage du programme
  *\return aucun
	*/

	namedWindow(menu,1);//on crée la fenêtre du menu
  char back[20]="piano.png";
  char chemin[200];


  strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
  strcat(chemin, back); // Fusionne chemin et not1 et le place dans chemin
	fond = imread(chemin);//l'image à afficher a une taille de 856*624px
	
	bouton1=Rect(20,550,200,50);//définit un rectangle bouton
	fond(bouton1)=Vec3b(100,100,100);//et le trace dans le menu
	putText(fond(bouton1),"FREESTYLE",Point(20,40),FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0));//affiche un texte sur le bouton	
	
	bouton2=Rect(636,550,200,50);//idem pour un deuxieme bouton
	fond(bouton2)=Vec3b(0,100,0);
	putText(fond(bouton2),"PLAY GAME",Point(20,40),FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0));
	
	imshow(menu,fond);//affiche le menu
	
	int flag=0;//variable indiquant quel bouton a été cliqué
	while(flag==0){//rien ne se passe tant qu'on a pas cliqué sur un bouton		
		if (waitKey(5) == 27){ //BREAK POUR SORTIR DE LA BOUCLE (echap)
       		cout<<"VOUS AVEZ QUITTE LE PROGRAMME"<<endl;
       		return 3;
    	}    
		setMouseCallback(menu,choix_menu,&flag);//rends les boutons cliquables
	}
	destroyWindow(menu);//on ferme le menu
	return flag;//retourne le mode de jeu sélectionné
}












int gameover(){
	/*!
	* @brief Fonction codant l'écran de game over du mode jeu
  * \return aucun
	*/
	
	namedWindow(game_over,1);//crée une fenêtre
  char GO[20]="gameover.jpg";
  char chemin[200];


  strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
  strcat(chemin, GO); // Fusionne chemin et not1 et le place dans chemin
	fond=imread(chemin);//affiche une image
	
	bouton1=Rect(20,500,200,50);//définit un rectangle bouton
	rectangle(fond,bouton1,Scalar(255,255,255),2);//le trace sur l'écran 
	putText(fond(bouton1),"MENU",Point(20,40),FONT_HERSHEY_SIMPLEX, 1, Scalar(255,255,255));//affiche un texte sur le bouton
	
	
	bouton2=Rect(636,500,200,50);//idem pour un deuxieme bouton
	rectangle(fond,bouton2,Scalar(255,255,255),2);
	putText(fond(bouton2),"QUITTER",Point(20,40),FONT_HERSHEY_SIMPLEX, 1, Scalar(255,255,255));
	imshow(game_over,fond);//affiche le menu
	int flag=0;//variable indiquant quel bouton a été cliqué
	while(flag==0){//rien ne se passe tant qu'on a pas cliqué sur un bouton		
		if (waitKey(5) == 27){ //BREAK POUR SORTIR DE LA BOUCLE (echap)
       		break;
    	}    
		setMouseCallback(game_over,choix_game_over,&flag);//rends les boutons cliquables
	}
	destroyWindow(game_over);//on ferme le game over
	return flag;//retourne le bouton sélectionné
}










//---------------------------------------------------------------------------------------------------------------------

//EVENEMENTS DU MODE JEU

void note(int* note){
	/*!
	* @brief Fonction choisissant d'activer une note parmi 4 possibilité et renvoyant sa valeur
  *\param note pointeur vers le bouton à activer
  *\return aucun
	*/


	*note=(rand()%4)+1;//on active une note aléatoire et modifie le pointeur de retour
}

void moteurs(int* pos){
	/*!
	* @brief Fonction choisissant d'activer le moteur horizontal dans un sens ou dans l'autres
  *\param pos pointeur indiquant la position actuelle du moteur (3 possibles)
  *\return aucun
	*/
	
	
	
	int a=rand()%2;//nombre aléatoire pour aller à gauche ou a droite
	if((a==0)&&(*pos>-1)){//on laisse le moteur se déplacer entre 3 emplacements (-1,0,1)
		*pos=*pos-1;//décrémente la position
		for(int b=0;b<30;b++){//on envoie en boucle une consigne d'incrémentation au moteur
			pFile = fopen(path,"w");
            fprintf(pFile,"%c",'g');
	        fclose (pFile);
		}
	}
	if((a==1)&&(*pos<1)){//idem dans l'autre sens
		*pos=*pos+1;
		for(int b=0;b<30;b++){
			pFile = fopen(path,"w");
            fprintf(pFile,"%c",'d');
	        fclose (pFile);
		}
	}	
}



//---------------------------------------------------------------------------------------------------------------------

//MODE JEU 


int piano_tiles(){//mode jeu vidéo
	/*!
	* @brief Fonction centrale du mode jeu vidéo. Le joueur doit activer des notes en déplacant un objet de couleur détecté par la caméra pour activer à temps des notes et faire grimper son score. La difficulté est croissante, le jeu s'arrête une fois les vies épuisées.
	* \param Aucun
  * \return Un entier : -1 si probleme d'initiasisation
  * 1 si game over
  * 0 si jeu quitté avec echap
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


   SDL_Window* pWindow{ nullptr };     
    SDL_Renderer* pRenderer{ nullptr };  


    pWindow = SDL_CreateWindow("SDL martigues", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 100, 75, SDL_WINDOW_SHOWN);   
    
    
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
    
    /*******************************COnverti Mat en SDL*************************************************************************************************/
    
    
     Mix_AllocateChannels(2); 

     // Mets le son a 100% en volume pour les premier cannaux
    
      
    Mix_Volume(0, MIX_MAX_VOLUME );  //son du jeu
    Mix_Volume(1, MIX_MAX_VOLUME /5);  // background son
    

char over[20]="game over.wav";
char joie[20]="point marque.wav";
char strt[20]="start jeu.wav";
char echec[20]="rate.wav";
char SI[20]="superidol.wav";

char chemin[200];


    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, over); // Fusionne chemin et not1 et le place dans chemin
    Mix_Chunk* gameO = Mix_LoadWAV(chemin);

    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, joie); // Fusionne chemin et not1 et le place dans chemin
    Mix_Chunk* success = Mix_LoadWAV(chemin);

    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, echec); //
    Mix_Chunk* fail = Mix_LoadWAV(chemin);

    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, strt); //
    Mix_Chunk* begin = Mix_LoadWAV(chemin);

    strcpy(chemin,chemin_acces); // copie ce qu'il y a dans chemin d'acces (Glob) dans chemin( local)
    strcat(chemin, SI); //
    Mix_Chunk* superIdol = Mix_LoadWAV(chemin);
  // le son background sera sur le channel 1

	Mat HSV_from_frame,mask;//création des matrices servant à la capture d'image
	pixel_frame.bl=0;//variables dans lesquelles seront stockées la couleur à suivre
	pixel_frame.gre=0;
	pixel_frame.re=0;
	HsV_frame.hue=0;
	HsV_frame.sat=0;
	HsV_frame.val=0;

	int centrey;
	int centrex;

    VideoCapture cap(ID);//lancement de la capture vidéo
    if (cap.isOpened() == false)//si echec du lancement de la capture  
        {
            cout << "changez le N° de port de camera svp" << endl;
            cin.get(); //on attends qu'une touche soit pressée pour quitter le programme
            return 0;//on quitte la fonction
        }      
   
    pFile = fopen(path,"w");//on définit le fichier texte servant au transfert de données par le port série		
	if(pFile==NULL){//si échec de création du fichier
          printf("erreur de port : les servomoteurs ne marcheront pas\n");
          return 0;
    }
    
    namedWindow("Original",1);// Creation d'une fenêtre affichée à l'écran
    
    
    
    
    
   //Déclarations des variables de jeu 
   //on utilise les flags et les timers pour ne pas avoir à mettre en pause l'exécution du jeu avec des delays;
    
    int etat_jeu=0;//variable indiquant si le jeu est lancé ou non
    int score=0;//score du joueur
    int score_aux=0;//sert à faire évoluer la difficulté avec le score
    clock_t start=clock();//variable servant à la temporisation des évènements
    int difficulte=1;//la difficulté code la fréquence des évènements
    int flag_note=0;//indique quelle note est active dans le jeu, 0=aucune, 1=haut, 2=bas,3=gauche,4=droite
    clock_t timer_note;//code le temps disponible pour aller sur un note active
    int flag_dead=0;//indique si une note a été ratée par le joueur
    clock_t timer_fin_note;//code la durée des évènements se passant après l'activation d'une note (échec ou réussite)
    int flag_valide=0;//indique si une note est validée par l'utilisateur
    int vies=VIES;//!nombre de vies du joueur
    int pos_moteur=0;//position de la caméra
    

    
    //NOTE VIVI lancer super idol
    Mix_PlayChannel(1, superIdol, 0);
    
    while(etat_jeu==0){//boucle centrale du jeu, le flux caméra est capturé à chaque tour
    
    	if (waitKey(5) == 27){ //sortie de secours (echap)
    		cout<<"fermeture du jeu"<<endl;
        	break;
     	}
    
    


    
		// 1) DETECTION ET SUIVI DE COULEUR
		//--------------------------------------------------------------------------------------------------------------------------
    	
    	cap.read(src);    // on lit le fichier camera       
        cvtColor(src, HSV_from_frame,COLOR_BGR2HSV);//on le convertit en HSV    
        setMouseCallback( "Original",capture_couleur, &src );//sélection d'une couleur à suivre en cliquant sur la fenêtre de jeu
        

        string rgbText = "BGR = (" + std::to_string(pixel_frame.bl) + ", " + std::to_string(pixel_frame.gre)//affichage des valeurs cliquées dans le terminal
         + ", " + std::to_string(pixel_frame.re) + ")";
        string hsvText = "HSV = (" + std::to_string(HsV_frame.hue) + ", " + std::to_string(HsV_frame.sat)
        + ", " + std::to_string(HsV_frame.val) + ")";

        // Affichage sur 2 fenetres de la couleur souhaitée : HSV ET BGR
        Mat translat_bgr(200,200,CV_8UC3,Scalar(pixel_frame.bl,pixel_frame.gre,pixel_frame.re)); // BGR : cree un matrice de pixels : image
        putText(translat_bgr,rgbText,Point(0,50),FONT_HERSHEY_COMPLEX_SMALL,0.7,Scalar(0,0,0),2);

        Mat translat_hsv(200,200,CV_8UC3,Scalar(HsV_frame.hue,HsV_frame.sat,HsV_frame.val)); // HSV : cree un matrice de pixels : image
        putText(translat_hsv,hsvText,Point(0,50),FONT_HERSHEY_COMPLEX_SMALL,0.7,Scalar(0,0,0),2);
     
        int eps=TOLERANCE; // tolérance hsv sur la detection de couleur
        // Construction des valeurs considerees comme detectables a partir des variables globales, modifiées depuis le callback
        Scalar lower(HsV_frame.hue-eps ,HsV_frame.sat-2*eps,HsV_frame.val-eps);
        Scalar upper(HsV_frame.hue +eps ,HsV_frame.sat+2*eps,HsV_frame.val+eps);

        // on fait le tri
        inRange(HSV_from_frame,lower,upper,mask);
          
        // on filtre le bruit
    erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		
        //trouve le centre de l'object detecté;				
		int sommex=0;
		int sommey=0;
		int somme=1;
		for(int i=0;i<TAILLEY;i++){
			for(int j=0;j<TAILLEX;j++){
				if(mask.at<uchar>(i,j)){//on dÃ©tecte un pixel blanc
					//cout << "flag" << endl;
					sommex=sommex+j;
					sommey=sommey+i;
					somme=somme+1;
				}
			}
		}

          
        if(somme>1000){ // on définit un centre que si suffisamment de points sont détectés (évite le bruit)
		     centrey=(sommey/somme);//le centre est la moyenne des coordonnées
		     centrex=(sommex/somme);//horizontales et verticales des points 		
		     Point pt1(centrex,centrey);//point central de l'objet détecté
		     circle(src,pt1,20,Scalar(pixel_frame.bl,pixel_frame.gre,pixel_frame.re),5);//dessine un cercle au centre de l'objet détecté
        }
        else {
        	somme=1; // 0 pixels de la couleur détecté
        }
        
        //dessin des éléments du jeu
        Scalar jaune=Scalar(0,255,255);//codes BGR de couleurs
        Scalar vert=Scalar(0,255,0);
        Scalar rouge=Scalar(0,0,255);
        Scalar noir=Scalar(0,0,0);
        
        //définition des boutons du jeu
        Rect boutonhaut=Rect(TAILLEX/2-100,40,200,50);
        Rect boutonbas=Rect(TAILLEX/2-100,TAILLEY-90,200,50);
        Rect boutongauche=Rect(40,TAILLEY/2-100,50,200);
        Rect boutondroite=Rect(TAILLEX-90,TAILLEY/2-100,50,200);        
       
		//on dessine tout les boutons vide à contours noirs de base
		rectangle(src,boutonhaut,noir,3);
	    rectangle(src,boutonbas,noir,3);
        rectangle(src,boutongauche,noir,3);
        rectangle(src,boutondroite,noir,3);
        
        switch(flag_note){//on affiche un éventuel bouton actif en jaune
        	case 1:rectangle(src,boutonhaut,jaune,-1);break;
        	case 2:rectangle(src,boutonbas,jaune,-1);break;
        	case 3:rectangle(src,boutongauche,jaune,-1);break;
        	case 4:rectangle(src,boutondroite,jaune,-1);break;
        }		
    	switch(flag_dead){//on affiche un éventuel bouton raté en rouge
        	case 1:rectangle(src,boutonhaut,rouge,-1);break;
        	case 2:rectangle(src,boutonbas,rouge,-1);break;
        	case 3:rectangle(src,boutongauche,rouge,-1);break;
        	case 4:rectangle(src,boutondroite,rouge,-1);break;
        }
    	switch(flag_valide){//on affiche un éventuel bouton validé en vert
    		case 0:break;
        	case 1:rectangle(src,boutonhaut,vert,-1);break;
        	case 2:rectangle(src,boutonbas,vert,-1);break;
        	case 3:rectangle(src,boutongauche,vert,-1);break;
        	case 4:rectangle(src,boutondroite,vert,-1);break;
        }
        
        
        flip(src,src,1);//affiche un symétrique de l'image pour que la gauche et la droite correspondent au mouvements 
        
        //affichage du score et des vies restantes du joueur
        char viestxt[30];
        char scoretxt[30];
        sprintf(viestxt,"%d",vies);
        sprintf(scoretxt, "%d", score);   
        Rect lifeboard=Rect(TAILLEY-40,0,200,50);//rectangle d'affichage des vies	    
        Rect scoreboard=Rect(0,0,200,50);//rectangle d'affichage du score
        rectangle(src,scoreboard,Scalar(250,250,250),-1);
        rectangle(src,lifeboard,Scalar(250,250,250),-1);
        putText(src(scoreboard),"score :",Point(0,40),FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0));//affichage dans les rectangles
        putText(src(scoreboard),scoretxt,Point(130,40),FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0));
		putText(src(lifeboard),viestxt,Point(20,40),FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0));
		putText(src(lifeboard),"vies",Point(70,40),FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0));
		
        imshow("Original",src);//affichage des images dans les fenetres
        //imshow("filtre",mask);//possibilité d'afficher le filtre de détection
		
		//---------------------------------------------------------------------------------------------------------------------------------


		//2) CODE DU JEU ET EVENEMENTS
		clock_t instant=clock();//mesure du temps actuel
		long int duree_event=(instant-start)/TEMPO;//mesure du temps écoulé depuis le dernier évènement (1 tick=4us);
		if((duree_event*difficulte>15)&&(flag_note==0)&&(flag_dead==0)&&(flag_valide==0)){//un évènement se déclenche
			start=clock();//mise à jour des timers
			int hasard=rand()%4;//une chance sur 4 de déclencher les moteurs
			//cout<<hasard<<endl;
			if(hasard==0){
				cout<<"moteurs"<<endl;
				moteurs(&pos_moteur);//on active un moteur
				
			}
			else{//on active un bouton 
				cout<<"note"<<endl;
				note(&flag_note);//fonction d'activation d'un bouton
				
				timer_note=clock();//lance le chrono pour aller sur le bouton
			}
		}
		long int duree_note=(instant-timer_note)/TEMPO;//mesure le temps écoulé depuis la dernière activation d'un bouton
		
		
		//si un bouton est active
		if(flag_note!=0){
		
			//on détecte si l'utilisateur entre dans une case
			if(boutonhaut.contains(Point(centrex,centrey))){
				flag_valide=1;//mise à jour d'un flag
				
			}
			if(boutonbas.contains(Point(centrex,centrey))){
				flag_valide=2;
				
			}
			if(boutongauche.contains(Point(centrex,centrey))){
				flag_valide=3;
				
			}
			if(boutondroite.contains(Point(centrex,centrey))){
				flag_valide=4;
				
			}
			
		
			//on vérifie si le bouton actif est validé ou raté
			if(flag_valide==flag_note){//on a validé le bon bouton
				timer_fin_note=clock();//on lance le timer de fin d'évènement
				flag_note=0;//le bouton n'est plus actif
				cout<<"Validé"<<endl;
				score=score+20;
				Mix_PlayChannel(0, success, 0);
			}
			if(duree_note>5-difficulte){//on a dépassé le délais pour aller sur le bouton
				flag_dead=flag_note;//on indique quel bouton a été raté
				timer_fin_note=clock();//on lance le timer de fin d'évènement
				flag_note=0;//le bouton n'est plus actif
				cout<<"Raté!"<<endl;
				vies=vies-1;
				 Mix_PlayChannel(0, fail, 0);
			}
			
		}
		
		long int duree_fin=(instant-timer_fin_note)/TEMPO;//mesure le temps écoulé depuis la fin d'un événement
		if(duree_fin>5-difficulte){//on remet les flags d'événements à 0 un temps après leur fin
			flag_dead=0;
			flag_valide=0;
		}
		
		if((score>score_aux+100)&&(difficulte<4)){//on augmente la difficulté tout les 100pts de score
			difficulte++;
			score_aux=score;
			cout<<"La difficulte augmente ! Niveau :  "<<difficulte<<endl;
		}
		
		if(vies<1){//si le joueur n'a plus de vies
			cout<<"GAME OVER ! VOTRE SCORE EST : "<<score<<endl;
			etat_jeu=1;//condition de sortie de la boucle while
		}			
			
			
		
	}
	destroyWindow("Original");//on ferme la fenêtre de jeu
	
	//retour à la position initiale des moteurs	
	if(pos_moteur==1){//si le moteur est à droite
		for(int b=0;b<30;b++){//on envoie en boucle une consigne d'incrémentation au moteur
			pFile = fopen(path,"w");
            fprintf(pFile,"%c",'g');//gauche
	        fclose (pFile);
		}
	}
	if(pos_moteur==-1){//si le moteur est à gauche
		for(int b=0;b<30;b++){//on envoie en boucle une consigne d'incrémentation au moteur
			pFile = fopen(path,"w");
            fprintf(pFile,"%c",'d');//droite
	        fclose (pFile);
		}
	}	
    Mix_FreeChunk(gameO);
    Mix_FreeChunk(success);
    Mix_FreeChunk(fail);
    Mix_FreeChunk(begin);

	return etat_jeu;//indique si le jeu a été quitté ou si il a eu game_over
}















