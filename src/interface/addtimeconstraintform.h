/***************************************************************************
                          addtimeconstraintform.h  -  description
                             -------------------
    begin                : Wed Apr 23 2003
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

#ifndef ADDTIMECONSTRAINTFORM_H
#define ADDTIMECONSTRAINTFORM_H

#include "addtimeconstraintform_template.h"
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

class AddTimeConstraintForm : public AddTimeConstraintForm_template  {
public:
	AddTimeConstraintForm();
	~AddTimeConstraintForm();

	void updatePeriodGroupBox();
	void updatePeriodGroupBox2();
	void updateHoursBox();
	void updateHoursBox2();
	void updateHoursBox3();
	void updateHoursBox4();
	void updateStudentsComboBox();
	void updateTeachersComboBox();
	void updateActivitiesComboBox();

	void teacherChanged(const QString& teacher_name);
	void studentsChanged(const QString& dummy);
	void startHourChanged();
	void endHourChanged();
	void dayChanged();
	void typeChanged();
	void weightChanged();
	void maxHoursChanged();
	void minHoursChanged();
	void activitiesSameStartingTimeTextChanged();
	void hoursDailyChanged();
	void activityChanged();

	void constraintChanged();

	void addCurrentConstraint();
	
	void addConstraintActivityPreferredTimes();
};

#endif
