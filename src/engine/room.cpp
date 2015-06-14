//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Liviu Lalescu <liviu@lalescu.ro>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "room.h"

Room::Room()
{
}

Room::~Room()
{
}

void Room::addEquipment(const QString& equipmentName)
{
	this->equipments.append(equipmentName);	
}

void Room::removeEquipment(const QString& equipmentName)
{
	this->equipments.remove(equipmentName);
}

bool Room::searchEquipment(const QString& equipmentName)
{
	int tmp=this->equipments.findIndex(equipmentName);
	if(tmp>=0)
		return true;
	else
		return false;
}

QString Room::getDescription()
{
	QString s=QObject::tr("N:");
	s+=this->name;
	s+=",";
	s+=QObject::tr("T:");
	s+=this->type;
	s+=",";
	s+=QObject::tr("C:");
	s+=QString::number(this->capacity);
	s+=",";

	for(QStringList::Iterator it=this->equipments.begin(); it!=this->equipments.end(); it++)
		s+="E:"+(*it)+", ";

	return s;
}

QString Room::getDetailedDescription()
{
	QString s=QObject::tr("Name=");
	s+=this->name;
	s+="\n";
	s+=QObject::tr("Type=");
	s+=this->type;
	s+="\n";
	s+=QObject::tr("Capacity=");
	s+=QString::number(this->capacity);
	s+="\n";

	for(QStringList::Iterator it=this->equipments.begin(); it!=this->equipments.end(); it++)
		s+="Equipment="+(*it)+"\n";

	return s;
}

QString Room::getXMLDescription()
{
	QString s="<Room>\n";
	s+="	<Name>"+this->name+"</Name>\n";
	s+="	<Type>"+this->type+"</Type>\n";
	s+="	<Capacity>"+QString::number(this->capacity)+"</Capacity>\n";
	
	for(QStringList::Iterator it=this->equipments.begin(); it!=this->equipments.end(); it++)
		s+="	<Equipment>"+(*it)+"</Equipment>\n";
	
	s+="</Room>\n";

	return s;
}
