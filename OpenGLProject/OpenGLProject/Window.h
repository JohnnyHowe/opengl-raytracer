#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Scene.h"

class Window {
public: glm::vec2 viewportMin;
public: glm::vec2 viewportMax;
public: int pixelSize;
public: Window();
public: void drawAt(glm::vec3 color, glm::vec2 picturePosition);
public: void draw(Scene scene);
};
