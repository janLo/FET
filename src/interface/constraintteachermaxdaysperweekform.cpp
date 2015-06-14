/***************************************************************************
                          constraintteachermaxdaysperweekform.cpp  -  description
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

#include "constraintteachermaxdaysperweekform.h"
#include "addconstraintteachermaxdaysperweekform.h"
#include "modifyconstraintteachermaxdaysperweekform.h"

ConstraintTeacherMaxDaysPerWeekForm::ConstraintTeacherMaxDaysPerWeekForm()
{
	teachersComboBox->insertItem("");
	for(Teacher* tch=gt.rules.teachersList.first(); tch; tch=gt.rules.teachersList.next())
		teachersComboBox->insertItem(tch->name);

	this->visibleConstraintsList.setAutoDelete(false);
	this->filterChanged();
}

ConstraintTeacherMaxDaysPerWeekForm::~ConstraintTeacherMaxDaysPerWeekForm()
{
}

bool ConstraintTeacherMaxDaysPerWeekForm::filterOk(TimeConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_TEACHER_MAX_DAYS_PER_WEEK){
		ConstraintTeacherMaxDaysPerWeek* c=(ConstraintTeacherMaxDaysPerWeek*) ctr;
		return c->teacher==teachersComboBox->currentText() || teachersComboBox->currentText()=="";
	}
	else
		return false;
}

void ConstraintTeacherMaxDaysPerWeekForm::filterChanged()
{
	this->visibleConstraintsList.clear();
	constraintsListBox->clear();
	for(TimeConstraint* ctr=gt.rules.timeConstraintsList.first(); ctr; ctr=gt.rules.timeConstraintsList.next())
		if(filterOk(ctr)){
			visibleConstraintsList.append(ctr);
			constraintsListBox->insertItem(ctr->getDescription(gt.rules));
		}
}

void ConstraintTeacherMaxDaysPerWeekForm::constraintChanged(int index)
{
	if(index<0)
		return;
	assert((uint)(index)<this->visibleConstraintsList.count());
	TimeConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=NULL);
	currentConstraintTextEdit->setText(ctr->getDetailedDescription(gt.rules));
}

void ConstraintTeacherMaxDaysPerWeekForm::addConstraint()
{
	AddConstraintTeacherMaxDaysPerWeekForm *addConstraintTeacherMaxDaysPerWeekForm=new AddConstraintTeacherMaxDaysPerWeekForm();
	addConstraintTeacherMaxDaysPerWeekForm->exec();

	filterChanged();
	
	constraintsListBox->setCurrentItem(constraintsListBox->count()-1);
}

void ConstraintTeacherMaxDaysPerWeekForm::modifyConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	TimeConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintTeacherMaxDaysPerWeekForm *modifyConstraintTeacherMaxDaysPerWeekForm
	 = new ModifyConstraintTeacherMaxDaysPerWeekForm((ConstraintTeacherMaxDaysPerWeek*)ctr);
	modifyConstraintTeacherMaxDaysPerWeekForm->exec();

	filterChanged();
	constraintsListBox->setCurrentItem(i);
}

void ConstraintTeacherMaxDaysPerWeekForm::removeConstraint()
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
