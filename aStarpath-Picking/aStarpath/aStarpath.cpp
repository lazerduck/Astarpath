#include "stdafx.h"
#include <iostream>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <stdio.h>
#include <crtdbg.h>
#include <sstream> 
#include <fstream>
#include <istream>
#include <string>
#include "Matrix.h"
#include <GL/glut.h>
#include "LinkedList.h"
#include "Pathfinding.h"


using namespace std;

//struct to store the start and the end
struct point
{
	float x;
	float y;
	point(float X, float Y)
	{
		x = X;
		y = Y;
	};
};

Matrix Room;
Path path(Room);

point start(0,0);
point goal(0,0);

bool mode = false;
bool lclick = true;

#pragma region Georges stuff
float* readTXT(string fileName )
{
	float* data;
	int i=0;
	ifstream myfile (fileName);
	if (myfile.is_open())
	{
		int size;
		myfile>>size;
		Room.SetRows(size);
		size = 0;
		myfile>>size;
		Room.SetColumns(size);
		data = new float[Room.GetColumns() * Room.GetRows()];
		while ( myfile.good())
		{
			if (i>Room.GetColumns() * Room.GetRows()-1) break;
			myfile >> *(data+i);
			// cout << *(data+i) << ' '; // This line display the converted data on the screen, you may comment it out. 
			i++;                                                             
		}

		myfile.close();
	}

	else cout << "Unable to open file"; 
	//cout << i;

	return data;
}
#pragma endregion

/*Key
*0 - Empty
*1 - Wall
*2 - Start
*3 - End
*4 - Path
*/

void init(void)
{

	glClearColor(0,0,0,1);	// Background color (RGB)- Black
	glColor3f(1.0,1.0,1.0);	// Drawing color (RGB) - White
	glLineWidth(3);			// Width of the drawing line
	glShadeModel (GL_FLAT);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluPerspective(60, 1, .1, 100);

	float* data = readTXT("Room2.txt");
	Room.SetData(data,Room.ReturnSize());
	delete []data;

	GLfloat light_position[] = { -5.0, 0.0, 5.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);	// Set Position of Light Source
	glLightf(GL_LIGHT0, GL_DIFFUSE, (0,0,0,1));			// Set Diffuse Component of Light
	glLightf(GL_LIGHT0, GL_AMBIENT, (0,0,0,1));			// Set Ambient Component of Light
	glLightf(GL_LIGHT0, GL_SPECULAR, (0,0,0,1));

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//------------ Material---
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat mat_shininess[] = { 20.0 };


	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);	// Set Specular Component of Material
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialf(GL_FRONT, GL_EMISSION, (0,0,0,0));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	//set the start and end
	for(int i = 0;i<Room.GetRows();i++)
	{
		for(int j = 0; j<Room.GetColumns();j++)
		{
			if(Room(i,j) == 2)
			{
				start.x = i;
				start.y = j;
			}
			if(Room(i,j) == 3)
			{
				goal.x = i;
				goal.y = j;
			}
		}
	}
}

void drawpick()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10,50,20,10,0,10,0,1,0); 

	for(int i = 0;i<Room.GetRows();i++)
	{
		for(int j = 0; j<Room.GetColumns();j++)
		{
			glLoadName((i*(Room.GetRows()+1))+j);
			glPushMatrix();
			glColor3f(1,1,0);
			glTranslatef(j,0,i);
			glutSolidCube(0.9f);
			glPopMatrix();
		}
	}
	glMatrixMode(GL_PROJECTION);
}

void display()
{

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	gluLookAt(10,50,20,10,0,10,0,1,0); 
	bool once = false;
	for(int i = 0;i<Room.GetRows();i++)
	{
		for(int j = 0; j<Room.GetColumns();j++)
		{
			if(Room(i,j) == 1)
			{
				glPushMatrix();
				glColor3f(0,1,0);
				glTranslatef(j,0,i);
				glutSolidCube(1);
				glPopMatrix();
			}

			if(Room(i,j) == 4)
			{
				glPushMatrix();
				glColor3f(0,0,1);
				glTranslatef(j,0,i);
				glScalef(0.2,1,0.2);
				glutSolidCube(1);
				glPopMatrix();
			}
			if(Room(i,j) ==5)
			{
				glPushMatrix();
				glColor3f(0,0,0);
				glTranslatef(j,0,i);
				glutSolidCube(1);
				glPopMatrix();
			}
			if(Room(i,j) ==6)
			{
				glPushMatrix();
				glColor3f(1,1,0);
				glTranslatef(j,0,i);
				glutSolidCube(1);
				glPopMatrix();
			}
		}
	}
	glPushMatrix();
	glColor3f(1,0,0);
	glTranslatef(start.y,0,start.x);
	glutSolidSphere(0.5,30,30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1,0,1);
	glTranslatef(goal.y,0,goal.x);
	glutSolidCube(1);
	glPopMatrix();

	glFlush();	
}

