#include "stdafx.h"
#include "DisplayableMonth.h"
#include "DisplayableDay.h"
#include "DisplayableComponentVisitor.h"
#include "MonthDisplayVisitor.h"
#include <iostream>

using namespace std;

DisplayableMonth::DisplayableMonth(std::tm d, std::shared_ptr<DisplayableComponent> p, string monthname, unsigned int numdays) : CalendarComponent(d, p), name(monthname), numberOfDays(numdays) {
	// initialize children vector, one spot for each day
	for (size_t i = 0; i < numberOfDays; ++i) {
		children.push_back(nullptr);
	}
}

void DisplayableMonth::accept(DisplayableComponentVisitor * visitor) {
	visitor->visit(this);
	for (int i = 0; i < children.size(); i++) {
		if (children[i] != nullptr) {
			children[i]->accept(visitor);
		}
	}
}

void DisplayableMonth::display() {
	DisplayableComponentVisitor * visitor = new MonthDisplayVisitor();
	accept(visitor);
}

shared_ptr<DisplayableComponent> DisplayableMonth::addComponent(shared_ptr<DisplayableComponent> comp) {
	// try to dynamically cast comp to a pointer to a DisplayableDay, will fail if the DisplayableComponent is not a day
	DisplayableDay* d = dynamic_cast<DisplayableDay *>(comp.get()); // can only cast regular pointers, not shared_ptrs
	if (d == nullptr) { // the cast failed
		return nullptr;
	}
	// otherwise, add the day to the correct location
	int dayOfMonth = d->dateInfo.tm_mday - 1;
	if (children[dayOfMonth] == nullptr) { // day does not already exist
		children[dayOfMonth] = comp;
		return comp;
	}
	else {  // day already exist, return existing day
		return children[dayOfMonth];
	}
}