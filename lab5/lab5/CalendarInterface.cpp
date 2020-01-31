#include "stdafx.h"
#include "CalendarInterface.h"
#include "FileCalendarBuilder.h"
#include "FullCalendarBuilder.h"
#include "DisplayableEvent.h"
#include "utilityfunctions.h"
#include "IncrementalBuilder.h"
#include <iostream>

using namespace std;

CalendarInterface::CalendarInterface(std::string builderType, std::string calendarName, size_t years) : builder(nullptr),cal(nullptr),currentDisplay(nullptr) {
	if (builderType == "full") {
		builder = make_shared<FullCalendarBuilder>();
		cal = builder->buildCalendar(calendarName, years);
		currentDisplay = cal;
	}
	else if (builderType == "file") {
		string fileName;
		builder = make_shared<FileCalendarBuilder>();
		shared_ptr<FileCalendarBuilder> fileBuilder = dynamic_pointer_cast<FileCalendarBuilder>(builder);
		std::cout << "File Name? (include .txt)" << endl;
		std::cin >> fileName;
		ifstream ifs(fileName);
		while (!ifs.is_open()) {
			std::cout << "Invalid File Name, make sure to inlude the .txt" << endl;
			std::cin >> fileName;
			ifs.open(fileName);
		}
		cal = fileBuilder->buildFromFile(ifs);
		currentDisplay = cal;
	}
	else if (builderType == "incremental") {
		builder = make_shared<IncrementalCalendarBuilder>();
		cal = builder->buildCalendar(calendarName, years);
		std::cout << "Built calendar" << endl;


		currentDisplay = cal->getChild(0)->getChild(cal->currentDate.tm_mon)->getChild(cal->currentDate.tm_mday - 1);
	}
}