void idle()
{
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);	// Viewport mapping
	// Projection transformation
	glMatrixMode(GL_PROJECTION); 	// Set matrix mode	
	glLoadIdentity(); 			// Clear
	gluPerspective(60, (float)w/h, .1, 800); 	// define and adjust projection
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 0.1, 100.0); 
	// ** Change the near gradually (add 0.5 each time) and observe the cube.
	glMatrixMode(GL_MODELVIEW);		// reset the matrix mode
}

//picking
void processHits(GLint hits, GLuint buffer[])
{
	unsigned int i, j;
	GLuint names, *ptr;

	printf("\n\n=============================\nhits = %d\n", hits);
	ptr = (GLuint *) buffer;
	for (i = 0; i < hits; i++) {  /* for each hit  */
		names = *ptr;
		printf(" number of names for hit = %d\n", names); ptr++;
		printf("  z1 is %g;", (float) *ptr/0x7fffffff); ptr++;
		printf(" z2 is %g\n", (float) *ptr/0x7fffffff); ptr++;
		printf("   the name is --- {[ ");
		for (j = 0; j < names; j++) {  /* for each name */
			cout<<*ptr; 
			for(int ii = 0; ii < Room.GetRows();ii++)
			{
				for(int jj = 0; jj < Room.GetColumns();jj++)
				{
					if(Room(ii,jj) == 4)
					{
						Room.SetValue(ii,jj,0);
					}
					if(ii == 0)
						Room.SetValue(ii,jj,1);
					if(ii == Room.GetRows()-1)
						Room.SetValue(ii,jj,1);
					if(jj == 0)
						Room.SetValue(ii,jj,1);
					if(jj == Room.GetColumns()-1)
						Room.SetValue(ii,jj,1);
				}
			}
			if(mode)
			{
				if(Room(*ptr/(Room.GetRows()+1),*ptr%(Room.GetRows()+1)) !=1)
				{
					if(lclick)
					{
					start.x = *ptr/(Room.GetRows()+1);
					start.y = *ptr%(Room.GetRows()+1);
					}
					else
					{
						goal.x = *ptr/(Room.GetRows()+1);
						goal.y = *ptr%(Room.GetRows()+1);
					}


					cout<<"\n"<<"x :"<<*ptr/(Room.GetRows()+1)<<" y :"<<*ptr%(Room.GetRows()+1);
				}
			}
			else
			{

				if(Room(*ptr/(Room.GetRows()+1),*ptr%(Room.GetRows()+1)) !=1)
				{
					Room.SetValue(*ptr/(Room.GetRows()+1),*ptr%(Room.GetRows()+1),1);
				}
				else
				{
					Room.SetValue(*ptr/(Room.GetRows()+1),*ptr%(Room.GetRows()+1),0);
				}
			}
			if(lclick)
			{
				Room.SetValue(start.x,start.y,2);
			}
			else
			{
				Room.SetValue(start.x,start.y,3);
			}
			for(int ii = 0; ii < Room.GetRows();ii++)
			{
				for(int jj = 0; jj < Room.GetColumns();jj++)
				{
					if(ii == 0)
						Room.SetValue(ii,jj,1);
					if(ii == Room.GetRows()-1)
						Room.SetValue(ii,jj,1);
					if(jj == 0)
						Room.SetValue(ii,jj,1);
					if(jj == Room.GetColumns()-1)
						Room.SetValue(ii,jj,1);
				}
			}
			path.Init();
			path.CalcHeuristic();
			path.FindPath(start.x,start.y,goal.x,goal.y);
			ptr++;
		}
		printf(" ]} --- \n");
	}
}

#define BUFSIZE 512

void pickObjects(int button, int state, int x, int y)
{
	GLuint selectBuf[BUFSIZE];
	GLint hits;
	GLint viewport[4];

	if (state == GLUT_DOWN)
	{
		if(button == GLUT_LEFT_BUTTON)
		{
			lclick = true;
		}
		else
		{
			lclick = false;
		}
		glGetIntegerv(GL_VIEWPORT, viewport);

		glSelectBuffer(BUFSIZE, selectBuf);
		(void) glRenderMode(GL_SELECT);

		glInitNames();
		glPushName(0);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		/*  create 5x5 pixel picking region near cursor location */
		gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3] - y),
			1.0, 1.0, viewport);

		gluPerspective(60, 1, .1, 100);  // Playing with Z (near/far) allow interacting with layers
		drawpick();
		glPopMatrix();
		glFlush();

		hits = glRenderMode(GL_RENDER);
		processHits(hits, selectBuf);
	}
}

void keypress(unsigned char key, int x, int y)
{
	if(key == 'w')
	{
		mode = true;
	}
	if(key == 'q')
	{
		mode = false;
	}
}

int main(int argc, char** argv) 
{
	glutInit(&argc, argv);	// GLUT Initialization 
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE); // Initializing the Display mode
	glutInitWindowSize(700,700);	// Define the window size
	glutCreateWindow("A* path finding");	// Create the window, with caption.

	init();	// All OpenGL initialization
	path.Init();
	path.CalcHeuristic();
	path.FindPath(start.x,start.y,goal.x,goal.y);

	//-- Callback functions ---------------------
	glutDisplayFunc(display);
	glutMouseFunc(pickObjects);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keypress);
	glutMainLoop();	// Loop waiting for event 

}
