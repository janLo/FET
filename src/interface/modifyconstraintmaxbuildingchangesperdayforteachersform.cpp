/***************************************************************************
                          modifyconstraintmaxbuildingchangesperdayforteachersform.cpp  -  description
                             -------------------
    begin                : July 11, 2005
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

#include "modifyconstraintmaxbuildingchangesperdayforteachersform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>

#define yesNo(x)	((x)==0?QObject::tr("no"):QObject::tr("yes"))

ModifyConstraintMaxBuildingChangesPerDayForTeachersForm::ModifyConstraintMaxBuildingChangesPerDayForTeachersForm(ConstraintMaxBuildingChangesPerDayForTeachers* ctr)
{
	this->_ctr=ctr;
	
	compulsoryCheckBox->setChecked(ctr->compulsory);
	weightLineEdit->setText(QString::number(ctr->weight));	
	maxBuildingChangesSpinBox->setValue(ctr->maxBuildingChanges);
}

ModifyConstraintMaxBuildingChangesPerDayForTeachersForm::~ModifyConstraintMaxBuildingChangesPerDayForTeachersForm()
{
}

void ModifyConstraintMaxBuildingChangesPerDayForTeachersForm::constraintChanged()
{
	QString s;
	s+=QObject::tr("Current constraint:");
	s+="\n";
	s+=QObject::tr("Max building changes per day for teachers");
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
	
	s+=QObject::tr("Max building changes=%1").arg(maxBuildingChangesSpinBox->value());
	s+="\n";

	currentConstraintTextEdit->setText(s);
}

void ModifyConstraintMaxBuildingChangesPerDayForTeachersForm::ok()
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

	this->_ctr->weight=weight;
	this->_ctr->compulsory=compulsory;
	this->_ctr->maxBuildingChanges=maxBuildingChangesSpinBox->value();
	
	gt.rules.internalStructureComputed=false;

	this->close();
}

void ModifyConstraintMaxBuildingChangesPerDayForTeachersForm::cancel()
{
	this->close();
}
