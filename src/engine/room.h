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
#ifndef ROOM_H
#define ROOM_H

#include "genetictimetable_defs.h"

#include <qstring.h>
#include <qptrlist.h>
#include <qstringlist.h>

class Room;

typedef QPtrList<Room> RoomsList;

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

	Room();
	~Room();
	
	void addEquipment(const QString& equipmentName);
	void removeEquipment(const QString& equipmentName);
	bool searchEquipment(const QString& equipmentName);

	QString getDescription();
	QString getDetailedDescription();
	QString getXMLDescription();
};

#endif
