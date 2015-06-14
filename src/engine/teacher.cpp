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
