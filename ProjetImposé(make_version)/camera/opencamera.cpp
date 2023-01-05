/**
 * @file opencamera.cpp
 * @author Victoria Nguyen et Matthieu Sysouvanh
 * @brief  Permet le tracking d'une couleur choisie via un click souris sur l'image. 
 * @version 1
 * @date 2022-06-08
 * 
 
 * 
 */




#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;


// Victoria 28 mai 2022

// code permettant de recuperer 
// les coordonnées du pixel sur lequel se situe la souris
// ses valeurs BGR
// ses valeurs HSV
// puis detecte les parties de l'image de cette meme couleur

//Matthieu 2juin 2022
//amélioration de l'interface graphique
//débuggage du contrôle moteurs


// taille de rectangle qui suivra l'objet
#define TAILLEX 640
#define TAILLEY 480
#define ID 2 // camera port
#define TOLERANCE 30 //tolérance sur le filtre de couleur
#define DEADZONE 50// longueur du carré où l'objet détecté n'entraine pas de mouvemement de caméra

FILE* pFile; 

// Variable GLOBALES : on declare 2 structures pour contenir les BGR ET HSV en sortie de callback
struct pixel {

int bl;
int gre;
int re;
 
 } pixel_frame;
struct HsV {

int hue;
int sat;
int val;
 
 } HsV_frame;
 
 


const char*  path ="/dev/ttyACM0";//chemin menant au port série de la carte




