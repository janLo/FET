/***************************************************************************
                          addconstraintactivitypreferredtimesform.h  -  description
                             -------------------
    begin                : October 3, 2003
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

#ifndef ADDCONSTRAINTACTIVITYPREFERREDTIMESFORM_H
#define ADDCONSTRAINTACTIVITYPREFERREDTIMESFORM_H

#include "addconstraintactivitypreferredtimesform_template.h"
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

class AddConstraintActivityPreferredTimesForm : public AddConstraintActivityPreferredTimesForm_template  {
public:
	AddConstraintActivityPreferredTimesForm();
	~AddConstraintActivityPreferredTimesForm();

	void updateActivitiesComboBox();

	void addConstraint();
	
	void tableClicked(int, int, int, const QPoint&);
};

#endif
