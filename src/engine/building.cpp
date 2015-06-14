//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Liviu Lalescu <Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "building.h"
#include "rules.h"

Building::Building()
{
}

Building::~Building()
{
}

QString Building::getDescription()
{
	QString s=QObject::tr("N:");
	s+=this->name;

	return s;
}

QString Building::getDetailedDescription()
{
	QString s=QObject::tr("Name=");
	s+=this->name;
	s+="\n";

	return s;
}

QString Building::getXmlDescription()
{
	QString s="<Building>\n";
	s+="	<Name>"+protect(this->name)+"</Name>\n";
	s+="</Building>\n";

	return s;
}

QString Building::getDetailedDescriptionWithConstraints(Rules& r)
{
	QString s=this->getDetailedDescription();

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Space constraints directly related to this building:");
	s+="\n";
	for(SpaceConstraint* c=r.spaceConstraintsList.first(); c; c=r.spaceConstraintsList.next()){
		if(c->isRelatedToBuilding(this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}
	s+="--------------------------------------------------\n";

	return s;
}

int BuildingsList::compareItems(QPtrCollection::Item item1, QPtrCollection::Item item2)
{
	if(((Room*)item1)->name > ((Room*)item2)->name)
		return 1;
	else if(((Room*)item1)->name < ((Room*)item2)->name)
		return -1;
	else
		return 0;
}
