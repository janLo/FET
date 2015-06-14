/***************************************************************************
                          roomsequipmentform.h  -  description
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

#ifndef ROOMSEQUIPMENTSFORM_H
#define ROOMSEQUIPMENTSFORM_H

#include "roomsequipmentsform_template.h"
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

class RoomsEquipmentsForm : public RoomsEquipmentsForm_template  {
public:
	RoomsEquipmentsForm();
	~RoomsEquipmentsForm();

	void updateRoomsComboBox();
	void updateAllEquipmentsListBox();

	void roomChanged(int i);

	void addEquipment();
	void removeEquipment();
};

#endif
