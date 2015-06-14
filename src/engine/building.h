//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Liviu Lalescu <liviu@lalescu.ro>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BUILDING_H
#define BUILDING_H

#include "genetictimetable_defs.h"

#include <qstring.h>
#include <qptrlist.h>
#include <qstringlist.h>

class Building;
class Rules;

class BuildingsList: public QPtrList<Building>
{
	int compareItems(QPtrCollection::Item, QPtrCollection::Item);
};

/**
This class represents a building

@author Liviu Lalescu
*/
class Building{
public:
	QString name;

	Building();
	~Building();
	
	QString getXmlDescription();
	QString getDescription();
	QString getDetailedDescription();
	QString getDetailedDescriptionWithConstraints(Rules& r);
};

#endif
