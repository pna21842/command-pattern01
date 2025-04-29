#include <memory>
#include <iostream>
#include <vector>
#include <iterator>
#include "Circle.h"
#include "ShapesModel.h"

using namespace std;

// Example model (Receiver)
class Model {

	vector<int>		list;

public:

	// Main command interface

	void setItem(int index, int value) {

		list[index] = value;
	}

	// insert item into the list - if index not specified or out of range push to end of list.  Return index of item once inserted
	int insertItem(int value, int index = -1) {

		if (index < 0 || index >= list.size()) {

			list.push_back(value);
			return list.size() - 1;
		}
		else {

			auto ins_iterator = std::next(list.begin(), index);
			list.insert(ins_iterator, value);
			return index;
		}
	};

	void deleteItem(int index) {

		if (index >= 0 && index < list.size()) {

			auto del_iterator = std::next(list.begin(), index);
			list.erase(del_iterator);
		}
	}

	// Other support / helper functions

	int getItem(int index) {

		return list[index];
	}

	int getNextInsertIndex() {

		return list.size();
	}

	void listContents() {

		cout << "{";

		if (list.size() > 0) {

			auto lastElement = std::next(list.begin(), list.size() - 1);
			auto i = list.begin();

			while (i != lastElement) {

				cout << *i << ", ";
				i++;
			}

			cout << *i;
		}

		cout << "}\n";
	}
};


// Command base-class
class Command {

public:

	virtual void execute() = 0;
	
};

// Specific commands...

// Set an item in the array
class SetCommand : public Command {

	Model*					model;
	int						index;
	int						value;

public:

	SetCommand(Model* model, int index, int value) {

		this->model = model;
		this->index = index;
		this->value = value;
	}

	void execute() override {

		model->setItem(index, value);
	}
};

// Insert a new item into the array
class InsertCommand : public Command {

	Model*			model;
	int				index;
	int				value;

public:

	InsertCommand(Model* model, int index, int value) {

		this->model = model;
		this->index = index;
		this->value = value;
	}

	void execute() override {

		model->insertItem(value, index);
	}
};

// Remove an item from the array
class DeleteCommand : public Command {

	Model*			model;
	int				index;

public:

	DeleteCommand(Model* model, int index) {

		this->model = model;
		this->index = index;
	}

	void execute() override {

		model->deleteItem(index);
	}
};


// Command pair encapsulated together in CmdAction class
class CmdAction {

	Command* cmd{ nullptr }; // note: must be pointer for virtual function resolution - CmdAction owns and destroys these
	Command* undoCmd{ nullptr };

public:

	CmdAction(Command* cmd, Command* undoCmd) {

		this->cmd = cmd;
		this->undoCmd = undoCmd;
	}

	~CmdAction() {

		if (cmd)
			delete cmd;
		if (undoCmd)
			delete undoCmd;
	}

	void doCommand() {

		cmd->execute();
	}

	void undoCommand() {

		undoCmd->execute();
	}
};




// perform stack operations, but allow 'popped' elements to be retained to facilitate undo
// Has 3 mains operations:
// insert:  Add new item at stack pointer, and move top to the location of the stack pointer, removing anything above this
// (so when an action is performed all subsequent redo operations are lost)
// undo: perform operation at stack pointer and move stack pointer down
// redo: perform operation at stack pointer and move stack pointer up
// sp refers to the stack pointer, top refers to the top element of the stack - this is the last creational operation we perform
class CommandStack {

public:

	vector<CmdAction*>	stack; // 'size' stores largest number of actions on stack at any one time
	int					sp = 0; // stack pointer (main insertion / undo point)
	int					top = 0; // last redo-able command

public:

	// push at stack pointer location
	void insert(CmdAction* action) {

		if (sp == stack.size()) { // top of stack so push_back brand new item into vector

			stack.push_back(action);
			sp++; // post increment on insert
			top = sp;
		}
		else { // sp < size so can override existing element and realign top with sp so no redo possible

			// insert at sp then post increment sp
			delete stack[sp];
			stack[sp] = action;
			sp++;
			top = sp;
		}
	}

	void insertAndExecute(CmdAction* action) {

		insert(action);
		action->doCommand();
	}

	void undo() {

		if (sp > 0) {

			sp--; // pre-decrement on undo
			stack[sp]->undoCommand();
		}
	}

	void redo() {

		if (canRedo()) {

			stack[sp]->doCommand();
			sp++; // post-increment on re-do
		}
	}

	bool canRedo() {

		return sp < top;
	}
};


int main(void) {

	// Example interaction with Model using commands...

	Model* model = new Model();
	CommandStack* undoStack = new CommandStack();
	
	model->listContents();

	// Add item
	undoStack->insertAndExecute(new CmdAction(
		new InsertCommand(model, model->getNextInsertIndex(), 1),
		new DeleteCommand(model, model->getNextInsertIndex()))); // stack has ownership of CmdActions
	
	// Add item
	undoStack->insertAndExecute(new CmdAction(
		new InsertCommand(model, model->getNextInsertIndex(), 2),
		new DeleteCommand(model, model->getNextInsertIndex())));

	// Add item
	undoStack->insertAndExecute(new CmdAction(
		new InsertCommand(model, model->getNextInsertIndex(), 3),
		new DeleteCommand(model, model->getNextInsertIndex())));

	model->listContents();	

	// Set value at index 1
	undoStack->insertAndExecute(new CmdAction(
		new SetCommand(model, 1, 10),
		new SetCommand(model, 1, model->getItem(1))));
	model->listContents();

	// undo set value (cmd4)
	undoStack->undo();
	model->listContents();

	// undo insert (cmd3)
	undoStack->undo();
	model->listContents();

	// undo insert (cmd2)
	undoStack->undo();
	model->listContents();

	// Should have only '1' left on stack
	// 
	// Let's add a new change so we void subseqent redo ops
	undoStack->insertAndExecute(new CmdAction(
		new SetCommand(model, 0, -5),
		new SetCommand(model, 0, model->getItem(0))));
	model->listContents();


	// 
	// undo insert (cmd1)
	//undoStack->undo();
	//model->listContents();

	// Next we need to not pop and manupulate stack pointer for redo!
	while (undoStack->canRedo()) {
		undoStack->redo();
	}
	model->listContents();


	undoStack->undo();
	model->listContents();

	undoStack->undo();
	model->listContents();


	return 0;
}
