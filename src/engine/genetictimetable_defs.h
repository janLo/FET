/***************************************************************************
                          genetictimetable_defs.h  -  description
                             -------------------
    begin                : Sat Mar 15 2003
    copyright            : (C) 2003 by Lalescu Liviu
    email                : liviu@lalescu.ro
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef GENETICTIMETABLE_DEFS
#define GENETICTIMETABLE_DEFS

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#ifdef WIN32
#define for		if(0);else for
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include <qstring.h>
#include <qtranslator.h>
#include <qdir.h>

typedef short int16;

/**
The version number
*/
extern const QString FET_VERSION;

/**
The language
*/
extern QString FET_LANGUAGE;

/**
The maximum dimension of the population
*/
const int MAX_POPULATION_SIZE=8192;

/**
The maximum number of different years of students
*/
const int MAX_YEARS=70;

/**
Maximum number of allowed groups per year of students
*/
const int MAX_GROUPS_PER_YEAR=100;

/**
Maximum number of allowed subgroups per group of students
*/
const int MAX_SUBGROUPS_PER_GROUP=50;

/**
The maximum total number of different groups of students
*/
const int MAX_TOTAL_GROUPS=MAX_YEARS*MAX_GROUPS_PER_YEAR;

/**
The maximum total number of different subgroups of students
*/
const int MAX_TOTAL_SUBGROUPS=5000;//MAX_YEARS*MAX_GROUPS_PER_YEAR*MAX_SUBGROUPS_PER_GROUP;

/**
This constant represents the maximum number of total allowed subgroups
for any activity.
*/
const int MAX_SUBGROUPS_PER_ACTIVITY=400;//MAX_SUBGROUPS_PER_GROUP * MAX_GROUPS_PER_YEAR;

/**
This constant represents the maximum number of teachers
for any activity.
*/
const int MAX_TEACHERS_PER_ACTIVITY = 6;

const int MAX_SUBGROUPS_PER_CONSTRAINT=100;//MAX_SUBGROUPS_PER_GROUP * MAX_GROUPS_PER_YEAR;

/**
The maximum number of different teachers
*/
const int MAX_TEACHERS=700;

/**
The maximum number of different subjects
*/
const int MAX_SUBJECTS=1000;

/**
The maximum number of activities
*/
const int MAX_ACTIVITIES=1250;

/**
The maximum number of equipments
*/
const int MAX_EQUIPMENTS=300;

/**
The maximum number of equipments for a constraint subject requires equipments
*/
const int MAX_EQUIPMENTS_FOR_A_CONSTRAINT=50;

/**
The maximum number of rooms
*/
const int MAX_ROOMS=300;

/**
This constant represents an unallocated activity
*/
const int16 UNALLOCATED_ACTIVITY = MAX_ACTIVITIES;

/**
The maximum number of working hours per day.
*/
const int MAX_HOURS_PER_DAY=30;

/**
The maximum number of working days per week.
*/
const int MAX_DAYS_PER_WEEK=14;

/**
The predefined names of the days of the week
*/
const QString PREDEFINED_DAYS_OF_THE_WEEK[]={"Monday", "Tuesday", "Wednesday",
	"Thursday", "Friday", "Saturday", "Sunday", "Monday2", 
	"Tuesday2", "Wednesday2", "Thursday2", "Friday2", "Saturday2", "Sunday2"};

/**
The maximum number of working hours in a week.
<p>
hours in a week are arranged like this:
           Mo Tu We Th Fr
0-1        0  1  2  3  4
1-2        5  6  7  8  9
2-3        10 11 12 13 14
3-4        15 16 17 18 19
4-5        20 21 22 23 24
5-6        25 26 27 28 29 etc.
*/
const int MAX_HOURS_PER_WEEK = MAX_HOURS_PER_DAY * MAX_DAYS_PER_WEEK;

