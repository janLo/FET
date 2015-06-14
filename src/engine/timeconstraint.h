/*
File timeconstraint.h
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

#ifndef TIMECONSTRAINT_H
#define TIMECONSTRAINT_H

#include "genetictimetable_defs.h"

#include <qstring.h>
#include <qptrlist.h>
#include <qstringlist.h>

class Rules;
class TimeChromosome;
class TimeConstraint;
class Activity;
class Teacher;
class Subject;
class SubjectTag;
class StudentsSet;

typedef QPtrList<TimeConstraint> TimeConstraintsList;

const int CONSTRAINT_GENERIC_TIME										=0;

const int CONSTRAINT_BASIC_COMPULSORY_TIME								=1;

const int CONSTRAINT_TEACHER_NOT_AVAILABLE								=2;
const int CONSTRAINT_TEACHERS_MAX_HOURS_CONTINUOUSLY					=3;
const int CONSTRAINT_TEACHERS_SUBGROUPS_MAX_HOURS_DAILY					=4;
const int CONSTRAINT_TEACHERS_NO_GAPS									=5;
const int CONSTRAINT_TEACHERS_MAX_HOURS_DAILY							=6;
const int CONSTRAINT_TEACHER_MAX_DAYS_PER_WEEK							=7;

const int CONSTRAINT_BREAK												=8;

const int CONSTRAINT_STUDENTS_EARLY										=9;
const int CONSTRAINT_STUDENTS_SET_NOT_AVAILABLE							=10;
const int CONSTRAINT_STUDENTS_N_HOURS_DAILY								=11;
const int CONSTRAINT_STUDENTS_SET_N_HOURS_DAILY							=12;
const int CONSTRAINT_STUDENTS_NO_GAPS									=13;
const int CONSTRAINT_STUDENTS_SET_NO_GAPS								=14;
const int CONSTRAINT_STUDENTS_SET_INTERVAL_MAX_DAYS_PER_WEEK			=15;

const int CONSTRAINT_ACTIVITY_PREFERRED_TIME							=16;
const int CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME						=17;
const int CONSTRAINT_ACTIVITIES_NOT_OVERLAPPING							=18;
const int CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES						=19;
const int CONSTRAINT_ACTIVITY_PREFERRED_TIMES							=20;
const int CONSTRAINT_ACTIVITY_ENDS_DAY									=21;
const int CONSTRAINT_2_ACTIVITIES_CONSECUTIVE							=22;
const int CONSTRAINT_2_ACTIVITIES_ORDERED								=23;
const int CONSTRAINT_2_ACTIVITIES_GROUPED								=24;
const int CONSTRAINT_ACTIVITIES_PREFERRED_TIMES							=25;
const int CONSTRAINT_ACTIVITIES_SAME_STARTING_HOUR						=26;

const int CONSTRAINT_TEACHERS_SUBJECT_TAGS_MAX_HOURS_CONTINUOUSLY		=27;
const int CONSTRAINT_TEACHERS_SUBJECT_TAG_MAX_HOURS_CONTINUOUSLY		=28;

/**
This class represents a time constraint
*/
class TimeConstraint{
public:
	/**
	The weight of this constraint
	*/
	double weight;

	/**
	Specifies the type of this constraint (using the above constants).
	*/
	int type;

	/**
	True for mandatory constraints, false for non-mandatory constraints.
	*/
	bool compulsory;

	/**
	Dummy constructor - needed for the static array of constraints.
	Any other use should be avoided.
	*/
	TimeConstraint();
	
	virtual ~TimeConstraint()=0;

	/**
	Constructor - please note that the maximum allowed weight is 100.0
	The reason: unallocated activities must have very big conflict weight,
	and any other restrictions must have much more lower weight,
	so that the timetable can evolve when starting with uninitialized activities.
	Also, it is preferred that you use integer weights for the moment.
	*/
	TimeConstraint(double w, bool c);

