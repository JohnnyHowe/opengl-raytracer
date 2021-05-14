#pragma once
#include <cmath>
#include "SceneObject.h"
#include "Ray.h"

class Scene {
public: int maxRaySteps;
public: std::vector<SceneObject*> objects;
public: glm::vec3 trace(Ray ray, int step, int maxRaySteps);
};