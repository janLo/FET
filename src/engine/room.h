//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Liviu Lalescu <Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ROOM_H
#define ROOM_H

#include "genetictimetable_defs.h"

#include <qstring.h>
#include <qptrlist.h>
#include <qstringlist.h>

class Room;
class Rules;

class RoomsList: public QPtrList<Room>
{
	int compareItems(QPtrCollection::Item, QPtrCollection::Item);
};

/**
This class represents a room

@author Liviu Lalescu
*/
class Room{ /*classroom :-)*/
public:
	QString name;
	QString type;
	int capacity;
	QStringList equipments;
	/**
	If empty string, it is ignored
	*/
	QString building; 
	
	/**
	Internal. If -1, it is ignored
	*/
	int buildingIndex;

	Room();
	~Room();
	
	void addEquipment(const QString& equipmentName);
	void removeEquipment(const QString& equipmentName);
	bool searchEquipment(const QString& equipmentName);
	
	void computeInternalStructure(Rules& r);

	QString getXmlDescription();
	QString getDescription();
	QString getDetailedDescription();
	QString getDetailedDescriptionWithConstraints(Rules& r);
};

#endif
