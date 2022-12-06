#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "object.h"

#include "constants.h"

class Camera: public Object {
	glm::vec3 direction;
	glm::vec3& eye = position;
public:
	Camera(glm::vec3 eye, glm::vec3 direction) {
		this->eye = eye;
		this->direction = direction;
	}

	void Rotate(glm::vec3 rot) {
		glm::vec3 newdiry = glm::rotate(direction, rot.y, WORLD_UP);
		glm::vec3 newdirxy = glm::rotate(newdiry, rot.x, Right());
		if (glm::length(glm::abs(newdirxy - WORLD_UP)) > 0.1)
			direction = newdirxy;
		else
			direction = newdiry;
	}

	glm::mat4 GetView() {
		return glm::lookAt(eye, position + direction, WORLD_UP);
	}

	glm::vec3 Up() {
		return glm::normalize(glm::cross(WORLD_RIGHT, direction));
	}

	glm::vec3 Right() {
		return glm::normalize(glm::cross(direction, WORLD_UP));
	}

	glm::vec3 Front() {
		return direction;
	}
};