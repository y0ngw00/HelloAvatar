#include "GLUTWindow.h"
#include "Camera.h"
#include "UIManager.h"
#include <iostream>
#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/glut.h>
	#include <GL/glew.h>
#endif


std::vector<GLUTWindow*> GLUTWindow::windows;
std::vector<int> GLUTWindow::winIDs;

GLUTWindow::
GLUTWindow()
{
}

void
GLUTWindow::
initWindow(int w,int h,const std::string& name)
{
	
	windows.emplace_back(this);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE | GLUT_ACCUM);

	glutInitWindowPosition(50, 50);
	glutInitWindowSize(w, h);

	winIDs.emplace_back(glutCreateWindow(name.c_str()));
	#ifndef __APPLE__
		GLenum err = glewInit();
		if (err) 
		{
			fprintf(stderr, "Error: %s", glewGetErrorString(err));
			exit(0);	
		}
	#endif

	glutDisplayFunc(displayEvent);
	glutReshapeFunc(reshapeEvent);
	glutKeyboardFunc(keyboardEvent);
	glutSpecialFunc(specialEvent);
	glutMouseFunc(mouseEvent);
	glutMotionFunc(motionEvent);
	glutTimerFunc(30, timerEvent, 0);
}
GLUTWindow*
GLUTWindow::
current()
{
	auto iter = std::find(winIDs.begin(),winIDs.end(),(int)glutGetWindow());
	if(iter == winIDs.end())
		exit(0);
	int index = std::distance(winIDs.begin(),iter);
	return windows[index];
}
void
GLUTWindow::
displayEvent()
{
	current()->display();
}
void
GLUTWindow::
keyboardEvent(unsigned char key, int x, int y)
{
	current()->keyboard(key,x,y);
}
void
GLUTWindow::
specialEvent(int key,int x,int y)
{
	current()->special(key,x,y);
}
void
GLUTWindow::
mouseEvent(int button, int state, int x,int y)
{
	current()->mouse(button,state,x,y);
}
void
GLUTWindow::
motionEvent(int x,int y)
{
	current()->motion(x,y);
}
void
GLUTWindow::
reshapeEvent(int w,int h)
{
	current()->reshape(w,h);
}
void
GLUTWindow::
timerEvent(int tic)
{
	current()->timer(tic);
}