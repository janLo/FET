/***************************************************************************
                          addspaceconstraintform.h  -  description
                             -------------------
    begin                : Thu Jan 8 2004
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

#ifndef ADDSPACECONSTRAINTFORM_H
#define ADDSPACECONSTRAINTFORM_H

#include "addspaceconstraintform_template.h"
#include "addconstraintroomnotavailableform.h"
#include "addconstraintroomtypenotallowedsubjectsform.h"
#include "addconstraintsubjectrequiresequipmentsform.h"
#include "addconstraintteacherrequiresroomform.h"
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


class AddSpaceConstraintForm : public AddSpaceConstraintForm_template  {
public:
	AddSpaceConstraintForm();
	~AddSpaceConstraintForm();

	void addConstraintRoomNotAvailable();
	void addConstraintRoomTypeNotAllowedSubjects();
	void addConstraintSubjectRequiresEquipments();
	void addConstraintTeacherRequiresRoom();
};

#endif
