/***************************************************************************
                          constraintsubjectrequiresequipmentsform.cpp  -  description
                             -------------------
    begin                : Feb 13, 2005
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

#include "constraintsubjectrequiresequipmentsform.h"
#include "addconstraintsubjectrequiresequipmentsform.h"
#include "modifyconstraintsubjectrequiresequipmentsform.h"

ConstraintSubjectRequiresEquipmentsForm::ConstraintSubjectRequiresEquipmentsForm()
{
	subjectsComboBox->insertItem("");
	for(Subject* sb=gt.rules.subjectsList.first(); sb; sb=gt.rules.subjectsList.next())
		subjectsComboBox->insertItem(sb->name);

	this->visibleConstraintsList.setAutoDelete(false);
	this->filterChanged();
}

ConstraintSubjectRequiresEquipmentsForm::~ConstraintSubjectRequiresEquipmentsForm()
{
}

bool ConstraintSubjectRequiresEquipmentsForm::filterOk(SpaceConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_SUBJECT_REQUIRES_EQUIPMENTS){
		ConstraintSubjectRequiresEquipments* c=(ConstraintSubjectRequiresEquipments*) ctr;
		return c->subjectName==subjectsComboBox->currentText() || subjectsComboBox->currentText()=="";
	}
	else
		return false;
}

void ConstraintSubjectRequiresEquipmentsForm::filterChanged()
{
	this->visibleConstraintsList.clear();
	constraintsListBox->clear();
	for(SpaceConstraint* ctr=gt.rules.spaceConstraintsList.first(); ctr; ctr=gt.rules.spaceConstraintsList.next())
		if(filterOk(ctr)){
			visibleConstraintsList.append(ctr);
			constraintsListBox->insertItem(ctr->getDescription(gt.rules));
		}
}

void ConstraintSubjectRequiresEquipmentsForm::constraintChanged(int index)
{
	if(index<0)
		return;
	assert((uint)(index)<this->visibleConstraintsList.count());
	SpaceConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=NULL);
	currentConstraintTextEdit->setText(ctr->getDetailedDescription(gt.rules));
}

void ConstraintSubjectRequiresEquipmentsForm::addConstraint()
{
	AddConstraintSubjectRequiresEquipmentsForm *addConstraintSubjectRequiresEquipmentsForm=new AddConstraintSubjectRequiresEquipmentsForm();
	addConstraintSubjectRequiresEquipmentsForm->exec();

	filterChanged();
	
	constraintsListBox->setCurrentItem(constraintsListBox->count()-1);
}

void ConstraintSubjectRequiresEquipmentsForm::modifyConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	SpaceConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintSubjectRequiresEquipmentsForm *modifyConstraintSubjectRequiresEquipmentsForm
	 = new ModifyConstraintSubjectRequiresEquipmentsForm((ConstraintSubjectRequiresEquipments*)ctr);
	modifyConstraintSubjectRequiresEquipmentsForm->exec();

	filterChanged();
	constraintsListBox->setCurrentItem(i);
}

void ConstraintSubjectRequiresEquipmentsForm::removeConstraint()
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
