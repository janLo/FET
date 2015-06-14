/***************************************************************************
                          constraintsubjectsubjecttagrequireequipmentsform.cpp  -  description
                             -------------------
    begin                : Feb 22, 2005
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

#include "constraintsubjectsubjecttagrequireequipmentsform.h"
#include "addconstraintsubjectsubjecttagrequireequipmentsform.h"
#include "modifyconstraintsubjectsubjecttagrequireequipmentsform.h"

ConstraintSubjectSubjectTagRequireEquipmentsForm::ConstraintSubjectSubjectTagRequireEquipmentsForm()
{
	subjectsComboBox->insertItem("");
	for(Subject* sb=gt.rules.subjectsList.first(); sb; sb=gt.rules.subjectsList.next())
		subjectsComboBox->insertItem(sb->name);
		
	subjectTagsComboBox->insertItem("");
	for(SubjectTag* st=gt.rules.subjectTagsList.first(); st; st=gt.rules.subjectTagsList.next())
		subjectTagsComboBox->insertItem(st->name);

	this->visibleConstraintsList.setAutoDelete(false);
	this->filterChanged();
}

ConstraintSubjectSubjectTagRequireEquipmentsForm::~ConstraintSubjectSubjectTagRequireEquipmentsForm()
{
}

bool ConstraintSubjectSubjectTagRequireEquipmentsForm::filterOk(SpaceConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_SUBJECT_SUBJECT_TAG_REQUIRE_EQUIPMENTS){
		ConstraintSubjectSubjectTagRequireEquipments* c=(ConstraintSubjectSubjectTagRequireEquipments*) ctr;
		return (c->subjectName==subjectsComboBox->currentText() || subjectsComboBox->currentText()=="") 
			&& (c->subjectTagName==subjectTagsComboBox->currentText() || subjectTagsComboBox->currentText()=="");
	}
	else
		return false;
}

void ConstraintSubjectSubjectTagRequireEquipmentsForm::filterChanged()
{
	this->visibleConstraintsList.clear();
	constraintsListBox->clear();
	for(SpaceConstraint* ctr=gt.rules.spaceConstraintsList.first(); ctr; ctr=gt.rules.spaceConstraintsList.next())
		if(filterOk(ctr)){
			visibleConstraintsList.append(ctr);
			constraintsListBox->insertItem(ctr->getDescription(gt.rules));
		}
}

void ConstraintSubjectSubjectTagRequireEquipmentsForm::constraintChanged(int index)
{
	if(index<0)
		return;
	assert((uint)(index)<this->visibleConstraintsList.count());
	SpaceConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=NULL);
	currentConstraintTextEdit->setText(ctr->getDetailedDescription(gt.rules));
}

void ConstraintSubjectSubjectTagRequireEquipmentsForm::addConstraint()
{
	AddConstraintSubjectSubjectTagRequireEquipmentsForm *addConstraintSubjectSubjectTagRequireEquipmentsForm=new AddConstraintSubjectSubjectTagRequireEquipmentsForm();
	addConstraintSubjectSubjectTagRequireEquipmentsForm->exec();

	filterChanged();
	
	constraintsListBox->setCurrentItem(constraintsListBox->count()-1);
}

void ConstraintSubjectSubjectTagRequireEquipmentsForm::modifyConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	SpaceConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintSubjectSubjectTagRequireEquipmentsForm *modifyConstraintSubjectSubjectTagRequireEquipmentsForm
	 = new ModifyConstraintSubjectSubjectTagRequireEquipmentsForm((ConstraintSubjectSubjectTagRequireEquipments*)ctr);
	modifyConstraintSubjectSubjectTagRequireEquipmentsForm->exec();

	filterChanged();
	constraintsListBox->setCurrentItem(i);
}

void ConstraintSubjectSubjectTagRequireEquipmentsForm::removeConstraint()
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
