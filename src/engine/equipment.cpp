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

#include "equipment.h"
#include "rules.h"

Equipment::Equipment()
{
}

Equipment::~Equipment()
{
}

QString Equipment::getDescription()
{
	QString s=QObject::tr("N:%1").arg(this->name);
	
	return s;
}

QString Equipment::getDetailedDescription()
{
	QString s=QObject::tr("Name=%1").arg(this->name);
	s+="\n";
	
	return s;
}

QString Equipment::getXmlDescription()
{
	QString s="<Equipment>\n";
	s+="	<Name>"; s+=protect(this->name); s+="</Name>\n";
	s+="</Equipment>\n";
	
	return s;
}

QString Equipment::getDetailedDescriptionWithConstraints(Rules& r)
{
	QString s=this->getDetailedDescription();

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Space constraints directly related to this equipment:");
	s+="\n";
	for(SpaceConstraint* c=r.spaceConstraintsList.first(); c; c=r.spaceConstraintsList.next()){
		if(c->isRelatedToEquipment(this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}
	s+="--------------------------------------------------\n";

	return s;
}

int EquipmentsList::compareItems(QPtrCollection::Item item1, QPtrCollection::Item item2)
{
	if(((Equipment*)item1)->name > ((Equipment*)item2)->name)
		return 1;
	else if(((Equipment*)item1)->name < ((Equipment*)item2)->name)
		return -1;
	else
		return 0;
}
