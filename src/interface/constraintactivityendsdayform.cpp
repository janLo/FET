/***************************************************************************
                          constraintactivityendsdayform.cpp  -  description
                             -------------------
    begin                : Feb 11, 2005
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

#include "constraintactivityendsdayform.h"
#include "addconstraintactivityendsdayform.h"
#include "modifyconstraintactivityendsdayform.h"

ConstraintActivityEndsDayForm::ConstraintActivityEndsDayForm()
{
	this->visibleConstraintsList.setAutoDelete(false);
	this->filterChanged();
}

ConstraintActivityEndsDayForm::~ConstraintActivityEndsDayForm()
{
}

bool ConstraintActivityEndsDayForm::filterOk(TimeConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_ACTIVITY_ENDS_DAY)
		return true;
	else
		return false;
}

void ConstraintActivityEndsDayForm::filterChanged()
{
	this->visibleConstraintsList.clear();
	constraintsListBox->clear();
	for(TimeConstraint* ctr=gt.rules.timeConstraintsList.first(); ctr; ctr=gt.rules.timeConstraintsList.next())
		if(filterOk(ctr)){
			visibleConstraintsList.append(ctr);
			constraintsListBox->insertItem(ctr->getDescription(gt.rules));
		}
}

void ConstraintActivityEndsDayForm::constraintChanged(int index)
{
	if(index<0)
		return;
	assert((uint)(index)<this->visibleConstraintsList.count());
	TimeConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=NULL);
	currentConstraintTextEdit->setText(ctr->getDetailedDescription(gt.rules));
}

void ConstraintActivityEndsDayForm::addConstraint()
{
	AddConstraintActivityEndsDayForm *addConstraintActivityEndsDayForm=new AddConstraintActivityEndsDayForm();
	addConstraintActivityEndsDayForm->exec();

	filterChanged();
	
	constraintsListBox->setCurrentItem(constraintsListBox->count()-1);
}

void ConstraintActivityEndsDayForm::modifyConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	TimeConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintActivityEndsDayForm *modifyConstraintActivityEndsDayForm
	 = new ModifyConstraintActivityEndsDayForm((ConstraintActivityEndsDay*)ctr);
	modifyConstraintActivityEndsDayForm->exec();

	filterChanged();
	constraintsListBox->setCurrentItem(i);
}

void ConstraintActivityEndsDayForm::removeConstraint()
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
		filterChanged();
		break;
	case 1: // The user clicked the Cancel or pressed Escape
		break;
	}
	
	if((uint)(i) >= constraintsListBox->count())
		i=constraintsListBox->count()-1;
	constraintsListBox->setCurrentItem(i);
}
