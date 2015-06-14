/***************************************************************************
                          addconstraintroomtypenotallowedsubjectsform.h  -  description
                             -------------------
    begin                : January 9, 2004
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

#ifndef ADDCONSTRAINTROOMTYPENOTALLOWEDSUBJECTSFORM_H
#define ADDCONSTRAINTROOMTYPENOTALLOWEDSUBJECTSFORM_H

#include "addconstraintroomtypenotallowedsubjectsform_template.h"
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

class AddConstraintRoomTypeNotAllowedSubjectsForm : public AddConstraintRoomTypeNotAllowedSubjectsForm_template  {
public:
	AddConstraintRoomTypeNotAllowedSubjectsForm();
	~AddConstraintRoomTypeNotAllowedSubjectsForm();

	void updateTypesComboBox();
	void updateSubjectsListBox();

	void addSubject();
	void removeSubject();

	void addConstraint();
};

#endif
