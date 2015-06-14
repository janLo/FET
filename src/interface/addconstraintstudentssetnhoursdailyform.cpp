/***************************************************************************
                          addconstraintstudentssetnhoursdailyform.cpp  -  description
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

#include "addconstraintstudentssetnhoursdailyform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>

#define yesNo(x)	((x)==0?QObject::tr("no"):QObject::tr("yes"))

AddConstraintStudentsSetNHoursDailyForm::AddConstraintStudentsSetNHoursDailyForm()
{
	minHoursSpinBox->setMinValue(-1);
	minHoursSpinBox->setMaxValue(gt.rules.nHoursPerDay);
	minHoursSpinBox->setValue(-1);
	maxHoursSpinBox->setMinValue(-1);
	maxHoursSpinBox->setMaxValue(gt.rules.nHoursPerDay);
	maxHoursSpinBox->setValue(-1);

	updateStudentsSetComboBox();
}

AddConstraintStudentsSetNHoursDailyForm::~AddConstraintStudentsSetNHoursDailyForm()
{
}

void AddConstraintStudentsSetNHoursDailyForm::updateStudentsSetComboBox()
{
	studentsComboBox->clear();	
	for(StudentsYear* sty=gt.rules.yearsList.first(); sty; sty=gt.rules.yearsList.next()){
		studentsComboBox->insertItem(sty->name);
		for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
			studentsComboBox->insertItem(stg->name);
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next()){
				studentsComboBox->insertItem(sts->name);
			}
		}
	}

	constraintChanged();
}

void AddConstraintStudentsSetNHoursDailyForm::constraintChanged()
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

	s+=QObject::tr("Students set n hours daily");
	s+="\n";
	s+=QObject::tr("Students set=%1").arg(studentsComboBox->currentText());
	s+="\n";

	int minHours=minHoursSpinBox->value();
	if(minHours>=0){
		s+=QObject::tr("Min. hours:%1").arg(minHours);
		s+="\n";
	}
	int maxHours=maxHoursSpinBox->value();
	if(maxHours>=0){
		s+=QObject::tr("Max. hours:%1").arg(maxHours);
		s+="\n";
	}

	currentConstraintTextEdit->setText(s);
}

void AddConstraintStudentsSetNHoursDailyForm::addCurrentConstraint()
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

	int minHours=minHoursSpinBox->value();
	int maxHours=maxHoursSpinBox->value();

	if(minHours==-1 && maxHours==-1){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Please specify at least min or max hours"));
		return;
	}
	
	if(minHours==-1 && maxHours==-1){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Please specify at least min or max hours"));
		return;
	}

	QString students_name=studentsComboBox->currentText();
	StudentsSet* s=gt.rules.searchStudentsSet(students_name);
	if(s==NULL){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid students set"));
		return;
	}

	ctr=new ConstraintStudentsSetNHoursDaily(weight, compulsory, maxHours, minHours, students_name);

	bool tmp2=gt.rules.addTimeConstraint(ctr);
	if(tmp2)
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Constraint added"));
	else{
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Constraint NOT added - please report error"));
		delete ctr;
	}
}
