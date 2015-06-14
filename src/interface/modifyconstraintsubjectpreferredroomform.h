/***************************************************************************
                          modifyconstraintsubjectpreferredroomform.h  -  description
                             -------------------
    begin                : April 8, 2005
    copyright            : (C) 2005 by Lalescu Liviu
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

#ifndef MODIFYCONSTRAINTSUBJECTPREFERREDROOMFORM_H
#define MODIFYCONSTRAINTSUBJECTPREFERREDROOMFORM_H

#include "modifyconstraintsubjectpreferredroomform_template.h"
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

class ModifyConstraintSubjectPreferredRoomForm : public ModifyConstraintSubjectPreferredRoomForm_template  {
	ConstraintSubjectPreferredRoom* _ctr;
public:
	ModifyConstraintSubjectPreferredRoomForm(ConstraintSubjectPreferredRoom* ctr);
	~ModifyConstraintSubjectPreferredRoomForm();

	void updateSubjectsComboBox();
	void updateRoomsComboBox();

	void ok();
	void cancel();
};

#endif
