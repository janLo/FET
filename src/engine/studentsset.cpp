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
#include "studentsset.h"
#include "rules.h"

StudentsSet::StudentsSet()
{
	this->type=STUDENTS_SET;
	this->numberOfStudents=0;
}

StudentsSet::~StudentsSet()
{
}

StudentsYear::StudentsYear()
	: StudentsSet()
{
	this->type=STUDENTS_YEAR;
	this->groupsList.setAutoDelete(true);
}

StudentsYear::~StudentsYear()
{
	assert(this->groupsList.autoDelete()==true);
	this->groupsList.clear();
}

QString StudentsYear::getXmlDescription()
{
	QString s="";
	s+="<Year>\n";
	s+="<Name>"+protect(this->name)+"</Name>\n";
	s+="<Number_of_Students>"+QString::number(this->numberOfStudents)+"</Number_of_Students>\n";
	for(StudentsGroup* stg=this->groupsList.first(); stg; stg=this->groupsList.next())
		s+=stg->getXmlDescription();
	s+="</Year>\n";

	return s;
}

QString StudentsYear::getDescription()
{
	QString s="";
	s+=QObject::tr("Yn:");
	s+=this->name;
	s+=",";
	s+=QObject::tr("No:");
	s+=QString::number(this->numberOfStudents);

	return s;
}

QString StudentsYear::getDetailedDescription()
{
	QString s="";
	s+=QObject::tr("Students set - year");
	s+="\n";
	s+=QObject::tr("Year name=");
	s+=this->name;
	s+="\n";
	s+=QObject::tr("Number of students=");
	s+=QString::number(this->numberOfStudents);
	s+="\n";

	return s;
}

QString StudentsYear::getDetailedDescriptionWithConstraints(Rules& r)
{
	QString s=this->getDetailedDescription();

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Time constraints directly related to this students year:");
	s+="\n";
	for(TimeConstraint* c=r.timeConstraintsList.first(); c; c=r.timeConstraintsList.next()){
		if(c->isRelatedToStudentsSet(r, this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Space constraints directly related to this students year:");
	s+="\n";
	for(SpaceConstraint* c=r.spaceConstraintsList.first(); c; c=r.spaceConstraintsList.next()){
		if(c->isRelatedToStudentsSet(r, this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}
	s+="--------------------------------------------------\n";

	return s;
}


StudentsGroup::StudentsGroup()
	: StudentsSet()
{
	this->type=STUDENTS_GROUP;
	this->subgroupsList.setAutoDelete(true);
}

StudentsGroup::~StudentsGroup()
{
	assert(this->subgroupsList.autoDelete()==true);
	this->subgroupsList.clear();
}

QString StudentsGroup::getXmlDescription()
{
	QString s="";
	s+="	<Group>\n";
	s+="	<Name>"+protect(this->name)+"</Name>\n";
	s+="	<Number_of_Students>"+QString::number(this->numberOfStudents)+"</Number_of_Students>\n";
	for(StudentsSubgroup* sts=this->subgroupsList.first(); sts; sts=this->subgroupsList.next())
		s+=sts->getXmlDescription();
	s+="	</Group>\n";

	return s;
}

QString StudentsGroup::getDescription()
{
	QString s="";
	s+=QObject::tr("Gn:");
	s+=this->name;
	s+=",";
	s+=QObject::tr("No:");
	s+=QString::number(this->numberOfStudents);

	return s;
}

QString StudentsGroup::getDetailedDescription()
{
	QString s="";
	s+=QObject::tr("Students set - group");
	s+="\n";
	s+=QObject::tr("Group name=");
	s+=this->name;
	s+="\n";
	s+=QObject::tr("Number of students=");
	s+=QString::number(this->numberOfStudents);
	s+="\n";

	return s;
}

QString StudentsGroup::getDetailedDescriptionWithConstraints(Rules& r)
{
	QString s=this->getDetailedDescription();

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Time constraints directly related to this students group:");
	s+="\n";
	for(TimeConstraint* c=r.timeConstraintsList.first(); c; c=r.timeConstraintsList.next()){
		if(c->isRelatedToStudentsSet(r, this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Space constraints directly related to this students group:");
	s+="\n";
	for(SpaceConstraint* c=r.spaceConstraintsList.first(); c; c=r.spaceConstraintsList.next()){
		if(c->isRelatedToStudentsSet(r, this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}
	s+="--------------------------------------------------\n";

	return s;
}


StudentsSubgroup::StudentsSubgroup()
	: StudentsSet()
{
	this->type=STUDENTS_SUBGROUP;
}

StudentsSubgroup::~StudentsSubgroup()
{
}

QString StudentsSubgroup::getXmlDescription()
{
	QString s="";
	s+="		<Subgroup>\n";
	s+="		<Name>"+protect(this->name)+"</Name>\n";
	s+="		<Number_of_Students>"+QString::number(this->numberOfStudents)+"</Number_of_Students>\n";
	s+="		</Subgroup>\n";

	return s;
}

QString StudentsSubgroup::getDescription()
{
	QString s="";
	s+=QObject::tr("Sgn:");
	s+=this->name;
	s+=",";
	s+=QObject::tr("No:");
	s+=QString::number(this->numberOfStudents);

	return s;
}

QString StudentsSubgroup::getDetailedDescription()
{
	QString s="";
	s+=QObject::tr("Students set - subgroup");
	s+="\n";
	s+=QObject::tr("Subgroup name=");
	s+=this->name;
	s+="\n";
	s+=QObject::tr("Number of students=");
	s+=QString::number(this->numberOfStudents);
	s+="\n";

	return s;
}

QString StudentsSubgroup::getDetailedDescriptionWithConstraints(Rules& r)
{
	QString s=this->getDetailedDescription();

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Time constraints directly related to this students subgroup:");
	s+="\n";
	for(TimeConstraint* c=r.timeConstraintsList.first(); c; c=r.timeConstraintsList.next()){
		if(c->isRelatedToStudentsSet(r, this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Space constraints directly related to this students subgroup:");
	s+="\n";
	for(SpaceConstraint* c=r.spaceConstraintsList.first(); c; c=r.spaceConstraintsList.next()){
		if(c->isRelatedToStudentsSet(r, this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}
	s+="--------------------------------------------------\n";

	return s;
}

int StudentsYearsList::compareItems(QPtrCollection::Item item1, QPtrCollection::Item item2)
{
	if(((StudentsYear*)item1)->name > ((StudentsYear*)item2)->name)
		return 1;
	else if(((StudentsYear*)item1)->name < ((StudentsYear*)item2)->name)
		return -1;
	else
		return 0;
}

int StudentsGroupsList::compareItems(QPtrCollection::Item item1, QPtrCollection::Item item2)
{
	if(((StudentsGroup*)item1)->name > ((StudentsGroup*)item2)->name)
		return 1;
	else if(((StudentsGroup*)item1)->name < ((StudentsGroup*)item2)->name)
		return -1;
	else
		return 0;
}

int StudentsSubgroupsList::compareItems(QPtrCollection::Item item1, QPtrCollection::Item item2)
{
	if(((StudentsSubgroup*)item1)->name > ((StudentsSubgroup*)item2)->name)
		return 1;
	else if(((StudentsSubgroup*)item1)->name < ((StudentsSubgroup*)item2)->name)
		return -1;
	else
		return 0;
}
