/*
File rules.h

Copyright 2002, 2003 Lalescu Liviu.

This file is part of FET.

FET is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

FET is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FET; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef RULES_H
#define RULES_H

#include "genetictimetable_defs.h"
#include "timeconstraint.h"
#include "spaceconstraint.h"
#include "activity.h"
#include "studentsset.h"
#include "teacher.h"
#include "subject.h"
#include "equipment.h"
#include "room.h"

#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qptrvector.h>

/**
This class contains the processed input (all the information regarding
the faculty: teachers, students, activities, constraints, etc.)
<p>
Or: Structure that keeps a representation of the requirements for the
timetable (all the input)
*/
class Rules{
public:
	/**
	The number of hours per day
	*/
	int nHoursPerDay;

	/**
	The number of days per week
	*/
	int nDaysPerWeek;

	/**
	The days of the week (names)
	*/
	QString daysOfTheWeek[MAX_DAYS_PER_WEEK];

	/**
	The hours of the day (names). This includes also the last hour (+1)
	*/
	QString hoursOfTheDay[MAX_HOURS_PER_DAY+1];

	/**
	The number of hours per week
	*/
	int nHoursPerWeek;

	/**
	The list of teachers
	*/
	TeachersList teachersList;

	/**
	The list of subjects
	*/
	SubjectsList subjectsList;

	/**
	The list of students (groups and subgroups included).
	Remember that every identifier (year, group or subgroup) must be UNIQUE.
	*/
	StudentsYearsList yearsList;

	/**
	The list of activities
	*/
	ActivitiesList activitiesList;

	/**
	The list of equipments
	*/
	EquipmentsList equipmentsList;

	/**
	The list of rooms
	*/
	RoomsList roomsList;

	/**
	The list of time constraints
	*/
	TimeConstraintsList timeConstraintsList;

	/**
	The list of space constraints
	*/
	SpaceConstraintsList spaceConstraintsList;
	
	/**
	This is the array which specifies a fixed day
	for some activities.
	-1 means that this activity has no fixed day
	*/
	int16 fixedDay[MAX_ACTIVITIES];
	
	/**
	This is the array which specifies a fixed hour
	for some activities.
	-1 means that this activity has no fixed hour
	*/
	int16 fixedHour[MAX_ACTIVITIES];
	
	/**
	This array specifies, for each activity (1), a reference to
	another activity (2). The starting time of activity 1 is taken
	as the starting time of activity 2.
	-1 means that the activity is independent of other activities.
	*/
	int sameTime[MAX_ACTIVITIES];
	
	/**
	This array specifies, for each activity (1), a reference to
	another activity (2). The starting time of activity 1 is taken
	as the ending time of activity 2.
	-1 means that the activity is independent of other activities.
	*/
	int consecutiveTime[MAX_ACTIVITIES];
	
	//The following variables contain redundant data and are used internally
	////////////////////////////////////////////////////////////////////////
	int nInternalTeachers;
	Teacher* internalTeachersList[MAX_TEACHERS];

	int nInternalSubjects;
	Subject* internalSubjectsList[MAX_SUBJECTS];

	int nInternalSubgroups;
	StudentsSubgroup* internalSubgroupsList[MAX_TOTAL_SUBGROUPS];

	//For speed, I used here not pointers, but static copies.
	int nInternalActivities;
	Activity internalActivitiesList[MAX_ACTIVITIES];

	int nInternalEquipments;
	Equipment* internalEquipmentsList[MAX_EQUIPMENTS];

	int nInternalRooms;
	Room* internalRoomsList[MAX_ROOMS];

	int nInternalTimeConstraints;
	TimeConstraint* internalTimeConstraintsList[MAX_TIME_CONSTRAINTS];

	int nInternalSpaceConstraints;
	SpaceConstraint* internalSpaceConstraintsList[MAX_SPACE_CONSTRAINTS];

	/**
	This matrix represents true if a certain room has the necessary equipment
	to support an activity.
	This matrix is initialized with true in function "Rules::computeInternalStructure"
	This matrix is calculated by the "computeInternalStructure"
	function of all the ConstraintSubjectRequiresEquipments-s.
	This matrix is used only by ConstraintSubjectRequiresEquipments-s.
	New adding: 2 matrices, one for compulsory and the other for non-compulsory constraints.
	*/
	bool roomHasActivityEquipmentsCompulsory[MAX_ROOMS][MAX_ACTIVITIES];
	bool roomHasActivityEquipmentsNonCompulsory[MAX_ROOMS][MAX_ACTIVITIES];

	////////////////////////////////////////////////////////////////////////

	/**
	True if the rules have been initialized in some way (new or loaded).
	*/
	bool initialized;

	/**
	True if the internal structure was computed.
	*/
	bool internalStructureComputed;

	/**
	Initializes the rules (empty)
	*/
	void init();

	/**
	Internal structure initializer.
	<p>
	After any modification of the activities or students or teachers
	or restrictions, there is a need to call this subroutine
	*/
	void computeInternalStructure();

	/**
	Terminator - basically clears the memory for the restrictions.
	*/
	void kill();

	Rules();

	~Rules();

	/**
	Adds a new teacher to the list of teachers
	(if not already in the list).
	Returns false/true (unsuccessful/successful).
	*/
	bool addTeacher(Teacher* teacher);

	/**
	Returns the index of this teacher in the teachersList,
	or -1 for inexistent teacher.
	*/
	int searchTeacher(const QString& teacherName);

