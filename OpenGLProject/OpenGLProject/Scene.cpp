#include <iostream>
#include "Scene.h"
#include "Ray.h"

glm::vec3 Scene::trace(Ray ray, int step, int maxRaySteps = 5) {
	glm::vec3 backgroundCol(0);						//Background colour = (0,0,0)
	glm::vec3 lightPos(10, 40, -3);					//Light's position
	glm::vec3 color(0);
	SceneObject* obj;

	ray.closestPt(objects);					//Compare the ray with all objects in the scene
	if (ray.index == -1) return backgroundCol;		//no intersection
	obj = objects[ray.index];					//object on which the closest point of intersection is found

	color = obj->lighting(lightPos, -ray.dir, ray.hit);						//Object's colour

	glm::vec3 lightVec = lightPos - ray.hit;
	Ray shadowRay(ray.hit, lightVec);
	shadowRay.closestPt(objects);

	if (shadowRay.index > -1 && shadowRay.dist < glm::length(lightVec)) {
		color = 0.2f * obj->getColor(); //0.2 = ambient scale factor
	}

	if (obj->isReflective() && step < maxRaySteps)
	{
		float rho = obj->getReflectionCoeff();
		glm::vec3 normalVec = obj->normal(ray.hit);
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
		Ray reflectedRay(ray.hit, reflectedDir);
		glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
		color = color + (rho * reflectedColor);
	}

	return color;
};