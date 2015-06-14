/***************************************************************************
                          addstudentssubgroupform.h  -  description
                             -------------------
    begin                : Sat Jan 24 2004
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

#ifndef ADDSTUDENTSSUBGROUPFORM_H
#define ADDSTUDENTSSUBGROUPFORM_H

#include "addstudentssubgroupform_template.h"

#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fet.h"

#include "fetmainform.h"

#include <qspinbox.h>
#include <qlineedit.h>
#include <qtextedit.h>


class AddStudentsSubgroupForm : public AddStudentsSubgroupForm_template {
public:
	AddStudentsSubgroupForm();
	~AddStudentsSubgroupForm();

	void addStudentsSubgroup();
};

#endif
