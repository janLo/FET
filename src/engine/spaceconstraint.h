/*
File spaceconstraint.h
*/

/*
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

#ifndef SPACECONSTRAINT_H
#define SPACECONSTRAINT_H

#include "timetable_defs.h"

#include <QString>
#include  <QList>
#include <QStringList>

class Rules;
class SpaceChromosome;
class SpaceConstraint;
class Activity;
class Teacher;
class Subject;
class ActivityTag;
class StudentsSet;
class Equipment;
class Building;
class Room;
class Solution;

typedef QList<SpaceConstraint*> SpaceConstraintsList;

const int CONSTRAINT_GENERIC_SPACE										=1000; //time constraints are beginning from 1

const int CONSTRAINT_BASIC_COMPULSORY_SPACE								=1001; //space constraints from 1001
const int CONSTRAINT_ROOM_NOT_AVAILABLE_TIMES							=1002;

const int CONSTRAINT_ACTIVITY_PREFERRED_ROOM							=1003;
const int CONSTRAINT_ACTIVITY_PREFERRED_ROOMS							=1004;

const int CONSTRAINT_STUDENTS_SET_HOME_ROOM								=1005;
const int CONSTRAINT_STUDENTS_SET_HOME_ROOMS							=1006;

const int CONSTRAINT_TEACHER_HOME_ROOM									=1007;
const int CONSTRAINT_TEACHER_HOME_ROOMS									=1008;

const int CONSTRAINT_SUBJECT_PREFERRED_ROOM								=1009;
const int CONSTRAINT_SUBJECT_PREFERRED_ROOMS							=1010;
const int CONSTRAINT_SUBJECT_ACTIVITY_TAG_PREFERRED_ROOM				=1011;
const int CONSTRAINT_SUBJECT_ACTIVITY_TAG_PREFERRED_ROOMS				=1012;

const int CONSTRAINT_STUDENTS_MAX_BUILDING_CHANGES_PER_DAY				=1013;
const int CONSTRAINT_STUDENTS_SET_MAX_BUILDING_CHANGES_PER_DAY			=1014;
const int CONSTRAINT_STUDENTS_MAX_BUILDING_CHANGES_PER_WEEK				=1015;
const int CONSTRAINT_STUDENTS_SET_MAX_BUILDING_CHANGES_PER_WEEK			=1016;
const int CONSTRAINT_STUDENTS_MIN_GAPS_BETWEEN_BUILDING_CHANGES			=1017;
const int CONSTRAINT_STUDENTS_SET_MIN_GAPS_BETWEEN_BUILDING_CHANGES		=1018;

const int CONSTRAINT_TEACHERS_MAX_BUILDING_CHANGES_PER_DAY				=1019;
const int CONSTRAINT_TEACHER_MAX_BUILDING_CHANGES_PER_DAY				=1020;
const int CONSTRAINT_TEACHERS_MAX_BUILDING_CHANGES_PER_WEEK				=1021;
const int CONSTRAINT_TEACHER_MAX_BUILDING_CHANGES_PER_WEEK				=1022;
const int CONSTRAINT_TEACHERS_MIN_GAPS_BETWEEN_BUILDING_CHANGES			=1023;
const int CONSTRAINT_TEACHER_MIN_GAPS_BETWEEN_BUILDING_CHANGES			=1024;

/**
This class represents a space constraint
*/
class SpaceConstraint{
public:
	/**
	The weight (percentage) of this constraint
	*/
	double weightPercentage;

	/**
	Specifies the type of this constraint (using the above constants).
	*/
	int type;

	/**
	Dummy constructor - needed for the static array of constraints.
	Use of this function must be avoided.
	*/
	SpaceConstraint();
	
	virtual ~SpaceConstraint()=0;

	/**
	Constructor - please note that the maximum allowed weight is 100.0
	The reason: unallocated activities must have very big conflict weight,
	and any other restrictions must have much more lower weight,
	so that the timetable can evolve when starting with uninitialized activities
	*/
	SpaceConstraint(double wp);

