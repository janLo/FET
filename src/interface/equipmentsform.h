//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Liviu Lalescu <liviu@lalescu.ro>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef EQUIPMENTSFORM_H
#define EQUIPMENTSFORM_H

#include "addequipmentform.h"
#include "equipmentsform_template.h"

/**
@author Liviu Lalescu
*/
class EquipmentsForm : public EquipmentsForm_template
{
public:
	EquipmentsForm();

	~EquipmentsForm();

	void addEquipment();

	void removeEquipment();

	void equipmentChanged(int index);
};

#endif
