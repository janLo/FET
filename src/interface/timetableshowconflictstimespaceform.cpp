//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Lalescu Liviu <Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)>, (C) 2005
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
