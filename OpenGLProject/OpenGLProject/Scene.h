#pragma once
#include <cmath>
#include "SceneObject.h"
#include "Ray.h"

class Scene {
public: int maxRaySteps;
std::vector<SceneObject*> objects;
glm::vec3 trace(Ray ray, int step, int maxRaySteps);
glm::vec3 sunPos = glm::vec3(10, 40, -3);
glm::vec3 spotlightPos = glm::vec3(0, 0, -80);
glm::vec3 spotlightDir = glm::vec3(0, -1, 0);
float spotlightSize = 0.5f;
};