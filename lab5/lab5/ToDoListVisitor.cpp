#include "stdafx.h"
#include "utilityfunctions.h"
#include "ToDoListVisitor.h"
using namespace std;

void ToDoListVisitor::visit(Calendar * calendar) {
	return;
}

void ToDoListVisitor::visit(DisplayableYear * year) {
	return;
}

void ToDoListVisitor::visit(DisplayableMonth * month) {
	return;
}

void ToDoListVisitor::visit(DisplayableDay * day) {
	return;
}

void ToDoListVisitor::visit(DisplayableEvent * event) {
	return;
}

void ToDoListVisitor::visit(ToDoList * list) {
	cout << "TODO List" << endl;
}

void ToDoListVisitor::visit(Task * task) {
	cout << "\t";
	cout << task->name << " " << dateString(task->dateInfo) << " " << timeString(task->dateInfo) << endl;
}