	/**
	The function that calculates the fitness of a chromosome, according to this
	constraint. We need the rules to compute this fitness factor.
	If conflictsString!=NULL,
	it will be initialized with a text explaining where this restriction is broken.
	*/
	virtual int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL)=0;

	/**
	Returns an XML description of this constraint
	*/
	virtual QString getXmlDescription(Rules& r)=0;

	/**
	Computes the internal structure for this constraint.
	
	It returns false if the constraint is an activity related
	one and it depends on only inactive activities.
	*/
	virtual bool computeInternalStructure(Rules& r)=0;

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
	Returns true if this constraint is related to this subject tag
	*/
	virtual bool isRelatedToSubjectTag(SubjectTag* s)=0;

	/**
	Returns true if this constraint is related to this students set
	*/
	virtual bool isRelatedToStudentsSet(Rules& r, StudentsSet* s)=0;
};

/**
This class comprises all the basic compulsory constraints (constraints
which must be fulfilled for any timetable) - the time allocation part
*/
class ConstraintBasicCompulsoryTime: public TimeConstraint{
public:
	ConstraintBasicCompulsoryTime();

	ConstraintBasicCompulsoryTime(double w);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a custom constraint.
It returns a fitness factor a number equal
to the product of this restriction's weight and
the number of conflicting hours for each teacher
(hours when he is not available, but a course is scheduled at that time).
For the moment, this is done for a certain day and an hour interval.
(For teacher "teacherName", on day "d", between hours "h1" and "h2").
*/
class ConstraintTeacherNotAvailable: public TimeConstraint{
public:

	/**
	The day
	*/
	int d;

	/**
	The start hour
	*/
	int h1;

	/**
	The end hour
	*/
	int h2;

	/**
	The teacher's name
	*/
	QString teacherName;

	/**
	The teacher's id, or index in the rules
	*/
	int teacher_ID;

	ConstraintTeacherNotAvailable();

	ConstraintTeacherNotAvailable(double w, bool c, const QString& tn, int day, int start_hour, int end_hour);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a custom constraint. It returns a fitness factor equal to the product of this
restriction's weight and the number of conflicting hours for each students' set
(hours when it is not available, but a course is scheduled at that time).
For the moment, this is done for a certain day and an hour interval.
(on day "d", between hours "h1" and "h2").
*/
class ConstraintStudentsSetNotAvailable: public TimeConstraint{
public:
	/**
	The day
	*/
	int d;

	/**
	The start hour
	*/
	int h1;

	/**
	The end hour
	*/
	int h2;

	/**
	The name of the students
	*/
	QString students;

	/**
	The number of subgroups involved in this restriction
	*/
	int nSubgroups;

	/**
	The subgroups involved in this restriction
	*/
	int subgroups[MAX_SUBGROUPS_PER_CONSTRAINT];

	ConstraintStudentsSetNotAvailable();

	ConstraintStudentsSetNotAvailable(double w, bool c, const QString& sn, int day, int start_hour, int end_hour);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint.
It aims at scheduling a set of activities at the same starting time.
The number of conflicts is considered the sum of differences
in the scheduling time for all pairs of activities.
The difference in the scheduling time for a pair of
activities is considered the sum between the difference in the starting
day and the difference in the starting hour.
TODO: Weekly activities are counted as two and bi-weekly activities as one
(really necessary?).
IMPORTANT: Starting with version 3.2.3, the compulsory constraints of this kind
implement chromosome repairing, so no conflicts will be reported
*/
class ConstraintActivitiesSameStartingTime: public TimeConstraint{
public:
	/**
	The number of activities involved in this constraint
	*/
	int n_activities;

	/**
	The activities involved in this constraint (id)
	*/
	int activitiesId[MAX_CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME];

	/**
	The number of activities involved in this constraint - internal structure
	*/
	int _n_activities;

