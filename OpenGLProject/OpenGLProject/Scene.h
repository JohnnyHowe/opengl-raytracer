#pragma once
#include <cmath>
#include <vector>
#include "SceneObject.h"
#include "Ray.h"

class Scene {
public: int maxRaySteps;
std::vector<SceneObject*> objects;
glm::vec3 trace(Ray ray, int step, int maxRaySteps);
glm::vec3 spotlightPos = glm::vec3(0, 20, -35);
glm::vec3 spotlightDir = glm::vec3(0, -1, -1.5);
float spotlightSize = 0.25f;
};