	/**
	Removes this teacher and all related activities and constraints.
	It returns false on failure. If successful, returns true.
	*/
	bool removeTeacher(const QString& teacherName);

	/**
	Adds a new subject to the list of subjects
	(if not already in the list).
	Returns false/true (unsuccessful/successful).
	*/
	bool addSubject(Subject* subject);

	/**
	Returns the index of this subject in the subjectsList,
	or -1 if not found.
	*/
	int searchSubject(const QString& subjectName);

	/**
	Removes this subject and all related activities and constraints.
	It returns false on failure.
	If successful, returns true.
	*/
	bool removeSubject(const QString& subjectName);

	/**
	Returns a pointer to the structure containing this student container
	(year, group or subgroup) or NULL.
	*/
	StudentsSet* searchStudentsSet(const QString& setName);

	/**
	Adds a new year of study to the academic structure
	*/
	bool addYear(StudentsYear* year);

	bool removeYear(const QString& yearName);

	/**
	Returns -1 if not found or the index of this year in the years list
	*/
	int searchYear(const QString& yearName);

	/**
	Adds a new group in a certain year of study to the academic structure
	*/
	bool addGroup(const QString& yearName, StudentsGroup* group);

	bool removeGroup(const QString& yearName, const QString& groupName);

	/**
	Returns -1 if not found or the index of this group in the groups list
	of this year.
	*/
	int searchGroup(const QString& yearName, const QString& groupName);

	/**
	Adds a new subgroup to a certain group in a certain year of study to
	the academic structure
	*/
	bool addSubgroup(const QString& yearName, const QString& groupName, StudentsSubgroup* subgroup);

	bool removeSubgroup(const QString& yearName, const QString& groupName, const QString& subgroupName);

	/**
	Returns -1 if not found or the index of the subgroup in the list of subgroups of this group
	*/
	int searchSubgroup(const QString& yearName, const QString& groupName, const QString& subgroupName);

	/**
	Adds a new indivisible activity (not split) to the list of activities.
	(It can add a subactivity of a split activity)
	Returns true if successful or false if the maximum
	number of activities was reached.
	There is automatically added a ConstraintActivityPreferredTime, if necessary
	*/
	bool addSimpleActivity(
		int _id,
		int _activityGroupId,
		const QStringList& _teachersNames,
		const QString& _subjectName,
		const QStringList& _studentsNames,
		int _duration, /*duration, in hours*/
		int _totalDuration,
		int _parity, /*parity: PARITY_WEEKLY or PARITY_BIWEEKLY*/
		int _preferredDay,
		int _preferredHour);

	/**
	Adds a new split activity to the list of activities.
	Returns true if successful or false if the maximum
	number of activities was reached.
	If _minDayDistance>0, there will automatically added a compulsory
	ConstraintMinNDaysBetweenActivities.
	Also, there are automatically added several ConstraintActivityPreferredTime, if necessary
	*/
	bool addSplitActivity(
		int _firstActivityId,
		int _activityGroupId,
		const QStringList& _teachersNames,
		const QString& _subjectName,
		const QStringList& _studentsNames,
		int _nSplits,
		int _totalDuration,
		int _durations[],
		int _parities[],
		int _minDayDistance,
		int _preferredDays[],
		int _preferredHours[]);

	/**
	Removes only the activity with this id.
	*/
	void removeActivity(int _id);

	/**
	If _activityGroupId==0, then this is a non-split activity
	(if >0, then this is a single sub-activity from a split activity.
	Removes this activity from the list of activities.
	For split activities, it removes all the sub-activities that are contained in it.
	*/
	void removeActivity(int _id, int _activityGroupId);

	/**
	Adds a new equipment (already allocated).
	Returns true on success, false for already existing equipments (same name).
	*/
	bool addEquipment(Equipment* eq);

	/**
	Returns -1 if not found or the index in the equipments list if found.
	*/
	int searchEquipment(const QString& equipmentName);

	/**
	Removes the equipment with this name.
	Returns true on success, false on failure (not found).
	*/
	bool removeEquipment(const QString& equipmentName);
	
	/**
	Adds a new room (already allocated).
	Returns true on success, false for already existing rooms (same name).
	*/
	bool addRoom(Room* rm);

	/**
	Returns -1 if not found or the index in the rooms list if found.
	*/
	int searchRoom(const QString& roomName);

	/**
	Removes the room with this name.
	Returns true on success, false on failure (not found).
	*/
	bool removeRoom(const QString& roomName);
	
	/**
	Adds a new time constraint (already allocated).
	Returns true on success, false for already existing constraints.
	*/
	bool addTimeConstraint(TimeConstraint* ctr);

	/**
	Removes this time constraint.
	Returns true on success, false on failure (not found).
	*/
	bool removeTimeConstraint(TimeConstraint* ctr);

	/**
	Adds a new space constraint (already allocated).
	Returns true on success, false for already existing constraints.
	*/
	bool addSpaceConstraint(SpaceConstraint* ctr);

	/**
	Removes this space constraint.
	Returns true on success, false on failure (not found).
	*/
	bool removeSpaceConstraint(SpaceConstraint* ctr);

	/**
	Reads the rules from the xml input file "filename".
	Returns true on success, false on failure (inexistent file or wrong format)
	*/
	bool read(const QString& filename);

	/**
	Write the rules to the xml input file "inputfile".
	*/
	void write(const QString& filename);
};

#endif
