#pragma once
#include "stdafx.h"
#include "DisplayableComponentVisitor.h"
#include "Calendar.h"
#include "DisplayableYear.h"
#include "DisplayableMonth.h"
#include "DisplayableDay.h"
#include "DisplayableEvent.h"
using namespace std;

class CalendarDisplayVisitor : public DisplayableComponentVisitor {
public:
	virtual void visit(Calendar *) override;
	virtual void visit(DisplayableYear *) override;
	virtual void visit(DisplayableMonth *) override;
	virtual void visit(DisplayableDay *) override;
	virtual void visit(DisplayableEvent *) override;
	int weekDayIndex;
	int monthDays;
	Calendar * cal;
};
