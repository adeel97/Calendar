/*
Author: Jon Shidal
Purpose: Define full calendar builder class, responsible for building and managing the structure of the calendar
*/
#include "stdafx.h"
#include "FullCalendarBuilder.h"
#include "CalendarComponent.h"
#include "DisplayableDay.h"
#include "DisplayableMonth.h"
#include "DisplayableYear.h"
#include "DisplayableEvent.h"
#include "utilityfunctions.h"

using namespace std;

shared_ptr<Calendar> FullCalendarBuilder::buildCalendar(string name, size_t years) {
	currentCalendar = make_shared<Calendar>(name, years);
	// construct each year in a recursive way, add each year as a child of the calendar
	for (unsigned int i = 0; i < years; ++i) {
		tm y = currentCalendar->dateInfo;
		y.tm_year += i;
		y.tm_wday = (y.tm_wday + CalendarComponent::DAYSINAYEAR * i) % CalendarComponent::DAYSINAWEEK; // calculate day of the week for first day of the year
		currentCalendar->addComponent(buildYear(y, currentCalendar));
	}
	return currentCalendar;
}

// you may decide to define this.
shared_ptr<DisplayableComponent> FullCalendarBuilder::buildEvent(shared_ptr<DisplayableComponent> cal, string name, tm when, int recurrEvery, int recurrFor) {
	Calendar * calendar = dynamic_cast<Calendar *>(cal.get());
	for (unsigned int i = 0; i <= recurrFor; i++) {
		int addedDays = i * recurrEvery;
		tm eventTime = addDays(when, addedDays);
		int yearIndex = eventTime.tm_year - calendar->dateInfo.tm_year;
		if (yearIndex < cal->children.size()) {
			shared_ptr<DisplayableComponent> year = cal->getChild(yearIndex);
			shared_ptr<DisplayableComponent> month = year->getChild(eventTime.tm_mon);
			shared_ptr<DisplayableComponent> day = month->getChild(eventTime.tm_mday - 1);
			shared_ptr<DisplayableComponent> newEvent = make_shared<DisplayableEvent>(eventTime, day, name);
			day->addComponent(newEvent);
			pair<string, shared_ptr<DisplayableComponent>> pair = make_pair(name, newEvent);
			calendar->events.insert(pair);
		}
	}
	return cal;
}

// you may decide to define this.
shared_ptr<DisplayableComponent> FullCalendarBuilder::getComponentByDate(shared_ptr<DisplayableComponent> cal, tm d, string granularity) {
	Calendar * calendar = dynamic_cast<Calendar*>(cal.get());
	shared_ptr<DisplayableComponent> yearComp = cal->getChild(d.tm_year - calendar->dateInfo.tm_year);
	shared_ptr<DisplayableComponent> monthComp = yearComp->getChild(d.tm_mon);
	shared_ptr<DisplayableComponent> dayComp = monthComp->getChild(d.tm_mday - 1);
	if (granularity == "year") {
		return yearComp;
	}
	else if (granularity == "month") {
		return monthComp;
	}
	else if (granularity == "day") {
		return dayComp;
	}
	return cal;
}

shared_ptr<DisplayableComponent> FullCalendarBuilder::buildDay(std::tm d, std::shared_ptr<DisplayableComponent> p) {
	shared_ptr<DisplayableComponent> day = make_shared<DisplayableDay>(d, p);
	return day;
}

shared_ptr<DisplayableComponent> FullCalendarBuilder::buildMonth(std::tm d, std::shared_ptr<DisplayableComponent> p) {
	int index = d.tm_mon;
	shared_ptr<DisplayableComponent> m = make_shared<DisplayableMonth>(d, p, CalendarComponent::months[index], CalendarComponent::days[index]);
	for (int i = 0; i < CalendarComponent::days[index]; ++i) { // for each day in the month
		m->addComponent(buildDay(d, m)); // construct day and add as a child of the month
		++(d.tm_mday); // increment day of the month
		d.tm_wday = (d.tm_wday + 1) % CalendarComponent::DAYSINAWEEK; // increment weekday, reset to 0 if needed
	}
	return m;
}
shared_ptr<DisplayableComponent> FullCalendarBuilder::buildYear(std::tm d, std::shared_ptr<DisplayableComponent> p) {
	shared_ptr<DisplayableComponent> y = make_shared<DisplayableYear>(d,p,false);
	// construct each month and add it as a child of the year
	for (unsigned int i = 0; i < CalendarComponent::MONTHS; ++i) {
		d.tm_mon = i;
		y->addComponent(buildMonth(d, y));
		// set week day of first day of the next month
		d.tm_wday = (d.tm_wday + CalendarComponent::days[i]) % CalendarComponent::DAYSINAWEEK; 
	}
	return y;
}