/**
This constant represents unallocated time for an activity
*/
const short UNALLOCATED_TIME = MAX_HOURS_PER_WEEK;

/**
This constant represents unallocated space for an activity
*/
const short UNALLOCATED_SPACE = MAX_ROOMS;


/**
The maximum number of time constraints
*/
const int MAX_TIME_CONSTRAINTS = 1500;

/**
The maximum number of space constraints
*/
const int MAX_SPACE_CONSTRAINTS = 1500;

/**
The maximum number of preferred times that can be considered
in this kind of constraint
*/
const int MAX_N_CONSTRAINT_ACTIVITY_PREFERRED_TIMES = 50;

/**
The maximum number of preferred times that can be considered
in this kind of constraint
*/
const int MAX_N_CONSTRAINT_ACTIVITIES_PREFERRED_TIMES = 50;

/**
The maximum number of activities that can be put in
a constraint of type ConstraintActivitiesMinNDaysBetweenActivities
I guess this variable must disappear and the
restriction modified to allocate dynamically the
necessary memory.
*/
const int MAX_CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES = 20;

/**
The maximum number of activities for a single
constraint of type
ConstraintActivitiesSameStartingTime
*/
const int MAX_CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME=20;

/**
The maximum number of activities for a single
constraint of type
ConstraintActivitiesSameStartingHour
*/
const int MAX_CONSTRAINT_ACTIVITIES_SAME_STARTING_HOUR=20;

/**
The maximum number of activities for a single
constraint of type
ConstraintActivitiesNotOverlapping
*/
const int MAX_CONSTRAINT_ACTIVITIES_NOT_OVERLAPPING=20;

/**
The maximum number of rooms for a single
constraint of type
ConstraintActivityPreferredRooms
*/
const int MAX_CONSTRAINT_ACTIVITY_PREFERRED_ROOMS=20;

/**
The maximum number of rooms for a single
constraint of type
ConstraintActivitiesSameRoom
*/
const int MAX_CONSTRAINT_ACTIVITIES_SAME_ROOM=20;

/**
The maximum number of activities which share the same subject name.
This constant is used (for now) in ConstraintSubjectRequiresEquipment.
*/
const int MAX_ACTIVITIES_FOR_A_SUBJECT=200;

/**
The maximum number of activities which share the same teacher name.
This constant is used (for now) in ConstraintTeacherRequiresRoom.
*/
const int MAX_ACTIVITIES_FOR_A_TEACHER=200;


/**
File and directory separator
*/
const QString FILE_SEP="/";


/**
The XML tag used for identification of the input file (old)
*/
const QString INPUT_FILE_TAG_3_6_1="FET_VERSION_3_6_1_AND_ABOVE_TIMETABLE_DATA_FILE";

/**
The XML tag used for identification of the output file
"Subgroup's Timetable"
*/
const QString STUDENTS_TIMETABLE_TAG="Students_Timetable";

/**
The XML tag used for identification of the output file
"Teacher's Timetable"
*/
const QString TEACHERS_TIMETABLE_TAG="Teachers_Timetable";

/**
The XML tag used for identification of the output file
"Rooms's Timetable"
*/
const QString ROOMS_TIMETABLE_TAG="Rooms_Timetable";

/**
The timetable's rules input file name
*/
extern QString INPUT_FILENAME_XML;

/**
The working directory
*/
extern QString WORKING_DIRECTORY;

/**
The initialization file for timetable program
*/
#ifdef WIN32
const QString INI_FILENAME="fet.ini";
#else
const QString INI_FILENAME=QDir::homeDirPath()+"/.fet/fet.ini";
#endif

//OUTPUT FILES

/**
The output directory. Please be careful when editing it,
because the functions add a FILE_SEP sign at the end of it
and then the name of a file. If you make OUTPUT_DIR="",
there will be problems.
*/
#ifdef WIN32
const QString OUTPUT_DIR="results";
#else
const QString OUTPUT_DIR=QDir::homeDirPath()+"/.fet/results";
#endif