void CalendarInterface::run() {
	// run until the user quits
	while (1) {
		// display the current display of the calendar
		currentDisplay->display();
		bool isEvent = false;
		// display options to the user and respond to user input accordingly
		


		DisplayableEvent * d = dynamic_cast<DisplayableEvent *>(currentDisplay.get()); // can only cast regular pointers, not shared_ptrs
		if (d == nullptr) { // the cast failed
			//do nothing
			isEvent = false;
		}
		else {
			//currentDiplay is an event
			isEvent = true;
			std::cout << "Delete event: d" << endl;
			std::cout << "Change event: c" << endl;
		}
		Calendar * calDisplay = dynamic_cast<Calendar *>(currentDisplay.get());
		DisplayableYear * yearDisplay = dynamic_cast<DisplayableYear *>(currentDisplay.get());
		DisplayableMonth * monthDisplay = dynamic_cast<DisplayableMonth *>(currentDisplay.get());
		DisplayableDay * dayDisplay = dynamic_cast<DisplayableDay *>(currentDisplay.get());
		DisplayableEvent * eventDisplay = dynamic_cast<DisplayableEvent *>(currentDisplay.get());


		ToDoList * list = dynamic_cast<ToDoList *>(currentDisplay.get());
		
		if (list != nullptr) {
			std::cout << "Add task: task, Mark Task complete: comp, back to calendar: back" << endl;
			string in;
			if (std::cin >> in) {
				if (in == "task") {
					unsigned int year;
					unsigned int month;
					unsigned int day;
					unsigned int hour;
					unsigned int minute;
					shared_ptr<DisplayableComponent> yearPointer;
					shared_ptr<DisplayableComponent> monthPointer;
					shared_ptr<DisplayableComponent> dayPointer;
					shared_ptr<CalendarComponent> dayPointerCalComp;
					cout << "Task Name?" << endl;
					string taskName;
					if (cin >> taskName) {
						unsigned int currYear = CalendarComponent::BASEYEAR + cal->dateInfo.tm_year;
						cout << "Year? (" << currYear << "-" << currYear + cal->children.size() - 1 << "): ";
						cin >> year;
						year = year - currYear;
						if (cal->getChild(year) == nullptr) {
							cout << "Year out  of bounds." << endl;
							return; //error
						}
						if (year >= 0 && year < cal->children.size()) {
							yearPointer = cal->getChild(year);
							cout << "Month? (1-" << yearPointer->children.size() << "): ";
							cin >> month;
							month--;
							if (month >= 0 && month < yearPointer->children.size()) {
								monthPointer = yearPointer->getChild(month);
								cout << "Day? (1-" << monthPointer->children.size() << "): ";
								cin >> day;
								day--;
								if (day >= 0 && day < monthPointer->children.size()) {
									dayPointer = monthPointer->getChild(day);
									cout << "Hour? (0-" << CalendarComponent::HOURSINADAY - 1 << "): ";
									cin >> hour;
									if (hour >= 0 && hour < CalendarComponent::HOURSINADAY) {
										cout << "Minute? (0-" << CalendarComponent::MINUTESINANHOUR - 1 << "): ";
										cin >> minute;
										if (minute >= 0 && minute < CalendarComponent::MINUTESINANHOUR) {
											dayPointerCalComp = dynamic_pointer_cast<CalendarComponent>(dayPointer);
											tm d = dayPointerCalComp->dateInfo;
											d.tm_hour = hour;
											d.tm_min = minute;

											builder->buildTask(d, currentDisplay, taskName);
										}
									}
								}
							}
						}
					}
				}
				else if (in == "comp") {
					unsigned int index;

					if (builder->toDoList()->children.size() > 0) {
						cout << "index? (0-" << builder->toDoList()->children.size() - 1 << ")" << endl;
						if (cin >> index) {
							if (index >= 0 && index < builder->toDoList()->children.size()) {
								shared_ptr<DisplayableComponent> thisList = builder->toDoList();
								ToDoList * theList = dynamic_cast<ToDoList*>(thisList.get());
								theList->markComplete(index);
							}
							else {
								cout << "invalid task index" << endl;
							}
						}
					}
					else {
						cout << "There are no tasks to delete" << endl;
					}
				}
				else if (in == "back") {
					currentDisplay = cal;
				}
			}
		}
		else if (calDisplay != nullptr || yearDisplay != nullptr || monthDisplay != nullptr || dayDisplay != nullptr || eventDisplay != nullptr) {
			cout << "zoom out: out, zoom in: in, quit: q, add event: e, jump to a date: j" << endl;
			cout << "search for event: s, todo list: t, save to file: save, restore from file: r" << endl;
			string in;
			if (cin >> in) {
				if (in == "in") {
					if (currentDisplay->children.size() > 0) {
						cout << "index? (0-" << currentDisplay->children.size() - 1 << ")" << endl;
						int index;
						if (cin >> index) {
							zoomIn(index);
						}
						else { //if extraction from cin fails
							cout << "Invalid Index" << endl;
							cin.clear(); //clear the stream
							cin.ignore(100, '\n');
							//go back and run it again
						}
					}
					else {
						cout << "Nothing to zoom in on" << endl;
					}
				}
				else if (in == "e") {
					addEvent();
				}
				else if (in == "s") {
					currentDisplay = searchForEvent();
				}
				else if (in == "j") {
					currentDisplay = jumpToDate();
				}
				else if (in == "out") {
					zoomOut();
				}
				else if (in == "d" && isEvent) {
					deleteEvent();
				}
				else if (in == "c" && isEvent) {
					changeEvent();
				}
				else if (in == "q") {
					break;
				}
				else if (in == "save") {
					saveCalendar();
				}
				else if (in == "t") {
					currentDisplay = builder->toDoList();
				}
				else if (in == "r") {
					string fileName;
					cout << "File name?" << endl;
					cin >> fileName;
					currentDisplay = restoreFromFile(fileName);
				}
				else {
					//do nothing
				}
			}

			else {
				//do nothing
				cout << "Failed to get input" << endl;
			}
		}
		
	}
}


