/***************************************************************************
                          constraint2activitiesgroupedform.cpp  -  description
                             -------------------
    begin                : 15 May 2004
    copyright            : (C) 2004 by Lalescu Liviu
    email                : liviu@lalescu.ro
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "constraint2activitiesgroupedform.h"
#include "addconstraint2activitiesgroupedform.h"

Constraint2ActivitiesGroupedForm::Constraint2ActivitiesGroupedForm()
{
	this->refreshConstraintsListBox();
}

Constraint2ActivitiesGroupedForm::~Constraint2ActivitiesGroupedForm()
{
}

void Constraint2ActivitiesGroupedForm::refreshConstraintsListBox()
{
	this->visibleConstraintsList.setAutoDelete(false);
	this->visibleConstraintsList.clear();
	constraintsListBox->clear();
	for(TimeConstraint* ctr=gt.rules.timeConstraintsList.first(); ctr; ctr=gt.rules.timeConstraintsList.next())
		if(filterOk(ctr)){
			QString s;
			s=ctr->getDescription(gt.rules);
			visibleConstraintsList.append(ctr);
			constraintsListBox->insertItem(s);
		}

	constraintsListBox->setCurrentItem(0);
	this->constraintChanged(constraintsListBox->currentItem());
}

bool Constraint2ActivitiesGroupedForm::filterOk(TimeConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_2_ACTIVITIES_GROUPED)
		return true;
	else
		return false;
}

void Constraint2ActivitiesGroupedForm::constraintChanged(int index)
{
	if(index<0)
		return;
	QString s;
	assert(index<this->visibleConstraintsList.count());
	TimeConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=NULL);
	s=ctr->getDetailedDescription(gt.rules);
	currentConstraintTextEdit->setText(s);
}

void Constraint2ActivitiesGroupedForm::addConstraint()
{
	AddConstraint2ActivitiesGroupedForm *addConstraint2ActivitiesGroupedForm=new AddConstraint2ActivitiesGroupedForm();
	addConstraint2ActivitiesGroupedForm->exec();

	this->refreshConstraintsListBox();
}

void Constraint2ActivitiesGroupedForm::removeConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	TimeConstraint* ctr=this->visibleConstraintsList.at(i);
	QString s;
	s=QObject::tr("Removing constraint:\n");
	s+=ctr->getDetailedDescription(gt.rules);
	s+=QObject::tr("\nAre you sure?");

	switch( QMessageBox::warning( this, QObject::tr("FET warning"),
		s, QObject::tr("OK"), QObject::tr("Cancel"), 0, 0, 1 ) ){
	case 0: // The user clicked the OK again button or pressed Enter
		gt.rules.removeTimeConstraint(ctr);
		this->refreshConstraintsListBox();
		break;
	case 1: // The user clicked the Cancel or pressed Escape
		break;
	}
}
