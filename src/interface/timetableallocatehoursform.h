/***************************************************************************
                          timetableallocatehoursform.h  -  description
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

#ifndef TIMETABLEALLOCATEHOURSFORM_H
#define TIMETABLEALLOCATEHOURSFORM_H

#include "timetableallocatehoursform_template.h"
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

class TimetableAllocateHoursForm;

class TimeSolvingThread: public QThread{
public:

	TimetableAllocateHoursForm* callingForm;

	TimeSolvingThread();

	void run();
};

class TimetableAllocateHoursForm : public TimetableAllocateHoursForm_template  {
public:

	TimetableAllocateHoursForm();

	~TimetableAllocateHoursForm();

	void start();

	void stop();

	void pause();

	void write();

	void generationLogging(int generation);

	void simulationRunning();

	void writeSimulationResults(TimeChromosome& c);

	/**
	Function writing the students' timetable xml format, to a file
	*/
	void writeStudentsTimetable(const QString& xmlfilename);

	/**
	Function writing the teachers' timetable in xml format
	*/
	void writeTeachersTimetable(const QString& xmlfilename);

	void getStudentsTimetable(TimeChromosome& c);

	void getTeachersTimetable(TimeChromosome& c);

	void savePosition();

	void loadPosition();

	void initializeUnallocated();

	void initializeRandomly();

	void closePressed();
};

#endif
