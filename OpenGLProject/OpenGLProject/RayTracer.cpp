/*==================================================================================
* COSC 363  Computer Graphics (2021)
* Department of Computer Science and Software Engineering, University of Canterbury.
*
* A basic ray tracer
* See Lab07.pdf  for details.
*===================================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include "Plane.h"
#include "TextureBMP.h"
#include <GL/freeglut.h>
#include "Scene.h"
#include "Window.h"
using namespace std;


Scene scene;
Window window;
TextureBMP texture;

void display() {
	window.draw(scene);
}

//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
	glMatrixMode(GL_PROJECTION);
    gluOrtho2D(window.viewportMin.x, window.viewportMax.x, window.viewportMin.y, window.viewportMax.y);
	texture = TextureBMP("G:/uni-repo/COSC363/labs/raytracing-2/Butterfly.bmp");

	glClearColor(0, 0, 0, 1);

	Plane* plane = new Plane(glm::vec3(-30., -15, -40), //Point A
		glm::vec3(30., -15, -40), //Point B
		glm::vec3(30., -15, -200), //Point C
		glm::vec3(-30., -15, -200)); //Point D
	plane->setColor(glm::vec3(0.8, 0.8, 0));
	scene.objects.push_back(plane);
	plane->setSpecularity(false);

	Sphere* sphere1 = new Sphere(glm::vec3(-5.0, 0.0, -90.0), 15.0);
	//sphere1->setRefractivity(true, 0.9, 2);
	sphere1->setColor(glm::vec3(0.5, 0.5, 1.0));   //Set colour to blue
	scene.objects.push_back(sphere1);
	sphere1->setTransparency(true, 0.8f);
	//sphere1->setReflectivity(true, 1);

	Sphere* sphere2 = new Sphere(glm::vec3(10, 3, -70.0), 5.0);
	sphere2->setColor(glm::vec3(0, 1, 0));   //Set colour to blue
	scene.objects.push_back(sphere2);

	Sphere* sphere3 = new Sphere(glm::vec3(10.0, -10, -65.0), 3.0);
	sphere3->setColor(glm::vec3(0, 0, 0));   //Set colour to blue
	scene.objects.push_back(sphere3);

	Sphere* sphere4 = new Sphere(glm::vec3(0.0, -15.0, -130.0), 20.0);
	sphere4->setColor(glm::vec3(0, 0, 1));
	scene.objects.push_back(sphere4);

}


int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("Raytracing");

	glutDisplayFunc(display);
	initialize();

	glutMainLoop();
	return 0;
}
