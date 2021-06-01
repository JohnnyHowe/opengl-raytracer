#include "Window.h"
#include <GL/freeglut.h>
#include <iostream>


const float WIDTH = 20.0;
const float HEIGHT = 20.0;
const float EDIST = 40.0;

const int NUMDIV = 200;
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
	antiAliasing = false;
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
		//std::cout << (float) i * 100.0f / (float) NUMDIV << "% \n";
		xp = XMIN + i * cellX;
		for (int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j * cellY;

			if (antiAliasing) {

				glm::vec3 dir1(xp, yp, -EDIST);	//direction of the primary ray
				glm::vec3 dir2(xp + cellX, yp, -EDIST);	//direction of the primary ray
				glm::vec3 dir3(xp + cellX, yp + cellY, -EDIST);	//direction of the primary ray
				glm::vec3 dir4(xp, yp + cellY, -EDIST);	//direction of the primary ray

				Ray ray1 = Ray(eye, dir1);
				Ray ray2 = Ray(eye, dir2);
				Ray ray3 = Ray(eye, dir3);
				Ray ray4 = Ray(eye, dir4);

				glm::vec3 col1 = scene.trace(ray1, 1, MAX_STEPS); //Trace the primary ray and get the colour value
				glm::vec3 col2 = scene.trace(ray2, 1, MAX_STEPS); //Trace the primary ray and get the colour value
				glm::vec3 col3 = scene.trace(ray3, 1, MAX_STEPS); //Trace the primary ray and get the colour value
				glm::vec3 col4 = scene.trace(ray4, 1, MAX_STEPS); //Trace the primary ray and get the colour value

				glm::vec3 col = (col1 + col2 + col3 + col4) * 0.25f;

				glColor3f(col.r, col.g, col.b);
				glVertex2f(xp, yp);				//Draw each cell with its color value
				glVertex2f(xp + cellX, yp);
				glVertex2f(xp + cellX, yp + cellY);
				glVertex2f(xp, yp + cellY);

			} else {
				glm::vec3 dir(xp + 0.5 * cellX, yp + 0.5 * cellY, -EDIST);	//direction of the primary ray
				Ray ray = Ray(eye, dir);

				glm::vec3 col = scene.trace(ray, 1, MAX_STEPS); //Trace the primary ray and get the colour value
				glColor3f(col.r, col.g, col.b);
				glVertex2f(xp, yp);				//Draw each cell with its color value
				glVertex2f(xp + cellX, yp);
				glVertex2f(xp + cellX, yp + cellY);
				glVertex2f(xp, yp + cellY);
			}
		}
	}
	glEnd();
	glFlush();
}

