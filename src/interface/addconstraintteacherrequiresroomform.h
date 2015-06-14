/***************************************************************************
                          addconstraintteacherrequiresroomform.h  -  description
                             -------------------
    begin                : September 13, 2004
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

#ifndef ADDCONSTRAINTTEACHERREQUIRESROOMFORM_H
#define ADDCONSTRAINTTEACHERREQUIRESROOMFORM_H

#include "addconstraintteacherrequiresroomform_template.h"
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

class AddConstraintTeacherRequiresRoomForm : public AddConstraintTeacherRequiresRoomForm_template  {
public:
	AddConstraintTeacherRequiresRoomForm();
	~AddConstraintTeacherRequiresRoomForm();

	void updateTeachersComboBox();
	void updateRoomsComboBox();

	void addConstraint();
};

#endif
