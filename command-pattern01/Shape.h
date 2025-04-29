#pragma once

#include <glm/vec4.hpp>
#include <glm/ext/quaternion_float.hpp>

class Shape {

private:

	glm::vec3		_position; // homogeneous coord (x, y, w); w = 1 for position
	float			_scale; // support uniform scale for now
	float			_rotation; // angle of rotation

public:

	Shape() {

		reset();
	}

	Shape(glm::vec3 position, float scale, float rotation) {

		this->_position = position;
		this->_scale = scale;
		this->_rotation = rotation;
	}

	virtual void reset() {

		_position = glm::vec3(0.0f, 0.0f, 1.0f);
		_scale = 1.0f;
		_rotation = 0.0f;
	}

	virtual void render() {
	}



	void translate(glm::vec2 T) {

		_position.x += T.x;
		_position.y += T.y;
	}

	void scale(float scale) {

		_scale *= scale;
	}

	void rotate(float theta) {

		_rotation += theta;
	}


};


