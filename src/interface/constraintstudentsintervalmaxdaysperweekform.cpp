/***************************************************************************
                          constraintstudentsintervalmaxdaysperweekform.cpp  -  description
                             -------------------
    begin                : 2008
    copyright            : (C) 2008 by Lalescu Liviu
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

#include "constraintstudentsintervalmaxdaysperweekform.h"
#include "addconstraintstudentsintervalmaxdaysperweekform.h"
#include "modifyconstraintstudentsintervalmaxdaysperweekform.h"

#include <QDesktopWidget>

ConstraintStudentsIntervalMaxDaysPerWeekForm::ConstraintStudentsIntervalMaxDaysPerWeekForm()
{
	//setWindowFlags(Qt::Window);
	/*setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	QDesktopWidget* desktop=QApplication::desktop();
	int xx=desktop->width()/2 - frameGeometry().width()/2;
	int yy=desktop->height()/2 - frameGeometry().height()/2;
	move(xx, yy);*/
	centerWidgetOnScreen(this);
	
/*	studentsComboBox->insertItem("");

	for(int i=0; i<gt.rules.yearsList.size(); i++){
		StudentsYear* sty=gt.rules.yearsList[i];
		studentsComboBox->insertItem(sty->name);
		for(int j=0; j<sty->groupsList.size(); j++){
			StudentsGroup* stg=sty->groupsList[j];
			studentsComboBox->insertItem(stg->name);
			for(int k=0; k<stg->subgroupsList.size(); k++){
				StudentsSubgroup* sts=stg->subgroupsList[k];
				studentsComboBox->insertItem(sts->name);
			}
		}
	}*/

	this->filterChanged();
}

ConstraintStudentsIntervalMaxDaysPerWeekForm::~ConstraintStudentsIntervalMaxDaysPerWeekForm()
{
}

bool ConstraintStudentsIntervalMaxDaysPerWeekForm::filterOk(TimeConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_STUDENTS_INTERVAL_MAX_DAYS_PER_WEEK){
		return true;
		//ConstraintStudentsIntervalMaxDaysPerWeek* c=(ConstraintStudentsIntervalMaxDaysPerWeek*) ctr;
		//return c->students==studentsComboBox->currentText() || studentsComboBox->currentText()=="";
	}
	else
		return false;
}

void ConstraintStudentsIntervalMaxDaysPerWeekForm::filterChanged()
{
	this->visibleConstraintsList.clear();
	constraintsListBox->clear();
	for(int i=0; i<gt.rules.timeConstraintsList.size(); i++){
		TimeConstraint* ctr=gt.rules.timeConstraintsList[i];
		if(filterOk(ctr)){
			visibleConstraintsList.append(ctr);
			constraintsListBox->insertItem(ctr->getDescription(gt.rules));
		}
	}
}

void ConstraintStudentsIntervalMaxDaysPerWeekForm::constraintChanged(int index)
{
	if(index<0)
		return;
	assert(index<this->visibleConstraintsList.size());
	TimeConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=NULL);
	currentConstraintTextEdit->setText(ctr->getDetailedDescription(gt.rules));
}

void ConstraintStudentsIntervalMaxDaysPerWeekForm::addConstraint()
{
	AddConstraintStudentsIntervalMaxDaysPerWeekForm *form=new AddConstraintStudentsIntervalMaxDaysPerWeekForm();
	form->exec();

	filterChanged();
	
	constraintsListBox->setCurrentItem(constraintsListBox->count()-1);
}

void ConstraintStudentsIntervalMaxDaysPerWeekForm::modifyConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, tr("FET information"), tr("Invalid selected constraint"));
		return;
	}
	TimeConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintStudentsIntervalMaxDaysPerWeekForm *form
	 = new ModifyConstraintStudentsIntervalMaxDaysPerWeekForm((ConstraintStudentsIntervalMaxDaysPerWeek*)ctr);
	form->exec();

	filterChanged();
	constraintsListBox->setCurrentItem(i);
}

void ConstraintStudentsIntervalMaxDaysPerWeekForm::removeConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, tr("FET information"), tr("Invalid selected constraint"));
		return;
	}
	TimeConstraint* ctr=this->visibleConstraintsList.at(i);
	QString s;
	s=tr("Removing constraint:\n");
	s+=ctr->getDetailedDescription(gt.rules);
	s+=tr("\nAre you sure?");

	switch( QMessageBox::warning( this, tr("FET warning"),
		s, tr("OK"), tr("Cancel"), 0, 0, 1 ) ){
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