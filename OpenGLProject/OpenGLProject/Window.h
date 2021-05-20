#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Scene.h"

class Window {
public: glm::vec2 viewportMin;
public: glm::vec2 viewportMax;
public: int pixelSize;
public: Window();
public: void draw(Scene scene);
public: bool antiAliasing = true;
};
