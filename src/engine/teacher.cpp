//
//
// C++ Implementation: $MODULE$
//
// Description:
//
//
// Author: Liviu Lalescu <Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "teacher.h"
#include "rules.h"

Teacher::Teacher()
{
}

Teacher::~Teacher()
{
}

QString Teacher::getXmlDescription()
{
	QString s="<Teacher>\n";
	s+="	<Name>"+protect(this->name)+"</Name>\n";
	s+="</Teacher>\n";

	return s;
}

QString Teacher::getDetailedDescription()
{
	QString s=QObject::tr("Teacher");
	s+="\n";
	s+=QObject::tr("Name=%1").arg(this->name);
	s+="\n";

	return s;
}

QString Teacher::getDetailedDescriptionWithConstraints(Rules& r)
{
	QString s=this->getDetailedDescription();

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Time constraints directly related to this teacher:");
	s+="\n";
	for(TimeConstraint* c=r.timeConstraintsList.first(); c; c=r.timeConstraintsList.next()){
		if(c->isRelatedToTeacher(this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Space constraints directly related to this teacher:");
	s+="\n";
	for(SpaceConstraint* c=r.spaceConstraintsList.first(); c; c=r.spaceConstraintsList.next()){
		if(c->isRelatedToTeacher(this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}
	s+="--------------------------------------------------\n";

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
