/***************************************************************************
                          addconstraintactivitypreferredtimeform.cpp  -  description
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

#include "addconstraintactivitypreferredtimeform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>

#define yesNo(x)	((x)==0?QObject::tr("no"):QObject::tr("yes"))

AddConstraintActivityPreferredTimeForm::AddConstraintActivityPreferredTimeForm()
{
	updatePeriodGroupBox();
	updateActivitiesComboBox();
}

AddConstraintActivityPreferredTimeForm::~AddConstraintActivityPreferredTimeForm()
{
}

void AddConstraintActivityPreferredTimeForm::updateActivitiesComboBox(){
	activitiesComboBox->clear();
	for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next())
		activitiesComboBox->insertItem(act->getDescription(gt.rules));

	constraintChanged();
}

void AddConstraintActivityPreferredTimeForm::updatePeriodGroupBox(){
	startHourComboBox->clear();
	startHourComboBox->insertItem(QObject::tr("Any"));
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		startHourComboBox->insertItem(gt.rules.hoursOfTheDay[i]);

	dayComboBox->clear();
	dayComboBox->insertItem(QObject::tr("Any"));
	for(int i=0; i<gt.rules.nDaysPerWeek; i++)
		dayComboBox->insertItem(gt.rules.daysOfTheWeek[i]);
}

void AddConstraintActivityPreferredTimeForm::constraintChanged()
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
	int tmp2=activitiesComboBox->currentItem();
	if(tmp2<0 || (uint)(tmp2)>=gt.rules.activitiesList.count()){
		s+=QObject::tr("Invalid activity");
		s+="\n";
	}
	else{
		int id=gt.rules.activitiesList.at(tmp2)->id;
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
}

void AddConstraintActivityPreferredTimeForm::addCurrentConstraint()
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

	int id;
	int tmp2=activitiesComboBox->currentItem();
	if(tmp2<0 || (uint)(tmp2)>=gt.rules.activitiesList.count()){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid activity"));
		return;
	}
	else
		id=gt.rules.activitiesList.at(tmp2)->id;
	
	ctr=new ConstraintActivityPreferredTime(weight, compulsory, id, day-1, startHour-1);

	bool tmp3=gt.rules.addTimeConstraint(ctr);
	if(tmp3)
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Constraint added"));
	else{
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Constraint NOT added - please report error"));
		delete ctr;
	}
}
