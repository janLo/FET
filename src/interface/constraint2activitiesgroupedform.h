/***************************************************************************
                          constraint2activitiesgrouped.h  -  description
                             -------------------
    begin                : 15 May 2004
    copyright            : (C) 2004 by Lalescu Liviu
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

#ifndef CONSTRAINT2ACTIVITIESGROUPEDFORM_H
#define CONSTRAINT2ACTIVITIESGROUPEDFORM_H

#include "constraint2activitiesgroupedform_template.h"
#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fet.h"
#include "fetmainform.h"

#include <qcombobox.h>
#include <qmessagebox.h>
#include <qgroupbox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qlistbox.h>

class Constraint2ActivitiesGroupedForm : public Constraint2ActivitiesGroupedForm_template  {
public:
	TimeConstraintsList visibleConstraintsList;

	Constraint2ActivitiesGroupedForm();
	~Constraint2ActivitiesGroupedForm();

	void constraintChanged(int index);
	void addConstraint();
	void removeConstraint();
	
	void refreshConstraintsListBox();

	bool filterOk(TimeConstraint* ctr);
};

#endif
