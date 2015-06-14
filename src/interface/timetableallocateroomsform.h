/***************************************************************************
                          timetableallocateroomsform.h  -  description
                             -------------------
    begin                : Tue Apr 22 2003
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

#ifndef TIMETABLEALLOCATEROOMSFORM_H
#define TIMETABLEALLOCATEROOMSFORM_H

#include "timetableallocateroomsform_template.h"
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

class TimetableAllocateRoomsForm;

class SpaceSolvingThread: public QThread{
public:

	TimetableAllocateRoomsForm* callingForm;

	SpaceSolvingThread();

	void run();
};

class TimetableAllocateRoomsForm : public TimetableAllocateRoomsForm_template
{
public:

	TimetableAllocateRoomsForm();

	~TimetableAllocateRoomsForm();

	void start();

	void stop();

	void pause();

	void write();

	void generationLogging(int generation);

	void simulationRunning();

	void writeSimulationResults(SpaceChromosome& c);

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

	void getRoomsTimetable(SpaceChromosome& c);

	void savePosition();

	void loadPosition();

	void initializeUnallocated();

	void initializeRandomly();
};

#endif
