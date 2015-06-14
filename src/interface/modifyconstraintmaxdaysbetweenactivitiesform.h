/***************************************************************************
                          modifyconstraintmaxdaysbetweenactivitiesform.h  -  description
                             -------------------
    begin                : 2009
    copyright            : (C) 2009 by Lalescu Liviu
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

#ifndef MODIFYCONSTRAINTMAXDAYSBETWEENACTIVITIESFORM_H
#define MODIFYCONSTRAINTMAXDAYSBETWEENACTIVITIESFORM_H

#include "ui_modifyconstraintmaxdaysbetweenactivitiesform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"
//#include "fetmainform.h"

#include <q3combobox.h>
#include <q3listbox.h>
#include <qmessagebox.h>
#include <q3groupbox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <q3textedit.h>

#include <QList>

class ModifyConstraintMaxDaysBetweenActivitiesForm : public QDialog, Ui::ModifyConstraintMaxDaysBetweenActivitiesForm_template  {
	Q_OBJECT	

public:
	ModifyConstraintMaxDaysBetweenActivitiesForm(ConstraintMaxDaysBetweenActivities* ctr);
	~ModifyConstraintMaxDaysBetweenActivitiesForm();

	bool filterOk(Activity* ac);
	
public slots:
	void addActivity();
	void removeActivity();

	void clear();

	void ok();
	void cancel();
	
	void filterChanged();

private:
	ConstraintMaxDaysBetweenActivities* _ctr;
	//the id's of the activities listed in the activities list
	QList<int> activitiesList;
	//the id-s of the activities listed in the list of selected activities
	QList<int> selectedActivitiesList;
};

#endif