void CalendarInterface::changeEvent() {
	string eventName;
	shared_ptr<DisplayableComponent> yearPointer;
	shared_ptr<DisplayableComponent> monthPointer;
	shared_ptr<DisplayableComponent> dayPointer;
	shared_ptr<CalendarComponent> dayPointerCalComp;
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minute;
	unsigned int recurrEvery;
	unsigned int recurrFor;
	string ans;

	shared_ptr<DisplayableComponent> toDelete = currentDisplay;

	DisplayableEvent * d = dynamic_cast<DisplayableEvent *>(toDelete.get()); // can only cast regular pointers, not shared_ptrs

	eventName = d->name;
	unsigned int yearPrev = d->dateInfo.tm_year;
	unsigned int monthPrev = d->dateInfo.tm_mon;
	unsigned int dayPrev = d->dateInfo.tm_mday;
	unsigned int hourPrev = d->dateInfo.tm_hour;
	unsigned int minutePrev = d->dateInfo.tm_min;



	unsigned int baseYear = CalendarComponent::BASEYEAR;
	unsigned int currYear = baseYear + cal->dateInfo.tm_year;
	cout << "Currently year is set to " << baseYear + yearPrev << ". Change year? y/n" << endl;
	cin >> ans;
	if (ans == "y") {
		cout << "New year? (" << currYear << "-" << currYear + cal->children.size() - 1 << "): ";
		cin >> year;
		year = year - currYear;
	}
	else if (ans == "n") {
		year = yearPrev - cal->dateInfo.tm_year;
	}
	else {
		return;
	}
	if (cal->getChild(year) == nullptr) {
		cout << "Year out  of bounds." << endl;
		return; //error
	}
	if (year >= 0 && year < cal->children.size()) {
		yearPointer = cal->getChild(year);
		ans = "";
		cout << "Currently month is set to " << monthPrev <<". Change month? y/n" << endl;
		cin >> ans;
		if (ans == "y") {
			cout << "New month? (1-" << yearPointer->children.size() << "): ";
			cin >> month;
			--month;
		}
		else if (ans == "n") {
			month = monthPrev;
		}
		else {
			return;
		}


		
		if (month >= 0 && month < yearPointer->children.size()) {
			monthPointer = yearPointer->getChild(month);
			ans = "";
			cout << "Currently day is set to " << dayPrev <<". Change day? y/n" << endl;
			
			cin >> ans;

			cout << "ans: " << ans << endl;
			if (ans == "y") {
				cout << "New day? (1-" << monthPointer->children.size() << "): ";
				cin >> day;
				--day;
			}
			else if (ans == "n") {
				day = dayPrev;
			}
			else {
				return;
			}
			if (day >= 0 && day < monthPointer->children.size()) {
				dayPointer = monthPointer->getChild(day);
				cout << "Currently, hour is set to " << hourPrev << ". Change hour? y/n" << endl;
				ans = "";
				cin >> ans;
				if (ans == "y") {
					cout << "New hour? (0-" << CalendarComponent::HOURSINADAY - 1 << "): ";
					cin >> hour;
				}
				else if (ans == "n") {
					hour = hourPrev;
				}
				else {
					return;
				}
				
				if (hour >= 0 && hour < CalendarComponent::HOURSINADAY) {
					cout << "Currently, minutes is set to " << minutePrev << ". Change minutes? y/n" << endl;
					ans = "";
					cin >> ans;

					if (ans == "y") {
						cout << "Minute? (0-" << CalendarComponent::MINUTESINANHOUR - 1 << "): ";
						cin >> minute;
					}
					else if (ans == "n") {
						minute = minutePrev;
					}
					else {
						return;
					}
					
					if (minute >= 0 && minute < CalendarComponent::MINUTESINANHOUR) {
						cout << "Recurr Every? ";
						cin >> recurrEvery;
						cout << endl;
						cout << "Recurr for? ";
						cin >> recurrFor;
						cout << endl;
						dayPointerCalComp = dynamic_pointer_cast<CalendarComponent>(dayPointer);
						tm d = dayPointerCalComp->dateInfo;
						d.tm_hour = hour;
						d.tm_min = minute;
						builder->buildEvent(cal, eventName, d, recurrEvery, recurrFor);
					}
				}
			}
		}
	}

	deleteEvent();

	return;


}



