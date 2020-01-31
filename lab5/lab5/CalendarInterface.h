/*
Author: Jon Shidal
Purpose: declare the user interface for our calendar
*/
#pragma once
#include "CalendarBuilder.h"
#include "CalendarComponent.h"
#include<memory>


class CalendarInterface {
	std::shared_ptr<Calendar> cal; // the calendar
	std::shared_ptr<CalendarBuilder> builder; // builder for managing construction of the calendar
	std::shared_ptr<DisplayableComponent> currentDisplay; // which component the user is currently viewing
	std::shared_ptr<ToDoList> toDoList;
public:
	// constructor
	// arguments: 1 - what type of builder? 2 - how many years to hold? 
	CalendarInterface(std::string builderType, std::string calendarName, size_t years);

	// calendar traversal functions
	void zoomIn(unsigned int index); // zoom in on a child of the current_display
	void zoomOut(); // zoom out to the parent of the current_display
	void run(); // display the current view to the user and prompt for input
	void changeEvent();
	void addEvent();
	void saveCalendar();
	void addEventHelper(string eventName,
		shared_ptr<DisplayableComponent> yearPointer, 
		shared_ptr<DisplayableComponent> monthPointer, 
		shared_ptr<DisplayableComponent> dayPointer, 
		shared_ptr<CalendarComponent> dayPointerCalComp, 
		unsigned int year, 
		unsigned int month, 
		unsigned int day, 
		unsigned int hour, 
		unsigned int minute, 
		unsigned int recurrEvery, 
		unsigned int recurrFor);
	void deleteEvent();
	shared_ptr<DisplayableComponent> jumpToDate();
	shared_ptr<DisplayableComponent> searchForEvent();
	shared_ptr<DisplayableComponent> restoreFromFile(string fileName);
};