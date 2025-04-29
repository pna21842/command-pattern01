#pragma once

#include "Shape.h"

class Circle : public Shape {

private:

	float		radius;

public:

	Circle() : Shape() {

		radius = 1.0f;
	}

	Circle(float radius) : Shape() {

		this->radius = radius;
	}

	Circle(float radius, glm::vec3 position, float scale, float rotation) : Shape(position, scale, rotation) {

		this->radius = radius;
	}

	void reset() override {

		Shape::reset();

		radius = 1.0f;
	}

	void render() override {
	}


	// accessor methods

	float get_radius() {

		return radius;
	}

	float set_radius(float radius) {

		this->radius = radius;
	}
};
