/***************************************************************************
                          addconstraintteachersmaxhourscontinuouslyform.h  -  description
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

#ifndef ADDCONSTRAINTTEACHERSMAXHOURSCONTINUOUSLYFORM_H
#define ADDCONSTRAINTTEACHERSMAXHOURSCONTINUOUSLYFORM_H

#include "ui_addconstraintteachersmaxhourscontinuouslyform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class AddConstraintTeachersMaxHoursContinuouslyForm : public QDialog, Ui::AddConstraintTeachersMaxHoursContinuouslyForm_template  {
	Q_OBJECT
public:
	AddConstraintTeachersMaxHoursContinuouslyForm();
	~AddConstraintTeachersMaxHoursContinuouslyForm();

	void updateMaxHoursSpinBox();

public slots:
	void constraintChanged();
	void addCurrentConstraint();
};

#endif
