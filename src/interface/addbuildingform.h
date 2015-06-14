/***************************************************************************
                          addbuildingform.h  -  description
                             -------------------
    begin                : 9 July 2005
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

#ifndef ADDBUILDINGFORM_H
#define ADDBUILDINGFORM_H

#include "addbuildingform_template.h"

#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fet.h"

#include "fetmainform.h"

#include <qspinbox.h>
#include <qlineedit.h>
#include <qtextedit.h>


class AddBuildingForm : public AddBuildingForm_template {
public:
	AddBuildingForm();
	~AddBuildingForm();

	void addBuilding();
};

#endif
