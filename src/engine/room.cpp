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
#include "rules.h"

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

void Room::computeInternalStructure(Rules& r)
{
	this->buildingIndex=r.searchBuilding(this->building);
}

QString Room::getDescription()
{
	QString s=QObject::tr("N:");
	s+=this->name;
	s+=",";
	if(this->building!=""){
		s+=QObject::tr("B:");
		s+=this->building;
		s+=",";
	}
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
	if(this->building!=""){
		s+=QObject::tr("Building=");
		s+=this->building;
		s+="\n";
	}
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

QString Room::getXmlDescription()
{
	QString s="<Room>\n";
	s+="	<Name>"+protect(this->name)+"</Name>\n";
	s+="	<Building>"+protect(this->building)+"</Building>\n";
	s+="	<Type>"+protect(this->type)+"</Type>\n";
	s+="	<Capacity>"+QString::number(this->capacity)+"</Capacity>\n";
	
	for(QStringList::Iterator it=this->equipments.begin(); it!=this->equipments.end(); it++)
		s+="	<Equipment>"+protect(*it)+"</Equipment>\n";
	
	s+="</Room>\n";

	return s;
}

QString Room::getDetailedDescriptionWithConstraints(Rules& r)
{
	QString s=this->getDetailedDescription();

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Space constraints directly related to this room:");
	s+="\n";
	for(SpaceConstraint* c=r.spaceConstraintsList.first(); c; c=r.spaceConstraintsList.next()){
		if(c->isRelatedToRoom(this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}
	s+="--------------------------------------------------\n";

	return s;
}

int RoomsList::compareItems(QPtrCollection::Item item1, QPtrCollection::Item item2)
{
	if(((Room*)item1)->name > ((Room*)item2)->name)
		return 1;
	else if(((Room*)item1)->name < ((Room*)item2)->name)
		return -1;
	else
		return 0;
}
