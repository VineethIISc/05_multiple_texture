#include "CommonHeader.h"
#include "TestQuad.h"


#include <QtGui/qevent.h>

#include "GLWindow.h"

GLWindow::GLWindow():screenWidth(0), screenHeight(0)
{
	//normal triangle
	quad = new TestQuad;


}


GLWindow::~GLWindow()
{
	if(quad)
		delete quad;


}

void GLWindow::initializeGL()
{
	//initialise glew at first before using opengl functions
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		std::cout << "Failed to initialise the glew" << std::endl;
		exit(EXIT_FAILURE);
	}

	//opengl version and graphic card driver
	std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;
	//graphic card
	std::cout << "GL renderer: " << glGetString(GL_RENDERER) << std::endl;

	//background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	 //black color

	//load data to GPU
	sendDataToGPU();

}

void GLWindow::sendDataToGPU()
{
	//load triangle data
	quad->loadData();

	
}



void GLWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	//draw objects
	drawObjects();

	//check for any error
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cerr << "OpenGL error: " << err << std::endl;
	}
	
}


void GLWindow::drawObjects()
{
	//draw quad
	quad->render();

	
}

void GLWindow::screenToClip(Vector2f& screenCoordinate)
{
	//y coordinate
	screenCoordinate.y = 1.0f - (screenCoordinate.y/ (0.5f * screenHeight) );
	screenCoordinate.x = (screenCoordinate.x / (0.5f * screenWidth)) - 1.0f;
}


void GLWindow::resizeGL(int w, int h)
{
	screenWidth = w;
	screenHeight = h;

}

void GLWindow::mousePressEvent(QMouseEvent* buttonPress)
{
	//Dstd::cout << "Mouse is pressed" << std::endl;

	//check for left button press event
	if (buttonPress->button() == Qt::LeftButton)
	{
		//get the position
		QPoint mousePos = buttonPress->pos();

		Vector2f mPos(mousePos.x(), mousePos.y());
		screenToClip(mPos);
		
	}

	//render at the end of every event: call paintGL
	update();
}