void CalendarInterface::deleteEvent() {
	shared_ptr<DisplayableComponent> toDelete = currentDisplay;
	currentDisplay = currentDisplay->getParent().lock();
	vector<shared_ptr<DisplayableComponent>> v = currentDisplay->children;
	vector<shared_ptr<DisplayableComponent>>::iterator itr = find(v.begin(), v.end(), toDelete);
	unsigned int indexToDelete;
	indexToDelete = distance(v.begin(), itr);
	DisplayableEvent * d = dynamic_cast<DisplayableEvent *>(toDelete.get());

	auto pairNums = cal->events.equal_range(d->name);
	for (auto it = pairNums.first; it != pairNums.second; it++) {
		if (it->second == toDelete) {
			cal->events.erase(it);
			break;
		}
	}
	currentDisplay->removeComponent(indexToDelete);
	return;
}

shared_ptr<DisplayableComponent> CalendarInterface::searchForEvent() {
	vector<shared_ptr<DisplayableComponent>> eventVector;
	string searchString;
	int numRes = 0;
	cout << "Event Name?" << endl;
	cin.ignore();
	getline(cin, searchString);
	for (auto it = cal->events.find(searchString); it != cal->events.end(); it++) {
		eventVector.push_back(it->second);
		++numRes;
	}
	if (eventVector.size() == 0) {
		cout << "Did not find event" << endl;

	}
	else if (eventVector.size() == 1) {
		cout << "Found one event" << endl;
		currentDisplay = eventVector[0];
	}
	else {
		int eventIndex;
		cout << "Found multiple events" << endl;
		cout << "Found events: " << endl;
		for (int i = 0; i < eventVector.size(); i++) {
			cout << "\t" << i << ".";
			DisplayableEvent * event = dynamic_cast<DisplayableEvent *>(eventVector[i].get());
			cout << dateString(event->dateInfo) << " " << timeString(event->dateInfo) << " " << event->name << endl;
		}
		cout << "index? (0-" << eventVector.size() - 1 << ")" << endl;
		cin >> eventIndex;
		if (eventIndex >= 0 && eventIndex < eventVector.size()) {
			currentDisplay = eventVector[eventIndex];
		}
		else {
			cout << "Invalid event index" << endl;
		}
	}
	return currentDisplay;
}

void CalendarInterface::zoomIn(unsigned int index) {
	shared_ptr<DisplayableComponent> temp = currentDisplay->getChild(index);
	IncrementalCalendarBuilder * icb = dynamic_cast<IncrementalCalendarBuilder*>(builder.get());
	
	if (icb == nullptr) {
		if (temp != nullptr) {
			currentDisplay = temp;
		}
	}

	else {
		//incremental functionality
		DisplayableYear * year = dynamic_cast<DisplayableYear*>(currentDisplay.get());
		DisplayableMonth * month = dynamic_cast<DisplayableMonth*>(currentDisplay.get());
		
		if (year != nullptr) { //we are zooming into a month
			auto monthPtr = currentDisplay->getChild(index);
			
			tm day = cal->dateInfo;
			day.tm_mon = index;
			currentDisplay = icb->buildRemainingDays(monthPtr, day);


		}

		else if (month != nullptr) {
			auto monthPtr = currentDisplay;

			tm day = cal->dateInfo;
			day.tm_mon = index;
			cout << "DAY WDAY: " << day.tm_wday << endl;
			currentDisplay = icb->buildRemainingDays(monthPtr, day);
		}

		

		





	}


	
	// if the child exists, update the current display to the child. Otherwise, the current view remains the same
	
}

