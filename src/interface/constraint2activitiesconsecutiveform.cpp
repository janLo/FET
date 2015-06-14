/***************************************************************************
                          constraint2activitiesconsecutiveform.cpp  -  description
                             -------------------
    begin                : 15 May 2004
    copyright            : (C) 2004 by Lalescu Liviu
    email                : Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "constraint2activitiesconsecutiveform.h"
#include "addconstraint2activitiesconsecutiveform.h"
#include "modifyconstraint2activitiesconsecutiveform.h"

Constraint2ActivitiesConsecutiveForm::Constraint2ActivitiesConsecutiveForm()
{
	this->refreshConstraintsListBox();
}

Constraint2ActivitiesConsecutiveForm::~Constraint2ActivitiesConsecutiveForm()
{
}

void Constraint2ActivitiesConsecutiveForm::refreshConstraintsListBox()
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

bool Constraint2ActivitiesConsecutiveForm::filterOk(TimeConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_2_ACTIVITIES_CONSECUTIVE)
		return true;
	else
		return false;
}

void Constraint2ActivitiesConsecutiveForm::constraintChanged(int index)
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

void Constraint2ActivitiesConsecutiveForm::addConstraint()
{
	AddConstraint2ActivitiesConsecutiveForm *addConstraint2ActivitiesConsecutiveForm=new AddConstraint2ActivitiesConsecutiveForm();
	addConstraint2ActivitiesConsecutiveForm->exec();

	this->refreshConstraintsListBox();
}

void Constraint2ActivitiesConsecutiveForm::removeConstraint()
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
	
	if((uint)(i)>=constraintsListBox->count())
		i=constraintsListBox->count()-1;
	constraintsListBox->setCurrentItem(i);
}

void Constraint2ActivitiesConsecutiveForm::modifyConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	TimeConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraint2ActivitiesConsecutiveForm *modifyConstraint2ActivitiesConsecutiveForm=
	 new ModifyConstraint2ActivitiesConsecutiveForm((Constraint2ActivitiesConsecutive*)ctr);
	modifyConstraint2ActivitiesConsecutiveForm->exec();

	this->refreshConstraintsListBox();
	
	constraintsListBox->setCurrentItem(i);
}
