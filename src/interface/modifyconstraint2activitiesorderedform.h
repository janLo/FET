/***************************************************************************
                          modifyconstraint2activitiesorderedform.h  -  description
                             -------------------
    begin                : Aug 21, 2007
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

#ifndef MODIFYCONSTRAINT2ACTIVITIESORDEREDFORM_H
#define MODIFYCONSTRAINT2ACTIVITIESORDEREDFORM_H

#include "modifyconstraint2activitiesorderedform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

#include <q3combobox.h>
#include <qmessagebox.h>
#include <q3groupbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <q3textedit.h>

class ModifyConstraint2ActivitiesOrderedForm : public ModifyConstraint2ActivitiesOrderedForm_template  {
public:
	Constraint2ActivitiesOrdered* _ctr;

	ModifyConstraint2ActivitiesOrderedForm(Constraint2ActivitiesOrdered* ctr);
	~ModifyConstraint2ActivitiesOrderedForm();

	void updateActivitiesComboBox();

	void constraintChanged();
	void ok();
	void cancel();

	bool filterOk(Activity* a);
	void filterChanged();
	
private:
	//the id's of the activities listed in the activities combo
	QList<int> firstActivitiesList;
	QList<int> secondActivitiesList;
};

#endif