void CalendarInterface::zoomOut() {
	
    // if the current display has a parent, update current display to the parent
	if (currentDisplay->getParent().lock() != nullptr) {
		IncrementalCalendarBuilder * icb = dynamic_cast<IncrementalCalendarBuilder*>(builder.get());
		if (icb == nullptr) { //not incremental 
			// create a shared_ptr from a weak_ptr, contributing to the objects reference count
			currentDisplay = currentDisplay->getParent().lock();
		}
		else { //incremental
			DisplayableDay * day = dynamic_cast<DisplayableDay*>(currentDisplay.get());
			DisplayableMonth * month = dynamic_cast<DisplayableMonth*>(currentDisplay.get());
			DisplayableYear * year = dynamic_cast<DisplayableYear*>(currentDisplay.get());

			if (day != nullptr) { //current display is a day
				auto monthPtr = currentDisplay->getParent().lock();
				DisplayableMonth * tempMonth = dynamic_cast<DisplayableMonth*>(monthPtr.get());
				currentDisplay = icb->buildRemainingDays(monthPtr, day->dateInfo);
			}
			if (month != nullptr) { //current display is a month
				auto yearPtr = currentDisplay->getParent().lock();
				DisplayableYear * tempYear = dynamic_cast<DisplayableYear*>(yearPtr.get());

				currentDisplay = icb->buildRemainingMonths(yearPtr, cal->dateInfo);
				


			} 
			
		}
		
	}
}

shared_ptr<DisplayableComponent> CalendarInterface::jumpToDate() {
	shared_ptr<DisplayableComponent> yearPointer;
	shared_ptr<DisplayableComponent> monthPointer;
	shared_ptr<DisplayableComponent> dayPointer;
	string granularity;
	tm d;
	unsigned int year;
	unsigned int month;
	unsigned int day;
	cout << "Year? (0-" << cal->children.size() - 1 << "): ";
	cin >> year;
	if (year >= 0 && year < cal->children.size()) {
		yearPointer = cal->getChild(year);
		cout << "Month? (0-" << yearPointer->children.size() - 1 << "): ";
		cin >> month;
		if (month >= 0 && month < yearPointer->children.size()) {
			monthPointer = yearPointer->getChild(month);
			cout << "Day? (0-" << monthPointer->children.size() - 1 << "): ";
			cin >> day;
			if (day >= 0 && day < monthPointer->children.size()) {
				dayPointer = monthPointer->getChild(day);
				cout << "Granularity? (day, month, or year)" << endl;
				cin >> granularity;
				DisplayableDay * thisDay = dynamic_cast<DisplayableDay*>(dayPointer.get());
				d = thisDay->dateInfo;
				if (granularity == "year") {
					return builder->getComponentByDate(cal, d, granularity);
				}
				else if (granularity == "month") {
					return builder->getComponentByDate(cal, d, granularity);
				}
				else if (granularity == "day") {
					return builder->getComponentByDate(cal, d, granularity);
				}
				else {
					cout << "Invalid granularity" << endl;
				}
			}
			else {
				cout << "Invalid Day" << endl;
			} 
		}
		else {
			cout << "Invalid Month" << endl;
		}
	}
	else {
		cout << "Invalid Year" << endl;
	}
}