char window_name[30] = "Original";
Mat src;
void onMouse( int event, int x, int y, int f, void* param){//fonction retournant la valeur HSV cliquée sur l'image source

    /**
     * @brief Fonction callback : appelée a chaque evenemet de la souris. Place les valeurs GRB et HSV du pixel a la position
     * (x,y) dans des variables globales, declarées dans des structures.
     * 
     * \param (x,y) de la souris
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


     cout<< "(B,G,R)=" << pixel_frame.bl <<", "<< pixel_frame.gre <<", "<< pixel_frame.re <<endl;
     cout<< "(H,S,V)=" << HsV_frame.hue<<", "<< HsV_frame.sat<<", "<< HsV_frame.val <<endl;

     }
}









int main(){
    /**
     * @brief  Fonction principale qui lance le tracking de couleur
     * 
     * \param   Aucun paramètre
     * 
     * \return un entier : -1 si une des initialisation n'a pas marché , 0 si l'utilisateur a fermé la fenetre
     * 
     */






  
   CascadeClassifier faceDetector;//Declaring an object named 'face detector' of CascadeClassifier class//
   faceDetector.load("/home/victoria/Bureau/haarcascade_frontalface_alt.xml");//loading the XML trained classifier in the object//
   vector<Rect>faces;//Declaring a rectangular vector named faces//
   


   
    Mat HSV_from_frame,mask;

	pixel_frame.bl=0;
	pixel_frame.gre=0;
	pixel_frame.re=0;

	HsV_frame.hue=0;
	HsV_frame.sat=0;
	HsV_frame.val=0;


	int centrey;
	int centrex;

     VideoCapture cap(ID);

     if (cap.isOpened() == false)  
        {
            cout << "changez le N° de port de camera svp" << endl;
            cin.get(); //on attends qu'une touche soit pressée pour quitter le programme
            return -1;
        }      
    namedWindow("Original",1);// Creation d'une fenêtre affichée à l'écran 
   
    pFile = fopen(path,"w");//on définit le fichier texte servant au transfert de données par le port série		
	if(pFile==NULL){
          printf("erreur de port : les servomoteurs ne marcheront pas");
     }
     
     
     while(1){

     	/*---------------------------------------------**********************************************-------*/
        /*---------------------------------------------**********************************************-------*/
        // Partie qui recupere les BRR et HSV de la ou on a cliqué et les stocke dans les structures globales
        cap.read(src);    // on lit le fichier camera       
        cvtColor(src, HSV_from_frame,COLOR_BGR2HSV);
          
        // On lie la fonction des que la souris bouge : met a jours les valeurs du pixel qu'on veut detecter
        setMouseCallback( "Original", onMouse, &src );


        string rgbText = "BGR = (" + std::to_string(pixel_frame.bl) + ", " + std::to_string(pixel_frame.gre)
         + ", " + std::to_string(pixel_frame.re) + ")";

        string hsvText = "HSV = (" + std::to_string(HsV_frame.hue) + ", " + std::to_string(HsV_frame.sat)
        + ", " + std::to_string(HsV_frame.val) + ")";

        // Afffichage sur 2 fenetres de la couleur souhaitée : HSV ET BGR
        Mat translat_bgr(200,200,CV_8UC3,Scalar(pixel_frame.bl,pixel_frame.gre,pixel_frame.re)); // BGR : cree un matrice de pixels : image
        putText(translat_bgr,rgbText,Point(0,50),FONT_HERSHEY_COMPLEX_SMALL,0.7,Scalar(0,0,0),2);

        Mat translat_hsv(200,200,CV_8UC3,Scalar(HsV_frame.hue,HsV_frame.sat,HsV_frame.val)); // BGR : cree un matrice de pixels : image
        putText(translat_hsv,hsvText,Point(0,50),FONT_HERSHEY_COMPLEX_SMALL,0.7,Scalar(0,0,0),2);
        /*---------------------------------------------**********************************************-------*/
        /*---------------------------------------------**********************************************-------*/


     
        int epsilon=TOLERANCE; // variation des HSV min et max pour la detection
        // Construction des valeurs consideres comme detectables a partir des variables globales, modifiées depuis le callback
        Scalar lower(HsV_frame.hue-epsilon ,HsV_frame.sat-2*epsilon,HsV_frame.val-epsilon);
        Scalar upper(HsV_frame.hue +epsilon ,HsV_frame.sat+2*epsilon,HsV_frame.val+epsilon);

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
          //printf("aire des pixels blancs = %d\n",somme);
          
          if(somme>1000){ // rectange que si il y a assez a detecté
		     centrey=(sommey/somme);//le centre est Ã  la moyenne des coordonnÃ©es
		     centrex=(sommex/somme);//horizontales et verticales des points 		
		     Point pt1(centrex-50,centrey+50);
		     Point pt2((centrex+50),centrey-50);		
		     rectangle(src,pt1,pt2,Scalar(pixel_frame.bl,pixel_frame.gre,pixel_frame.re),8);//dessine un carré autour de la détection, de la couleur détectée
          }
          else {
          	somme=1; // 0 pixels de la couleur détécté
          }

          imshow("Original",src);//affichage des imaages dans les fenetres
          imshow("filtre",mask);











          // CONTROL SERVO
          int centre_image_x=TAILLEX/2;
          int centre_image_y=TAILLEY/2;
          
          if(somme>300){//assez de points on été détectés pour faire un moyenne correcte
          	if (centrex<centre_image_x-DEADZONE){//dépassement par la gauche

				pFile = fopen(path,"w");
            	fprintf(pFile,"%c",'d'); //commande de suivi de caméra 
	            fclose (pFile);
				cout << "/depassement gauche" << endl;
            }
           
		
            if (centrex>centre_image_x+DEADZONE){//dépassement par la droite

				pFile = fopen(path,"w");
            	fprintf(pFile,"%c",'g');
	            fclose (pFile);
				cout << "/depassement droite" << endl;
            }  
            if (centrey>centre_image_y+DEADZONE){//dépassement par le bas

				pFile = fopen(path,"w");
            	fprintf(pFile,"%c",'b');
	            fclose (pFile);
				cout << "/depassement bas" << endl;
            } 
            if (centrey<centre_image_y-DEADZONE){//dépassement par le haut

				pFile = fopen(path,"w");
            	fprintf(pFile,"%c",'h');
	            fclose (pFile);
				cout << "/depassement haut" << endl;
            }
                   
          }

          
          
        
         
          if (waitKey(5) == 27){ //BREAK POUR SORTIR DE LA BOUCLE (echap)
        		 break;
     		 }
     }
}
            
    

 
 


