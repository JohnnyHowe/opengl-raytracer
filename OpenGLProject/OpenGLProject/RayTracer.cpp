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
#include "SceneObject.h"
using namespace std;


Scene scene;
Window window;
TextureBMP texture;

void display() {
	window.draw(scene);
}


glm::vec3 groundShader(glm::vec3 lightPos, glm::vec3 viewVec, glm::vec3 hit, SceneObject *self) {
	glm::vec3 color;
	int stripeWidth = 5;
	int iz = (hit.z + 1000.0) / stripeWidth;
	int ix = (hit.x + 1000.0) / stripeWidth;
	int k = (iz + ix) % 2; //2 colors
	if (k == 0) color = glm::vec3(0, 1, 0);
	else color = glm::vec3(1, 1, 0.5);
	return color;
}

glm::vec3 fancySphereShader(glm::vec3 lightPos, glm::vec3 viewVec, glm::vec3 hit, SceneObject *self) {
	glm::vec3 color(0.5, 0.5, 0.9);
	float t = 2.0f;
	color.y = sin((hit.y + hit.x + hit.z) * t);
	color.x = cos((-hit.y + hit.x + hit.z) * t);
	return color;
}


glm::vec3 textureShader(glm::vec3 lightPos, glm::vec3 viewVec, glm::vec3 hit, SceneObject *self) {
	float roomWidth = 70.0f;
	float roofHeight = 70.0f;
	float cx = -roomWidth / 2.0f;
	float cy = -roofHeight / 2.0f;
	return texture.getColorAt((hit.x - cx) / roomWidth, (hit.y - cy) / roofHeight);
}


