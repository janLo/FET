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
#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "genetictimetable_defs.h"

#include <qstring.h>
#include <qptrlist.h>
#include <qstringlist.h>

class Equipment;

typedef QPtrList<Equipment> EquipmentsList;

/**
This class represents an equipment (a special device)
*/
class Equipment
{
public:
	QString name;

	Equipment();
	~Equipment();

	QString getDescription();
	QString getDetailedDescription();
	QString getXMLDescription();
};

#endif