	/**
	The activities involved in this constraint (indexes in the rules) - internal structure
	*/
	int _activities[MAX_CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME];

	ConstraintActivitiesSameStartingTime();

	/**
	Constructor, using:
	the weight, the number of activities and the list of activities' id-s.
	*/
	ConstraintActivitiesSameStartingTime(double w, bool c, int n_act, const int act[]);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	/**
	Removes useless activities from the _activities and activitiesId array
	*/
	void removeUseless(Rules& r);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint.
It aims at scheduling a set of activities so that they do not overlap.
The number of conflicts is considered the number of overlapping
hours.
*/
class ConstraintActivitiesNotOverlapping: public TimeConstraint{
public:
	/**
	The number of activities involved in this constraint
	*/
	int n_activities;

	/**
	The activities involved in this constraint (id)
	*/
	int activitiesId[MAX_CONSTRAINT_ACTIVITIES_NOT_OVERLAPPING];

	/**
	The number of activities involved in this constraint - internal structure
	*/
	int _n_activities;

	/**
	The activities involved in this constraint (index in the rules) - internal structure
	*/
	int _activities[MAX_CONSTRAINT_ACTIVITIES_NOT_OVERLAPPING];

	ConstraintActivitiesNotOverlapping();

	/**
	Constructor, using:
	the weight, the number of activities and the list of activities.
	*/
	ConstraintActivitiesNotOverlapping(double w, bool c, int n_act, const int act[]);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	/**
	Removes useless activities from the _activities array
	*/
	void removeUseless(Rules &r);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint.
It aims at scheduling a set of activities so that they
have a minimum of N days between any two of them.
*/
class ConstraintMinNDaysBetweenActivities: public TimeConstraint{
public:
	/**
	The number of activities involved in this constraint
	*/
	int n_activities;

	/**
	The activities involved in this constraint (id)
	*/
	int activitiesId[MAX_CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES];

	/**
	The number of minimum days between each 2 activities
	*/
	int minDays;

	//internal structure (redundant)

	/**
	The number of activities involved in this constraint - internal structure
	*/
	int _n_activities;

	/**
	The activities involved in this constraint (index in the rules) - internal structure
	*/
	int _activities[MAX_CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES];

	ConstraintMinNDaysBetweenActivities();

	/**
	Constructor, using:
	the weight, the number of activities and the list of activities.
	*/
	ConstraintMinNDaysBetweenActivities(double w, bool c, int n_act, const int act[], int n);

	/**
	Comparison operator - to be sure that we do not introduce duplicates
	*/
	bool operator==(ConstraintMinNDaysBetweenActivities& c);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	/**
	Removes useless activities from the _activities array
	*/
	void removeUseless(Rules &r);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint, aimed at obtaining timetables
which do not allow more than X hours in a row for any teacher
*/
class ConstraintTeachersMaxHoursContinuously: public TimeConstraint{
public:
	/**
	The maximum hours continuously
	*/
	int maxHoursContinuously;

	ConstraintTeachersMaxHoursContinuously();

	ConstraintTeachersMaxHoursContinuously(double w, bool c, int maxhours);

	QString getXmlDescription(Rules& r);

