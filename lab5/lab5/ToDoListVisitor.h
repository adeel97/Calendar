#pragma once

#include "stdafx.h"
#include "DisplayableComponentVisitor.h"
#include "Calendar.h"
#include "DisplayableYear.h"
#include "DisplayableMonth.h"
#include "DisplayableDay.h"
#include "DisplayableEvent.h"
#include "ToDoList.h"
using namespace std;


class ToDoListVisitor : public DisplayableComponentVisitor {
public:
	virtual void visit(Calendar *) override;
	virtual void visit(DisplayableYear *) override;
	virtual void visit(DisplayableMonth *) override;
	virtual void visit(DisplayableDay *) override;
	virtual void visit(DisplayableEvent *) override;
	void visit(ToDoList *);
	void visit(Task *);
	int weekDayIndex;
	int monthDays;
	DisplayableYear * year;
	DisplayableMonth * currentMonth;
	DisplayableDay * currentDay;
};