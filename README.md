# ProjectC_NGUYEN_SYSOUVANH
## ProjectC ROB3

### Instructions for use 


This is our repository for the ROB3 project C, 2021-2022. This project was realized in binomial: Victoria Nguyen and Matthieu Sysouvanh

### Requirements :

Implementation of a virtual machine, in the spirit of Pixar. Based on the OpenCV library for computer vision and a webcam, moving via 2 servomotors (2 degrees of freedom).


**Contains two files:**

1. Imposed project : Color tracking
2. Free project : implementation of virtual instruments + game similar to a piano Tiles, based on servomotors and the movement of an object tracked by its color

Contains a doc file, containing the html documentation linked to each of the 2 folders for each program.




## Requirements and tasks to be done for the project to work: 
 
 - Have the Arch Pro Linux board + two servo motors + a webcam connected to the servo motors.
 - To be in an environment without too much light, not to be against the light.
 - Have the latest version of OPEN CV, available via this link: https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html
 - To make the servomotors work, plug the MBED card and put the binary file located in the folder _binary_file_ in the card. Wait until the LED blinks, sign that the code is well put in the card.
 - For the Free project : it is necessary to install the SDL2 library, to download via this link :
 https://devopssec.fr/article/telechargement-compilation-projet-sdl#begin-article-section
 and the SDL_Mixer library, in order to have the sound. These libraries must be installed in the path file of the user environment for the makefile to work.
 SDL_mixer is installed by following the commands https://zoomadmin.com/HowToInstall/UbuntuPackage/libsdl2-mixer-2.0-0
 
 Note: if the SDL2 and SDL_Mixer files are not recognized, change the `#include` in the program headers so that the compiler has the right path. 
 Put the `SDL_Mixer.h` file in the same folder as the .h of SDL2.
 
 
 
**At the end of the README there is a video link to have a visual demonstration of the project**.

 
# PROJECT REQUIRES :
 
 **To launch it, open a terminal in the location of the file .../projectImpose , then launch `make` to compile, then `./main` to execute.**
 **The executable is the main file**. 
 
 
 This one is organized in a `.cpp file` and a makeFile.   To detect a color, you have to click on the image displayed on the screen. The place (=pixel) where you click
 will be tracked by the program. If, during the tracking, you move to points where the brightness changes, you have to click again on the place on the screen where the color is tracked.
 
 If you go too far to the right, then the camera, via the servomotors, will move to follow the tracked object.
 
 **Remember**: it is important, so that the algorithm knows which color to track, to click on the image the object to track. For example, if you want to track a red balloon, you 
 at startup :
 
 1. place the balloon in the camera's field of view
 2. click on the balloon in the image feedback
 3. a rectangle of the color of the pixel you clicked on is displayed and surrounds the tracked object with the tracked color
 4. Move the object : the rectangle moves to follow the image
 If, when moving the object, the rectangle does not follow the balloon anymore, it means that the luminosity has changed since the moment you clicked. You must then click again on the current position
 of the balloon, ect...
 
 

 
# PROJECT Free
 
 - When the program starts, there is a graphic window : it is the menu of the game. You can choose between a manual mode (=freeestyle) and a video game mode. 

 
 
 
## Freestyle MODE :
 
When launching the program, a window linked to the webcam is displayed. In the background, we see a piano with 12 keys, as well as 3 boxes playing a particular sound. To begin, you must in order:

1. Take an object with a color not very present in the external environment, in order to limit the problems of detection. In the example, we use a felt pen with a red cap. Use an object of color X, with a small surface, to be precise. A pen is a good choice. 

2. Two windows will be displayed at runtime: one window is the rendering with instruments and detection, the other is the window where you have to choose the color to track. This is based on the same principle as the imposed project; you click with the mouse on the color to detect. A green outline is then displayed according to the outline of the detected color shape. Do not hesitate to click several times on the same place, changing the orientation, so that the green outline surrounds only the object of the tracked color. (on the picture, only the cap of the pen).

3. once the choice of the color is made, you can move around the screen via the center of the created contour; as soon as this center passes in one of the 12 keys of the piano, or in one of the 3 colored boxes, the corresponding sound is played. The rhythm is similar for each sound, allowing a pleasant sound rendering. Some sounds are short (one note), others are longer (drums).

4. A red rectangle surrounds the virtual instruments: if the center of the tracked object goes out from the left or from the right, then the servo will move accordingly, in order to stay aligned with the user.

# Important! 
**For the sounds to work, it is necessary to modify the path of each sound: To do this, open the file `play.cpp` and modify line 91 with the absolute path to get to the folder. This detail is very important for the playback of the sound files !


### BEFORE :
 `char path_acces[200]="/home/victoria/Bureau/ProjetROB3/ProjetC_NGUYEN_SYSOUVANH/projetLibre/samples piano/";`

### AFTER 
 char path_acces[200]=".....A REPLACE...../ProjetC_NGUYEN_SYSOUVANH/projetLibre/samples piano/";`







 
### VIDEO GAME MODE
 
When starting this mode the menu window closes and is replaced by the game window. The game window has buttons and a score and life counter. 

First click on the colored object to follow in the camera return. Uniform lighting without reflections or backlighting is best. You can click on the screen again at any time to reset the color to be detected.

Once the object is selected, a circle appears in its center. The goal of the game is to use it as a cursor to activate the buttons that turn yellow.

If you manage to do this in time you will score points.
If you don't, you will lose a life.

The camera can move randomly between 3 positions, so you will have to move to stay in the frame and keep the pace.

The difficulty increases gradually with the score, the events follow each other faster and you have less time to press the buttons.

The game stops when all lives are used up or when `echap` is pressed.


#### SETTINGS:

Several global variables manage adaptable game parameters. 
The timing of events depends on the number of ticks/s of your system, so the speed of the game is proportional to the #define TEMPO

The number of lives is managed by `#define VIES`.

**Important: The program requires the camera and servo motors to be connected to the computer. The choice of the camera port is made by editing the #define ID variable and the path to the serial port must be specified correctly if you are not using `/dev/ttyACM0`.




# Illustration

**In order to have a concrete example of use, click on this [Youtube link](https://www.youtube.com/watch?v=3uba5YjnqWo) to understand how the program works.**



