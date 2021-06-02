cmake_minimum_required(VERSION 2.8)

project(OpenGLProject)
add_executable(OpenGLProject/OpenGLProject/joh29_raytracer OpenGLProject/OpenGLProject/RayTracer.cpp OpenGLProject/OpenGLProject/SceneObject.cpp OpenGLProject/OpenGLProject/Plane.cpp OpenGLProject/OpenGLProject/Ray.cpp OpenGLProject/OpenGLProject/Scene.cpp OpenGLProject/OpenGLProject/Sphere.cpp OpenGLProject/OpenGLProject/TextureBMP.cpp OpenGLProject/OpenGLProject/Window.cpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(OpenGLProject/OpenGLProject/joh29_raytracer ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
