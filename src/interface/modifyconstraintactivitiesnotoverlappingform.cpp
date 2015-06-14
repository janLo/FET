/***************************************************************************
                          modifyconstraintactivitiesnotoverlappingform.cpp  -  description
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

#include "modifyconstraintactivitiesnotoverlappingform.h"
#include "spaceconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

ModifyConstraintActivitiesNotOverlappingForm::ModifyConstraintActivitiesNotOverlappingForm(ConstraintActivitiesNotOverlapping* ctr)
{
	this->_ctr=ctr;
	updateActivitiesListBox();
	
	for(int i=0; i<ctr->n_activities; i++){
		int actId=ctr->activitiesId[i];
		this->notOverlappingActivitiesList.append(actId);
		Activity* act;
		for(act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next())
			if(act->id==actId)
				break;
		assert(act);
		this->notOverlappingActivitiesListBox->insertItem(act->getDescription(gt.rules));
	}
	
	compulsoryCheckBox->setChecked(ctr->compulsory);
	weightLineEdit->setText(QString::number(ctr->weight));			
}

ModifyConstraintActivitiesNotOverlappingForm::~ModifyConstraintActivitiesNotOverlappingForm()
{
}

void ModifyConstraintActivitiesNotOverlappingForm::updateActivitiesListBox()
{
	activitiesListBox->clear();
	notOverlappingActivitiesListBox->clear();

	this->activitiesList.clear();
	this->notOverlappingActivitiesList.clear();

	for(Activity* ac=gt.rules.activitiesList.first(); ac; ac=gt.rules.activitiesList.next()){
		activitiesListBox->insertItem(ac->getDescription(gt.rules));
		this->activitiesList.append(ac->id);
	}
}

void ModifyConstraintActivitiesNotOverlappingForm::ok()
{
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

	if(this->notOverlappingActivitiesList.count()==0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Empty list of not overlapping activities"));
		return;
	}
	if(this->notOverlappingActivitiesList.count()==1){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Only one selected activity"));
		return;
	}
	if(this->notOverlappingActivitiesList.count()>(uint)(MAX_CONSTRAINT_ACTIVITIES_NOT_OVERLAPPING)){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Please report error to the author\nMAX_CONSTRAINT_ACTIVITIES_NOT_OVERLAPPING must be increased (you have too many activities)"));
		return;
	}
	
	int i;
	QValueList<int>::iterator it;
	for(i=0, it=this->notOverlappingActivitiesList.begin(); it!=this->notOverlappingActivitiesList.end(); it++, i++)
		this->_ctr->activitiesId[i]=*it;
	this->_ctr->n_activities=i;
		
	this->_ctr->weight=weight;
	this->_ctr->compulsory=compulsory;
	
	gt.rules.internalStructureComputed=false;
	
	this->close();
}

void ModifyConstraintActivitiesNotOverlappingForm::cancel()
{
	this->close();
}

void ModifyConstraintActivitiesNotOverlappingForm::addActivity()
{
	if(activitiesListBox->currentItem()<0)
		return;
	int tmp=activitiesListBox->currentItem();
	int _id=*(this->activitiesList.at(tmp));
	
	QString actName=activitiesListBox->currentText();
	assert(actName!="");
	uint i;
	//duplicate?
	for(i=0; i<notOverlappingActivitiesListBox->count(); i++)
		if(actName==notOverlappingActivitiesListBox->text(i))
			break;
	if(i<notOverlappingActivitiesListBox->count())
		return;
	notOverlappingActivitiesListBox->insertItem(actName);
	
	this->notOverlappingActivitiesList.append(_id);
}

void ModifyConstraintActivitiesNotOverlappingForm::removeActivity()
{
	if(notOverlappingActivitiesListBox->currentItem()<0 || notOverlappingActivitiesListBox->count()<=0)
		return;		
	int tmp=notOverlappingActivitiesListBox->currentItem();
	int _id=*(this->notOverlappingActivitiesList.at(tmp));
	
	notOverlappingActivitiesListBox->removeItem(notOverlappingActivitiesListBox->currentItem());
	int tmp2=this->notOverlappingActivitiesList.remove(_id);
	assert(tmp2==1);
}
