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

#include "genetictimetable_defs.h"

#include <qstring.h>
#include <qptrlist.h>
#include <qstringlist.h>

class Rules;
class SpaceChromosome;
class SpaceConstraint;

typedef QPtrList<SpaceConstraint> SpaceConstraintsList;

const int CONSTRAINT_GENERIC_SPACE						=1001; //time constraints are beginning from 1
const int CONSTRAINT_BASIC_COMPULSORY_SPACE				=1002; //space constraints from 1001
const int CONSTRAINT_ROOM_TYPE_NOT_ALLOWED_SUBJECTS		=1003;
const int CONSTRAINT_ROOM_NOT_AVAILABLE					=1004;
const int CONSTRAINT_SUBJECT_REQUIRES_EQUIPMENTS		=1005;
const int CONSTRAINT_TEACHER_REQUIRES_ROOM				=1006;

/**
This class represents a space constraint
*/
class SpaceConstraint{
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
	Is this constraint compulsory (mandatory) or just a wish?
	*/
	bool compulsory;

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
	SpaceConstraint(double w, bool c);

	/**
	The function that calculates the fitness of a space chromosome, according to this
	constraint. We need the rules to compute this fitness factor.
	We need also the allocation of the activities on days and hours.
	If conflictsString!=NULL,
	it will be initialized with a text explaining where this restriction is broken.
	*/
	virtual int fitness(SpaceChromosome& c, Rules& r, const int days[/*MAX_ACTIVITIES*/], const int hours[/*MAX_ACTIVITIES*/], QString* conflictsString=NULL)=0;

	/**
	Returns an XML description of this constraint
	*/
	virtual QString getXMLDescription(Rules& r)=0;

	/**
	Computes the internal structure for this constraint
	*/
	virtual void computeInternalStructure(Rules& r)=0;

	/**
	Returns a small description string for this constraint
	*/
	virtual QString getDescription(Rules& r)=0;

	/**
	Returns a detailed description string for this constraint
	*/
	virtual QString getDetailedDescription(Rules& r)=0;
};

/**
This class comprises all the basic compulsory constraints (constraints
which must be fulfilled for any timetable) - the space allocation part
*/
class ConstraintBasicCompulsorySpace: public SpaceConstraint{
public:
	ConstraintBasicCompulsorySpace();

	ConstraintBasicCompulsorySpace(double w);

	void computeInternalStructure(Rules& r);

	QString getXMLDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(SpaceChromosome& c, Rules& r, const int days[/*MAX_ACTIVITIES*/], const int hours[/*MAX_ACTIVITIES*/], QString* conflictsString=NULL);
};

/**
This is a custom constraint.
It returns a fitness factor a number equal
to the product of this restriction's weight and
the number of conflicting hours for each room
(hours when it is not available, but an activity is scheduled at that time).
For the moment, this is done for a certain day and an hour interval.
(For room "roomName", on day "d", between hours "h1" and "h2").
*/
class ConstraintRoomNotAvailable: public SpaceConstraint{
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
	The room's name
	*/
	QString roomName;

	/**
	The room's id, or index in the rules
	*/
	int room_ID;

	ConstraintRoomNotAvailable();

	ConstraintRoomNotAvailable(double w, bool c, const QString& rn, int day, int start_hour, int end_hour);

	void computeInternalStructure(Rules& r);

	QString getXMLDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(SpaceChromosome& c, Rules& r, const int days[/*MAX_ACTIVITIES*/], const int hours[/*MAX_ACTIVITIES*/], QString* conflictsString=NULL);
};

/**
This is a constraint. Its purpose: a subject must not be taught in
a room which does not support the room's type.
*/
class ConstraintRoomTypeNotAllowedSubjects: public SpaceConstraint{
private:

	//The number of rooms referred to by this constraint
	int _nRooms;

	//The list of rooms referred to by this constraint. This is
	//a list of indices in the rules internal rooms list.
	int _rooms[MAX_ROOMS];

	//The number of activities referred to by this constraint
	int _nActivities;

	//The list of activities referred to by this constraint.
	//This is a list of indices in the rules internal activities list.
	int _activities[MAX_ACTIVITIES];

public:

	QString roomType;

	QStringList subjects;

	ConstraintRoomTypeNotAllowedSubjects();

	ConstraintRoomTypeNotAllowedSubjects(double w, bool c, const QString& room_type);

	void addNotAllowedSubject(const QString& subjectName);

	/**
	Returns the number of removed subjects (must be 0 or 1, of course)
	*/
	int removeNotAllowedSubject(const QString& subjectName);

	/**
	Returns true if the subject is in the list of non allowed subjects,
	false otherwise.
	*/
	bool searchNotAllowedSubject(const QString& subjectName);

	void computeInternalStructure(Rules& r);

	QString getXMLDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(SpaceChromosome& c, Rules& r, const int days[/*MAX_ACTIVITIES*/], const int hours[/*MAX_ACTIVITIES*/], QString* conflictsString=NULL);
};

/**
This is a constraint. Its purpose: a subject must be taught in
a room which has the necessary equipments.
*/
class ConstraintSubjectRequiresEquipments: public SpaceConstraint{
private:
	
	//The number of activities referred to by this constraint
	int _nActivities;

	//The list of activities referred to by this constraint.
	//This is a list of indices in the rules internal activities list.
	int _activities[MAX_ACTIVITIES_FOR_A_SUBJECT];

public:

	QString subjectName;

	QStringList equipmentsNames;

	ConstraintSubjectRequiresEquipments();

	ConstraintSubjectRequiresEquipments(double w, bool c, const QString& subj);

	void addRequiredEquipment(const QString& equip);

	/**
	Returns the number of removed equipments (must be 0 or 1, of course)
	*/
	int removeRequiredEquipment(const QString& equip);

	/**
	Returns true if the subject is in the list of required equipments,
	false otherwise.
	*/
	bool searchRequiredEquipment(const QString& equip);

	void computeInternalStructure(Rules& r);

	QString getXMLDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(SpaceChromosome& c, Rules& r, const int days[/*MAX_ACTIVITIES*/], const int hours[/*MAX_ACTIVITIES*/], QString* conflictsString=NULL);
};

/**
This is a constraint. Its purpose: a teacher must teach in
his preferred room.
*/
class ConstraintTeacherRequiresRoom: public SpaceConstraint{
private:
	
	//The number of activities referred to by this constraint
	int _nActivities;

	//The list of activities referred to by this constraint.
	//This is a list of indices in the rules internal activities list.
	int _activities[MAX_ACTIVITIES_FOR_A_TEACHER];
	
	//The index of the room
	int _room; 

public:

	QString teacherName;

	QString roomName;

	ConstraintTeacherRequiresRoom();

	ConstraintTeacherRequiresRoom(double w, bool c, const QString& teach, const QString& room);

	void computeInternalStructure(Rules& r);

	QString getXMLDescription(Rules& r);

	QString getDescription(Rules& r);

	QString getDetailedDescription(Rules& r);

	int fitness(SpaceChromosome& c, Rules& r, const int days[/*MAX_ACTIVITIES*/], const int hours[/*MAX_ACTIVITIES*/], QString* conflictsString=NULL);
};

#endif
