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

#include "timetableshowconflictsspaceform.h"

#include "genetictimetable.h"

#include <qtextedit.h>
#include <qstring.h>

extern GeneticTimetable gt;
extern QString spaceConflictsString;

TimetableShowConflictsSpaceForm::TimetableShowConflictsSpaceForm()
 : TimetableShowConflictsSpaceForm_template()
{
	conflictsTextEdit->setText(spaceConflictsString);
}


TimetableShowConflictsSpaceForm::~TimetableShowConflictsSpaceForm()
{
}
