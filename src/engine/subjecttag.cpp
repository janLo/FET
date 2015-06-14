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
#include "rules.h"

SubjectTag::SubjectTag()
{
}

SubjectTag::~SubjectTag()
{
}

QString SubjectTag::getXmlDescription()
{
	QString s="<Subject_Tag>\n";
	s+="	<Name>"+protect(this->name)+"</Name>\n";
	s+="</Subject_Tag>\n";

	return s;
}

QString SubjectTag::getDetailedDescription()
{
	QString s=QObject::tr("Subject tag");
	s+="\n";
	s+=QObject::tr("Name=%1").arg(this->name);
	s+="\n";

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

QString SubjectTag::getDetailedDescriptionWithConstraints(Rules& r)
{
	QString s=this->getDetailedDescription();

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Time constraints directly related to this subject tag:");
	s+="\n";
	for(TimeConstraint* c=r.timeConstraintsList.first(); c; c=r.timeConstraintsList.next()){
		if(c->isRelatedToSubjectTag(this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Space constraints directly related to this subject tag:");
	s+="\n";
	for(SpaceConstraint* c=r.spaceConstraintsList.first(); c; c=r.spaceConstraintsList.next()){
		if(c->isRelatedToSubjectTag(this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}
	s+="--------------------------------------------------\n";

	return s;
}
