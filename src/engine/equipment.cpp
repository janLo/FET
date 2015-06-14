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

QString Equipment::getXMLDescription()
{
	QString s="<Equipment>\n";
	s+="	<Name>"; s+=this->name; s+="</Name>\n";
	s+="</Equipment>\n";
	
	return s;
}
