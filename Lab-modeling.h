/*
Name: Qiao Ma
CIN: 302599315
*/

#ifndef LAB_Modeling_H
#define LAB_Modeling_H


//Global Variables
const int screenWidth = 500;	   // width of screen window in pixels
const int screenHeight = 500;	   // height of screen window in pixels

const float worldWidth = 100.0;
const float worldHeight = 100.0; 

const int maxUserInput = 100;

int NoOfPts=0;
int displayMode = 0;

bool inputVertex = true, cameraMove = false, drawFile = false;

GLdouble angleCamera = 0.0;

//Point3 base[3];
Point3 base[maxUserInput];

Mesh triPrism;

// global values for camera
GLdouble eyex=0.0, eyey=0.0, eyez=50.0;
GLdouble lookx=0.0, looky=0.0, lookz=0.0;

#endif