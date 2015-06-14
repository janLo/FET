/***************************************************************************
                          modifyconstraintactivityendsdayform.cpp  -  description
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

#include "modifyconstraintactivityendsdayform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>

#define yesNo(x)	((x)==0?QObject::tr("no"):QObject::tr("yes"))

ModifyConstraintActivityEndsDayForm::ModifyConstraintActivityEndsDayForm(ConstraintActivityEndsDay* ctr)
{
	this->_ctr=ctr;
	
	compulsoryCheckBox->setChecked(ctr->compulsory);
	weightLineEdit->setText(QString::number(ctr->weight));
	
	updateActivitiesComboBox();
}

ModifyConstraintActivityEndsDayForm::~ModifyConstraintActivityEndsDayForm()
{
}

void ModifyConstraintActivityEndsDayForm::updateActivitiesComboBox(){
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

void ModifyConstraintActivityEndsDayForm::constraintChanged()
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

	s+=QObject::tr("Activity ends day");
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

	currentConstraintTextEdit->setText(s);
}

void ModifyConstraintActivityEndsDayForm::ok()
{
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

	int tmp2=activitiesComboBox->currentItem();
	if(tmp2<0 || (uint)(tmp2)>=gt.rules.activitiesList.count()){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid activity"));
		return;
	}
	int id=gt.rules.activitiesList.at(tmp2)->id;

	this->_ctr->weight=weight;
	this->_ctr->compulsory=compulsory;
	this->_ctr->activityId=id;
	
	gt.rules.internalStructureComputed=false;

	this->close();
}

void ModifyConstraintActivityEndsDayForm::cancel()
{
	this->close();
}
