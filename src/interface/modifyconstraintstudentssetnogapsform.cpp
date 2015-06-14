/***************************************************************************
                          modifyconstraintstudentssetnogapsform.cpp  -  description
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

#include "modifyconstraintstudentssetnogapsform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>

#define yesNo(x)	((x)==0?QObject::tr("no"):QObject::tr("yes"))

ModifyConstraintStudentsSetNoGapsForm::ModifyConstraintStudentsSetNoGapsForm(ConstraintStudentsSetNoGaps* ctr)
{
	this->_ctr=ctr;
	
	compulsoryCheckBox->setChecked(ctr->compulsory);
	weightLineEdit->setText(QString::number(ctr->weight));
	
	updateStudentsComboBox();

	constraintChanged();
}

ModifyConstraintStudentsSetNoGapsForm::~ModifyConstraintStudentsSetNoGapsForm()
{
}

void ModifyConstraintStudentsSetNoGapsForm::updateStudentsComboBox(){
	studentsComboBox->clear();
	int i=0, j=-1;
	for(StudentsYear* sty=gt.rules.yearsList.first(); sty; sty=gt.rules.yearsList.next()){
		studentsComboBox->insertItem(sty->name);
		if(sty->name==this->_ctr->students)
			j=i;
		i++;
		for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
			studentsComboBox->insertItem(stg->name);
			if(stg->name==this->_ctr->students)
				j=i;
			i++;
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next()){
				studentsComboBox->insertItem(sts->name);
				if(sts->name==this->_ctr->students)
					j=i;
				i++;
			}
		}
	}
	assert(j>=0);
	studentsComboBox->setCurrentItem(j);																

	constraintChanged();
}

void ModifyConstraintStudentsSetNoGapsForm::constraintChanged()
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

	s+=QObject::tr("Students set no gaps");
	s+="\n";
	s+=QObject::tr("Students set=%1").arg(studentsComboBox->currentText());
	s+="\n";

	currentConstraintTextEdit->setText(s);
}

void ModifyConstraintStudentsSetNoGapsForm::ok()
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

	QString students_name=studentsComboBox->currentText();
	StudentsSet* s=gt.rules.searchStudentsSet(students_name);
	if(s==NULL){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid students set"));
		return;
	}

	this->_ctr->weight=weight;
	this->_ctr->compulsory=compulsory;
	this->_ctr->students=students_name;

	gt.rules.internalStructureComputed=false;
	
	this->close();
}

void ModifyConstraintStudentsSetNoGapsForm::cancel()
{
	this->close();
}
