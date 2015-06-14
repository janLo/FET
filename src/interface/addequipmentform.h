/***************************************************************************
                          addequipmentform.h  -  description
                             -------------------
    begin                : Thu Jan 29 2004
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

#ifndef ADDEQUIPMENTFORM_H
#define ADDEQUIPMENTFORM_H

#include "addequipmentform_template.h"

#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fet.h"

#include "fetmainform.h"

#include <qspinbox.h>
#include <qlineedit.h>
#include <qtextedit.h>


class AddEquipmentForm : public AddEquipmentForm_template {
public:
	AddEquipmentForm();
	~AddEquipmentForm();

	void addEquipment();
};

#endif
