# ProjetC_NGUYEN_SYSOUVANH
## ProjetC ROB3

### Notice d'utilisation 


Ceci est notre dépot pour le projet C de ROB3, 2021-2022. Ce projet a été realisé en binôme : Victoria Nguyen et Matthieu Sysouvanh

### Cahier des charges :

Implementation d'une machine virtuelle, dans l'esprit de Pixar. Se base sur la libraire OpenCV pour vision par ordinateur et une caméra webcam, en mouvement via 2 servomoteurs ( 2 degrés de liberté).


**Contient deux dossiers :**

1. Projet Imposé : Suivi de couleur
2. Projet Libre : implementation d'instruments virtuels + jeu similaire a un piano Tiles, basé sur les servomoteurs et le mouvement d'un objet tracké par sa couleur

Contient un fichier doc, contenant la documentation html relié a chacun des 2 dossiers pour chaque programme.




## Pré Requis et tâches à réaliser pour que le projet fonctionne : 
 
 - avoir la carte Arch Pro Linux + deux servomoteurs + une webcam reliée aux servomoteurs.
 - Etre dans un environnement sans trop de lumière, ne pas être à contre jour.
 - Avoir la dernière version d'OPEN CV, disponible via ce lien : https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html
 - Pour que les servomoteurs fonctionnent, brancher la carte MBED et mettre le fichier binaire situé dans le dossier _fichier_binaire_ dans la carte. Attendez que la LED clignote, signe que le code est bien mis dans la carte.
 - Pour le projet Libre : il est nécessaire d'installer la librairie SDL2 , à telecharger via ce lien :
 https://devopssec.fr/article/telechargement-compilation-projet-sdl#begin-article-section
 ainsi que la bibliotheque SDL_Mixer, afin de pouvoir avoir le son. Ces librairies doivent être installées dans le fichier path de l'environnement utilisateur pour que le makefile fonctionne.
 SDL_mixer s'installe en suivant les commandes https://zoomadmin.com/HowToInstall/UbuntuPackage/libsdl2-mixer-2.0-0
 
 Remarque : en cas de non reconnaissance des fichiers SDL2 et SDL_Mixer, modifier le `#include` dans les entêtes des programmes `#include` pour que le compilateur ait le bon chemin. 
 Mettre le fichier `SDL_Mixer.h` dans le même dossier que les .h de SDL2.
 
 
 
**A la fin du README se trouve un lien vidéo permettant d'avoir une démonstration visuelle du projet**

 
# PROJET IMPOSE :
 
 **Pour le lancer, ouvrir un terminal dans l'emplacement du fichier .../projetImpose , puis lancer `make` pour compiler, puis `./main` pur executer.**
 **L'éxécutable est le fichier main**. 
 
 
 Celui_ci s'organise en un `fichier.cpp` et un makeFile.   Pour detecter une couleur, il faut cliquer sur l'image affichée sur l'écran. L'endroit (=pixel) où l'on clique
 sera tracké par le programme. Si, lors du tracking, on se deplace dans des points où la luminosité change, il faut re-appuyer sur l'endroit de l'écran de la couleur trackée.
 
 Si on part trop à droite, alors la caméra, via les servomoteurs, se deplacera pour suivre l'objet tracké.
 
 **A retenir** : il est important, pour que l'algorithme sache quelle couleur tracker, de cliquer sur l'image l'object à tracker. Par exemple, si on veut tracker un ballon rouge, il 
 faut au demarrage :
 
 1. placer le ballon dans le champ de vision de la caméra
 2. cliquer sur ballon dans le retour image
 3. un Rectangle de la couleur du pixel sur lequel on a cliqué s'affiche et entoure l'objet tracké de la couleur trackée
 4. Bouger l'objet : le rectangle se deplace pour suivre l'image
 Si, lors du déplacement, le rectangle ne suit plus le ballon, c'est que la luminosité a changé depuis le moment où l'on a cliqué. Il faut alors reclique sur la position courante
 du ballon , ect...
 
 

 
# PROJET Libre
 
 - Lorsque le programme se lance, il y a une fenetre graphique : c'est le menu du jeu. On a le choix entre un mode manuel (=freeestyle) et un mode jeu vidéo. 

 
 
## MODE Freestyle :
 
