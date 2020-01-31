#include "stdafx.h"
#include "CalendarComponent.h"
#include "ToDoList.h"
#include "ToDoListVisitor.h"
#include "utilityfunctions.h"

shared_ptr<DisplayableComponent> ToDoList::addComponent(shared_ptr<DisplayableComponent> task) {
	cout << "Adding task" << endl;
	Task * thisTask = dynamic_cast<Task *>(task.get());
	if (thisTask == nullptr) {
		cout << "This task is null" << endl;
		return nullptr;
	}
	else {
		cout << "Task is not null" << endl;
		children.push_back(task);
		return task;
	}
}

Task::Task(tm when, shared_ptr<DisplayableComponent> p, string s) : DisplayableComponent(p), name(s), dateInfo(when), isComplete(false) {}

void Task::accept(DisplayableComponentVisitor * visitor) {
	return;
}

void Task::display() {
	cout << "\t";
	cout << name << " " << dateString(dateInfo) << " " << timeString(dateInfo);
	if (isComplete) {
		cout << " " << "COMPLETE" << endl;
	}
	else {
		cout << " " << "TODO" << endl;
	}
}

ToDoList::ToDoList(shared_ptr<Calendar> p) : DisplayableComponent(p) {}

void ToDoList::accept(DisplayableComponentVisitor * visitor) {
	return;
}

void ToDoList::display() {
	cout << "TODO List" << endl;
	for (int i = 0; i < children.size(); i++) {
		children[i]->display();
	}
}

void ToDoList::markToDo(unsigned int a) {
	/*shared_ptr<DisplayableComponent> comp = getChild(a);
	Task * task = dynamic_cast<Task *>(comp.get());
	task->isComplete = true;*/
	
}
void ToDoList::markComplete(unsigned int b) {
	shared_ptr<DisplayableComponent> task = children[b];
	Task * thisTask = dynamic_cast<Task *>(task.get());
	thisTask->isComplete = true;
}