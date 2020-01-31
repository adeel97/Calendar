#include "stdafx.h"
#include "YearDisplayVisitor.h"
#include "utilityfunctions.h"
using namespace std;

void YearDisplayVisitor::visit(Calendar * calendar) {
	return;
}

void YearDisplayVisitor::visit(DisplayableYear * year) {
	cout << "Year " << year->dateInfo.tm_year + CalendarComponent::BASEYEAR << ":" << endl;
	return;
}

void YearDisplayVisitor::visit(DisplayableMonth * month) {
	currentMonth = month;
	cout << "\t";
	cout << currentMonth->dateInfo.tm_mon << ". " << CalendarComponent::months[currentMonth->dateInfo.tm_mon] << endl;
	return;
}

void YearDisplayVisitor::visit(DisplayableDay * day) {
	currentDay = day;
	return;
}

void YearDisplayVisitor::visit(DisplayableEvent * event) {
	cout << "\t\t";
	cout << dateString(currentDay->dateInfo);
	cout << " " << timeString(event->dateInfo);
	cout << " " << event->name << endl;
	return;
}