/***************************************************************************
                          modifyconstraintactivitypreferredtimeform.cpp  -  description
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

#include "modifyconstraintactivitypreferredtimeform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>

#define yesNo(x)	((x)==0?QObject::tr("no"):QObject::tr("yes"))

ModifyConstraintActivityPreferredTimeForm::ModifyConstraintActivityPreferredTimeForm(ConstraintActivityPreferredTime* ctr)
{
	this->_ctr=ctr;
	
	compulsoryCheckBox->setChecked(ctr->compulsory);
	weightLineEdit->setText(QString::number(ctr->weight));
	
	updatePeriodGroupBox();
	updateActivitiesComboBox();

	dayComboBox->setCurrentItem(ctr->day+1);
	startHourComboBox->setCurrentItem(ctr->hour+1);

	constraintChanged();
}

ModifyConstraintActivityPreferredTimeForm::~ModifyConstraintActivityPreferredTimeForm()
{
}

void ModifyConstraintActivityPreferredTimeForm::updateActivitiesComboBox(){
	activitiesComboBox->clear();
	int i=0, j=-1;
	for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next(), i++){
		activitiesComboBox->insertItem(act->getDescription(gt.rules));
		if(act->id==this->_ctr->activityId)
			j=i;
	}
	assert(j>=0);
	activitiesComboBox->setCurrentItem(j);

	constraintChanged();
}

void ModifyConstraintActivityPreferredTimeForm::updatePeriodGroupBox(){
	startHourComboBox->clear();
	startHourComboBox->insertItem(QObject::tr("Any"));
	for(int i=0; i<=gt.rules.nHoursPerDay; i++)
		startHourComboBox->insertItem(gt.rules.hoursOfTheDay[i]);

	dayComboBox->clear();
	dayComboBox->insertItem(QObject::tr("Any"));
	for(int i=0; i<gt.rules.nDaysPerWeek; i++)
		dayComboBox->insertItem(gt.rules.daysOfTheWeek[i]);
}

void ModifyConstraintActivityPreferredTimeForm::constraintChanged()
{
	QString s;
	s+=QObject::tr("Current constraint:");
	s+="\n";

	double weight;
	QString tmp=weightLineEdit->text();
	sscanf(tmp, "%lf", &weight);
	s+=QObject::tr("Weight=%1").arg(weight);
	s+="\n";

	bool compulsory=false;
	if(compulsoryCheckBox->isChecked())
		compulsory=true;
	s+=QObject::tr("Compulsory=%1").arg(yesNo(compulsory));
	s+="\n";

	s+=QObject::tr("Activity preferred time");
	s+="\n";
	
	int id;
	int tmp2=activitiesComboBox->currentItem();
	if(tmp2<0 || (uint)(tmp2)>=gt.rules.activitiesList.count()){
		s+=QObject::tr("Invalid activity");
		s+="\n";
	}
	else{
		id=gt.rules.activitiesList.at(tmp2)->id;
		s+=QObject::tr("Activity id=%1").arg(id);
		s+="\n";
	}

	int day=dayComboBox->currentItem();
	if(day<0 || day>gt.rules.nDaysPerWeek){
		s+=QObject::tr("Invalid day");
		s+="\n";
	}
	else{
		s+=QObject::tr("Day:%1").arg(dayComboBox->currentText());
		s+="\n";
	}

	int startHour=startHourComboBox->currentItem();
	if(startHour<0 || startHour>gt.rules.nHoursPerDay){
		s+=QObject::tr("Invalid start hour");
		s+="\n";
	}
	else{
		s+=QObject::tr("Start hour:%1").arg(startHourComboBox->currentText());
		s+="\n";
	}

	currentConstraintTextEdit->setText(s);
}

void ModifyConstraintActivityPreferredTimeForm::ok()
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

	int day=dayComboBox->currentItem();
	if(day<0 || day>gt.rules.nDaysPerWeek){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid day"));
		return;
	}
	int startHour=startHourComboBox->currentItem();
	if(startHour<0 || startHour>gt.rules.nHoursPerDay){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid start hour"));
		return;
	}

	if(startHour==0 && day==0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Please specify at least a day or an hour"));
		return;
	}
	
	int tmp2=activitiesComboBox->currentItem();
	if(tmp2<0 || (uint)(tmp2)>=gt.rules.activitiesList.count()){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid activity"));
		return;
	}
	int id=gt.rules.activitiesList.at(tmp2)->id;

	this->_ctr->weight=weight;
	this->_ctr->compulsory=compulsory;
	this->_ctr->day=day-1;
	this->_ctr->hour=startHour-1;
	this->_ctr->activityId=id;
	
	gt.rules.internalStructureComputed=false;

	this->close();
}

void ModifyConstraintActivityPreferredTimeForm::cancel()
{
	this->close();
}