/**
A log of the time simulation.
*/
const QString TIME_LOG_FILENAME_TXT="time_log.txt";

/**
A log of the space simulation.
*/
const QString SPACE_LOG_FILENAME_TXT="space_log.txt";

/**
A log of the time and space simulation.
*/
const QString TIME_SPACE_LOG_FILENAME_TXT="time_space_log.txt";

/**
An output file containing the timetable for each subgroup,
arranged in xml format
*/
const QString STUDENTS_TIMETABLE_FILENAME_XML="studentstimetable.xml";

/**
An output file containing the timetable for each teacher,
arranged in xml format
*/
const QString TEACHERS_TIMETABLE_FILENAME_XML="teacherstimetable.xml";

/**
An output file containing the timetable for each room,
arranged in xml format
*/
const QString ROOMS_TIMETABLE_FILENAME_XML="roomstimetable.xml";

/**
An output file containing the timetable for each subgroup,
arranged in html format
*/
const QString STUDENTS_TIMETABLE_FILENAME_HTML="studentstimetable.html";

/**
An output file containing the timetable for each teacher,
arranged in html format. Version 1 - flyers for each teacher
*/
const QString TEACHERS_TIMETABLE_1_FILENAME_HTML="teacherstimetable1.html";

/**
An output file containing the timetable for each teacher,
arranged in html format. Version 2 - overall
*/
const QString TEACHERS_TIMETABLE_2_FILENAME_HTML="teacherstimetable2.html";

/**
An output file containing the timetable for each room,
arranged in html format
*/
const QString ROOMS_TIMETABLE_FILENAME_HTML="roomstimetable.html";

/**
An output file containing the timetable (with rooms) for each subgroup,
arranged in html format
*/
const QString STUDENTS_TIMETABLE_WITH_ROOMS_FILENAME_HTML="studentstimetablewithrooms.html";

/**
An output file containing the timetable for each teacher (with rooms),
arranged in html format. Version 1 - flyers for each teacher
*/
const QString TEACHERS_TIMETABLE_WITH_ROOMS_1_FILENAME_HTML="teacherstimetablewithrooms1.html";

/**
An output file containing the timetable for each teacher (with rooms),
arranged in html format. Version 2 - overall
*/
const QString TEACHERS_TIMETABLE_WITH_ROOMS_2_FILENAME_HTML="teacherstimetablewithrooms2.html";

/**
A log file explaining how the xml input file was parsed
*/
const QString XML_PARSING_LOG_FILENAME="xmlreadinglog.txt";

/**
A file containing the time conflicts
*/
const QString TIME_CONFLICTS_FILENAME="time_conflicts.txt";

/**
A file containing the space conflicts
*/
const QString SPACE_CONFLICTS_FILENAME="space_conflicts.txt";

/**
A file containing the time&space conflicts
*/
const QString TIME_SPACE_CONFLICTS_FILENAME="time_space_conflicts.txt";

/**
Probability for Evolution1 Crossover, in percents
*/
extern int METHOD1_CROSSOVER_PROBABILITY;

/**
Probability for Evolution1 Mutation1, in percents
*/
extern int METHOD1_MUTATION1_PROBABILITY;

/**
Probability for Evolution1 Mutation2, in percents
*/
extern int METHOD1_MUTATION2_PROBABILITY;

/**
Probability for Evolution2 Crossover, in percents
*/
extern int METHOD2_CROSSOVER_PROBABILITY;

/**
Probability for Evolution2 Mutation1, in percents
*/
extern int METHOD2_MUTATION1_PROBABILITY;

/**
Probability for Evolution2 Mutation2, in percents
*/
extern int METHOD2_MUTATION2_PROBABILITY;

/**
Probability for Evolution2 propagation, in percents
*/
extern int METHOD2_PROPAGATION_PROBABILITY;

#endif
