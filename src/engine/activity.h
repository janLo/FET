/*
File activity.h
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

#ifndef ACTIVITY_H
#define ACTIVITY_H

#include "genetictimetable_defs.h"

#include <qstring.h>
#include <qfile.h>
#include <qptrlist.h>
#include <qstringlist.h>

class Rules;
class Activity;

typedef QPtrList<Activity> ActivitiesList;

/**
Used for an activity's parity, if it was not yet initialized.
*/
const int PARITY_NOT_INITIALIZED=-1;

/**
Used for an activity, if it is weekly.
*/
const int PARITY_WEEKLY=0;

/**
Used for an activity, if it is occuring once at two weeks.
*/
const int PARITY_BIWEEKLY=1;

/**
This class represents an activity.
<p>
An activity is a certain course (lecture), taught by a certain teacher (or more),
to a certain year (or group, or subgroup) of students (or more).
*/
class Activity{
public:
	/**
	The teachers' names.
	*/
	QStringList teachersNames;

	/**
	The name of the subject.
	*/
	QString subjectName;

	/**
	The name of the subject tag.
	*/
	QString subjectTagName;

	/**
	The names of the sets of students involved in this activity (years, groups or subgroups).
	*/
	QStringList studentsNames;

	/**
	The duration, in hours.
	*/
	int duration;

	/**
	The parity: weekly (PARITY_WEEKLY) or once at two weeks (PARITY_BIWEEKLY).
	*/
	int parity;

	/**
	This value is used only for split activities (for high-schools).
	If totalDuration==duration, then this activity is not split.
	If totalDuration>duration, then this activity is split.
	*/
	int totalDuration;

	/**
	A unique ID for any activity. This is NOT the index (activities might be erased,
	but this ID remains the same).
	*/
	int id;

	/**
	The activities generated from a split activity have the same activityGroupId.
	For non-split activities, activityGroupId==0
	*/
	int activityGroupId;

	/**
	The number of students who are attending this activity
	*/
	int nTotalStudents;
	
	/**
	If the teachers, subject, students, duration and parity are identical, returns true.
	TODO: add a more intelligent comparison
	*/
	bool operator==(Activity &a);

	//internal structure
	
	/**
	The number of teachers who are teaching this activity
	*/
	int nTeachers;
	
	/**
	The indices of the teachers who are teaching this activity.
	*/
	int teachers[MAX_TEACHERS_PER_ACTIVITY];

	/**
	The index of the subject.
	*/
	int subjectIndex;

	/**
	The number of subgroups implied in this activity.
	*/
	int nSubgroups;

	/**
	The indices of the subgroups implied in this activity.
	*/
	int subgroups[MAX_SUBGROUPS_PER_ACTIVITY];
	
	/**
	Simple constructor, used only indirectly by the static variable
	"Activity internalActivitiesList[MAX_ACTIVITIES]".
	Any other use of this function should be avoided.
	*/
	Activity();

	/**
	Complete constructor.
	If _totalDuration!=duration, then this activity is a part of a bigger (split)
	activity.
	<p>
	As a must, for split activities, _activityGroupId==0.
	For the non-split ones, it is >0
	*/
	Activity(
		Rules& r,
		int _id,
		int _activityGroupId,
		const QStringList& _teachersNames,
		const QString& _subjectName,
		const QString& _subjectTagName,
		const QStringList& _studentsNames,
		int _duration,
		int _totalDuration,
		int _parity);

	/**
	Removes this teacher from the list of teachers
	*/
	void removeTeacher(const QString& teacherName);

	/**
	Renames this teacher in the list of teachers
	*/
	void renameTeacher(const QString& initialTeacherName, const QString& finalTeacherName);

	/**
	Removes this students set from the list of students
	*/
	void removeStudents(const QString& studentsName);

	/**
	Renames this students set in the list of students
	*/
	void renameStudents(const QString& initialStudentsName, const QString& finalStudentsName);

	/**
	Computes the internal structure
	*/
	void computeInternalStructure(Rules& r);

	/**
	Returns a representation of this activity (xml format).
	*/
	QString getXMLDescription(Rules& r);

	/**
	Returns a representation of this activity.
	*/
	QString getDescription(Rules& r);

	/**
	Returns a representation of this activity (more detailed).
	*/
	QString getDetailedDescription(Rules& r);

	/**
	Returns true if this activity is split into more lessons per week.
	*/
	bool isSplit();
};

#endif