void CalendarInterface::addEvent() {
	cout << "You have chosen to add an event" << endl;
	string eventName;
	shared_ptr<DisplayableComponent> yearPointer;
	shared_ptr<DisplayableComponent> monthPointer;
	shared_ptr<DisplayableComponent> dayPointer;
	shared_ptr<CalendarComponent> dayPointerCalComp;
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minute;
	unsigned int recurrEvery;
	unsigned int recurrFor;
	cout << "Event name: ";
	cin.ignore();
	getline(cin, eventName);
	unsigned int currYear = CalendarComponent::BASEYEAR + cal->dateInfo.tm_year;
	cout << "Year? (" << currYear << "-" << currYear + cal->children.size() - 1 << "): ";
	cin >> year;
	year = year - currYear;
	if (cal->getChild(year) == nullptr) {
		cout << "Year out  of bounds." << endl;
		return; //error
	}
	if (year >= 0 && year < cal->children.size()) {
		yearPointer = cal->getChild(year);
		cout << "Month? (1-" << yearPointer->children.size() << "): ";
		cin >> month;
		--month;
		if (month >= 0 && month < yearPointer->children.size()) {
			monthPointer = yearPointer->getChild(month);
			cout << "Day? (1-" << monthPointer->children.size() << "): ";
			cin >> day;
			--day;
			if (day >= 0 && day < monthPointer->children.size()) {
				dayPointer = monthPointer->getChild(day);
				cout << "Hour? (0-" << CalendarComponent::HOURSINADAY - 1 << "): ";
				cin >> hour;
				if (hour >= 0 && hour < CalendarComponent::HOURSINADAY) {
					cout << "Minute? (0-" << CalendarComponent::MINUTESINANHOUR - 1 << "): ";
					cin >> minute;
					if (minute >= 0 && minute < CalendarComponent::MINUTESINANHOUR) {
						cout << "Recurr Every? ";
						cin >> recurrEvery;
						cout << "Recurr for? ";
						cin >> recurrFor;
						cout << endl;
						dayPointerCalComp = dynamic_pointer_cast<CalendarComponent>(dayPointer);
						tm d = dayPointerCalComp->dateInfo;
						d.tm_hour = hour;
						d.tm_min = minute;
						cout << "Your event name is: " << eventName << ". It will take place on: " << CalendarComponent::daysoftheweek[d.tm_wday] << " " << d.tm_mon+1 << "/" << d.tm_mday << "/" << d.tm_year + CalendarComponent::BASEYEAR << " At: " << hour << ":" << minute << endl;
						builder->buildEvent(cal, eventName, d, recurrEvery, recurrFor);
					}
				}
			}
		}
	}

}


void CalendarInterface::addEventHelper(
	string eventName,
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
	unsigned int recurrFor) {

	dayPointerCalComp = dynamic_pointer_cast<CalendarComponent>(dayPointer);
	tm d = dayPointerCalComp->dateInfo;
	d.tm_hour = hour;
	d.tm_min = minute;
	cout << "Your event name is: " << eventName << ". It will take place on: " << CalendarComponent::daysoftheweek[d.tm_wday] << " " << d.tm_mon << "/" << d.tm_mday << "/" << d.tm_year + CalendarComponent::BASEYEAR << " At: " << hour << ":" << minute << endl;
	builder->buildEvent(cal, eventName, d, recurrEvery, recurrFor);
	return;
}

void CalendarInterface::saveCalendar() {
	string calName = cal->name;
	string fileName = calName;
	fileName.append(".txt");
	size_t years = cal->yearsToHold;
	multimap<string, shared_ptr<DisplayableComponent>> eventMap = cal->events;
	ofstream ofs(fileName);
	string firstLineString = calName + " " + to_string(years);
	ofs << firstLineString << endl;
	for (auto it = eventMap.begin(); it != eventMap.end(); it++) {
		shared_ptr<DisplayableComponent> eventPointer = it->second;
		shared_ptr<DisplayableEvent> thisEvent = dynamic_pointer_cast<DisplayableEvent>(eventPointer);
		tm eventWhen = thisEvent->dateInfo;
		ofs << thisEvent->name << " " << eventWhen.tm_mon << " " << eventWhen.tm_mday << " " << eventWhen.tm_wday << " " << eventWhen.tm_year << " " << eventWhen.tm_hour << " " << eventWhen.tm_min << endl;
	}
	cout << "Calendar saved" << endl;
}

shared_ptr<DisplayableComponent> CalendarInterface::restoreFromFile(string fileName) {
	return nullptr;
}
