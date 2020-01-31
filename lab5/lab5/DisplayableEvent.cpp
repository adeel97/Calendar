#include "stdafx.h"
#include "DisplayableComponentVisitor.h"
#include "DisplayableEvent.h"
#include "EventDisplayVisitor.h"
#include <iostream>

using namespace std;

//DisplayableEvent::DisplayableEvent() : CalendarComponent

DisplayableEvent::DisplayableEvent(tm d, shared_ptr<DisplayableComponent> p, string name) : CalendarComponent(d, p), name(name) { }

void DisplayableEvent::accept(DisplayableComponentVisitor * visitor) {
	visitor->visit(this);
}


void DisplayableEvent::display() {
	DisplayableComponentVisitor * visitor = new EventDisplayVisitor();
	accept(visitor);
}

bool DisplayableEvent::operator< (const DisplayableEvent & event) const {
	if (event.dateInfo.tm_hour < dateInfo.tm_hour) {
		return true;
	}
	else if (event.dateInfo.tm_hour == dateInfo.tm_hour) {
		return (event.dateInfo.tm_min < dateInfo.tm_min);
	}
	else {
		return false;
	}
}



