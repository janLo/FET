/***************************************************************************
                          constraintsubjectsubjecttagpreferredroomform.cpp  -  description
                             -------------------
    begin                : Apr 8, 2005
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

#include "constraintsubjectsubjecttagpreferredroomform.h"
#include "addconstraintsubjectsubjecttagpreferredroomform.h"
#include "modifyconstraintsubjectsubjecttagpreferredroomform.h"

ConstraintSubjectSubjectTagPreferredRoomForm::ConstraintSubjectSubjectTagPreferredRoomForm()
{
	roomsComboBox->insertItem("");
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next())
		roomsComboBox->insertItem(rm->name);

	subjectsComboBox->insertItem("");
	for(Subject* sb=gt.rules.subjectsList.first(); sb; sb=gt.rules.subjectsList.next())
		subjectsComboBox->insertItem(sb->name);

	subjectTagsComboBox->insertItem("");
	for(SubjectTag* st=gt.rules.subjectTagsList.first(); st; st=gt.rules.subjectTagsList.next())
		subjectTagsComboBox->insertItem(st->name);

	this->visibleConstraintsList.setAutoDelete(false);
	this->filterChanged();
}

ConstraintSubjectSubjectTagPreferredRoomForm::~ConstraintSubjectSubjectTagPreferredRoomForm()
{
}

bool ConstraintSubjectSubjectTagPreferredRoomForm::filterOk(SpaceConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_SUBJECT_SUBJECT_TAG_PREFERRED_ROOM){
		ConstraintSubjectSubjectTagPreferredRoom* c=(ConstraintSubjectSubjectTagPreferredRoom*) ctr;
		return (c->roomName==roomsComboBox->currentText() || roomsComboBox->currentText()=="")
		 && (c->subjectName==subjectsComboBox->currentText() || subjectsComboBox->currentText()=="")
		 && (c->subjectTagName==subjectTagsComboBox->currentText() || subjectTagsComboBox->currentText()=="");
	}
	else
		return false;
}

void ConstraintSubjectSubjectTagPreferredRoomForm::filterChanged()
{
	this->visibleConstraintsList.clear();
	constraintsListBox->clear();
	for(SpaceConstraint* ctr=gt.rules.spaceConstraintsList.first(); ctr; ctr=gt.rules.spaceConstraintsList.next())
		if(filterOk(ctr)){
			visibleConstraintsList.append(ctr);
			constraintsListBox->insertItem(ctr->getDescription(gt.rules));
		}
}

void ConstraintSubjectSubjectTagPreferredRoomForm::constraintChanged(int index)
{
	if(index<0)
		return;
	assert((uint)(index)<this->visibleConstraintsList.count());
	SpaceConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=NULL);
	currentConstraintTextEdit->setText(ctr->getDetailedDescription(gt.rules));
}

void ConstraintSubjectSubjectTagPreferredRoomForm::addConstraint()
{
	AddConstraintSubjectSubjectTagPreferredRoomForm *addConstraintSubjectSubjectTagPreferredRoomForm=new AddConstraintSubjectSubjectTagPreferredRoomForm();
	addConstraintSubjectSubjectTagPreferredRoomForm->exec();

	filterChanged();
	
	constraintsListBox->setCurrentItem(constraintsListBox->count()-1);
}

void ConstraintSubjectSubjectTagPreferredRoomForm::modifyConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	SpaceConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintSubjectSubjectTagPreferredRoomForm *modifyConstraintSubjectSubjectTagPreferredRoomForm
	 = new ModifyConstraintSubjectSubjectTagPreferredRoomForm((ConstraintSubjectSubjectTagPreferredRoom*)ctr);
	modifyConstraintSubjectSubjectTagPreferredRoomForm->exec();

	filterChanged();
	constraintsListBox->setCurrentItem(i);
}

void ConstraintSubjectSubjectTagPreferredRoomForm::removeConstraint()
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
