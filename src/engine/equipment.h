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
class Rules;

class EquipmentsList: public QPtrList<Equipment>
{
	int compareItems(QPtrCollection::Item, QPtrCollection::Item);
};

/**
This class represents an equipment
*/
class Equipment
{
public:
	QString name;

	Equipment();
	~Equipment();

	QString getXmlDescription();
	QString getDescription();
	QString getDetailedDescription();
	QString getDetailedDescriptionWithConstraints(Rules& r);
};

#endif
