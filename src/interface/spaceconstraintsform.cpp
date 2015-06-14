/***************************************************************************
                          spaceconstraintsform.cpp  -  description
                             -------------------
    begin                : Thu Jan 8 2004
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

#include <iostream>

using namespace std;

#include "spaceconstraintsform.h"
#include "addspaceconstraintform.h"

SpaceConstraintsForm::SpaceConstraintsForm()
{
	this->visibleConstraintsList.setAutoDelete(false);
	this->filterChanged();
}

SpaceConstraintsForm::~SpaceConstraintsForm()
{
}

bool SpaceConstraintsForm::filterOk(SpaceConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_BASIC_COMPULSORY_SPACE)
		return basicCompulsorySpaceCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_ROOM_NOT_AVAILABLE)
		return roomNotAvailableCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_ROOM_TYPE_NOT_ALLOWED_SUBJECTS)
		return roomTypeNotAllowedSubjectsCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_SUBJECT_REQUIRES_EQUIPMENTS)
		return subjectRequiresEquipmentsCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_TEACHER_REQUIRES_ROOM)
		return teacherRequiresRoomCheckBox->isChecked();
	else{
		cout << ctr->type << endl;
		assert(0);
		return false;
	}
}

void SpaceConstraintsForm::filterChanged()
{
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

void SpaceConstraintsForm::constraintChanged(int index)
{
	if(index<0)
		return;
	QString s;
	assert(index<this->visibleConstraintsList.count());
	SpaceConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=NULL);
	s=ctr->getDetailedDescription(gt.rules);
	currentConstraintTextEdit->setText(s);
}

void SpaceConstraintsForm::addConstraint()
{
	AddSpaceConstraintForm *addSpaceConstraintForm=new AddSpaceConstraintForm();
	addSpaceConstraintForm->exec();

	filterChanged();
}

void SpaceConstraintsForm::removeConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	SpaceConstraint* ctr=this->visibleConstraintsList.at(i);
	if(ctr->type==CONSTRAINT_BASIC_COMPULSORY_SPACE){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("You cannot erase the basic space constraints"));
		return;
	}
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
}
