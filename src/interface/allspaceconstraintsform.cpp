/***************************************************************************
                          allspaceconstraintsform.cpp  -  description
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

#include <iostream>

using namespace std;

#include "allspaceconstraintsform.h"

#include "modifyconstraintbasiccompulsoryspaceform.h"
#include "modifyconstraintroomnotavailableform.h"
#include "modifyconstraintactivitypreferredroomform.h"
#include "modifyconstraintactivitypreferredroomsform.h"
#include "modifyconstraintsubjectpreferredroomform.h"
#include "modifyconstraintsubjectpreferredroomsform.h"
#include "modifyconstraintsubjectsubjecttagpreferredroomform.h"
#include "modifyconstraintsubjectsubjecttagpreferredroomsform.h"

#include <QDesktopWidget>

AllSpaceConstraintsForm::AllSpaceConstraintsForm()
{
	setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	QDesktopWidget* desktop=QApplication::desktop();
	int xx=desktop->width()/2 - frameGeometry().width()/2;
	int yy=desktop->height()/2 - frameGeometry().height()/2;
	move(xx, yy);
	
	this->filterChanged();
}

AllSpaceConstraintsForm::~AllSpaceConstraintsForm()
{
}

bool AllSpaceConstraintsForm::filterOk(SpaceConstraint* ctr)
{
	if(ctr!=NULL) //to avoid "unused parameter" warning
		;

	return true;
}

void AllSpaceConstraintsForm::filterChanged()
{
	this->visibleConstraintsList.clear();
	constraintsListBox->clear();
	for(int i=0; i<gt.rules.spaceConstraintsList.size(); i++){
		SpaceConstraint* ctr=gt.rules.spaceConstraintsList[i];
		if(filterOk(ctr)){
			QString s;
			s=ctr->getDescription(gt.rules);
			visibleConstraintsList << ctr; //append
			constraintsListBox->insertItem(s);
		}
	}

	constraintsListBox->setCurrentItem(0);
	this->constraintChanged(constraintsListBox->currentItem());
}

void AllSpaceConstraintsForm::constraintChanged(int index)
{
	if(index<0)
		return;
	QString s;
	assert(index<this->visibleConstraintsList.size());
	SpaceConstraint* ctr=this->visibleConstraintsList[index];
	assert(ctr!=NULL);
	s=ctr->getDetailedDescription(gt.rules);
	currentConstraintTextEdit->setText(s);
}

void AllSpaceConstraintsForm::modifyConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	SpaceConstraint* ctr=this->visibleConstraintsList[i];

	if(ctr->type==CONSTRAINT_BASIC_COMPULSORY_SPACE){
		ModifyConstraintBasicCompulsorySpaceForm* form=
		 new ModifyConstraintBasicCompulsorySpaceForm((ConstraintBasicCompulsorySpace*)ctr);
		form->exec();
	}
	else if(ctr->type==CONSTRAINT_ROOM_NOT_AVAILABLE){
		ModifyConstraintRoomNotAvailableForm* form=
		 new ModifyConstraintRoomNotAvailableForm((ConstraintRoomNotAvailable*)ctr);
		form->exec();
	}
	else if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_ROOM){
		ModifyConstraintActivityPreferredRoomForm* form=
		 new ModifyConstraintActivityPreferredRoomForm((ConstraintActivityPreferredRoom*)ctr);
		form->exec();
	}
	else if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_ROOMS){
		ModifyConstraintActivityPreferredRoomsForm* form=
		 new ModifyConstraintActivityPreferredRoomsForm((ConstraintActivityPreferredRooms*)ctr);
		form->exec();
	}
	else if(ctr->type==CONSTRAINT_SUBJECT_PREFERRED_ROOM){
		ModifyConstraintSubjectPreferredRoomForm* form=
		 new ModifyConstraintSubjectPreferredRoomForm((ConstraintSubjectPreferredRoom*)ctr);
		form->exec();
	}
	else if(ctr->type==CONSTRAINT_SUBJECT_PREFERRED_ROOMS){
		ModifyConstraintSubjectPreferredRoomsForm* form=
		 new ModifyConstraintSubjectPreferredRoomsForm((ConstraintSubjectPreferredRooms*)ctr);
		form->exec();
	}
	else if(ctr->type==CONSTRAINT_SUBJECT_SUBJECT_TAG_PREFERRED_ROOM){
		ModifyConstraintSubjectSubjectTagPreferredRoomForm* form=
		 new ModifyConstraintSubjectSubjectTagPreferredRoomForm((ConstraintSubjectSubjectTagPreferredRoom*)ctr);
		form->exec();
	}
	else if(ctr->type==CONSTRAINT_SUBJECT_SUBJECT_TAG_PREFERRED_ROOMS){
		ModifyConstraintSubjectSubjectTagPreferredRoomsForm* form=
		 new ModifyConstraintSubjectSubjectTagPreferredRoomsForm((ConstraintSubjectSubjectTagPreferredRooms*)ctr);
		form->exec();
	}
	else{
		assert(0);
		exit(1);
	}

	filterChanged();

	constraintsListBox->setCurrentItem(i);
}

void AllSpaceConstraintsForm::removeConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	SpaceConstraint* ctr=this->visibleConstraintsList[i];
	/*if(ctr->type==CONSTRAINT_BASIC_COMPULSORY_SPACE){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("You cannot erase the basic space constraints"));
		return;
	}*/
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
	
	if((uint)(i)>=constraintsListBox->count())
		i=constraintsListBox->count()-1;
	constraintsListBox->setCurrentItem(i);
}
