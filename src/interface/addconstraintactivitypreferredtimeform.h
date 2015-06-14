/***************************************************************************
                          addconstraintactivitypreferredtimeform.h  -  description
                             -------------------
    begin                : Feb 11, 2005
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

#ifndef ADDCONSTRAINTACTIVITYPREFERREDTIMEFORM_H
#define ADDCONSTRAINTACTIVITYPREFERREDTIMEFORM_H

#include "addconstraintactivitypreferredtimeform_template.h"
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

class AddConstraintActivityPreferredTimeForm : public AddConstraintActivityPreferredTimeForm_template  {
public:
	AddConstraintActivityPreferredTimeForm();
	~AddConstraintActivityPreferredTimeForm();

	void updatePeriodGroupBox();
	void updateActivitiesComboBox();

	void constraintChanged();
	void addCurrentConstraint();
};

#endif
