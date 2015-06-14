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

	void dataInstitutionName();
	void dataComments();
	void dataDays();
	void dataHours();
	void dataTeachers();
	void dataSubjects();
	void dataSubjectTags();
	void dataYears();
	void dataGroups();
	void dataSubgroups();
	void dataActivities();
	void dataEquipments();
	void dataRooms();
	void dataAllTimeConstraints();
	void dataAllSpaceConstraints();

	void dataSpaceConstraintsBasicCompulsorySpace();
	void dataSpaceConstraintsTeacherRequiresRoom();
	void dataSpaceConstraintsRoomNotAvailable();
	void dataSpaceConstraintsActivityPreferredRoom();
	void dataSpaceConstraintsMinimizeNumberOfRoomsForStudents();
	void dataSpaceConstraintsRoomTypeNotAllowedSubjects();
	void dataSpaceConstraintsSubjectRequiresEquipments();
	void dataSpaceConstraintsSubjectSubjectTagRequireEquipments();
	void dataSpaceConstraintsActivitiesSameRoom();
	void dataSpaceConstraintsActivityPreferredRooms();
	
	void dataTimeConstraints2ActivitiesConsecutive();
	void dataTimeConstraints2ActivitiesGrouped();
	void dataTimeConstraintsActivitiesPreferredTimes();
	void dataTimeConstraintsActivitiesSameStartingTime();
	void dataTimeConstraintsActivitiesSameStartingHour();
	void dataTimeConstraintsTeacherNotAvailable();
	void dataTimeConstraintsBasicCompulsoryTime();
	void dataTimeConstraintsStudentsSetNotAvailable();
	void dataTimeConstraintsBreak();
	void dataTimeConstraintsTeacherMaxDaysPerWeek();
	void dataTimeConstraintsTeachersNoMoreThanXHoursContinuously();
	void dataTimeConstraintsTeachersNoMoreThanXHoursDaily();
	void dataTimeConstraintsTeachersSubgroupsNoMoreThanXHoursDaily();
	void dataTimeConstraintsActivityPreferredTime();
	void dataTimeConstraintsStudentsSetNoGaps();
	void dataTimeConstraintsStudentsNoGaps();
	void dataTimeConstraintsTeachersNoGaps();
	void dataTimeConstraintsStudentsEarly();
	void dataTimeConstraintsStudentsSetIntervalMaxDaysPerWeek();
	void dataTimeConstraintsStudentsSetNHoursDaily();
	void dataTimeConstraintsStudentsNHoursDaily();
	void dataTimeConstraintsActivityEndsDay();
	void dataTimeConstraintsActivitiesNotOverlapping();
	void dataTimeConstraintsMinNDaysBetweenActivities();
	void dataTimeConstraintsActivityPreferredTimes();

	void helpAbout();
	void helpFAQ();

	void timetableAllocateHours();
	void timetableViewStudents();
	void timetableViewTeachers();
	void timetableViewStudentsWithRooms();
	void timetableViewTeachersWithRooms();
	void timetableViewStudentsWithRooms2();
	void timetableViewTeachersWithRooms2();
	void timetableShowConflictsTime();
	void timetableAllocateRooms();
	void timetableViewRooms();
	void timetableViewRooms2();
	void timetableShowConflictsSpace();
	void timetableShowConflictsTimeSpace();
	void timetableAllocateHoursRooms();
	void timetableExport();
	
	void languageEnglish();
	void languageFrench();
	void languageCatalan();
	void languageRomanian();
	
	void parametersPopulationNumber();
};

#endif
