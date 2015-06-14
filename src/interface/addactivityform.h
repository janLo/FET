/***************************************************************************
                          addactivityform.h  -  description
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

#ifndef ADDACTIVITYFORM_H
#define ADDACTIVITYFORM_H

#include "addactivityform_template.h"

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


class AddActivityForm : public AddActivityForm_template  {
public: 
	AddActivityForm();
	~AddActivityForm();

	void updateStudentsComboBox();
	void updateTeachersComboBox();
	void updateSubjectsComboBox();
	void updatePreferredDaysComboBox();
	void updatePreferredHoursComboBox();

	void teacherChanged(const QString& dummy);
	void subjectChanged(const QString& dummy);
	void studentsChanged(const QString& dummy);
	void hourChanged();
	void dayChanged();
	void parityChanged();
	void durationChanged();
	void activityChanged();
	void splitChanged();
	
	void showYearsChanged();
	void showGroupsChanged();
	void showSubgroupsChanged();

	void addActivity();
};

#endif
