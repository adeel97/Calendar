#pragma once
/*
Author: Jon Shidal
Purpose: declare the DisplayableDay class. This class may be a component of a calendar. It inherits from CalendarComponent and defines pure virtual functions
declared in DisplayableComponent
*/

#include "CalendarComponent.h"
// forward declarations needed to avoid circular includes, used to declare friends only
class FullCalendarBuilder;
class CalendarInterface;
class DisplayableMonth;
class CalendarDisplayVisitor;
class YearDisplayVisitor;
class MonthDisplayVisitor;
class DayDisplayVisitor;
class IncrementalCalendarBuilder;

class DisplayableDay : public CalendarComponent {
	friend class DisplayableMonth;
	friend FullCalendarBuilder;
	friend CalendarInterface;
	friend CalendarDisplayVisitor;
	friend YearDisplayVisitor;
	friend MonthDisplayVisitor;
	friend DayDisplayVisitor;
	friend IncrementalCalendarBuilder;
public:
	// 1st argument = start date/timeof the day, 2nd argument = its parent
	DisplayableDay(std::tm, std::shared_ptr<DisplayableComponent>);
	virtual void display() override;
	virtual void accept(DisplayableComponentVisitor *) override;

	// currently a leaf class, so no need to override addComponent()
protected:
	virtual shared_ptr<DisplayableComponent> addComponent(std::shared_ptr<DisplayableComponent>) override;
	virtual shared_ptr<DisplayableComponent> removeComponent(unsigned int index) override;
};

