/***************************************************************************
                          addconstraintactivityendsdayform.cpp  -  description
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

#include "addconstraintactivityendsdayform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>

#define yesNo(x)	((x)==0?QObject::tr("no"):QObject::tr("yes"))

AddConstraintActivityEndsDayForm::AddConstraintActivityEndsDayForm()
{
	updateActivitiesComboBox();
}

AddConstraintActivityEndsDayForm::~AddConstraintActivityEndsDayForm()
{
}

void AddConstraintActivityEndsDayForm::updateActivitiesComboBox(){
	activitiesComboBox->clear();
	for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next())
		activitiesComboBox->insertItem(act->getDescription(gt.rules));

	constraintChanged();
}

void AddConstraintActivityEndsDayForm::constraintChanged()
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
}

void AddConstraintActivityEndsDayForm::addCurrentConstraint()
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

	int id;
	int tmp2=activitiesComboBox->currentItem();
	if(tmp2<0 || (uint)(tmp2)>=gt.rules.activitiesList.count()){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid activity"));
		return;
	}
	else
		id=gt.rules.activitiesList.at(tmp2)->id;
	
	ctr=new ConstraintActivityEndsDay(weight, compulsory, id);

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
