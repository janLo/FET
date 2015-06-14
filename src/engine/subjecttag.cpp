//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Liviu Lalescu <liviu@lalescu.ro>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "subjecttag.h"

SubjectTag::SubjectTag()
{
}

SubjectTag::~SubjectTag()
{
}

QString SubjectTag::getXMLDescription()
{
	QString s="<Subject_Tag>\n";
	s+="	<Name>"+this->name+"</Name>\n";
	s+="</Subject_Tag>\n";

	return s;
}

int SubjectTagsList::compareItems(QPtrCollection::Item item1, QPtrCollection::Item item2)
{
	if(((SubjectTag*)item1)->name > ((SubjectTag*)item2)->name)
		return 1;
	else if(((SubjectTag*)item1)->name < ((SubjectTag*)item2)->name)
		return -1;
	else
		return 0;
}
