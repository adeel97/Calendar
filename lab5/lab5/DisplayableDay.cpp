#include "stdafx.h"
#include "DisplayableDay.h"
#include "DisplayableComponentVisitor.h"
#include "DayDisplayVisitor.h"
#include "DisplayableEvent.h"
#include <iostream>

using namespace std;

DisplayableDay::DisplayableDay(std::tm d, std::shared_ptr<DisplayableComponent> p) : CalendarComponent(d, p) { }

void DisplayableDay::display() {
	DisplayableComponentVisitor * visitor = new DayDisplayVisitor();
	accept(visitor);
}

void DisplayableDay::accept(DisplayableComponentVisitor * visitor) {
	visitor->visit(this);
	for (int i = 0; i < children.size(); i++) {
		if (children[i] != nullptr) {
			children[i]->accept(visitor);
		}
	}
}

shared_ptr<DisplayableComponent> DisplayableDay::addComponent(shared_ptr<DisplayableComponent> event) {
	DisplayableEvent * d = dynamic_cast<DisplayableEvent *>(event.get()); // can only cast regular pointers, not shared_ptrs
	if (d == nullptr) { // the cast failed
		return nullptr;
	}
	else {
		children.push_back(event);
		sort(children.begin(), children.end());
		return event;
	}
}

//we need one to change the component
shared_ptr<DisplayableComponent> DisplayableDay::removeComponent(unsigned int index) {
	if (index < children.size()) {
		DisplayableEvent * d = dynamic_cast<DisplayableEvent *>(children[index].get()); // can only cast regular pointers, not shared_ptrs
		if (d == nullptr) { // the cast failed
			return nullptr;
		}
		else {
			children.erase(children.begin() + index);
			return nullptr;
		}
	}
	else {
		return nullptr;
	}
}
