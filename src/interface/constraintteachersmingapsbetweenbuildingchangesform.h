/***************************************************************************
                          constraintteachersmingapsbetweenbuildingchangesform.h  -  description
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

#ifndef CONSTRAINTTEACHERSMINGAPSBETWEENBUILDINGCHANGESFORM_H
#define CONSTRAINTTEACHERSMINGAPSBETWEENBUILDINGCHANGESFORM_H

#include "ui_constraintteachersmingapsbetweenbuildingchangesform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

#include <q3combobox.h>
#include <qmessagebox.h>
#include <q3groupbox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <q3textedit.h>
#include <q3listbox.h>

class ConstraintTeachersMinGapsBetweenBuildingChangesForm : public QDialog, Ui::ConstraintTeachersMinGapsBetweenBuildingChangesForm_template  {
	Q_OBJECT
public:
	SpaceConstraintsList visibleConstraintsList;

	ConstraintTeachersMinGapsBetweenBuildingChangesForm();
	~ConstraintTeachersMinGapsBetweenBuildingChangesForm();

	bool filterOk(SpaceConstraint* ctr);

public slots:
	void constraintChanged(int index);
	void addConstraint();
	void modifyConstraint();
	void removeConstraint();

	void filterChanged();
};

#endif
