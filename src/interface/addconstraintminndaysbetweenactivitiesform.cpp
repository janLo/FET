/***************************************************************************
                          addconstraintminndaysbetweenactivitiesform.cpp  -  description
                             -------------------
    begin                : Feb 11, 2005
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

#include "addconstraintminndaysbetweenactivitiesform.h"
#include "spaceconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

AddConstraintMinNDaysBetweenActivitiesForm::AddConstraintMinNDaysBetweenActivitiesForm()
{
	updateActivitiesListBox();
	
	minDaysSpinBox->setMinValue(1);
	minDaysSpinBox->setMaxValue(gt.rules.nDaysPerWeek-1);
}

AddConstraintMinNDaysBetweenActivitiesForm::~AddConstraintMinNDaysBetweenActivitiesForm()
{
}

void AddConstraintMinNDaysBetweenActivitiesForm::updateActivitiesListBox()
{
	activitiesListBox->clear();
	selectedActivitiesListBox->clear();

	this->activitiesList.clear();
	this->selectedActivitiesList.clear();

	for(Activity* ac=gt.rules.activitiesList.first(); ac; ac=gt.rules.activitiesList.next()){
		activitiesListBox->insertItem(ac->getDescription(gt.rules));
		this->activitiesList.append(ac->id);
	}
}

void AddConstraintMinNDaysBetweenActivitiesForm::addConstraint()
{
	TimeConstraint *ctr=NULL;

	double weight;
	QString tmp=weightLineEdit->text();
	sscanf(tmp, "%lf", &weight);
	if(weight<0.0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid weight"));
		return;
	}

	bool compulsory=false;
	if(compulsoryCheckBox->isChecked())
		compulsory=true;

	if(this->selectedActivitiesList.count()==0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Empty list of selected activities"));
		return;
	}
	if(this->selectedActivitiesList.count()==1){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Only one selected activity"));
		return;
	}
	if(this->selectedActivitiesList.count()>(uint)(MAX_CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES)){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Please report error to the author\nMAX_CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES must be increased (you have too many activities)"));
		return;
	}
	
	int ids[MAX_CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES];
	int i;
	QValueList<int>::iterator it;
	for(i=0, it=this->selectedActivitiesList.begin(); it!=this->selectedActivitiesList.end(); it++, i++)
		ids[i]=*it;
	
	ctr=new ConstraintMinNDaysBetweenActivities(weight, compulsory, this->selectedActivitiesList.count(), ids, minDaysSpinBox->value());
	bool tmp2=gt.rules.addTimeConstraint(ctr);
	
	if(tmp2){
		QString s=QObject::tr("Constraint added:");
		s+="\n";
		s+=ctr->getDetailedDescription(gt.rules);
		QMessageBox::information(this, QObject::tr("FET information"), s);
	}
	else{
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Constraint NOT added - please report error"));
		delete ctr;
	}
}

void AddConstraintMinNDaysBetweenActivitiesForm::addActivity()
{
	if(activitiesListBox->currentItem()<0)
		return;
	int tmp=activitiesListBox->currentItem();
	int _id=*(this->activitiesList.at(tmp));
	
	QString actName=activitiesListBox->currentText();
	assert(actName!="");
	uint i;
	//duplicate?
	for(i=0; i<selectedActivitiesListBox->count(); i++)
		if(actName==selectedActivitiesListBox->text(i))
			break;
	if(i<selectedActivitiesListBox->count())
		return;
	selectedActivitiesListBox->insertItem(actName);
	
	this->selectedActivitiesList.append(_id);
}

void AddConstraintMinNDaysBetweenActivitiesForm::removeActivity()
{
	if(selectedActivitiesListBox->currentItem()<0 || selectedActivitiesListBox->count()<=0)
		return;		
	int tmp=selectedActivitiesListBox->currentItem();
	int _id=*(this->selectedActivitiesList.at(tmp));
	
	selectedActivitiesListBox->removeItem(selectedActivitiesListBox->currentItem());
	int tmp2=this->selectedActivitiesList.remove(_id);
	assert(tmp2==1);
}
