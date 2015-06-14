/***************************************************************************
                          timetableallocatehoursroomsform.h  -  description
                             -------------------
    begin                : Feb 14, 2005
    copyright            : (C) 2005 by Lalescu Liviu
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

#ifndef TIMETABLEALLOCATHOURSROOMSFORM_H
#define TIMETABLEALLOCATEHOURSROOMSFORM_H

#include "timetableallocatehoursroomsform_template.h"
#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fet.h"

#include <qcombobox.h>
#include <qmessagebox.h>
#include <qgroupbox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qtable.h>
#include <qapplication.h>
#include <qstring.h>
#include <qthread.h>
#include <qmutex.h>

class TimetableAllocateHoursRoomsForm;

class TimeSpaceSolvingThread: public QThread{
public:

	TimetableAllocateHoursRoomsForm* callingForm;

	TimeSpaceSolvingThread();

	void run();
};

class TimetableAllocateHoursRoomsForm : public TimetableAllocateHoursRoomsForm_template
{
public:

	TimetableAllocateHoursRoomsForm();

	~TimetableAllocateHoursRoomsForm();

	void start();

	void stop();

	void pause();

	void write();

	void generationLogging(int generation);

	void simulationRunning();

	void writeSimulationResults(TimeSpaceChromosome& c);

	/**
	Function writing the students' timetable in xml format, to a file
	*/
	void writeStudentsTimetableXml(const QString& xmlfilename);

	/**
	Function writing the teachers' timetable in xml format, to a file
	*/
	void writeTeachersTimetableXml(const QString& xmlfilename);

	/**
	Function writing the rooms' timetable in xml format, to a file
	*/
	void writeRoomsTimetableXml(const QString& xmlfilename);

	/**
	Function writing the students' timetable (with rooms) in html format, to a file
	*/
	void writeStudentsTimetableWithRoomsHtml(const QString& htmlfilename);

	/**
	Function writing the teachers' timetable (with rooms) in html format, to a file
	Variant 1.
	*/
	void writeTeachersTimetableWithRooms1Html(const QString& htmlfilename);

	/**
	Function writing the teachers' timetable (with rooms) in html format, to a file
	Variant 2.
	*/
	void writeTeachersTimetableWithRooms2Html(const QString& htmlfilename);

	/**
	Function writing the rooms' timetable in html format, to a file
	*/
	void writeRoomsTimetableHtml(const QString& htmlfilename);

	void getTeachersTimetable(TimeSpaceChromosome& c);

	void getStudentsTimetable(TimeSpaceChromosome& c);

	void getRoomsTimetable(TimeSpaceChromosome& c);

	void savePosition();

	void loadPosition();

	void initializeUnallocated();

	void initializeRandomly();
};

#endif
