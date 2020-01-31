/*
Author: Jon Shidal
Purpose: create a CalendarInterface object and call run
*/

#include "stdafx.h"
#include "CalendarInterface.h"

using namespace std;

int main(int argc, char * argv[]) {
	string builder = argv[1];
	if (builder == "full") {
		string calName;
		unsigned int years;
		cout << "Calendar name?" << endl;
		cin >> calName;
		cout << "Number of years?" << endl;
		cin >> years;
		CalendarInterface ci("full", calName, years);
		ci.run();
		return 0;
	}
	else if (builder == "file") {
		CalendarInterface ci("file", "", 0);
		ci.run();
		return 0;
	}
	else if (builder == "incremental") {
		string calName;
		cout << "Calendar name?" << endl;
		cin >> calName;
		CalendarInterface ci("incremental", calName, 1);
		ci.run();
		return 0;
	}
	else {
		cout << "Invalid Calendar builder" << endl;
		return 1;
	}
	
}