Lors du lancement du programme, une fenetre liée a la webcam s'affiche. En arrière plan, on voit un piano à 12 touches, ainsi que 3 cases jouant un son particulier. Pour commencer , il faut dans l'ordre :

1. Prendre un objet avec une couleur peu présente dans l'environnement extérieur, afin de limiter les problèmes de detection. Dans l'exemple, on utilise un feutre avec un capuchon rouge. Utiliser un objet de couleur X, possedant une surface petite, afin d'etre précis. Un stylo convient parfaitement. 

2. Deux fenetres s'afficheront à l'éxécution : une fenêtre est le rendu avec les instruments et la detection, l'autre est la fenetre ou il faut choisir la couleur a tracker. Cela se base sur le me principe que le projet imposé ; on clique avec la souris sur la couleur à detecter. Un contour vert s'affiche alors selon le contour de la forme de couleur detectée. N'hesiter pas à cliquer plusieurs fois au même endroit, en changeant l'orientation, afin que le contour vert n'entoure bien uniquement que l'objet de couleur trackée. (sur la photo, uniquement le bouchon du stylo).

3. une fois le choix de la couleur faite, on peut se balader dans l'écran via le centre du contour crée ; dès lors que ce centre passe dans l'une des 12 touches du piano, ou dans l'un des 3 cases de couleur, le son correspondant est joué. Le rythme est similaire pour chaque son, permettant un rendu sonore agréable. Certains sons sont courts (une note), d'autre sont plus longs ( batterie).

4. Un rectangle rouge entoure les instruments virtuels : si le centre de l'objet tracké sort par la gauche où par la droite, alors le servomoteur bougera en conséquence, afin de rester aligné sur l'utilisateur.

# Important ! 
**Pour que les sons fonctionnent, il est nécessaire de modifier les chemins d'accès de chaque son : Pour cela , ouvrir le fichier `play.cpp`  et modifier la ligne 91 avec le path absolu pour arriver jusqu'au dossier. Ce détail est très important pour qu'il y ait lecture des fichiers sons !!**


### AVANT :
 `char chemin_acces[200]="/home/victoria/Bureau/ProjetROB3/ProjetC_NGUYEN_SYSOUVANH/projetLibre/samples piano/";`

### APRES 
 `char chemin_acces[200]=".....A REMPLACER...../ProjetC_NGUYEN_SYSOUVANH/projetLibre/samples piano/";`







 
### MODE JEU VIDEO
 
En lançant ce mode la fênetre de menu se ferme et est remplacée par la fenêtre de jeu. Celle ci comporte boutons ainsi qu'un compteur de score et de vies. 

Cliquez dans un premier temps sur l'objet de couleur à suivre dans le retour caméra. Un éclairage uniforme sans reflets ni contrejour est préférable. Vous pouvez à tout moment re-cliquer sur l'écran pour réinitialiser la couleur à détecter.

Une fois l'objet sélectionné un cercle apparait en son centre. Le but du jeu est de s'en servir comme curseur pour activer les boutons qui passent au jaune.

Si vous y arrivez à temps vous marquerez des points.
Si vous n'y arrivez par vous perdrez une vie.

La caméra pourra bouger aléatoirement entre 3 positions, vous devrez alors vous déplacer pour rester dans le cadre et garder le rythme.

La difficulté augmente graduellement avec le score, les événements s'enchainent plus vite et vous avez moins de temps pour appuyer sur les boutons.

Le jeu s'arrête une fois toutes les vies épuisées ou lors d'un appui sur `echap`.


#### PARAMETRES:

Plusieurs variables globales gèrent des paramètres adaptables du jeu. 
La temporisation des évènements dépends du nombre de ticks/s de votre sytème, aussi la vitesse du jeu est proportionnelle au #define TEMPO

Le nombre de vies est géré par `#define VIES`

**Important  !: Le programme nécessite que la caméra et les servos moteurs soient reliés à l'ordinateur. Le choix du port de la caméra se fait en éditant la variable #define ID et le chemin menant au port série doit être spécifié correctement si vous n'utilizez pas `/dev/ttyACM0`.**




# Illustration

**Afin d'avoir un exemple concret d'utilisation, cliquez sur ce [lien Youtube](https://www.youtube.com/watch?v=3uba5YjnqWo) afin de bien comprendre comment marche le programme.**


