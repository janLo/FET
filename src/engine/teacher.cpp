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
#include "teacher.h"

Teacher::Teacher()
{
}

Teacher::~Teacher()
{
}

QString Teacher::getXMLDescription()
{
	QString s="<Teacher>\n";
	s+="	<Name>"+this->name+"</Name>\n";
	s+="</Teacher>\n";

	return s;
}

int TeachersList::compareItems(QPtrCollection::Item item1,QPtrCollection::Item item2)
{
	if(((Teacher*)item1)->name>((Teacher*)item2)->name)
		return 1;
	else if(((Teacher*)item1)->name<((Teacher*)item2)->name)
		return -1;
	else
		return 0;
}
