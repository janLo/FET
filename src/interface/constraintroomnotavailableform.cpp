/***************************************************************************
                          constraintroomnotavailableform.cpp  -  description
                             -------------------
    begin                : Feb 13, 2005
    copyright            : (C) 2005 by Lalescu Liviu
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

#include "constraintroomnotavailableform.h"
#include "addconstraintroomnotavailableform.h"
#include "modifyconstraintroomnotavailableform.h"

ConstraintRoomNotAvailableForm::ConstraintRoomNotAvailableForm()
{
	roomsComboBox->insertItem("");
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next())
		roomsComboBox->insertItem(rm->name);

	this->visibleConstraintsList.setAutoDelete(false);
	this->filterChanged();
}

ConstraintRoomNotAvailableForm::~ConstraintRoomNotAvailableForm()
{
}

bool ConstraintRoomNotAvailableForm::filterOk(SpaceConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_ROOM_NOT_AVAILABLE){
		ConstraintRoomNotAvailable* c=(ConstraintRoomNotAvailable*) ctr;
		return c->roomName==roomsComboBox->currentText() || roomsComboBox->currentText()=="";
	}
	else
		return false;
}

void ConstraintRoomNotAvailableForm::filterChanged()
{
	this->visibleConstraintsList.clear();
	constraintsListBox->clear();
	for(SpaceConstraint* ctr=gt.rules.spaceConstraintsList.first(); ctr; ctr=gt.rules.spaceConstraintsList.next())
		if(filterOk(ctr)){
			visibleConstraintsList.append(ctr);
			constraintsListBox->insertItem(ctr->getDescription(gt.rules));
		}
}

void ConstraintRoomNotAvailableForm::constraintChanged(int index)
{
	if(index<0)
		return;
	assert((uint)(index)<this->visibleConstraintsList.count());
	SpaceConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=NULL);
	currentConstraintTextEdit->setText(ctr->getDetailedDescription(gt.rules));
}

void ConstraintRoomNotAvailableForm::addConstraint()
{
	AddConstraintRoomNotAvailableForm *addConstraintRoomNotAvailableForm=new AddConstraintRoomNotAvailableForm();
	addConstraintRoomNotAvailableForm->exec();

	filterChanged();
	
	constraintsListBox->setCurrentItem(constraintsListBox->count()-1);
}

void ConstraintRoomNotAvailableForm::modifyConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	SpaceConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintRoomNotAvailableForm *modifyConstraintRoomNotAvailableForm
	 = new ModifyConstraintRoomNotAvailableForm((ConstraintRoomNotAvailable*)ctr);
	modifyConstraintRoomNotAvailableForm->exec();

	filterChanged();
	constraintsListBox->setCurrentItem(i);
}

void ConstraintRoomNotAvailableForm::removeConstraint()
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
