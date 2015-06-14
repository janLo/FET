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
#include "subject.h"

Subject::Subject()
{
}

Subject::~Subject()
{
}

QString Subject::getXMLDescription()
{
	QString s="<Subject>\n";
	s+="	<Name>"+this->name+"</Name>\n";
	s+="</Subject>\n";

	return s;
}

int SubjectsList::compareItems(QPtrCollection::Item item1, QPtrCollection::Item item2)
{
	if(((Subject*)item1)->name > ((Subject*)item2)->name)
		return 1;
	else if(((Subject*)item1)->name < ((Subject*)item2)->name)
		return -1;
	else
		return 0;
}
