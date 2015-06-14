/***************************************************************************
                          addconstraintteachersmaxhoursdailyform.h  -  description
                             -------------------
    begin                : Feb 10, 2005
    copyright            : (C) 2005 by Lalescu Liviu
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

#ifndef ADDCONSTRAINTTEACHERSMAXHOURSDAILYFORM_H
#define ADDCONSTRAINTTEACHERSMAXHOURSDAILYFORM_H

#include "addconstraintteachersmaxhoursdailyform_template.h"
#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fet.h"
#include "fetmainform.h"

#include <q3combobox.h>
#include <qmessagebox.h>
#include <q3groupbox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <q3textedit.h>

class AddConstraintTeachersMaxHoursDailyForm : public AddConstraintTeachersMaxHoursDailyForm_template  {
public:
	AddConstraintTeachersMaxHoursDailyForm();
	~AddConstraintTeachersMaxHoursDailyForm();

	void updateMaxHoursSpinBox();

	void constraintChanged();
	void addCurrentConstraint();
};

#endif
