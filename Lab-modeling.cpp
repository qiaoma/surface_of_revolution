/*
Name: Qiao Ma
CIN: 302599315
*/

#include <windows.h>  //suitable when using Windows 95/98/NT
#include <gl/Gl.h>
#include <gl/Glu.h>
#include <gl/glut.h>
#include <iostream>

#include "mesh.h"
#include "Point3.h"
#include "Lab-modeling.h"

using namespace std;

int anim=0;
float diff=0;

void myInit()
{
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0f, 1.0f, 1.0f,0.0f);  // background is white
		
	glMatrixMode(GL_PROJECTION); // set the view volume shape
	glLoadIdentity();
	glOrtho(-worldWidth/2, worldWidth/2, -worldHeight/2, worldHeight/2, 0.1, 120);
}

void getInputVertex(int x, int y){
	int ry = screenHeight - y;
	base[NoOfPts].set(x*worldWidth / (float)screenWidth - worldWidth / 2,
		ry*worldHeight / (float)screenHeight - worldHeight / 2,
		0);
	NoOfPts++;
}

void myMouse(int button, int state, int x, int y)
{		
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && NoOfPts < maxUserInput && inputVertex)
	{		
		getInputVertex(x, y);
		
	}	
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && NoOfPts < maxUserInput && inputVertex)
	{
		getInputVertex(x, y);

		inputVertex = false;

		//triPrism.createPrism(NoOfPts, base, 10);
		triPrism.createMash(NoOfPts, base);
		anim = 1;

	}
	glutPostRedisplay();
}

	

void drawAxes()
{
	glBegin(GL_LINES);
		//x
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-100,0,0);
		glVertex3f(100,0,0);
		//y
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0,-100,0);
		glVertex3f(0,100,0);
		//z
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0,0,-100);
		glVertex3f(0,0,100);
	glEnd();
}


void myDisplay()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // clear the screen 
	glMatrixMode(GL_MODELVIEW);								// position and aim the camera
	glLoadIdentity();
	
	if(!triPrism.ready2draw) {

		gluLookAt(eyex, eyey, eyez, lookx, looky, lookz, 0.0, 1.0, 0.0);
	    
		drawAxes();
		
		// draws a triangle based on the input
		if (NoOfPts <= maxUserInput) {
			glColor3f(0.5f, 0.5f, 0.5f);
			//glBegin(GL_POLYGON);
			glBegin(GL_LINE_STRIP);
			for(int i=0; i<NoOfPts; i++)
				glVertex3f(base[i].x, base[i].y, base[i].z);
			glEnd();
		}		
	}
	else  {
		
		// set new eye locations
		gluLookAt(eyex, eyey, eyez, lookx, looky, lookz, 0.0, 1.0, 0.0);
	    
		drawAxes();
		
		if(displayMode)
			triPrism.drawWireframe();
		else 
			triPrism.drawSolid();  

		if (drawFile){

			triPrism.drawSolidFromFile();
		}

		Sleep(1);
	}
	
	glFlush();
}

void myIdle()
{
	
	if (cameraMove){
		eyex = 40 * cos(angleCamera);
		eyez = 40 * sin(angleCamera);
		angleCamera += 0.0005;
	}

	if(anim && diff < 50)
	{

		eyex+=0.1;;
		eyey+=0.1;
		diff+=0.1;

	}
    glutPostRedisplay();
}


void myKeyboard(unsigned char key, int x, int y)
{
	//int length;

	char* filename = new char[100];
	switch(key) {
		case 'm':
		/*	if(!triPrism.ready2draw) {
				length=10;
				triPrism.createPrism(NoOfPts, base, length);
				anim=1;
			}
			*/
			cameraMove = true;
			break;
		case 'd':
			displayMode = (displayMode + 1) % 2;
			break;
		case 'r':			
			cout << "Please input the file name to read: ";	
			cin >> filename;			
			triPrism.readOBJ(filename);
			anim = 1;
			drawFile = true;
			break;
		case 'w':
			cout << "Please input the file name to write: ";
			cin >> filename;
			triPrism.saveMesh2obj(filename);
			break;
		default:
			break;
	}	
	
	glutPostRedisplay();
}


//<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char **argv)
{	

	glutInit(&argc, argv);          // initialize the toolkit
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // set display mode
	glutInitWindowSize(screenWidth, screenHeight); // set window size
	glutInitWindowPosition(100, 100); // set window position on screen
	glutCreateWindow("HW3 - mesh representation"); // open the screen window

	myInit();
	
	glutDisplayFunc(myDisplay);	
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutIdleFunc(myIdle);

	glutMainLoop();

	return( 0 );
}
