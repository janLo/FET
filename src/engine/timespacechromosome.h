/*
File timespacechromosome.h
*/

/*
Copyright 2005 Lalescu Liviu.

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


#ifndef TIMESPACECHROMOSOME_H
#define TIMESPACECHROMOSOME_H

#include "genetictimetable_defs.h"
#include "timechromosome.h"
#include "spacechromosome.h"

#include <qstring.h>
#include <qtextstream.h>
#include <q3valuevector.h>

class Rules;

/**
This class represents a chromosome (time and space allocation for the activities).
<p>
Every chromosome represents a solution candidate for the timetabling problem.
*/
class TimeSpaceChromosome: public TimeChromosome, SpaceChromosome{
public:
	/*
	You will need to set this to true if altering the times array values.
	The conflicts calculating routine will reset this to false
	at the first teachers matrix and subgroups matrix calculation.
	*/
	//bool timeChangedForMatrixCalculation;

	/*
	You will need to set this to true if altering the rooms array values.
	The conflicts calculating routine will reset this to false
	at the first rooms matrix calculation.
	*/
	//bool spaceChangedForMatrixCalculation;

	/**
	The genes of this chromosome (time).
	<p>
	This array represents every activity's start time
	(time is a unified representation of hour and day,
	stored as an integer value). We have a special value here:
	UNALLOCATED_TIME, which is a large number.
	*/
	//int16 times[MAX_ACTIVITIES];
	
	/**
	The genes of this chromosome (space).
	<p>
	This array represents every activity's allocated room.
	We have a special value here:
	UNALLOCATED_SPACE, which is a large number.
	*/
	//int16 rooms[MAX_ACTIVITIES];

	/**
	Hard fitness; it is calculated only at the initialization
	of this chromosome or at the modification of it.
	Important assumption: the rules have to ramain the same;
	otherwise the user has to reset this value to -1
	*/
	int _hardFitness;

	/**
	Soft fitness; it is calculated only at the initialization
	of this chromosome or at the modification of it.
	Important assumption: the rules have to ramain the same;
	otherwise the user has to reset this value to -1
	*/
	int _softFitness;

	/**
	Assignment method. We need to have access to the Rules instantiation
	to know the number of activities.
	*/
	void copy(Rules& r, TimeSpaceChromosome& c);

	/**
	Initializes a chromosome, marking all activities as unscheduled (time & space)
	*/
	void init(Rules& r);

	/**
	Marks the starting time and rooms of all the activities as undefined
	(all activities are unallocated).
	This is the first method of generating an initial population.
	*/
	void makeTimesRoomsUnallocated(Rules& r);

	/**
	Randomizes the starting time and rooms of all the activities.
	This is the second method of generating an initial population.
	*/
	void makeTimesRoomsRandom(Rules& r);

	/**
	Reads this chromosome from the disk (reads a saved solution).
	Returns false on failure, true on success.
	*/
	bool read(Rules& r, const QString &filename);

	/**
	Reads this chromosome from the disk (reads a saved solution).
	Returns false on failure, true on success.
	*/
	bool read(Rules& r, QTextStream &tis);

	/**
	Saves this chromosome to the disk (saves this solution).
	*/
	void write(Rules& r, const QString &filename);

	/**
	Saves this chromosome to the disk (saves this solution).
	*/
	void write(Rules &r, QTextStream &tos);

	/**
	Computes the unsatisfied compulsory constraints (stores the result in the
	variable _hardFitness, so that it is not recomputed when needed again later).
	ATTENTION: if the rules change, the user has to reset _hardFitness to -1
	<p>
	If conflictsString is not null, then this function will
	append at this string an explanation of the conflicts.
	*/
	int hardFitness(Rules& r, QString* conflictsString=NULL);

	/**
	Computes the unsatisfied non-compulsory constraints (stores the result in the
	variable _softFitness, so that it is not recomputed when needed again later).
	ATTENTION: if the rules change, the user has to reset _softFitness to -1
	<p>
	If conflictsString is not null, then this function will
	append at this string an explanation of the conflicts.
	*/
	int softFitness(Rules& r, QString* conflictsString=NULL);

	/**
	This chromosome is generated by the crossover of the two
	input chromosomes. Generic method.
	*/
	void crossover(Rules& r, TimeSpaceChromosome& c1, TimeSpaceChromosome& c2);

	/**
	This chromosome is generated by the crossover of the two
	input chromosomes. One point method.
	*/
	void onePointCrossover(Rules& r, TimeSpaceChromosome& c1, TimeSpaceChromosome& c2);

	/**
	This chromosome is generated by the crossover of the two
	input chromosomes. 2 point method.
	*/
	void twoPointCrossover(Rules& r, TimeSpaceChromosome& c1, TimeSpaceChromosome& c2);

