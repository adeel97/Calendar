#pragma once

#include "CalendarComponent.h"
#include <string>

// forward declarations needed to avoid circular includes, used to declare friends only
class FullCalendarBuilder;
class CalendarInterface;
class DisplayableDay;
class CalendarDisplayVisitor;
class YearDisplayVisitor;
class DayDisplayVisitor;
class EventDisplayVisitor;
class IncrementalCalendarBuilder;

class DisplayableEvent : public CalendarComponent {
	friend class DisplayableDay;
	friend FullCalendarBuilder;
	friend CalendarInterface;
	friend CalendarDisplayVisitor;
	friend YearDisplayVisitor;
	friend DayDisplayVisitor;
	friend EventDisplayVisitor;
	friend IncrementalCalendarBuilder;
	//string eventName;
public:
	bool operator< (const DisplayableEvent &) const;
	// 1st argument = start date/timeof the day, 2nd argument = its parent
	//DisplayableEvent(tm d, shared_ptr<DisplayableComponent> p, string eventName) : CalendarComponent(d, p) {} ;
	DisplayableEvent(tm d, shared_ptr<DisplayableComponent> p, string name);
	virtual void display() override;
	virtual void accept(DisplayableComponentVisitor *) override;
	// currently a leaf class, so no need to override addComponent()
private:
	string name;

};