	/**
	The function that calculates the fitness of a space chromosome, according to this
	constraint. We need the rules to compute this fitness factor.
	We need also the allocation of the activities on days and hours.
	If conflictsString!=NULL,
	it will be initialized with a text explaining where this restriction is broken.
	*/
	virtual double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL)=0;

	/**
	Returns an XML description of this constraint
	*/
	virtual QString getXmlDescription(Rules& r)=0;

	/**
	Computes the internal structure for this constraint
	*/
	virtual bool computeInternalStructure(Rules& r)=0;
	
	virtual bool hasInactiveActivities(Rules& r)=0;

	/**
	Returns a small description string for this constraint
	*/
	virtual QString getDescription(Rules& r)=0;

	/**
	Returns a detailed description string for this constraint
	*/
	virtual QString getDetailedDescription(Rules& r)=0;
	
	/**
	Returns true if this constraint is related to this activity
	*/
	virtual bool isRelatedToActivity(Activity* a)=0;

	/**
	Returns true if this constraint is related to this teacher
	*/
	virtual bool isRelatedToTeacher(Teacher* t)=0;

	/**
	Returns true if this constraint is related to this subject
	*/
	virtual bool isRelatedToSubject(Subject* s)=0;

	/**
	Returns true if this constraint is related to this activity tag
	*/
	virtual bool isRelatedToActivityTag(ActivityTag* s)=0;

	/**
	Returns true if this constraint is related to this students set
	*/
	virtual bool isRelatedToStudentsSet(Rules& r, StudentsSet* s)=0;

	/**
	Returns true if this constraint is related to this room
	*/
	virtual bool isRelatedToRoom(Room* r)=0;
};

/**
This class comprises all the basic compulsory constraints (constraints
which must be fulfilled for any timetable) - the space allocation part
*/
class ConstraintBasicCompulsorySpace: public SpaceConstraint{
public:
	ConstraintBasicCompulsorySpace();

