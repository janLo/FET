/***************************************************************************
                          constraintteacherssubgroupsnomorethanxhoursdailyform.cpp  -  description
                             -------------------
    begin                : Feb 10, 2005
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

#include "constraintteacherssubgroupsnomorethanxhoursdailyform.h"
#include "addconstraintteacherssubgroupsnomorethanxhoursdailyform.h"
#include "modifyconstraintteacherssubgroupsnomorethanxhoursdailyform.h"

ConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm::ConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm()
{
	this->visibleConstraintsList.setAutoDelete(false);
	this->filterChanged();
}

ConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm::~ConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm()
{
}

bool ConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm::filterOk(TimeConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_TEACHERS_SUBGROUPS_NO_MORE_THAN_X_HOURS_DAILY)
		return true;
	else
		return false;
}

void ConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm::filterChanged()
{
	this->visibleConstraintsList.clear();
	constraintsListBox->clear();
	for(TimeConstraint* ctr=gt.rules.timeConstraintsList.first(); ctr; ctr=gt.rules.timeConstraintsList.next())
		if(filterOk(ctr)){
			visibleConstraintsList.append(ctr);
			constraintsListBox->insertItem(ctr->getDescription(gt.rules));
		}
}

void ConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm::constraintChanged(int index)
{
	if(index<0)
		return;
	assert((uint)(index)<this->visibleConstraintsList.count());
	TimeConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=NULL);
	currentConstraintTextEdit->setText(ctr->getDetailedDescription(gt.rules));
}

void ConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm::addConstraint()
{
	AddConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm *addConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm=new AddConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm();
	addConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm->exec();

	filterChanged();
	
	constraintsListBox->setCurrentItem(constraintsListBox->count()-1);
}

void ConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm::modifyConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	TimeConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm *modifyConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm
	 = new ModifyConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm((ConstraintTeachersSubgroupsNoMoreThanXHoursDaily*)ctr);
	modifyConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm->exec();

	filterChanged();
	constraintsListBox->setCurrentItem(i);
}

void ConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm::removeConstraint()
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
