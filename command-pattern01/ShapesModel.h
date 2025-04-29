#pragma once

#include <vector>
#include <iterator>
#include "Shape.h"
#include "Circle.h"

// Shape model to act as command reciever
class ShapesModel {

	std::vector<Shape> shape_list;

	int selected_object = -1; // index to shape_list (-1 indicates no selection) - for this model, we don't store selection changes in the command stack

public:

	void set_selected_shape(int index) {

		selected_object = index;
	}


	// interface for commands
	
	int createCircle() { // add new shape to end of vector - return index 

		Circle circle(1.0);
		shape_list.push_back(circle);

		// return index of new object
		return shape_list.size() - 1;
	}

	void deleteAtIndex(int i) {

		auto iterator = std::next(shape_list.begin(), i);
		shape_list.erase(iterator);
	}


};