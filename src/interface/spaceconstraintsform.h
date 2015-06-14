/***************************************************************************
                          spaceconstraintsform.h  -  description
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

#ifndef SPACECONSTRAINTSFORM_H
#define SPACECONSTRAINTSFORM_H

#include "spaceconstraintsform_template.h"
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
#include <qlistbox.h>

class SpaceConstraintsForm : public SpaceConstraintsForm_template  {
public:
	SpaceConstraintsList visibleConstraintsList;

	SpaceConstraintsForm();
	~SpaceConstraintsForm();

	void constraintChanged(int index);
	void addConstraint();
	void removeConstraint();

	void filterChanged();

	bool filterOk(SpaceConstraint* ctr);
};

#endif
