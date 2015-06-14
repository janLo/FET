/***************************************************************************
                          constraintteacherssubjecttagsmaxhourscontinuouslyform.h  -  description
                             -------------------
    begin                : Apr 28, 2005
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

#ifndef CONSTRAINTTEACHERSSUBJECTTAGSMAXHOURSCONTINUOUSLYFORM_H
#define CONSTRAINTTEACHERSSUBJECTTAGSMAXHOURSCONTINUOUSLYFORM_H

#include "constraintteacherssubjecttagsmaxhourscontinuouslyform_template.h"
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
#include <q3listbox.h>

class ConstraintTeachersSubjectTagsMaxHoursContinuouslyForm : public ConstraintTeachersSubjectTagsMaxHoursContinuouslyForm_template  {
public:
	TimeConstraintsList visibleConstraintsList;

	ConstraintTeachersSubjectTagsMaxHoursContinuouslyForm();
	~ConstraintTeachersSubjectTagsMaxHoursContinuouslyForm();

	void constraintChanged(int index);
	void addConstraint();
	void modifyConstraint();
	void removeConstraint();

	void filterChanged();

	bool filterOk(TimeConstraint* ctr);
};

#endif
