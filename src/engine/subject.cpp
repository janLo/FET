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
#include "subject.h"
#include "rules.h"

Subject::Subject()
{
}

Subject::~Subject()
{
}

QString Subject::getXmlDescription()
{
	QString s="<Subject>\n";
	s+="	<Name>"+protect(this->name)+"</Name>\n";
	s+="</Subject>\n";

	return s;
}

QString Subject::getDetailedDescription()
{
	QString s=QObject::tr("Subject");
	s+="\n";
	s+=QObject::tr("Name=%1").arg(this->name);
	s+="\n";

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

QString Subject::getDetailedDescriptionWithConstraints(Rules& r)
{
	QString s=this->getDetailedDescription();

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Time constraints directly related to this subject:");
	s+="\n";
	for(TimeConstraint* c=r.timeConstraintsList.first(); c; c=r.timeConstraintsList.next()){
		if(c->isRelatedToSubject(this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Space constraints directly related to this subject:");
	s+="\n";
	for(SpaceConstraint* c=r.spaceConstraintsList.first(); c; c=r.spaceConstraintsList.next()){
		if(c->isRelatedToSubject(this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}
	s+="--------------------------------------------------\n";

	return s;
}