	/**
	This chromosome is generated by the crossover of the two
	input chromosomes. Uniform method.
	*/
	void uniformCrossover(Rules& r, TimeSpaceChromosome& c1, TimeSpaceChromosome& c2);

	/**
	This chromosome suffers mutation 1
	Random swap between the starting times of two activities
	or random swap between 2 rooms
	(classical method).
	*/
	void mutate1(Rules& r);

	/**
	This chromosome suffers mutation 2.
	Changes an hour or a room into a random
	(experimental method).
	*/
	void mutate2(Rules& r);

	/**
	This is a function that retrieves the teachers' timetable from
	this chromosome's "times" array.
	We have 2 matrices: the most used is the first, for weekly activities
	(in this case the corresponding position in the second matrix is a
	special value, let's say UNALLOCATED_ACTIVITY)
	For fortnightly activities we use both matrices: the first matrix
	keeps the activity scheduled for the first week, while the second
	matrix keeps the activity scheduled for the second week.
	The arrays a1 and a2 will contain the index of the activity in the rules.
	*/
	void getTeachersTimetable(Rules& r, int16 a1[MAX_TEACHERS][MAX_DAYS_PER_WEEK][MAX_HOURS_PER_DAY],int16 a2[MAX_TEACHERS][MAX_DAYS_PER_WEEK][MAX_HOURS_PER_DAY]);

	/**
	This is a function that retrieves the subgroups' timetable from
	this chromosome's "times" array.
	We have 2 matrices: the most used is the first, for weekly activities
	(in this case the corresponding position in the second matrix is a
	special value, let's say UNALLOCATED_ACTIVITY)
	For fortnightly activities we use both matrices: the first matrix
	keeps the activity scheduled for the first week, while the second
	matrix keeps the activity scheduled for the second week.
	The arrays a1 and a2 will contain the index of the activity in the rules.
	*/
	void getSubgroupsTimetable(Rules& r, int16 a1[MAX_TOTAL_SUBGROUPS][MAX_DAYS_PER_WEEK][MAX_HOURS_PER_DAY],int16 a2[MAX_TOTAL_SUBGROUPS][MAX_DAYS_PER_WEEK][MAX_HOURS_PER_DAY]);

	/**
	The following function is very similar to GetsubgroupsTimetable,
	except that it is used in fitness calculation: it computes a matrix
	that for each subgroup and day and hour keeps the double
	of the number of courses attended, for weekly activities. For fortnightly
	activities it keeps the sum for the two weeks (not multiplied by two).
	A value bigger than 2 in this matrix is considered
	a clash (increases hard fitness).
	Return value: the number of subgroups exhaustions (sum of values which are
	over 2).
	*/
	int getSubgroupsMatrix(Rules& r, int16 a[MAX_TOTAL_SUBGROUPS][MAX_DAYS_PER_WEEK][MAX_HOURS_PER_DAY]);

	/**
	The following function is very similar to GetTeachersTimetable,
	except that it is used in fitness calculation: it computes a matrix
	that for each teacher, day and hour, keeps the double
	of the number of courses attended, for weekly activities. For fortnightly
	activities it keeps the sum for the two weeks (not multiplied by two).
	A value bigger than 2 in this matrix is considered
	a clash (increases hard fitness).
	Return value: the number of teachers exhaustions (sum of values which are
	over 2).
	*/
	int getTeachersMatrix(Rules& r, int16 a[MAX_TEACHERS][MAX_DAYS_PER_WEEK][MAX_HOURS_PER_DAY]);
	
	/**
	This is a function that retrieves the rooms' timetable. Please see spacechromosome.h for more details
	*/
	void getRoomsTimetable(Rules& r, int16 a1[MAX_ROOMS][MAX_DAYS_PER_WEEK][MAX_HOURS_PER_DAY], int16 a2[MAX_ROOMS][MAX_DAYS_PER_WEEK][MAX_HOURS_PER_DAY]);

	/**
	This is a function that retrieves the rooms' matrix. Please see spacechromosome.h for more details
	*/
	int getRoomsMatrix(Rules& r, int16 a[MAX_ROOMS][MAX_DAYS_PER_WEEK][MAX_HOURS_PER_DAY]);
};

/**
This is a boolean function.
It returns 1 if c1 is better than c2.
*/
int better(Rules& r, TimeSpaceChromosome& c1, TimeSpaceChromosome& c2);

/**
This is a boolean function.
It returns 1 if c1 is better than c2. We have as parameters only the
fitnesses of the two chromosomes.
*/
int better(int hf1, int sf1, int hf2, int sf2);

#endif
