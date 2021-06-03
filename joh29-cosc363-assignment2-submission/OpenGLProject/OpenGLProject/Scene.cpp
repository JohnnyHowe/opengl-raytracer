#include <iostream>
#include <algorithm>
#include "Scene.h"
#include "Ray.h"

glm::vec3 Scene::trace(Ray ray, int step, int maxRaySteps = 5) {
	glm::vec3 backgroundCol(0);						//Background colour = (0,0,0)

	glm::vec3 color(0);
	SceneObject* obj;

	ray.closestPt(objects);					//Compare the ray with all objects in the scene
	if (ray.index == -1) return backgroundCol;		//no intersection
	obj = objects[ray.index];					//object on which the closest point of intersection is found

	color = obj->lighting(spotlightPos, spotlightDir, spotlightSize, -ray.dir, ray.hit, objects);						//Object's colour

	// Refective
	if (obj->isReflective() && step < maxRaySteps)
	{
		float rho = obj->getReflectionCoeff();
		glm::vec3 normalVec = obj->normal(ray.hit);
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
		Ray reflectedRay(ray.hit, reflectedDir);
		glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
		color = color + (rho * reflectedColor);
	}

	// Refraction
	if (obj->isRefractive() && step < maxRaySteps) {
		float eta = 1.0 / obj->getRefractiveIndex();
		glm::vec3 d = ray.dir;
		glm::vec3 n = obj->normal(ray.hit);
		glm::vec3 g = glm::refract(d, n, eta);
		Ray refRay(ray.hit, g);
		refRay.closestPt(objects);
		glm::vec3 m = obj->normal(refRay.hit);
		glm::vec3 h = glm::refract(g, -m, obj->getRefractiveIndex());
		Ray exitRay(refRay.hit, h);
		glm::vec3 refractedColor = trace(exitRay, step + 1);
		color = color + (refractedColor * obj->getRefractionCoeff());
	}

	// Transparency
	if (obj->isTransparent() && step < maxRaySteps && obj->getTransparencyCoeff() != 0) {
		Ray exitRay(ray.hit, ray.dir);
		glm::vec3 transColor = trace(exitRay, step + 1);

		float t = std::min(std::max(0.0f, obj->getTransparencyCoeff()), 1.0f);
		color = color * (1 - t) + transColor * t;
	}

	return color;
};