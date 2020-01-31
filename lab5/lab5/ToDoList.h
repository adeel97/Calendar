#pragma once
#include "stdafx.h"
#include <vector>
#include <string>
#include <iostream>
#include "DisplayableComponent.h"
#include "DisplayableComponentVisitor.h"
#include "CalendarComponent.h"
using namespace std;

class ToDoListVisitor;
class DisplayableComponentVisitor;

class Task :public DisplayableComponent {
	friend class ToDoList;
	friend ToDoListVisitor;
	friend DisplayableComponentVisitor;
protected:
	string name;
	tm dateInfo;
	bool isComplete; 
public:
	Task(tm when,  shared_ptr<DisplayableComponent> p, string s);
	void display() override;
	void accept(DisplayableComponentVisitor *) override;
};

class ToDoList :public DisplayableComponent {
	friend ToDoListVisitor;
	friend DisplayableComponentVisitor;
protected:
	string name;
public:
	ToDoList(shared_ptr<Calendar> p);
	shared_ptr<DisplayableComponent> addComponent(shared_ptr<DisplayableComponent>) override;
	virtual void accept(DisplayableComponentVisitor * visitor) override;
	void display() override;
	void markToDo(unsigned int);
	void markComplete(unsigned int);
};