void createBox(glm::vec3 center, glm::vec3 size, glm::vec3 color, float transparency=0.0f) {
	// Yes this is super ugly but I have zero motivation right now
	float minx = center.x - size.x / 2;
	float maxx = center.x + size.x / 2;
	float miny = center.y - size.y / 2;
	float maxy = center.y + size.y / 2;
	float minz = center.z - size.z / 2;
	float maxz = center.z + size.z / 2;
	Plane* plane1 = new Plane(glm::vec3(minx, miny, minz), glm::vec3(maxx, maxy, minz), glm::vec3(maxx, miny, minz), glm::vec3(minx, maxy, minz));
	Plane* plane2 = new Plane(glm::vec3(minx, miny, maxz), glm::vec3(maxx, miny, maxz), glm::vec3(maxx, maxy, maxz), glm::vec3(minx, maxy, maxz));
	Plane* plane3 = new Plane(glm::vec3(minx, maxy, minz), glm::vec3(minx, maxy, maxz), glm::vec3(maxx, maxy, maxz), glm::vec3(maxx, maxy, minz));
	Plane* plane4 = new Plane(glm::vec3(minx, miny, minz), glm::vec3(minx, miny, maxz), glm::vec3(maxx, miny, maxz), glm::vec3(maxx, miny, minz));
	Plane* plane5 = new Plane(glm::vec3(minx, miny, minz), glm::vec3(minx, maxy, minz), glm::vec3(minx, maxy, maxz), glm::vec3(minx, miny, maxz));
	Plane* plane6 = new Plane(glm::vec3(maxx, miny, minz), glm::vec3(maxx, maxy, minz), glm::vec3(maxx, maxy, maxz), glm::vec3(maxx, miny, maxz));
	scene.objects.push_back(plane1);
	scene.objects.push_back(plane2);
	scene.objects.push_back(plane3);
	scene.objects.push_back(plane4);
	scene.objects.push_back(plane5);
	scene.objects.push_back(plane6);
	plane1->setSpecularity(false);
	plane2->setSpecularity(false);
	plane3->setSpecularity(false);
	plane4->setSpecularity(false);
	plane5->setSpecularity(false);
	plane6->setSpecularity(false);
	plane1->setColor(color);
	plane2->setColor(color);
	plane3->setColor(color);
	plane4->setColor(color);
	plane5->setColor(color);
	plane6->setColor(color);
	if (transparency > 0) {
		plane1->setTransparency(true, transparency);
		plane2->setTransparency(true, transparency);
		plane3->setTransparency(true, transparency);
		plane4->setTransparency(true, transparency);
		plane5->setTransparency(true, transparency);
		plane6->setTransparency(true, transparency);
	}
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
	texture = TextureBMP("house.bmp");

	window.antiAliasing = true;
	glClearColor(0, 0, 0, 1);

	glm::vec3 cameraPos(0, 40, 0);
	float roomDepth = 200.0f;
	float roomWidth = 70.0f;
	float roofHeight = 50.0f;

	// Floor
	Plane* floor = new Plane(glm::vec3(-roomWidth / 2.0f, -15, -40), //Point A
							 glm::vec3(roomWidth / 2.0f, -15, -40), //Point B
						     glm::vec3(roomWidth / 2.0f, -15, -40 - roomDepth), //Point C
							 glm::vec3(-roomWidth / 2.0f, -15, -40 - roomDepth)); //Point D
	floor->setColor(glm::vec3(0.8, 0.8, 0));
	scene.objects.push_back(floor);
	floor->setSpecularity(false);
	floor->shader = &groundShader;
	floor->useCustomShader = true;

	// Roof
	Plane* roof = new Plane(glm::vec3(-roomWidth / 2.0f, -15 + roofHeight, -40), //Point A
							glm::vec3(roomWidth / 2.0f, -15 + roofHeight, -40), //Point B
						    glm::vec3(roomWidth / 2.0f, -15 + roofHeight, -40 - roomDepth), //Point C
							glm::vec3(-roomWidth / 2.0f, -15 + roofHeight, -40 - roomDepth)); //Point D
	roof->setColor(glm::vec3(0.8, 0.8, 0));
	scene.objects.push_back(roof);
	roof->setSpecularity(false);

	// Back
	Plane* back = new Plane(glm::vec3(-roomWidth / 2.0f, -15 + roofHeight, -40 - roomDepth), //Point A
							glm::vec3(roomWidth / 2.0f, -15 + roofHeight, -40 - roomDepth), //Point B
						    glm::vec3(roomWidth / 2.0f, -16, -40 - roomDepth), //Point C
							glm::vec3(-roomWidth / 2.0f, -16, -40 - roomDepth)); //Point D
	back->setColor(glm::vec3(0.8, 0.8, 0));
	scene.objects.push_back(back);
	back->setSpecularity(false);
	back->useCustomShader = true;
	back->shader = textureShader;

	createBox(glm::vec3(0, -12.5, -115.0), glm::vec3(20, 5, 40), glm::vec3(1.0, 0.5, 0.5));

	Sphere* sphere1 = new Sphere(glm::vec3(0.0, 5, -105.0), 15.0);
	sphere1->setColor(glm::vec3(0.9, 0.9, 1.0));   //Set colour to blue
	scene.objects.push_back(sphere1);
	sphere1->setRefractivity(true, 1.0f, 1.3f);
	//sphere1->setTransparency(true, 0.8f);

	Sphere* sphere3 = new Sphere(glm::vec3(10.0, -10.0, -82.0), 5.0);
	sphere3->setColor(glm::vec3(0, 1, 0));
	scene.objects.push_back(sphere3);
	sphere3->setTransparency(true, 0.7f);

	Sphere* sphere2 = new Sphere(glm::vec3(5.0, -13.0, -75.0), 2.0);
	sphere2->setColor(glm::vec3(1, 0, 0));
	scene.objects.push_back(sphere2);
	sphere2->useCustomShader = true;
	sphere2->shader = fancySphereShader;
}


int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("Raytracing");

	glutDisplayFunc(display);
	initialize();

	glutMainLoop();
	return 0;
}
