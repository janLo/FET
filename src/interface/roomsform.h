//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Liviu Lalescu <liviu@lalescu.ro>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ROOMSFORM_H
#define ROOMSFORM_H

#include "roomsform_template.h"

/**
@author Liviu Lalescu
*/
class RoomsForm : public RoomsForm_template
{
public:
	RoomsForm();
	~RoomsForm();

	void addRoom();
	void removeRoom();
	void modifyRoom();
	void sortRooms();
	void roomChanged(int index);
	void roomsEquipments();
};

#endif
