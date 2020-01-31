#include "stdafx.h"
#include "CalendarDisplayVisitor.h"
using namespace std;

void CalendarDisplayVisitor::visit(Calendar * calendar) {
	cout << "Calendar: " << calendar->name << endl;
	cal = calendar;
	return;
}

void CalendarDisplayVisitor::visit(DisplayableYear * year) {
	int yearIndex = year->dateInfo.tm_year - cal->dateInfo.tm_year;
	cout << yearIndex << ". " << year->dateInfo.tm_year + CalendarComponent::BASEYEAR << endl;
}

void CalendarDisplayVisitor::visit(DisplayableMonth * month) {
	/*monthDays = CalendarComponent::days[month->dateInfo.tm_mon];
	weekDayIndex = month->dateInfo.tm_wday;
	cout << CalendarComponent::months[month->dateInfo.tm_mon] << endl;
	for (int i = 0; i < CalendarComponent::daysoftheweek.size(); i++) {
		cout << setw(3) << CalendarComponent::daofthwe[i];
	}
	cout << endl;
	for (int i = 0; i < month->dateInfo.tm_wday; i++) {
		cout << setw(3) << " ";
	}*/
	return;
}

void CalendarDisplayVisitor::visit(DisplayableDay * day) {
	/*cout << setw(3) << day->dateInfo.tm_mday;
	if (weekDayIndex % 7 == 6 || (day->dateInfo.tm_mday == monthDays)) {
		cout << endl;
	}
	++weekDayIndex;*/
	return;
}

void CalendarDisplayVisitor::visit(DisplayableEvent * event) {
	/*tm d = event->dateInfo;
	cout << d.tm_wday << " " << d.tm_mon + 1 << "/" << d.tm_mday << "/" << d.tm_year + CalendarComponent::BASEYEAR << endl;
	cout << event->name << " " << d.tm_hour << ":" << d.tm_min << endl;*/
	return;
}