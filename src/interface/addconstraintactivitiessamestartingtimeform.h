/***************************************************************************
                          addconstraintactivitiessamestartingtimeform.h  -  description
                             -------------------
    begin                : June 23, 2004
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

#ifndef ADDCONSTRAINTACTIVITIESSAMESTARTINGTIMEFORM_H
#define ADDCONSTRAINTACTIVITIESSAMESTARTINGTIMEFORM_H

#include "addconstraintactivitiessamestartingtimeform_template.h"
#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fet.h"
#include "fetmainform.h"

#include <qcombobox.h>
#include <qlistbox.h>
#include <qmessagebox.h>
#include <qgroupbox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qvaluelist.h>

class AddConstraintActivitiesSameStartingTimeForm : public AddConstraintActivitiesSameStartingTimeForm_template  {
public:
	AddConstraintActivitiesSameStartingTimeForm();
	~AddConstraintActivitiesSameStartingTimeForm();

	void updateActivitiesListBox();
	
	void blockChanged();

	void addActivity();
	void removeActivity();

	void addConstraint();
	
private:
	//the id's of the activities listed in the activities list
	QValueList<int> activitiesList;
	//the id-s of the activities listed in the list of simultaneous activities
	QValueList<int> simultaneousActivitiesList;
};

#endif
