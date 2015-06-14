/***************************************************************************
                          addconstraintactivitiespreferredtimesform.cpp  -  description
                             -------------------
    begin                : 15 May 2004
    copyright            : (C) 2004 by Lalescu Liviu
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

#include "addconstraintactivitiespreferredtimesform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

#define YES	(QObject::tr("Yes"))
#define NO	(QObject::tr("No"))

AddConstraintActivitiesPreferredTimesForm::AddConstraintActivitiesPreferredTimesForm()
{
	updateTeachersComboBox();
	updateStudentsComboBox();
	updateSubjectsComboBox();
	updateSubjectTagsComboBox();

	preferredTimesTable->setNumRows(gt.rules.nHoursPerDay+1);
	preferredTimesTable->setNumCols(gt.rules.nDaysPerWeek+1);

	for(int j=0; j<gt.rules.nDaysPerWeek; j++)
		preferredTimesTable->setText(0, j+1, gt.rules.daysOfTheWeek[j]);
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		preferredTimesTable->setText(i+1, 0, gt.rules.hoursOfTheDay[i]);

	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++)
			preferredTimesTable->setText(i+1, j+1, NO);
}

AddConstraintActivitiesPreferredTimesForm::~AddConstraintActivitiesPreferredTimesForm()
{
}

void AddConstraintActivitiesPreferredTimesForm::tableClicked(int row, int col, int button, const QPoint& mousePos)
{
	if(&button!=NULL && &mousePos!=NULL)
		; //to avoid "unused parameter" compiler warning

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

void AddConstraintActivitiesPreferredTimesForm::updateTeachersComboBox(){
	teachersComboBox->clear();
	teachersComboBox->insertItem("");
	for(Teacher* t=gt.rules.teachersList.first(); t; t=gt.rules.teachersList.next())
		teachersComboBox->insertItem(t->name);
}

void AddConstraintActivitiesPreferredTimesForm::updateStudentsComboBox(){
	studentsComboBox->clear();
	studentsComboBox->insertItem("");
	for(StudentsYear* sty=gt.rules.yearsList.first(); sty; sty=gt.rules.yearsList.next()){
		studentsComboBox->insertItem(sty->name);
		for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
			studentsComboBox->insertItem(stg->name);
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next()){
				studentsComboBox->insertItem(sts->name);
			}
		}
	}
}

void AddConstraintActivitiesPreferredTimesForm::updateSubjectsComboBox(){
	subjectsComboBox->clear();
	subjectsComboBox->insertItem("");
	for(Subject* s=gt.rules.subjectsList.first(); s; s=gt.rules.subjectsList.next())
		subjectsComboBox->insertItem(s->name);
}

void AddConstraintActivitiesPreferredTimesForm::updateSubjectTagsComboBox(){
	subjectTagsComboBox->clear();
	subjectTagsComboBox->insertItem("");
	for(SubjectTag* s=gt.rules.subjectTagsList.first(); s; s=gt.rules.subjectTagsList.next())
		subjectTagsComboBox->insertItem(s->name);
}

void AddConstraintActivitiesPreferredTimesForm::addConstraint()
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

	QString teacher=teachersComboBox->currentText();
	if(teacher!="")
		assert(gt.rules.searchTeacher(teacher)>=0);

	QString students=studentsComboBox->currentText();
	if(students!="")
		assert(gt.rules.searchStudentsSet(students)!=NULL);

	QString subject=subjectsComboBox->currentText();
	if(subject!="")
		assert(gt.rules.searchSubject(subject)>=0);
		
	QString subjectTag=subjectTagsComboBox->currentText();
	if(subjectTag!="")
		assert(gt.rules.searchSubjectTag(subjectTag)>=0);
		
	if(teacher=="" && students=="" && subject=="" && subjectTag==""){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Please be careful - you are considering all the activities"
			"\n(no teacher, students, subject or subject tag specified)"));
	}

	int days[MAX_N_CONSTRAINT_ACTIVITIES_PREFERRED_TIMES];
	int hours[MAX_N_CONSTRAINT_ACTIVITIES_PREFERRED_TIMES];
	int n=0;
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++)
			if(preferredTimesTable->text(i+1, j+1)==YES){
				if(n>=MAX_N_CONSTRAINT_ACTIVITIES_PREFERRED_TIMES){
					QString s=QObject::tr("Not enough slots (too many \"Yes\" values).");
					s+="\n";
					s+=QObject::tr("Please increase the variable MAX_N_CONSTRAINT_ACTIVITIES_PREFERRED_TIMES");
					s+="\n";
					s+=QObject::tr("Currently, it is %1").arg(MAX_N_CONSTRAINT_ACTIVITIES_PREFERRED_TIMES);
					QMessageBox::critical(this, QObject::tr("FET information"), s);
					
					return;
				}
				
				days[n]=j;
				hours[n]=i;
				n++;
			}

	ctr=new ConstraintActivitiesPreferredTimes(weight, compulsory, teacher, students, subject, subjectTag, n, days, hours);

	bool tmp2=gt.rules.addTimeConstraint(ctr);
	if(tmp2){
		QString s=QObject::tr("Constraint added:");
		s+="\n";
		s+=ctr->getDetailedDescription(gt.rules);
		QMessageBox::information(this, QObject::tr("FET information"), s);
	}
	else{
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Constraint NOT added - duplicate?"));
		delete ctr;
	}
}

#undef YES
#undef NO
