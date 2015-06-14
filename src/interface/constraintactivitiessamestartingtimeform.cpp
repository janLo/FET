/***************************************************************************
                          constraintactivitiessamestartingtimeform.cpp  -  description
                             -------------------
    begin                : 23 June 2004
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

#include "constraintactivitiessamestartingtimeform.h"
#include "addconstraintactivitiessamestartingtimeform.h"

ConstraintActivitiesSameStartingTimeForm::ConstraintActivitiesSameStartingTimeForm()
{
	this->refreshConstraintsListBox();
}

ConstraintActivitiesSameStartingTimeForm::~ConstraintActivitiesSameStartingTimeForm()
{
}

void ConstraintActivitiesSameStartingTimeForm::refreshConstraintsListBox()
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

bool ConstraintActivitiesSameStartingTimeForm::filterOk(TimeConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME)
		return true;
	else
		return false;
}

void ConstraintActivitiesSameStartingTimeForm::constraintChanged(int index)
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

void ConstraintActivitiesSameStartingTimeForm::addConstraint()
{
	AddConstraintActivitiesSameStartingTimeForm *addConstraintActivitiesSameStartingTimeForm=new AddConstraintActivitiesSameStartingTimeForm();
	addConstraintActivitiesSameStartingTimeForm->exec();

	this->refreshConstraintsListBox();
}

void ConstraintActivitiesSameStartingTimeForm::removeConstraint()
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
