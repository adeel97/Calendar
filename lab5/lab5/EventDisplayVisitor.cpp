#include "stdafx.h"
#include "utilityfunctions.h"
#include "EventDisplayVisitor.h"
#include "CalendarComponent.h"
using namespace std;

void EventDisplayVisitor::visit(Calendar * calendar) {
	return;
}

void EventDisplayVisitor::visit(DisplayableYear * year) {
	return;
}

void EventDisplayVisitor::visit(DisplayableMonth * month) {
	return;
}

void EventDisplayVisitor::visit(DisplayableDay * day) {
	return;
}

void EventDisplayVisitor::visit(DisplayableEvent * event) {
	cout << "\t";
	cout << event->daysoftheweek[event->dateInfo.tm_wday] << " " << dateString(event->dateInfo) << endl;
	cout << "\t";
	cout << timeString(event->dateInfo) << " " << event->name << endl;
	return;
}