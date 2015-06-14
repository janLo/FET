/***************************************************************************
                          constraintminimizebuildingchangesforteachersform.cpp  -  description
                             -------------------
    begin                : July 9, 2005
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

#include "constraintminimizebuildingchangesforteachersform.h"
#include "addconstraintminimizebuildingchangesforteachersform.h"
#include "modifyconstraintminimizebuildingchangesforteachersform.h"

ConstraintMinimizeBuildingChangesForTeachersForm::ConstraintMinimizeBuildingChangesForTeachersForm()
{
	this->visibleConstraintsList.setAutoDelete(false);
	this->filterChanged();
}

ConstraintMinimizeBuildingChangesForTeachersForm::~ConstraintMinimizeBuildingChangesForTeachersForm()
{
}

bool ConstraintMinimizeBuildingChangesForTeachersForm::filterOk(SpaceConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_MINIMIZE_BUILDING_CHANGES_FOR_TEACHERS)
		return true;
	else
		return false;
}

void ConstraintMinimizeBuildingChangesForTeachersForm::filterChanged()
{
	this->visibleConstraintsList.clear();
	constraintsListBox->clear();
	for(SpaceConstraint* ctr=gt.rules.spaceConstraintsList.first(); ctr; ctr=gt.rules.spaceConstraintsList.next())
		if(filterOk(ctr)){
			visibleConstraintsList.append(ctr);
			constraintsListBox->insertItem(ctr->getDescription(gt.rules));
		}
}

void ConstraintMinimizeBuildingChangesForTeachersForm::constraintChanged(int index)
{
	if(index<0)
		return;
	assert((uint)(index)<this->visibleConstraintsList.count());
	SpaceConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=NULL);
	currentConstraintTextEdit->setText(ctr->getDetailedDescription(gt.rules));
}

void ConstraintMinimizeBuildingChangesForTeachersForm::addConstraint()
{
	AddConstraintMinimizeBuildingChangesForTeachersForm *addConstraintMinimizeBuildingChangesForTeachersForm=new AddConstraintMinimizeBuildingChangesForTeachersForm();
	addConstraintMinimizeBuildingChangesForTeachersForm->exec();

	filterChanged();
	
	constraintsListBox->setCurrentItem(constraintsListBox->count()-1);
}

void ConstraintMinimizeBuildingChangesForTeachersForm::modifyConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	SpaceConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintMinimizeBuildingChangesForTeachersForm *modifyConstraintMinimizeBuildingChangesForTeachersForm
	 = new ModifyConstraintMinimizeBuildingChangesForTeachersForm((ConstraintMinimizeBuildingChangesForTeachers*)ctr);
	modifyConstraintMinimizeBuildingChangesForTeachersForm->exec();

	filterChanged();
	constraintsListBox->setCurrentItem(i);
}

void ConstraintMinimizeBuildingChangesForTeachersForm::removeConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	SpaceConstraint* ctr=this->visibleConstraintsList.at(i);
	QString s;
	s=QObject::tr("Removing constraint:\n");
	s+=ctr->getDetailedDescription(gt.rules);
	s+=QObject::tr("\nAre you sure?");

	switch( QMessageBox::warning( this, QObject::tr("FET warning"),
		s, QObject::tr("OK"), QObject::tr("Cancel"), 0, 0, 1 ) ){
	case 0: // The user clicked the OK again button or pressed Enter
		gt.rules.removeSpaceConstraint(ctr);
		filterChanged();
		break;
	case 1: // The user clicked the Cancel or pressed Escape
		break;
	}
	
	if((uint)(i) >= constraintsListBox->count())
		i=constraintsListBox->count()-1;
	constraintsListBox->setCurrentItem(i);
}
