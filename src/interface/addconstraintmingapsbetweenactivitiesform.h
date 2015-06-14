/***************************************************************************
                          addconstraintmingapsbetweenactivitiesform.h  -  description
                             -------------------
    begin                : 10 July 2008
    copyright            : (C) 2008 by Lalescu Liviu
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

#ifndef ADDCONSTRAINTMINGAPSBETWEENACTIVITIESFORM_H
#define ADDCONSTRAINTMINGAPSBETWEENACTIVITIESFORM_H

#include "ui_addconstraintmingapsbetweenactivitiesform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

#include <QList>

class AddConstraintMinGapsBetweenActivitiesForm : public QDialog, Ui::AddConstraintMinGapsBetweenActivitiesForm_template  {
	Q_OBJECT

public:
	AddConstraintMinGapsBetweenActivitiesForm();
	~AddConstraintMinGapsBetweenActivitiesForm();

	void updateActivitiesListBox();

	bool filterOk(Activity* a);

public slots:
	void addActivity();
	void removeActivity();

	void addConstraint();

	void filterChanged();	
	
	void clear();
	
private:
	//the id's of the activities listed in the activities list
	QList<int> activitiesList;
	//the id-s of the activities listed in the list of selected activities
	QList<int> selectedActivitiesList;
};

#endif
