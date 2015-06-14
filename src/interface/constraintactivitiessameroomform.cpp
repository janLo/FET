/***************************************************************************
                          constraintactivitiessameroomform.cpp  -  description
                             -------------------
    begin                : 28 March 2005
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

#include "constraintactivitiessameroomform.h"
#include "addconstraintactivitiessameroomform.h"
#include "modifyconstraintactivitiessameroomform.h"

ConstraintActivitiesSameRoomForm::ConstraintActivitiesSameRoomForm()
{
	this->refreshConstraintsListBox();
}

ConstraintActivitiesSameRoomForm::~ConstraintActivitiesSameRoomForm()
{
}

void ConstraintActivitiesSameRoomForm::refreshConstraintsListBox()
{
	this->visibleConstraintsList.setAutoDelete(false);
	this->visibleConstraintsList.clear();
	constraintsListBox->clear();
	for(SpaceConstraint* ctr=gt.rules.spaceConstraintsList.first(); ctr; ctr=gt.rules.spaceConstraintsList.next())
		if(filterOk(ctr)){
			QString s;
			s=ctr->getDescription(gt.rules);
			visibleConstraintsList.append(ctr);
			constraintsListBox->insertItem(s);
		}

	constraintsListBox->setCurrentItem(0);
	this->constraintChanged(constraintsListBox->currentItem());
}

bool ConstraintActivitiesSameRoomForm::filterOk(SpaceConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_ACTIVITIES_SAME_ROOM)
		return true;
	else
		return false;
}

void ConstraintActivitiesSameRoomForm::constraintChanged(int index)
{
	if(index<0)
		return;
	QString s;
	assert((uint)(index)<this->visibleConstraintsList.count());
	SpaceConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=NULL);
	s=ctr->getDetailedDescription(gt.rules);
	currentConstraintTextEdit->setText(s);
}

void ConstraintActivitiesSameRoomForm::addConstraint()
{
	AddConstraintActivitiesSameRoomForm *addConstraintActivitiesSameRoomForm=new AddConstraintActivitiesSameRoomForm();
	addConstraintActivitiesSameRoomForm->exec();

	this->refreshConstraintsListBox();
}

void ConstraintActivitiesSameRoomForm::modifyConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	SpaceConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintActivitiesSameRoomForm *modifyConstraintActivitiesSameRoomForm=new ModifyConstraintActivitiesSameRoomForm((ConstraintActivitiesSameRoom*)ctr);
	modifyConstraintActivitiesSameRoomForm->exec();

	this->refreshConstraintsListBox();
	
	constraintsListBox->setCurrentItem(i);
}

void ConstraintActivitiesSameRoomForm::removeConstraint()
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
		this->refreshConstraintsListBox();
		break;
	case 1: // The user clicked the Cancel or pressed Escape
		break;
	}
}
