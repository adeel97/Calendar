#include "stdafx.h"
#include "CalendarBuilder.h"
#include "DisplayableComponent.h"
#include "ToDoList.h"
using namespace std;

shared_ptr<DisplayableComponent> CalendarBuilder::toDoList() {
	if (currentCalendar->list == nullptr) {
		shared_ptr<DisplayableComponent> list = make_shared<ToDoList>(currentCalendar);
		currentCalendar->list = list;
		return list;
	}
	else {
		return currentCalendar->list;
	}
}

shared_ptr<DisplayableComponent> CalendarBuilder::buildTask(tm d, shared_ptr<DisplayableComponent> p, string taskName) {
	shared_ptr<DisplayableComponent> task = make_shared<Task>(d, p, taskName);
	currentCalendar->list->addComponent(task);
	return task;
}