	bool computeInternalStructure(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint, aimed at obtaining timetables
which do not allow more than X hours in a day for any teacher
*/
class ConstraintTeachersMaxHoursDaily: public TimeConstraint{
public:
	/**
	The maximum hours daily
	*/
	int maxHoursDaily;

	ConstraintTeachersMaxHoursDaily();

	ConstraintTeachersMaxHoursDaily(double w, bool c, int maxhours);

	QString getXmlDescription(Rules& r);

	bool computeInternalStructure(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
A constraint aimed at obtaining timetables
which do not allow for a certain teacher and a certain
subgroup more than X hours per day
*/
class ConstraintTeachersSubgroupsMaxHoursDaily: public TimeConstraint{
public:
	/**
	The maximum allowed hours daily
	*/
	int maxHoursDaily;

	ConstraintTeachersSubgroupsMaxHoursDaily();

	ConstraintTeachersSubgroupsMaxHoursDaily(double w, bool c, int maxhours);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint.
The resulting timetable must respect the requirement
that this teacher must not have too much working
days per week.
*/
class ConstraintTeacherMaxDaysPerWeek: public TimeConstraint{
public:
	/**
	The number of maximum allowed working days per week (-1 for don't care)
	*/
	int maxDaysPerWeek;

	/**
	The teacher's name
	*/
	QString teacher;

	/**
	The teacher's id, or index in the rules
	*/
	int teacher_ID;

	ConstraintTeacherMaxDaysPerWeek();

	ConstraintTeacherMaxDaysPerWeek(double w, bool c, int maxnd, QString t);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint.
It constrains the timetable to not schedule any activity
in the specified day, during the start hour and end hour.
*/
class ConstraintBreak: public TimeConstraint{
public:
	/**
	The day
	*/
	int d;

	/**
	The start hour
	*/
	int h1;

	/**
	The end hour
	*/
	int h2;

	ConstraintBreak();

	ConstraintBreak(double w, bool c, int day, int start_hour, int end_hour);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint. It adds, to the fitness of
the chromosome, a conflicts factor computed from the gaps
existing in the timetable (regarding the students).
The overall result is a timetable having less gaps for the students.
*/
class ConstraintStudentsNoGaps: public TimeConstraint{
public:
	ConstraintStudentsNoGaps();

	ConstraintStudentsNoGaps(double w, bool c);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint. It adds, to the fitness of
the chromosome, a conflicts factor computed from the gaps
existing in the timetable (regarding the specified students set).
*/
class ConstraintStudentsSetNoGaps: public TimeConstraint{
public:
	/**
	The name of the students set for this constraint
	*/
	QString students;

	//internal redundant data

	/**
	The number of subgroups
	*/
	int nSubgroups;

	/**
	The subgroups
	*/
	int subgroups[MAX_SUBGROUPS_PER_CONSTRAINT];

	ConstraintStudentsSetNoGaps();

	ConstraintStudentsSetNoGaps(double w, bool c, const QString& st );

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint. It adds, to the fitness of the chromosome, a
conflicts factor computed from the gaps existing in the timetable
(regarding the teachers). The overall result is a timetable having
less gaps for the teachers.
*/
class ConstraintTeachersNoGaps: public TimeConstraint{
public:

	ConstraintTeachersNoGaps();

	ConstraintTeachersNoGaps(double w, bool c);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint. It adds, to the fitness of
the chromosome, a fitness factor that is related to how early
the students begin their courses. The result is a timetable
having more activities scheduled at the beginning of the day.
IMPORTANT: biweekly activities are treated as weekly ones,
for speed and because in normal situations this does not matter.
*/
class ConstraintStudentsEarly: public TimeConstraint{
public:

	ConstraintStudentsEarly();

	ConstraintStudentsEarly(double w, bool compulsory);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint. 
The result is a timetable respecting the condition that the students
must not have too little or too much hours in a day.
*/
class ConstraintStudentsNHoursDaily: public TimeConstraint{
public:
	/**
	The number of maximum allowed hours per day (-1 for don't care)
	*/
	int maxHoursDaily;

	/**
	The number of minimum allowed hours per day (-1 for don't care)
	*/
	int minHoursDaily;

	ConstraintStudentsNHoursDaily();

	ConstraintStudentsNHoursDaily(double w, bool c, int maxnh, int minnh);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint. 
The result is a timetable respecting the condition that this students set
must not have too little or too much hours in a day.
*/
class ConstraintStudentsSetNHoursDaily: public TimeConstraint{
public:
	/**
	The number of maximum allowed hours per day (-1 for don't care)
	*/
	int maxHoursDaily;

	/**
	The number of minimum allowed hours per day (-1 for don't care)
	*/
	int minHoursDaily;

	/**
	The students set name
	*/
	QString students;

	//internal variables

	/**
	The number of subgroups
	*/
	int nSubgroups;

	/**
	The subgroups
	*/
	int subgroups[MAX_SUBGROUPS_PER_CONSTRAINT];

	ConstraintStudentsSetNHoursDaily();

	ConstraintStudentsSetNHoursDaily(double w, bool c, int maxnh, int minnh, QString s);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a custom constraint.
For a certain students set:
The purpose is that a certain interval not be scheduled more than n times in a week
(for example, there must be only 2 occupied intervals between hours 3 and 6,
which might be on Monday and on Tuesday.
*/
class ConstraintStudentsSetIntervalMaxDaysPerWeek: public TimeConstraint{
public:
	/**
	The start hour
	*/
	int h1;

	/**
	The end hour
	*/
	int h2;

	/**
	The name of the students
	*/
	QString students;

	/**
	The max. number of intervals
	*/
	int n;

	//internal variables

	/**
	The number of subgroups involved in this restriction
	*/
	int nSubgroups;

	/**
	The subgroups involved in this restriction
	*/
	int subgroups[MAX_SUBGROUPS_PER_CONSTRAINT];

	ConstraintStudentsSetIntervalMaxDaysPerWeek();

	ConstraintStudentsSetIntervalMaxDaysPerWeek(double w, bool c, const QString& sn, int start_hour, int end_hour, int n_intervals);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint.
It adds, to the fitness of the chromosome, a fitness factor that
grows as the activity is scheduled farther from the wanted time
For the moment, fitness factor increases with one unit for every hour
and one unit for every day.
*/
class ConstraintActivityPreferredTime: public TimeConstraint{
public:
	/**
	Activity id
	*/
	int activityId;

	/**
	The preferred day. If -1, then the user does not care about the day.
	*/
	int day;

	/**
	The preferred hour. If -1, then the user does not care about the hour.
	*/
	int hour;

	//internal variables
	/**
	The index of the activity in the rules (from 0 to rules.nActivities-1) - it is not the id of the activity
	*/
	int activityIndex;

	ConstraintActivityPreferredTime();

	ConstraintActivityPreferredTime(double w, bool c, int actId, int d, int h);

	/**
	Comparison operator - to be sure that we do not introduce duplicates
	*/
	bool operator==(ConstraintActivityPreferredTime& c);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint.
It returns conflicts if the activity is scheduled in another interval
than the preferred set of times.
*/
class ConstraintActivityPreferredTimes: public TimeConstraint{
public:
	/**
	Activity id
	*/
	int activityId;

	/**
	The number of preferred times
	*/
	int nPreferredTimes;

	/**
	The preferred days. If -1, then the user does not care about the day.
	*/
	int days[MAX_N_CONSTRAINT_ACTIVITY_PREFERRED_TIMES];

	/**
	The preferred hour. If -1, then the user does not care about the hour.
	*/
	int hours[MAX_N_CONSTRAINT_ACTIVITY_PREFERRED_TIMES];

	//internal variables
	/**
	The index of the activity in the rules (from 0 to rules.nActivities-1) - it is not the id of the activity
	*/
	int activityIndex;

	ConstraintActivityPreferredTimes();

	ConstraintActivityPreferredTimes(double w, bool c, int actId, int nPT, int d[], int h[]);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint.
It returns a conflicts factor that increases with the number of lessons
that follow after this activity (in the same day).
*/
class ConstraintActivityEndsDay: public TimeConstraint{
public:
	/**
	Activity id
	*/
	int activityId;

	//internal variables
	/**
	The index of the activity in the rules (from 0 to rules.nActivities-1) - it is not the id of the activity
	*/
	int activityIndex;

	ConstraintActivityEndsDay();

	ConstraintActivityEndsDay(double w, bool c, int actId);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a time constraint.
PURPOSE: you have two activities that you want to schedule one after
the other, in the same day. Order is important.
It adds, to the fitness of the chromosome, a value that
grows as the 2 activities are scheduled farther one from each other.
For the moment, fitness factor increases with one unit for every additional 
hour and one unit for every day (the optimal being 0 - when the starting time
of the second activity is the ending time of the first one).
*/
class Constraint2ActivitiesConsecutive: public TimeConstraint{
public:
	/**
	First activity id
	*/
	int firstActivityId;

	/**
	Second activity id
	*/
	int secondActivityId;

	//internal variables
	/**
	The index of the first activity in the rules (from 0 to rules.nActivities-1) - it is not the id of the activity
	*/
	int firstActivityIndex;

	/**
	The index of the second activity in the rules (from 0 to rules.nActivities-1) - it is not the id of the activity
	*/
	int secondActivityIndex;

	Constraint2ActivitiesConsecutive();

	Constraint2ActivitiesConsecutive(double w, bool c, int firstActId, int secondActId);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a time constraint.
PURPOSE: you have two activities that you want to schedule one after
the other, not necessarily in the same day or adjacent. Order is important.
It adds, to the fitness of the chromosome, the weight multimplied with 2 if the first
activity is weekly (not bi-weekly) and with again with 2 if the second activity
is weekly, if the condition is broken.
*/
class Constraint2ActivitiesOrdered: public TimeConstraint{
public:
	/**
	First activity id
	*/
	int firstActivityId;

	/**
	Second activity id
	*/
	int secondActivityId;

	//internal variables
	/**
	The index of the first activity in the rules (from 0 to rules.nActivities-1) - it is not the id of the activity
	*/
	int firstActivityIndex;

	/**
	The index of the second activity in the rules (from 0 to rules.nActivities-1) - it is not the id of the activity
	*/
	int secondActivityIndex;

	Constraint2ActivitiesOrdered();

	Constraint2ActivitiesOrdered(double w, bool c, int firstActId, int secondActId);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a time constraint.
PURPOSE: you have two activities that you want to schedule one after
the other, in the same day. Order is not important.
It adds, to the fitness of the chromosome, a value that
grows as the 2 activities are scheduled farther one from each other.
For the moment, fitness factor increases with one unit for every additional
hour and one unit for every day.
(For hours, we have a 0 as minimum if the activities
are OK, a positive constant value if they overlap and a positive
increasing value if they are not touching.
For days - we have a simple absolute difference).
*/
class Constraint2ActivitiesGrouped: public TimeConstraint{
public:
	/**
	First activity id
	*/
	int firstActivityId;

	/**
	Second activity id
	*/
	int secondActivityId;

	//internal variables
	/**
	The index of the first activity in the rules (from 0 to rules.nActivities-1) - it is not the id of the activity
	*/
	int firstActivityIndex;

	/**
	The index of the second activity in the rules (from 0 to rules.nActivities-1) - it is not the id of the activity
	*/
	int secondActivityIndex;

	Constraint2ActivitiesGrouped();

	Constraint2ActivitiesGrouped(double w, bool c, int firstActId, int secondActId);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint.
It returns conflicts if a set of activities is scheduled in another interval
than the preferred set of times.
The set of activities is specified by a subject, teacher, students or a combination
of these.
*/
class ConstraintActivitiesPreferredTimes: public TimeConstraint{
public:
	/**
	The teacher. If void, all teachers.
	*/
	QString teacherName;

	/**
	The students. If void, all students.
	*/
	QString studentsName;

	/**
	The subject. If void, all subjects.
	*/
	QString subjectName;

	/**
	The subject tag. If void, all subjects tags.
	*/
	QString subjectTagName;

	/**
	The number of preferred times
	*/
	int nPreferredTimes;

	/**
	The preferred days. If -1, then the user does not care about the day.
	*/
	int days[MAX_N_CONSTRAINT_ACTIVITIES_PREFERRED_TIMES];

	/**
	The preferred hours. If -1, then the user does not care about the hour.
	*/
	int hours[MAX_N_CONSTRAINT_ACTIVITIES_PREFERRED_TIMES];

	//internal variables
	
	/**
	The number of activities which are represented by the subject, teacher and students requirements.
	*/
	int nActivities;
	
	/**
	The indices of the activities in the rules (from 0 to rules.nActivities-1)
	These are indices in the internal list -> Rules::internalActivitiesList
	*/
	int activitiesIndices[MAX_ACTIVITIES];

	ConstraintActivitiesPreferredTimes();

	ConstraintActivitiesPreferredTimes(double w, bool c, QString te,
		QString st, QString su, QString sut, int nPT, int d[], int h[]);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
This is a constraint.
It aims at scheduling a set of activities at the same starting hour.
The number of conflicts is considered the sum of differences
in the scheduling time for all pairs of activities.
The difference in the scheduling time for a pair of
activities is considered the difference in the starting hour.
TODO: Weekly activities are counted as two and bi-weekly activities as one
(really necessary?).
IMPORTANT: The compulsory constraints of this kind
implement chromosome repairing, so no conflicts will be reported
*/
class ConstraintActivitiesSameStartingHour: public TimeConstraint{
public:
	/**
	The number of activities involved in this constraint
	*/
	int n_activities;

	/**
	The activities involved in this constraint (id)
	*/
	int activitiesId[MAX_CONSTRAINT_ACTIVITIES_SAME_STARTING_HOUR];

	/**
	The number of activities involved in this constraint - internal structure
	*/
	int _n_activities;

	/**
	The activities involved in this constraint (index in the rules) - internal structure
	*/
	int _activities[MAX_CONSTRAINT_ACTIVITIES_SAME_STARTING_HOUR];

	ConstraintActivitiesSameStartingHour();

	/**
	Constructor, using:
	the weight, the number of activities and the list of activities' id-s.
	*/
	ConstraintActivitiesSameStartingHour(double w, bool c, int n_act, const int act[]);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	/**
	Removes useless activities from the _activities array
	*/
	void removeUseless(Rules& r);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
A constraint aimed at obtaining timetables
which do not allow for a certain teacher (from all teachers) and 
a certain subject tag (from all subject tags) more 
than max hours countinuously
*/
class ConstraintTeachersSubjectTagsMaxHoursContinuously: public TimeConstraint{
public:
	/**
	The maximum allowed hours continuously
	*/
	int maxHoursContinuously;
	
	ConstraintTeachersSubjectTagsMaxHoursContinuously();

	ConstraintTeachersSubjectTagsMaxHoursContinuously(double w, bool c, int maxhours);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

/**
A constraint aimed at obtaining timetables
which do not allow for a certain teacher from all teachers and 
a certain subject tag more than max hours countinuously
*/
class ConstraintTeachersSubjectTagMaxHoursContinuously: public TimeConstraint{
public:
	/**
	The maximum allowed hours continuously
	*/
	int maxHoursContinuously;
	
	/**
	The subject tag
	*/
	QString subjectTagName;
	
	/**
	The subject tag index
	*/
	int subjectTagIndex;
	
	ConstraintTeachersSubjectTagMaxHoursContinuously();

	ConstraintTeachersSubjectTagMaxHoursContinuously(double w, bool c, int maxhours, const QString& subjecttag);

	bool computeInternalStructure(Rules& r);

	QString getXmlDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(TimeChromosome& c, Rules& r, QString* conflictsString=NULL);

	bool isRelatedToActivity(Activity* a);
	
	bool isRelatedToTeacher(Teacher* t);

	bool isRelatedToSubject(Subject* s);

	bool isRelatedToSubjectTag(SubjectTag* s);
	
	bool isRelatedToStudentsSet(Rules& r, StudentsSet* s);
};

#endif
