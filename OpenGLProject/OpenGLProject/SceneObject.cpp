/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The object class
*  This is a generic type for storing objects in the scene
*  Sphere, Plane etc. must be defined as subclasses of Object.
*  Being an abstract class, this class cannot be instantiated.
-------------------------------------------------------------*/

#include "SceneObject.h"
#include "Ray.h"
#include <iostream>

glm::vec3 SceneObject::getColor()
{
	return color_;
}

glm::vec3 SceneObject::lighting(glm::vec3 lightPos, glm::vec3 viewVec, glm::vec3 hit, std::vector<SceneObject*> objects, bool lit)
{
	glm::vec3 color;
	float shadow = 0.0f;
		float ambientTerm = 0.1;
	if (!useCustomShader) {
		float diffuseTerm = 0;
		float specularTerm = 0;

		glm::vec3 normalVec = normal(hit);
		glm::vec3 lightVec = lightPos - hit;
		lightVec = glm::normalize(lightVec);

		float lDotn = glm::dot(lightVec, normalVec);
		if (spec_)
		{
			glm::vec3 reflVec = glm::reflect(-lightVec, normalVec);
			float rDotv = glm::dot(reflVec, viewVec);
			if (rDotv > 0) specularTerm = pow(rDotv, shin_);
		}
		glm::vec3 colorSum;
		if (lit) {
			shadow = 1- std::max(lDotn, 0.0f);
			colorSum = color_ + specularTerm * glm::vec3(1);
		}
		else {
			shadow = 1;
			colorSum = color_;
		}
		color = colorSum;
	}
	else {
		color = (*shader)(lightPos, viewVec, hit);
	}

	// Shadow
	glm::vec3 lightVec = lightPos - hit;
	Ray* shadowRay;
	shadowRay = new Ray(hit, lightVec);
	shadowRay->closestPt(objects);
	for (int i = 0; i < 5; i++) {
		if (shadowRay->index > -1 && shadowRay->dist < glm::length(lightVec)) {

			SceneObject* hitObj = objects[shadowRay->index];

			if (hitObj->isTransparent()) {
				shadow += (1-shadow) * (1-hitObj->getTransparencyCoeff());

				shadowRay = new Ray(shadowRay->hit, lightVec);
				shadowRay->closestPt(objects);
			}
			else {
				shadow = 1;
				break;
			}
		}
	}
	float shadowScale = (1-ambientTerm) * (1 - std::min(std::max(0.0f, shadow), 1.0f)) + ambientTerm;
	color = shadowScale * color;
	return color;
}

glm::vec3 SceneObject::lighting(glm::vec3 lightPos, glm::vec3 lightDirection, float maxLightAngle, glm::vec3 viewVec, glm::vec3 hit, std::vector<SceneObject*> objects) {
	glm::vec3 normalVec = normal(hit);
	glm::vec3 da=glm::normalize(lightDirection);
	glm::vec3 db = glm::normalize(hit - lightPos);
	float angle = glm::acos(glm::dot(da, db));
	bool lit = angle <= maxLightAngle;
	return lighting(lightPos, viewVec, hit, objects, lit);
}

float SceneObject::getReflectionCoeff()
{
	return reflc_;
}

float SceneObject::getRefractionCoeff()
{
	return refrc_;
}

float SceneObject::getTransparencyCoeff()
{
	return tranc_;
}

float SceneObject::getRefractiveIndex()
{
	return refri_;
}

float SceneObject::getShininess()
{
	return shin_;
}

bool SceneObject::isReflective()
{
	return refl_;
}

bool SceneObject::isRefractive()
{
	return refr_;
}


bool SceneObject::isSpecular()
{
	return spec_;
}


bool SceneObject::isTransparent()
{
	return tran_;
}

void SceneObject::setColor(glm::vec3 col)
{
	color_ = col;
}

void SceneObject::setReflectivity(bool flag)
{
	refl_ = flag;
}

void SceneObject::setReflectivity(bool flag, float refl_coeff)
{
	refl_ = flag;
	reflc_ = refl_coeff;
}

void SceneObject::setRefractivity(bool flag)
{
	refr_ = flag;
}

void SceneObject::setRefractivity(bool flag, float refr_coeff, float refr_index)
{
	refr_ = flag;
	refrc_ = refr_coeff;
	refri_ = refr_index;
}

void SceneObject::setShininess(float shininess)
{
	shin_ = shininess;
}

void SceneObject::setSpecularity(bool flag)
{
	spec_ = flag;
}

void SceneObject::setTransparency(bool flag)
{
	tran_ = flag;
}

void SceneObject::setTransparency(bool flag, float tran_coeff)
{
	tran_ = flag;
	tranc_ = tran_coeff;
}