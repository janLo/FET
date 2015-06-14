/***************************************************************************
                          addconstraintactivitiessamestartingtimeform.cpp  -  description
                             -------------------
    begin                : Wed June 23 2004
    copyright            : (C) 2004 by Lalescu Liviu
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

#include "addconstraintactivitiessamestartingtimeform.h"
#include "spaceconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

AddConstraintActivitiesSameStartingTimeForm::AddConstraintActivitiesSameStartingTimeForm()
{
	updateActivitiesListBox();
}

AddConstraintActivitiesSameStartingTimeForm::~AddConstraintActivitiesSameStartingTimeForm()
{
}

void AddConstraintActivitiesSameStartingTimeForm::updateActivitiesListBox()
{
	activitiesListBox->clear();
	simultaneousActivitiesListBox->clear();

	this->activitiesList.clear();
	this->simultaneousActivitiesList.clear();

	if(blockCheckBox->isChecked())
		//show only non-split activities and split activities which are the representatives
		for(Activity* ac=gt.rules.activitiesList.first(); ac; ac=gt.rules.activitiesList.next()){
			if(ac->activityGroupId==0){
				activitiesListBox->insertItem(ac->getDescription(gt.rules));
				this->activitiesList.append(ac->id);
			}
			else if(ac->id==ac->activityGroupId){
				activitiesListBox->insertItem(ac->getDescription(gt.rules));
				this->activitiesList.append(ac->id);
			}
		}
	else
		for(Activity* ac=gt.rules.activitiesList.first(); ac; ac=gt.rules.activitiesList.next()){
			activitiesListBox->insertItem(ac->getDescription(gt.rules));
			this->activitiesList.append(ac->id);
		}
}

void AddConstraintActivitiesSameStartingTimeForm::blockChanged()
{
	this->updateActivitiesListBox();
}

void AddConstraintActivitiesSameStartingTimeForm::addConstraint()
{
	TimeConstraint *ctr=NULL;

	double weight;
	QString tmp=weightLineEdit->text();
	sscanf(tmp, "%lf", &weight);
	if(weight<=0.0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid weight"));
		return;
	}

	bool compulsory=false;
	if(compulsoryCheckBox->isChecked())
		compulsory=true;

	if(this->simultaneousActivitiesList.count()==0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Empty list of simultaneous activities"));
		return;
	}
	if(this->simultaneousActivitiesList.count()==1){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Only one selected activity - impossible"));
		return;
	}
	if(this->simultaneousActivitiesList.count()>=(uint)(MAX_CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME)){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Too many activities - please report error\n(CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME too little)"));
		return;
	}
	
if(blockCheckBox->isChecked()){ //block constraints
	///////////phase 1 - how many constraints will be added?
	int nConstraints=0;
	QValueList<int>::iterator it;
	for(it=this->simultaneousActivitiesList.begin(); it!=this->simultaneousActivitiesList.end(); it++){
		int _id=(*it);
		int tmp=0; //tmp represents the number of sub-activities represented by the current (sub)activity

		for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next()){
			if(act->activityGroupId==0){
				if(act->id==_id){
					assert(tmp==0);
					tmp=1;
				}
			}
			else{
				if(act->id==_id){
					assert(act->activityGroupId==act->id);
					assert(tmp==0);
					tmp=1;
				}
				else if(act->activityGroupId==_id)
					tmp++;
			}
		}

		if(nConstraints==0){
			nConstraints=tmp;
		}
		else{
			if(tmp!=nConstraints){
				QString s=QObject::tr("Sub-activities do not correspond. Mistake:\n");
				s+=QObject::tr("1. First (sub)activity has id=%1 and represents %2 sub-activities\n")
					.arg(*(this->simultaneousActivitiesList.at(0)))
					.arg(nConstraints);
				s+=QObject::tr("2. Current (sub)activity has id=%1 and represents %2 sub-activities\n")
					.arg(_id)
					.arg(tmp);
				QMessageBox::warning(this, QObject::tr("FET information"), s);
				return;				
			}
		}
	}
	
	/////////////phase 2 - compute the indices of all the (sub)activities
#ifdef WIN32
	int ids[10][MAX_CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME];
#else
	int ids[nConstraints][this->simultaneousActivitiesList.count()];
#endif
	int k;
	for(k=0, it=this->simultaneousActivitiesList.begin(); it!=this->simultaneousActivitiesList.end(); k++, it++){
		int _id=(*it);
		int tmp=0; //tmp represents the number of sub-activities represented by the current (sub)activity

		for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next()){
			if(act->activityGroupId==0){
				if(act->id==_id){
					assert(tmp==0);
					ids[tmp][k]=_id;
					tmp=1;
				}
			}
			else{
				if(act->id==_id){
					assert(act->activityGroupId==act->id);
					assert(tmp==0);
					ids[tmp][k]=_id;
					tmp=1;
				}
				else if(act->activityGroupId==_id){
					ids[tmp][k]=act->id;
					tmp++;
				}
			}
		}
	}
	
	////////////////phase 3 - add the constraints
	for(k=0; k<nConstraints; k++){
		ctr=new ConstraintActivitiesSameStartingTime(weight, compulsory, this->simultaneousActivitiesList.count(), ids[k]);
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
}
else{
	int ids[MAX_CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME];
	QValueList<int>::iterator it;
	int i;
	for(i=0, it=this->simultaneousActivitiesList.begin(); it!=this->simultaneousActivitiesList.end(); i++,it++){
		assert(i<MAX_CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME);
		ids[i]=*it;
	}
	ctr=new ConstraintActivitiesSameStartingTime(weight, compulsory, this->simultaneousActivitiesList.count(), ids);

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
}

void AddConstraintActivitiesSameStartingTimeForm::addActivity()
{
	if(activitiesListBox->currentItem()<0)
		return;
	int tmp=activitiesListBox->currentItem();
	int _id=*(this->activitiesList.at(tmp));
	
	QString actName=activitiesListBox->currentText();
	assert(actName!="");
	uint i;
	//duplicate?
	for(i=0; i<simultaneousActivitiesListBox->count(); i++)
		if(actName==simultaneousActivitiesListBox->text(i))
			break;
	if(i<simultaneousActivitiesListBox->count())
		return;
	simultaneousActivitiesListBox->insertItem(actName);
	
	this->simultaneousActivitiesList.append(_id);
}

void AddConstraintActivitiesSameStartingTimeForm::removeActivity()
{
	if(simultaneousActivitiesListBox->currentItem()<0 || simultaneousActivitiesListBox->count()<=0)
		return;		
	int tmp=simultaneousActivitiesListBox->currentItem();
	int _id=*(this->simultaneousActivitiesList.at(tmp));
	
	simultaneousActivitiesListBox->removeItem(simultaneousActivitiesListBox->currentItem());
	int tmp2=this->simultaneousActivitiesList.remove(_id);
	assert(tmp2==1);
}
