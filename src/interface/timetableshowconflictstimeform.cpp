//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Lalescu Liviu <liviu@lalescu.ro>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "timetableshowconflictstimeform.h"

#include "genetictimetable.h"

#include <qtextedit.h>
#include <qstring.h>

extern GeneticTimetable gt;
extern QString timeConflictsString;

TimetableShowConflictsTimeForm::TimetableShowConflictsTimeForm()
 : TimetableShowConflictsTimeForm_template()
{
	conflictsTextEdit->setText(timeConflictsString);
}


TimetableShowConflictsTimeForm::~TimetableShowConflictsTimeForm()
{
}
