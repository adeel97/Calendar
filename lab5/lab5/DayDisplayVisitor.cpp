#include "stdafx.h"
#include "utilityfunctions.h"
#include "DayDisplayVisitor.h"
using namespace std;

void DayDisplayVisitor::visit(Calendar * calendar) {
	return;
}

void DayDisplayVisitor::visit(DisplayableYear * year) {
	return;
}

void DayDisplayVisitor::visit(DisplayableMonth * month) {
	return;
}

void DayDisplayVisitor::visit(DisplayableDay * day) {
	cout << "\t";
	cout << CalendarComponent::daysoftheweek[day->dateInfo.tm_wday] << " " << dateString(day->dateInfo) << endl;;
}

void DayDisplayVisitor::visit(DisplayableEvent * event) {
	cout << "\t";
	cout << timeString(event->dateInfo) << " " << event->name << endl;
	return;
}