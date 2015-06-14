/***************************************************************************
                          constraintactivitiessamestartinghourform.cpp  -  description
                             -------------------
    begin                : 15 Feb 2005
    copyright            : (C) 2005 by Lalescu Liviu
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

#include "constraintactivitiessamestartinghourform.h"
#include "addconstraintactivitiessamestartinghourform.h"
#include "modifyconstraintactivitiessamestartinghourform.h"

ConstraintActivitiesSameStartingHourForm::ConstraintActivitiesSameStartingHourForm()
{
	this->refreshConstraintsListBox();
}

ConstraintActivitiesSameStartingHourForm::~ConstraintActivitiesSameStartingHourForm()
{
}

void ConstraintActivitiesSameStartingHourForm::refreshConstraintsListBox()
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

bool ConstraintActivitiesSameStartingHourForm::filterOk(TimeConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_ACTIVITIES_SAME_STARTING_HOUR)
		return true;
	else
		return false;
}

void ConstraintActivitiesSameStartingHourForm::constraintChanged(int index)
{
	if(index<0)
		return;
	QString s;
	assert((uint)(index)<this->visibleConstraintsList.count());
	TimeConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=NULL);
	s=ctr->getDetailedDescription(gt.rules);
	currentConstraintTextEdit->setText(s);
}

void ConstraintActivitiesSameStartingHourForm::addConstraint()
{
	AddConstraintActivitiesSameStartingHourForm *addConstraintActivitiesSameStartingHourForm=new AddConstraintActivitiesSameStartingHourForm();
	addConstraintActivitiesSameStartingHourForm->exec();

	this->refreshConstraintsListBox();
}

void ConstraintActivitiesSameStartingHourForm::modifyConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	TimeConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintActivitiesSameStartingHourForm *modifyConstraintActivitiesSameStartingHourForm=new ModifyConstraintActivitiesSameStartingHourForm((ConstraintActivitiesSameStartingHour*)ctr);
	modifyConstraintActivitiesSameStartingHourForm->exec();

	this->refreshConstraintsListBox();
	
	constraintsListBox->setCurrentItem(i);
}

void ConstraintActivitiesSameStartingHourForm::removeConstraint()
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
