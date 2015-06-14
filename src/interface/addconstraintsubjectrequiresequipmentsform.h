/***************************************************************************
                          addconstraintsubjectrequiresequipmentsform.h  -  description
                             -------------------
    begin                : January 29, 2004
    copyright            : (C) 2004 by Lalescu Liviu
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

#ifndef ADDCONSTRAINTSUBJECTREQUIRESEQUIPMENTSFORM_H
#define ADDCONSTRAINTSUBJECTREQUIRESEQUIPMENTSFORM_H

#include "addconstraintsubjectrequiresequipmentsform_template.h"
#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fet.h"
#include "fetmainform.h"

#include <qcombobox.h>
#include <qlistbox.h>
#include <qmessagebox.h>
#include <qgroupbox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qtextedit.h>

class AddConstraintSubjectRequiresEquipmentsForm : public AddConstraintSubjectRequiresEquipmentsForm_template  {
public:
	AddConstraintSubjectRequiresEquipmentsForm();
	~AddConstraintSubjectRequiresEquipmentsForm();

	void updateSubjectsComboBox();
	void updateEquipmentsListBox();

	void addEquipment();
	void removeEquipment();

	void addConstraint();
};

#endif
