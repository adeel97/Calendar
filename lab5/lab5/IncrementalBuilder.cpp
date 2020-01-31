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
#include "IncrementalBuilder.h"

using namespace std;

shared_ptr<Calendar> IncrementalCalendarBuilder::buildCalendar(string name, size_t years) {
	currentCalendar = make_shared<Calendar>(name, years);
	tm currentDate = currentCalendar->currentDate;
	// construct each year in a recursive way, add each year as a child of the calendar
	
		tm y = currentCalendar->dateInfo;
		y.tm_mon = currentDate.tm_mon;

		currentCalendar->addComponent(buildYear(y, currentCalendar));
	return currentCalendar;
}

// you may decide to define this.
shared_ptr<DisplayableComponent> IncrementalCalendarBuilder::buildEvent(shared_ptr<DisplayableComponent> cal, string name, tm when, int recurrEvery, int recurrFor) {
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
shared_ptr<DisplayableComponent> IncrementalCalendarBuilder::getComponentByDate(shared_ptr<DisplayableComponent> cal, tm d, string granularity) {
	return nullptr;
}

shared_ptr<DisplayableComponent> IncrementalCalendarBuilder::buildDay(std::tm d, std::shared_ptr<DisplayableComponent> p) {
	
	shared_ptr<DisplayableComponent> day = make_shared<DisplayableDay>(d, p);
	return day;
}

shared_ptr<DisplayableComponent> IncrementalCalendarBuilder::buildMonth(std::tm d, std::shared_ptr<DisplayableComponent> p) {
	int index = d.tm_mon;
	shared_ptr<DisplayableComponent> m = make_shared<DisplayableMonth>(d, p, CalendarComponent::months[index], CalendarComponent::days[index]);
	tm currentDate = currentCalendar->currentDate;
	for (int i = 0; i < CalendarComponent::days[index]; ++i) { // for each day in the month
		if ((i == currentDate.tm_mday - 1) && d.tm_mon == currentDate.tm_mon) {
			m->addComponent(buildDay(currentDate, m));
		}
		else {
			m->addComponent(buildDay(d, m));
		}
		 // construct day and add as a child of the month
		++(d.tm_mday); // increment day of the month
		d.tm_wday = (d.tm_wday + 1) % CalendarComponent::DAYSINAWEEK; // increment weekday, reset to 0 if needed
	}
	return m;
}

shared_ptr<DisplayableComponent> IncrementalCalendarBuilder::buildMonthEmpty(std::tm d, std::shared_ptr<DisplayableComponent> p) {
	int index = d.tm_mon;
	shared_ptr<DisplayableComponent> m = make_shared<DisplayableMonth>(d, p, CalendarComponent::months[index], CalendarComponent::days[index]);
	m->addComponent(buildDay(d, m));
	return m;
}

shared_ptr<DisplayableComponent> IncrementalCalendarBuilder::buildYearEmpty(std::tm d, std::shared_ptr<DisplayableComponent> p) {
	shared_ptr<DisplayableComponent> year = make_shared<DisplayableYear>(d, p, false);
	shared_ptr<DisplayableComponent> month = year->addComponent(buildMonthEmpty(d, year));
	return year;
}


shared_ptr<DisplayableComponent> IncrementalCalendarBuilder::buildYear(std::tm d, std::shared_ptr<DisplayableComponent> p) {
	shared_ptr<DisplayableComponent> y = make_shared<DisplayableYear>(d, p, false);
	// construct each month and add it as a child of the year
	shared_ptr<DisplayableComponent> month = y->addComponent(buildMonth(d, y));
	// set week day of first day of the next month
	return y;
}

shared_ptr<DisplayableComponent> IncrementalCalendarBuilder::buildRemainingDays(std::shared_ptr<DisplayableComponent> month, tm d) {
	cout << "buildRemainingDays" << endl;
	int index = d.tm_mon;
	int dayIndex = currentCalendar->currentDate.tm_mday;
	int w = currentCalendar->currentDate.tm_wday;
	int wday = d.tm_wday;
	int firstDayWeek;

	if (dayIndex > wday) {
		firstDayWeek = (dayIndex - wday) % CalendarComponent::DAYSINAWEEK;
	}
	else {
		firstDayWeek = wday - dayIndex + 1;
	}

	if (d.tm_mon == currentCalendar->currentDate.tm_mon) {
		d.tm_wday = (d.tm_wday + CalendarComponent::days[index]) % CalendarComponent::DAYSINAWEEK;
	} 

	else if (d.tm_mon > currentCalendar->currentDate.tm_mon) {
		int diff = d.tm_mon - currentCalendar->currentDate.tm_mon;
		int totalDays = 0;
		for (int i = 0; i < diff; i++) {
			totalDays += CalendarComponent::days[currentCalendar->currentDate.tm_mon + i];
		}
		cout << totalDays << endl;
		int adjustment = totalDays % CalendarComponent::DAYSINAWEEK;
		if (adjustment < d.tm_wday) {
			d.tm_wday -= adjustment;
		}
		else {
			adjustment -= d.tm_wday;
		}
		d.tm_wday = d.tm_wday % CalendarComponent::DAYSINAWEEK;

	}

	else if (d.tm_mon < currentCalendar->currentDate.tm_mon) {
		int diff = currentCalendar->currentDate.tm_mon - d.tm_mon;
		
		int totalDays = 0;
		for (int i = 0; i<diff; i++) {
			totalDays += CalendarComponent::days[d.tm_mon + i];
		}
		cout << totalDays << endl;
		int adjustment = totalDays % CalendarComponent::DAYSINAWEEK;
		if (adjustment < w) {
			w -= adjustment;
		}
		else {
			w = adjustment - w;
		}
		
		w = w % CalendarComponent::DAYSINAWEEK;
	}

	
	


	DisplayableMonth * tempMonth = dynamic_cast<DisplayableMonth*>(month.get());

	tempMonth->dateInfo.tm_wday = firstDayWeek;


	d.tm_wday = w;

	for (int i = 1; i <= CalendarComponent::days[index]; ++i) { // for each day in the month
		if (month->getChild(i-1) == nullptr) {
			//cout << "i: " << i << endl;
			d.tm_mday = i; // set day of the month
			d.tm_wday = ((i - 1)+ d.tm_wday) % CalendarComponent::DAYSINAWEEK; // increment weekday, reset to 0 if needed

			month->addComponent(buildDay(d, month)); // construct day and add as a child of the month
		}

	}
	return month;
}

shared_ptr<DisplayableComponent> IncrementalCalendarBuilder::buildRemainingMonths(std::shared_ptr<DisplayableComponent> year, tm d) {
	cout << "buildRemainingMonths" << endl;
	int totalMonths = 12;
	int currMonth = currentCalendar->currentDate.tm_mon;	
	tm date = d;

	int dayIndex = currentCalendar->currentDate.tm_mday;
	int wday = d.tm_wday;

	int firstDayWeek;

	if (dayIndex > wday) {
		firstDayWeek = (dayIndex - wday) % CalendarComponent::DAYSINAWEEK;
	}
	else {
		firstDayWeek = wday - dayIndex + 1;
	}

	for (int i = 0; i < totalMonths; ++i) { // for each month in the year
		if (i != currMonth) {
			d.tm_mon = i; // set month of the year			
			year->addComponent(buildMonthEmpty(d, year)); // construct day and add as a child of the month
			d.tm_wday = (d.tm_wday + CalendarComponent::days[i]) % CalendarComponent::DAYSINAWEEK;
		} 
	}
	return year;
}

