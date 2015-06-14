//
//
// C++ Interface: $MODULE$
//
// Description:
//
//
// Author: Lalescu Liviu <liviu@lalescu.ro>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FETMAINFORM_H
#define FETMAINFORM_H

#include "fetmainform_template.h"

#include <qmutex.h>
#include <qthread.h>
#include <qmessagebox.h>

class FetMainForm : public FetMainForm_template
{
public:
	FetMainForm();
	~FetMainForm();

	void fileNew();
	void fileSave();
	void fileSaveAs();
	void fileExit();
	void fileOpen();

	void dataDays();
	void dataHours();
	void dataTeachers();
	void dataSubjects();
	void dataYears();
	void dataGroups();
	void dataSubgroups();
	void dataActivities();
	void dataEquipments();
	void dataRooms();
	void dataTimeConstraints();
	void dataSpaceConstraints();
	
	void dataTimeConstraints22ActivitiesConsecutive();
	void dataTimeConstraints22ActivitiesGrouped();
	void dataTimeConstraints2ActivitiesPreferredTimes();
	void dataTimeConstraints2ActivitiesSameStartingTime();

	void helpAbout();
	void helpFAQ();

	void timetableAllocateHours();
	void timetableViewStudents();
	void timetableViewTeachers();
	void timetableShowConflictsTime();
	void timetableAllocateRooms();
	void timetableViewRooms();
	void timetableShowConflictsSpace();
	void timetableExport();
	
	void languageEnglish();
	void languageFrench();
	void languageRomanian();
	
	void parametersPopulationNumber();
};

#endif
