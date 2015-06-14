//
//
// C++ Interface: $MODULE$
//
// Description:
//
//
// Author: Lalescu Liviu <Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)>, (C) 2003
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
	void dataBuildings();
	void dataRooms();
	void dataAllTimeConstraints();
	void dataAllSpaceConstraints();

	void dataSpaceConstraintsBasicCompulsorySpace();
	void dataSpaceConstraintsTeacherRequiresRoom();
	void dataSpaceConstraintsTeacherSubjectRequireRoom();
	void dataSpaceConstraintsRoomNotAvailable();
	void dataSpaceConstraintsActivityPreferredRoom();
	void dataSpaceConstraintsMinimizeNumberOfRoomsForStudents();
	void dataSpaceConstraintsMinimizeNumberOfRoomsForTeachers();
	void dataSpaceConstraintsRoomTypeNotAllowedSubjects();
	void dataSpaceConstraintsSubjectRequiresEquipments();
	void dataSpaceConstraintsSubjectSubjectTagRequireEquipments();
	void dataSpaceConstraintsActivitiesSameRoom();
	void dataSpaceConstraintsActivityPreferredRooms();
	void dataSpaceConstraintsSubjectPreferredRoom();
	void dataSpaceConstraintsSubjectSubjectTagPreferredRoom();
	void dataSpaceConstraintsSubjectPreferredRooms();
	void dataSpaceConstraintsSubjectSubjectTagPreferredRooms();
	void dataSpaceConstraintsMaxBuildingChangesPerDayForTeachers();
	void dataSpaceConstraintsMaxBuildingChangesPerDayForStudents();
	void dataSpaceConstraintsMaxRoomChangesPerDayForTeachers();
	void dataSpaceConstraintsMaxRoomChangesPerDayForStudents();
	
	void dataTimeConstraints2ActivitiesConsecutive();
	void dataTimeConstraints2ActivitiesOrdered();
	void dataTimeConstraints2ActivitiesGrouped();
	void dataTimeConstraintsActivitiesPreferredTimes();
	void dataTimeConstraintsActivitiesSameStartingTime();
	void dataTimeConstraintsActivitiesSameStartingHour();
	void dataTimeConstraintsTeacherNotAvailable();
	void dataTimeConstraintsBasicCompulsoryTime();
	void dataTimeConstraintsStudentsSetNotAvailable();
	void dataTimeConstraintsBreak();
	void dataTimeConstraintsTeacherMaxDaysPerWeek();
	void dataTimeConstraintsTeachersMaxHoursContinuously();
	void dataTimeConstraintsTeachersMaxHoursDaily();
	void dataTimeConstraintsTeachersSubgroupsMaxHoursDaily();
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
	void dataTimeConstraintsTeachersSubjectTagsMaxHoursContinuously();
	void dataTimeConstraintsTeachersSubjectTagMaxHoursContinuously();

	void helpAbout();
	void helpFAQ();
	void helpTimeConstraints();
	void helpTimeConstraintsSubtags();

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
	void timetableExportXmlHtml();
	void timetableExportiCalTeachers();
	void timetableExportiCalStudents();
	void timetableExportiCalTeachersWithRooms1();
	void timetableExportiCalStudentsWithRooms1();
	void timetableExportiCalTeachersWithRooms2();
	void timetableExportiCalStudentsWithRooms2();
	
	void languageEnglish();
	void languageFrench();
	void languageCatalan();
	void languageRomanian();
	void languageMalay();
	void languagePolish();
	void languageTurkish();
	void languageDutch();
	
	void parametersPopulationNumber();
};

#endif
