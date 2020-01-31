#pragma once
#include "stdafx.h"
#include "Calendar.h"
#include "DisplayableDay.h"
#include "DisplayableEvent.h"
#include "DisplayableMonth.h"
#include "DisplayableYear.h"
#include "ToDoList.h"


using namespace std;

class DisplayableComponentVisitor {
public:
	virtual void visit(Calendar *) = 0;
	virtual void visit(DisplayableDay *) = 0;
	virtual void visit(DisplayableMonth *) = 0;
	virtual void visit(DisplayableYear *) = 0;
	virtual void visit(DisplayableEvent *) = 0;
	
};