	ConstraintBasicCompulsorySpace(double wp);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintRoomNotAvailableTimes: public SpaceConstraint{
public:

	QList<int> days;
	QList<int> hours;

	/**
	The room's name
	*/
	QString room;

	/**
	The room's id, or index in the rules
	*/
	int room_ID;

	ConstraintRoomNotAvailableTimes();

	ConstraintRoomNotAvailableTimes(double wp, const QString& rn, QList<int> d, QList<int> h);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

/**
This is a constraint. Its purpose: an activity must take part in
the preferred room.
*/
class ConstraintActivityPreferredRoom: public SpaceConstraint{
public:
	
	//The activity referred to by this constraint.
	//This is an index in the rules internal activities list.
	int _activity;
	
	//The index of the room
	int _room; 

	//----------------------------------------------------------

	int activityId;

	QString roomName;
	
	bool permanentlyLocked;

	ConstraintActivityPreferredRoom();

	ConstraintActivityPreferredRoom(double wp, int aid, const QString& room, bool perm);
	
	/**
	Comparison operator - to be sure we do not introduce duplicates
	*/
	bool operator==(ConstraintActivityPreferredRoom& c);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	//int fitness(Solution& c, Rules& r, const int days[/*MAX_ACTIVITIES*/], const int hours[/*MAX_ACTIVITIES*/], QString* conflictsString=NULL);
	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

/**
This is a constraint. Its purpose: an activity must take part in
the preferred rooms.
*/
class ConstraintActivityPreferredRooms: public SpaceConstraint{
public:
	
	//The activity referred to by this constraint.
	//This is an index in the rules internal activities list.
	int _activity;
	
	//The indexes of the rooms
	QList<int> _rooms;

	//----------------------------------------------------------

	int activityId;

	QStringList roomsNames;

	ConstraintActivityPreferredRooms();

	ConstraintActivityPreferredRooms(double wp, int aid, const QStringList& roomsList);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintStudentsSetHomeRoom: public SpaceConstraint{
public:

	QList<int> _activities;
	
	// The index of the room
	int _room;
	
public:

	QString studentsName;

	QString roomName;

	ConstraintStudentsSetHomeRoom();

	ConstraintStudentsSetHomeRoom(double wp, QString st, QString rm);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintStudentsSetHomeRooms: public SpaceConstraint{
public:
	
	//The list of activities referred to by this constraint.
	//This is a list of indices in the rules internal activities list.
	QList<int> _activities;
	
	//The indexes of the rooms
	QList<int> _rooms;
	
public:

	QString studentsName;

	QStringList roomsNames;

	ConstraintStudentsSetHomeRooms();

	ConstraintStudentsSetHomeRooms(double wp, QString st, const QStringList& rms);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintTeacherHomeRoom: public SpaceConstraint{
public:

	QList<int> _activities;
	
	// The index of the room
	int _room;
	
public:

	QString teacherName;

	QString roomName;

	ConstraintTeacherHomeRoom();

	ConstraintTeacherHomeRoom(double wp, QString tc, QString rm);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintTeacherHomeRooms: public SpaceConstraint{
public:
	
	//The list of activities referred to by this constraint.
	//This is a list of indices in the rules internal activities list.
	QList<int> _activities;
	
	//The indexes of the rooms
	QList<int> _rooms;
	
public:

	QString teacherName;

	QStringList roomsNames;

	ConstraintTeacherHomeRooms();

	ConstraintTeacherHomeRooms(double wp, QString st, const QStringList& rms);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

/**
This is a constraint. Its purpose: a subject must be taught in
a certain room.
*/
class ConstraintSubjectPreferredRoom: public SpaceConstraint{
public:

	QList<int> _activities;
	
	// The index of the room
	int _room;
	
public:

	QString subjectName;

	QString roomName;

	ConstraintSubjectPreferredRoom();

	ConstraintSubjectPreferredRoom(double wp, const QString& subj, const QString& rm);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	//int fitness(Solution& c, Rules& r, const int days[/*MAX_ACTIVITIES*/], const int hours[/*MAX_ACTIVITIES*/], QString* conflictsString=NULL);
	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

/**
This is a constraint. Its purpose: a subject must be taught in
certain rooms.
*/
class ConstraintSubjectPreferredRooms: public SpaceConstraint{
public:

	QList<int> _activities;
	
	QList<int> _rooms;
	
public:

	QString subjectName;

	QStringList roomsNames;

	ConstraintSubjectPreferredRooms();

	ConstraintSubjectPreferredRooms(double wp, const QString& subj, const QStringList& rms);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	//int fitness(Solution& c, Rules& r, const int days[/*MAX_ACTIVITIES*/], const int hours[/*MAX_ACTIVITIES*/], QString* conflictsString=NULL);
	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

/**
This is a constraint. Its purpose: a subject+subject tag must be taught in
a certain room.
*/
class ConstraintSubjectActivityTagPreferredRoom: public SpaceConstraint{
public:

	QList<int> _activities;
	
	// The index of the room
	int _room;
	
public:

	QString subjectName;

	QString activityTagName;

	QString roomName;

	ConstraintSubjectActivityTagPreferredRoom();

	ConstraintSubjectActivityTagPreferredRoom(double wp, const QString& subj, const QString& actTag, const QString& rm);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	//int fitness(Solution& c, Rules& r, const int days[/*MAX_ACTIVITIES*/], const int hours[/*MAX_ACTIVITIES*/], QString* conflictsString=NULL);
	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

/**
This is a constraint. Its purpose: a subject+subject tag must be taught in
certain rooms.
*/
class ConstraintSubjectActivityTagPreferredRooms: public SpaceConstraint{
public:
	
	QList<int> _activities;
	
	QList<int> _rooms;

public:

	QString subjectName;

	QString activityTagName;

	QStringList roomsNames;

	ConstraintSubjectActivityTagPreferredRooms();

	ConstraintSubjectActivityTagPreferredRooms(double wp, const QString& subj, const QString& actTag, const QStringList& rms);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintStudentsSetMaxBuildingChangesPerDay: public SpaceConstraint{
public:
	//internal variables
	QList<int> iSubgroupsList;

public:

	int maxBuildingChangesPerDay;

	QString studentsName;

	ConstraintStudentsSetMaxBuildingChangesPerDay();

	ConstraintStudentsSetMaxBuildingChangesPerDay(double wp, QString st, int mc);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintStudentsMaxBuildingChangesPerDay: public SpaceConstraint{
public:

	int maxBuildingChangesPerDay;

	ConstraintStudentsMaxBuildingChangesPerDay();

	ConstraintStudentsMaxBuildingChangesPerDay(double wp, int mc);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintStudentsSetMaxBuildingChangesPerWeek: public SpaceConstraint{
public:
	//internal variables
	QList<int> iSubgroupsList;

public:

	int maxBuildingChangesPerWeek;

	QString studentsName;

	ConstraintStudentsSetMaxBuildingChangesPerWeek();

	ConstraintStudentsSetMaxBuildingChangesPerWeek(double wp, QString st, int mc);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintStudentsMaxBuildingChangesPerWeek: public SpaceConstraint{
public:

	int maxBuildingChangesPerWeek;

	ConstraintStudentsMaxBuildingChangesPerWeek();

	ConstraintStudentsMaxBuildingChangesPerWeek(double wp, int mc);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintStudentsSetMinGapsBetweenBuildingChanges: public SpaceConstraint{
public:
	//internal variables
	QList<int> iSubgroupsList;

public:

	int minGapsBetweenBuildingChanges;

	QString studentsName;

	ConstraintStudentsSetMinGapsBetweenBuildingChanges();

	ConstraintStudentsSetMinGapsBetweenBuildingChanges(double wp, QString st, int mg);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintStudentsMinGapsBetweenBuildingChanges: public SpaceConstraint{
public:

	int minGapsBetweenBuildingChanges;

	ConstraintStudentsMinGapsBetweenBuildingChanges();

	ConstraintStudentsMinGapsBetweenBuildingChanges(double wp, int mg);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintTeacherMaxBuildingChangesPerDay: public SpaceConstraint{
public:
	//internal variables
	int teacher_ID;

public:

	int maxBuildingChangesPerDay;

	QString teacherName;

	ConstraintTeacherMaxBuildingChangesPerDay();

	ConstraintTeacherMaxBuildingChangesPerDay(double wp, QString tc, int mc);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintTeachersMaxBuildingChangesPerDay: public SpaceConstraint{
public:

	int maxBuildingChangesPerDay;

	ConstraintTeachersMaxBuildingChangesPerDay();

	ConstraintTeachersMaxBuildingChangesPerDay(double wp, int mc);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintTeacherMaxBuildingChangesPerWeek: public SpaceConstraint{
public:
	//internal variables
	int teacher_ID;

public:

	int maxBuildingChangesPerWeek;

	QString teacherName;

	ConstraintTeacherMaxBuildingChangesPerWeek();

	ConstraintTeacherMaxBuildingChangesPerWeek(double wp, QString tc, int mc);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintTeachersMaxBuildingChangesPerWeek: public SpaceConstraint{
public:

	int maxBuildingChangesPerWeek;

	ConstraintTeachersMaxBuildingChangesPerWeek();

	ConstraintTeachersMaxBuildingChangesPerWeek(double wp, int mc);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintTeacherMinGapsBetweenBuildingChanges: public SpaceConstraint{
public:
	//internal variables
	int teacher_ID;

public:

	int minGapsBetweenBuildingChanges;

	QString teacherName;

	ConstraintTeacherMinGapsBetweenBuildingChanges();

	ConstraintTeacherMinGapsBetweenBuildingChanges(double wp, QString tc, int mg);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};

class ConstraintTeachersMinGapsBetweenBuildingChanges: public SpaceConstraint{
public:

	int minGapsBetweenBuildingChanges;

	ConstraintTeachersMinGapsBetweenBuildingChanges();

	ConstraintTeachersMinGapsBetweenBuildingChanges(double wp, int mg);

	bool computeInternalStructure(Rules& r);

	bool hasInactiveActivities(Rules& r);
	
	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	double fitness(Solution& c, Rules& r, QList<double>& cl, QList<QString>& dl, QString* conflictsString=NULL);
	
	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToActivityTag(ActivityTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);

	bool isRelatedToRoom(Room* r);
};



#endif
