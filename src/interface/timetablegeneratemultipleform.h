/***************************************************************************
                          timetablegeneratemultipleform.h  -  description
                             -------------------
    begin                : Aug 20 2007
    copyright            : (C) 2007 by Lalescu Liviu
    email                : Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TIMETABLEGENERATEMULTIPLEFORM_H
#define TIMETABLEGENERATEMULTIPLEFORM_H

#include "timetablegeneratemultipleform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

#include <qthread.h>

class GenerateMultipleThread: public QThread{
	Q_OBJECT

public:
	void run();

signals:
	void timetableGenerated(int timetable, const QString& description, bool ok);
	
	void finished();
};

class TimetableGenerateMultipleForm : public TimetableGenerateMultipleForm_template  {
	Q_OBJECT

public:

	TimetableGenerateMultipleForm();

	~TimetableGenerateMultipleForm();
	
	void help();

	void start();

	void stop();
	
	void simulationFinished();

	void closePressed();
	
	void writeTimetableDataFile();

private slots:
	void timetableGenerated(int timetable, const QString& description, bool ok);
	
	void finished();
	
	void activityPlaced(int na);
};

#endif
