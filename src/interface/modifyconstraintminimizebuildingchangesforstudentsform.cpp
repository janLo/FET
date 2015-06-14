/***************************************************************************
                          modifyconstraintminimizebuildingchangesforstudentsform.cpp  -  description
                             -------------------
    begin                : July 9, 2005
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

#include "modifyconstraintminimizebuildingchangesforstudentsform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>

#define yesNo(x)	((x)==0?QObject::tr("no"):QObject::tr("yes"))

ModifyConstraintMinimizeBuildingChangesForStudentsForm::ModifyConstraintMinimizeBuildingChangesForStudentsForm(ConstraintMinimizeBuildingChangesForStudents* ctr)
{
	this->_ctr=ctr;
	
	compulsoryCheckBox->setChecked(ctr->compulsory);
	weightLineEdit->setText(QString::number(ctr->weight));
}

ModifyConstraintMinimizeBuildingChangesForStudentsForm::~ModifyConstraintMinimizeBuildingChangesForStudentsForm()
{
}

void ModifyConstraintMinimizeBuildingChangesForStudentsForm::constraintChanged()
{
	QString s;
	s+=QObject::tr("Current constraint:");
	s+="\n";
	s+=QObject::tr("Minimize building changes for students");
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

	currentConstraintTextEdit->setText(s);
}

void ModifyConstraintMinimizeBuildingChangesForStudentsForm::ok()
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
	
	gt.rules.internalStructureComputed=false;

	this->close();
}

void ModifyConstraintMinimizeBuildingChangesForStudentsForm::cancel()
{
	this->close();
}
