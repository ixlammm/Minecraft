#pragma once

#include <glm/glm.hpp>

class Object {
protected:
	glm::vec3 position;

public:

	void SetPosition(glm::vec3 position) {
		this->position = position;
	}

	void Translate(glm::vec3 t) {
		position += t;
	}

	glm::vec3 GetPosition() {
		return position;
	}

	virtual ~Object() {}
};
