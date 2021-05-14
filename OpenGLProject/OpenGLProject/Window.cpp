#include "Window.h"
#include <GL/freeglut.h>
#include <iostream>


const float WIDTH = 20.0;
const float HEIGHT = 20.0;
const float EDIST = 40.0;

const int NUMDIV = 500;
const int MAX_STEPS = 5;

const float XMIN = -WIDTH * 0.5;
const float XMAX = WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX = HEIGHT * 0.5;


Window::Window() {
	pixelSize = 1;
	viewportMin.x = XMIN;
	viewportMin.y = YMIN;
	viewportMax.x = XMAX;
	viewportMax.y = YMAX;
}

void Window::draw(Scene scene) {
	float xp, yp;  //grid point
	float cellX = (XMAX - XMIN) / NUMDIV;  //cell width
	float cellY = (YMAX - YMIN) / NUMDIV;  //cell height
	glm::vec3 eye(0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a tiny quad.

	for (int i = 0; i < NUMDIV; i++)	//Scan every cell of the image plane
	{
		xp = XMIN + i * cellX;
		for (int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j * cellY;

			glm::vec3 dir(xp + 0.5 * cellX, yp + 0.5 * cellY, -EDIST);	//direction of the primary ray

			Ray ray = Ray(eye, dir);

			glm::vec3 col = scene.trace(ray, 1, MAX_STEPS); //Trace the primary ray and get the colour value
			//drawAt(col, glm::vec2(xp, yp));
			glColor3f(col.r, col.g, col.b);
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp + cellX, yp);
			glVertex2f(xp + cellX, yp + cellY);
			glVertex2f(xp, yp + cellY);
		}
	}
	glEnd();
	glFlush();
}

void Window::drawAt(glm::vec3 color, glm::vec2 picturePosition) { 
	glColor3f(color.r, color.g, color.b);
	glVertex2f(picturePosition.x, picturePosition.y);
	glVertex2f(picturePosition.x + pixelSize, picturePosition.y);
	glVertex2f(picturePosition.x + pixelSize, picturePosition.y + pixelSize);
	glVertex2f(picturePosition.x, picturePosition.y + pixelSize);
}

//Window::Window() {
//	
//};
//
//void Window::drawAt(glm::vec3 color, glm::vec2 picturePosition) {
//	glColor3f(color.r, color.g, color.b);
//	glVertex2f(picturePosition.x, picturePosition.y);
//	//glVertex2f(xp + cellX, yp);
//	//glVertex2f(xp + cellX, yp + cellY);
//	//glVertex2f(xp, yp + cellY);
//}