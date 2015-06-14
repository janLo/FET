/***************************************************************************
                          constraintstudentssethomeroomsform.h  -  description
                             -------------------
    begin                : 8 April 2005
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

#ifndef CONSTRAINTSTUDENTSSETHOMEROOMSFORM_H
#define CONSTRAINTSTUDENTSSETHOMEROOMSFORM_H

#include "ui_constraintstudentssethomeroomsform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class ConstraintStudentsSetHomeRoomsForm : public QDialog, Ui::ConstraintStudentsSetHomeRoomsForm_template  {
	Q_OBJECT
public:
	SpaceConstraintsList visibleConstraintsList;

	ConstraintStudentsSetHomeRoomsForm();
	~ConstraintStudentsSetHomeRoomsForm();

	bool filterOk(SpaceConstraint* ctr);
	
	void refreshConstraintsListBox();

public slots:
	void constraintChanged(int index);
	void addConstraint();
	void removeConstraint();
	void modifyConstraint();
	
	void filterChanged();
};

#endif
