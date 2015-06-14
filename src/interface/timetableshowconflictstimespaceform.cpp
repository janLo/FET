//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Lalescu Liviu <liviu@lalescu.ro>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "timetableshowconflictstimespaceform.h"

#include "genetictimetable.h"

#include <qtextedit.h>
#include <qstring.h>

extern GeneticTimetable gt;
extern QString timeSpaceConflictsString;

TimetableShowConflictsTimeSpaceForm::TimetableShowConflictsTimeSpaceForm()
 : TimetableShowConflictsTimeSpaceForm_template()
{
	conflictsTextEdit->setText(timeSpaceConflictsString);
}


TimetableShowConflictsTimeSpaceForm::~TimetableShowConflictsTimeSpaceForm()
{
}
