#include "stdafx.h"
#include "MonthDisplayVisitor.h"
using namespace std;

void MonthDisplayVisitor::visit(Calendar * calendar) {
	return;
}

void MonthDisplayVisitor::visit(DisplayableYear * year) {
	return;
}

void MonthDisplayVisitor::visit(DisplayableMonth * month) {
	monthDays = CalendarComponent::days[month->dateInfo.tm_mon];
	weekDayIndex = month->dateInfo.tm_wday;
	cout << CalendarComponent::months[month->dateInfo.tm_mon] << endl;
	for (int i = 0; i < CalendarComponent::daysoftheweek.size(); i++) {
	cout << setw(11) << left << CalendarComponent::daysoftheweek[i];
	}
	cout << endl;
	for (int i = 0; i < month->dateInfo.tm_wday; i++) {
	cout << setw(11) << " ";
	}
	return;
}

void MonthDisplayVisitor::visit(DisplayableDay * day) {
	currentDay = day;
	string mDayString = to_string(day->dateInfo.tm_mday);
	for (int i = 0; i < day->children.size(); i++) {
		mDayString.append("*");
	}
	cout << setw(11) << left << mDayString;
	if (weekDayIndex % 7 == 6 || (day->dateInfo.tm_mday == monthDays)) {
	cout << endl;
	}
	++weekDayIndex;
	return;
}

void MonthDisplayVisitor::visit(DisplayableEvent * event) {
	return;
}