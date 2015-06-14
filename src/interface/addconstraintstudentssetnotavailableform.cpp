/***************************************************************************
                          addconstraintstudentssetnotavailableform.cpp  -  description
                             -------------------
    begin                : Feb 10, 2005
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

#include "addconstraintstudentssetnotavailableform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>

#define yesNo(x)	((x)==0?QObject::tr("no"):QObject::tr("yes"))

AddConstraintStudentsSetNotAvailableForm::AddConstraintStudentsSetNotAvailableForm()
{
	updatePeriodGroupBox();
	updateStudentsSetComboBox();
}

AddConstraintStudentsSetNotAvailableForm::~AddConstraintStudentsSetNotAvailableForm()
{
}

void AddConstraintStudentsSetNotAvailableForm::updateStudentsSetComboBox()
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

void AddConstraintStudentsSetNotAvailableForm::updatePeriodGroupBox(){
	startHourComboBox->clear();
	for(int i=0; i<=gt.rules.nHoursPerDay; i++)
		startHourComboBox->insertItem(gt.rules.hoursOfTheDay[i]);

	endHourComboBox->clear();
	for(int i=0; i<=gt.rules.nHoursPerDay; i++)
		endHourComboBox->insertItem(gt.rules.hoursOfTheDay[i]);

	dayComboBox->clear();
	for(int i=0; i<gt.rules.nDaysPerWeek; i++)
		dayComboBox->insertItem(gt.rules.daysOfTheWeek[i]);
}

void AddConstraintStudentsSetNotAvailableForm::constraintChanged()
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

	s+=QObject::tr("Students set not available");
	s+="\n";
	s+=QObject::tr("Students set=%1").arg(studentsComboBox->currentText());
	s+="\n";

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

	int endHour=endHourComboBox->currentItem();
	if(endHour<0 || endHour>gt.rules.nHoursPerDay){
		s+=QObject::tr("Invalid end hour");
		s+="\n";
	}
	else{
		s+=QObject::tr("End hour:%1").arg(endHourComboBox->currentText());
		s+="\n";
	}

	currentConstraintTextEdit->setText(s);
}

void AddConstraintStudentsSetNotAvailableForm::addCurrentConstraint()
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
	int endHour=-1;
	endHour=endHourComboBox->currentItem();
	if(endHour<0 || endHour>gt.rules.nHoursPerDay){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid end hour"));
		return;
	}
	if(endHour<=startHour){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("End hour <= start hour - impossible"));
		return;
	}

	QString students_name=studentsComboBox->currentText();
	StudentsSet* s=gt.rules.searchStudentsSet(students_name);
	if(s==NULL){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid students set"));
		return;
	}

	ctr=new ConstraintStudentsSetNotAvailable(weight, compulsory, students_name, day, startHour, endHour);

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
