/***************************************************************************
                          constraintteachersubjectrequireroomform.cpp  -  description
                             -------------------
    begin                : July 2, 2005
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

#include "constraintteachersubjectrequireroomform.h"
#include "addconstraintteachersubjectrequireroomform.h"
#include "modifyconstraintteachersubjectrequireroomform.h"

ConstraintTeacherSubjectRequireRoomForm::ConstraintTeacherSubjectRequireRoomForm()
{
	teachersComboBox->insertItem("");
	for(Teacher* tch=gt.rules.teachersList.first(); tch; tch=gt.rules.teachersList.next())
		teachersComboBox->insertItem(tch->name);
	subjectsComboBox->insertItem("");
	for(Subject* sb=gt.rules.subjectsList.first(); sb; sb=gt.rules.subjectsList.next())
		subjectsComboBox->insertItem(sb->name);
	roomsComboBox->insertItem("");
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next())
		roomsComboBox->insertItem(rm->name);

	this->visibleConstraintsList.setAutoDelete(false);
	this->filterChanged();
}

ConstraintTeacherSubjectRequireRoomForm::~ConstraintTeacherSubjectRequireRoomForm()
{
}

bool ConstraintTeacherSubjectRequireRoomForm::filterOk(SpaceConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_TEACHER_SUBJECT_REQUIRE_ROOM){
		ConstraintTeacherSubjectRequireRoom* c=(ConstraintTeacherSubjectRequireRoom*) ctr;
		return (c->teacherName==teachersComboBox->currentText() || teachersComboBox->currentText()=="")
		 && (c->subjectName==subjectsComboBox->currentText() || subjectsComboBox->currentText()=="")
		 && (c->roomName==roomsComboBox->currentText() || roomsComboBox->currentText()=="");
	}
	else
		return false;
}

void ConstraintTeacherSubjectRequireRoomForm::filterChanged()
{
	this->visibleConstraintsList.clear();
	constraintsListBox->clear();
	for(SpaceConstraint* ctr=gt.rules.spaceConstraintsList.first(); ctr; ctr=gt.rules.spaceConstraintsList.next())
		if(filterOk(ctr)){
			visibleConstraintsList.append(ctr);
			constraintsListBox->insertItem(ctr->getDescription(gt.rules));
		}
}

void ConstraintTeacherSubjectRequireRoomForm::constraintChanged(int index)
{
	if(index<0)
		return;
	assert((uint)(index)<this->visibleConstraintsList.count());
	SpaceConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=NULL);
	currentConstraintTextEdit->setText(ctr->getDetailedDescription(gt.rules));
}

void ConstraintTeacherSubjectRequireRoomForm::addConstraint()
{
	AddConstraintTeacherSubjectRequireRoomForm *addConstraintTeacherSubjectRequireRoomForm=new AddConstraintTeacherSubjectRequireRoomForm();
	addConstraintTeacherSubjectRequireRoomForm->exec();

	filterChanged();
	
	constraintsListBox->setCurrentItem(constraintsListBox->count()-1);
}

void ConstraintTeacherSubjectRequireRoomForm::modifyConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	SpaceConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintTeacherSubjectRequireRoomForm *modifyConstraintTeacherSubjectRequireRoomForm
	 = new ModifyConstraintTeacherSubjectRequireRoomForm((ConstraintTeacherSubjectRequireRoom*)ctr);
	modifyConstraintTeacherSubjectRequireRoomForm->exec();

	filterChanged();
	constraintsListBox->setCurrentItem(i);
}

void ConstraintTeacherSubjectRequireRoomForm::removeConstraint()
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
