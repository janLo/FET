/***************************************************************************
                          addconstraintactivitiespreferredtimesform.h  -  description
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

#ifndef ADDCONSTRAINTACTIVITIESPREFERREDTIMESFORM_H
#define ADDCONSTRAINTACTIVITIESPREFERREDTIMESFORM_H

#include "addconstraintactivitiespreferredtimesform_template.h"
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
#include <qpoint.h>

class AddConstraintActivitiesPreferredTimesForm : public AddConstraintActivitiesPreferredTimesForm_template  {
public:
	AddConstraintActivitiesPreferredTimesForm();
	~AddConstraintActivitiesPreferredTimesForm();

	void updateTeachersComboBox();
	void updateStudentsComboBox();
	void updateSubjectsComboBox();

	void addConstraint();
	
	void tableClicked(int, int, int, const QPoint&);
};

#endif
