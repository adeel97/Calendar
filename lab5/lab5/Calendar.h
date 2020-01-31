#pragma once

#include "CalendarComponent.h"
#include "ToDoList.h"
using namespace std;
// forward declarations needed to avoid circular includes, used to declare friends only
class FullCalendarBuilder;
class CalendarInterface;
class CalendarDisplayVisitor;
class FileCalendarBuilder;
class CalendarBuilder;
class IncrementalCalendarBuilder;

class Calendar : public CalendarComponent, enable_shared_from_this<Calendar> {
	// friends
	friend FullCalendarBuilder;
	friend CalendarInterface;
	friend CalendarDisplayVisitor;
	friend FileCalendarBuilder;
	friend CalendarBuilder;
	friend IncrementalCalendarBuilder;
protected:
	multimap<string, shared_ptr<DisplayableComponent>> events;
	std::string name;
	size_t yearsToHold;
	std::tm currentDate; // current date and time
						 // dateInfo is the start date and time

						 // Calendar contains years, so override accordingly
	virtual std::shared_ptr<DisplayableComponent> addComponent(std::shared_ptr<DisplayableComponent>) override;
	shared_ptr<DisplayableComponent>  list;
public:
	// arguments: name of the calendar, length of the calendar in years
	Calendar(std::string n, size_t y);
	// inherited methods
	virtual void display() override;
	virtual void accept(DisplayableComponentVisitor *) override;
	shared_ptr<DisplayableComponent> toDoList();
};