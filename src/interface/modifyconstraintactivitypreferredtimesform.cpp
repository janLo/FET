/***************************************************************************
                          modifyconstraintactivitypreferredtimesform.cpp  -  description
                             ------------------
    begin                : Feb 15, 2005
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

#include "modifyconstraintactivitypreferredtimesform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

#define YES	(QObject::tr("Yes"))
#define NO	(QObject::tr("No"))

ModifyConstraintActivityPreferredTimesForm::ModifyConstraintActivityPreferredTimesForm(ConstraintActivityPreferredTimes* ctr)
{
	this->_ctr=ctr;
	
	weightLineEdit->setText(QString::number(ctr->weight));
	compulsoryCheckBox->setChecked(ctr->compulsory);

	updateActivitiesComboBox();

	preferredTimesTable->setNumRows(gt.rules.nHoursPerDay+1);
	preferredTimesTable->setNumCols(gt.rules.nDaysPerWeek+1);

	for(int j=0; j<gt.rules.nDaysPerWeek; j++)
		preferredTimesTable->setText(0, j+1, gt.rules.daysOfTheWeek[j]);
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		preferredTimesTable->setText(i+1, 0, gt.rules.hoursOfTheDay[i]);
		
	bool currentMatrix[MAX_HOURS_PER_DAY][MAX_DAYS_PER_WEEK];
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++)
			currentMatrix[i][j]=false;
	for(int k=0; k<ctr->nPreferredTimes; k++){
		if(ctr->days[k]==-1 || ctr->hours[k]==-1)
			assert(0);
		int i=ctr->hours[k];
		int j=ctr->days[k];
		currentMatrix[i][j]=true;
	}

	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++)
			if(!currentMatrix[i][j])
				preferredTimesTable->setText(i+1, j+1, NO);
			else
				preferredTimesTable->setText(i+1, j+1, YES);
}

ModifyConstraintActivityPreferredTimesForm::~ModifyConstraintActivityPreferredTimesForm()
{
}

void ModifyConstraintActivityPreferredTimesForm::tableClicked(int row, int col, int button, const QPoint& mousePos)
{
	if(&button!=NULL && &mousePos!=NULL)
		;

	row--; col--;
	if(row>=0 && row<gt.rules.nHoursPerDay && col>=0 && col<gt.rules.nDaysPerWeek){
		QString s=preferredTimesTable->text(row+1, col+1);
		if(s==YES)
			s=NO;
		else{
			assert(s==NO);
			s=YES;
		}
		preferredTimesTable->setText(row+1, col+1, s);
	}
}

void ModifyConstraintActivityPreferredTimesForm::updateActivitiesComboBox(){
	int i=0, j=-1;
	activitiesComboBox->clear();
	for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next()){
		activitiesComboBox->insertItem(act->getDescription(gt.rules));
		if(act->id==this->_ctr->activityId)
			j=i;
		i++;
	}
	assert(j>=0);
	activitiesComboBox->setCurrentItem(j);
}

void ModifyConstraintActivityPreferredTimesForm::cancel()
{
	this->close();
}

void ModifyConstraintActivityPreferredTimesForm::ok()
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

	int i=activitiesComboBox->currentItem();
	if(i<0 || activitiesComboBox->count()<=0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid activity"));
		return;
	}
	Activity* act=gt.rules.activitiesList.at(i);
	
	int days[MAX_N_CONSTRAINT_ACTIVITY_PREFERRED_TIMES];
	int hours[MAX_N_CONSTRAINT_ACTIVITY_PREFERRED_TIMES];
	int n=0;
	for(i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++)
			if(preferredTimesTable->text(i+1, j+1)==YES){
				if(n>=MAX_N_CONSTRAINT_ACTIVITY_PREFERRED_TIMES){
					QString s=QObject::tr("Not enough slots (too many \"Yes\" values).");
					s+="\n";
					s+=QObject::tr("Please increase the variable MAX_N_CONSTRAINT_ACTIVITY_PREFERRED_TIMES");
					s+="\n";
					s+=QObject::tr("Currently, it is %1").arg(MAX_N_CONSTRAINT_ACTIVITY_PREFERRED_TIMES);
					QMessageBox::warning(this, QObject::tr("FET information"), s);
					
					return;
				}
				
				days[n]=j;
				hours[n]=i;
				n++;
			}

	this->_ctr->weight=weight;
	this->_ctr->compulsory=compulsory;
	this->_ctr->activityId=act->id;
	this->_ctr->nPreferredTimes=n;
	for(int i=0; i<n; i++){
		this->_ctr->days[i]=days[i];
		this->_ctr->hours[i]=hours[i];
	}

	gt.rules.internalStructureComputed=false;
	
	this->close();
}

#undef YES
#undef NO
