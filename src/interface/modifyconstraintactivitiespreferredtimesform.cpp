/***************************************************************************
                          modifyconstraintactivitiespreferredtimesform.cpp  -  description
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

#include "modifyconstraintactivitiespreferredtimesform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

#define YES	(QObject::tr("Yes"))
#define NO	(QObject::tr("No"))

ModifyConstraintActivitiesPreferredTimesForm::ModifyConstraintActivitiesPreferredTimesForm(ConstraintActivitiesPreferredTimes* ctr)
{
	this->_ctr=ctr;

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

	bool currentMatrix[MAX_HOURS_PER_DAY][MAX_DAYS_PER_WEEK];
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++)
			currentMatrix[i][j]=false;
	for(int k=0; k<ctr->nPreferredTimes; k++){
		if(ctr->hours[k]==-1 || ctr->days[k]==-1)
			assert(0);
		int i=ctr->hours[k];
		int j=ctr->days[k];
		currentMatrix[i][j]=true;
	}

	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++)
			if(!currentMatrix[i][j])
				preferredTimesTable->setText(i+1, j+1, NO);
			else
				preferredTimesTable->setText(i+1, j+1, YES);
				
	compulsoryCheckBox->setChecked(ctr->compulsory);
	weightLineEdit->setText(QString::number(ctr->weight));
}

ModifyConstraintActivitiesPreferredTimesForm::~ModifyConstraintActivitiesPreferredTimesForm()
{
}

void ModifyConstraintActivitiesPreferredTimesForm::tableClicked(int row, int col, int button, const QPoint& mousePos)
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

void ModifyConstraintActivitiesPreferredTimesForm::updateTeachersComboBox(){
	int i=0, j=-1;
	teachersComboBox->clear();
	teachersComboBox->insertItem("");
	if(this->_ctr->teacherName=="")
		j=i;
	i++;
	for(Teacher* t=gt.rules.teachersList.first(); t; t=gt.rules.teachersList.next()){
		teachersComboBox->insertItem(t->name);
		if(t->name==this->_ctr->teacherName)
			j=i;
		i++;
	}
	assert(j>=0);
	teachersComboBox->setCurrentItem(j);
}

void ModifyConstraintActivitiesPreferredTimesForm::updateStudentsComboBox(){
	int i=0, j=-1;
	studentsComboBox->clear();
	studentsComboBox->insertItem("");
	if(this->_ctr->studentsName=="")
		j=i;
	i++;
	for(StudentsYear* sty=gt.rules.yearsList.first(); sty; sty=gt.rules.yearsList.next()){
		studentsComboBox->insertItem(sty->name);
		if(sty->name==this->_ctr->studentsName)
			j=i;
		i++;
		for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
			studentsComboBox->insertItem(stg->name);
			if(stg->name==this->_ctr->studentsName)
				j=i;
			i++;
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next()){
				studentsComboBox->insertItem(sts->name);
				if(sts->name==this->_ctr->studentsName)
					j=i;
				i++;
			}
		}
	}
	assert(j>=0);
	studentsComboBox->setCurrentItem(j);
}

void ModifyConstraintActivitiesPreferredTimesForm::updateSubjectsComboBox(){
	int i=0, j=-1;
	subjectsComboBox->clear();
	subjectsComboBox->insertItem("");
	if(this->_ctr->subjectName=="")
		j=i;
	i++;
	for(Subject* s=gt.rules.subjectsList.first(); s; s=gt.rules.subjectsList.next()){
		subjectsComboBox->insertItem(s->name);
		if(s->name==this->_ctr->subjectName)
			j=i;
		i++;
	}
	assert(j>=0);
	subjectsComboBox->setCurrentItem(j);
}

void ModifyConstraintActivitiesPreferredTimesForm::updateSubjectTagsComboBox(){
	int i=0, j=-1;
	subjectTagsComboBox->clear();
	subjectTagsComboBox->insertItem("");
	if(this->_ctr->subjectTagName=="")
		j=i;
	i++;
	for(SubjectTag* s=gt.rules.subjectTagsList.first(); s; s=gt.rules.subjectTagsList.next()){
		subjectTagsComboBox->insertItem(s->name);
		if(s->name==this->_ctr->subjectTagName)
			j=i;
		i++;
	}
	assert(j>=0);
	subjectTagsComboBox->setCurrentItem(j);
}

void ModifyConstraintActivitiesPreferredTimesForm::ok()
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
		
	if(teacher=="" && students=="" && subject=="" && subjectTag=="")
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Please be careful - you are considering all the activities"
			"\n(no teacher, students, subject or subject tag specified)"));

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

	this->_ctr->weight=weight;
	this->_ctr->compulsory=compulsory;
	this->_ctr->teacherName=teacher;
	this->_ctr->studentsName=students;
	this->_ctr->subjectName=subject;
	this->_ctr->nPreferredTimes=n;
	for(int i=0; i<n; i++){
		this->_ctr->days[i]=days[i];
		this->_ctr->hours[i]=hours[i];
	}

	gt.rules.internalStructureComputed=false;
	
	this->close();
}

void ModifyConstraintActivitiesPreferredTimesForm::cancel()
{
	this->close();
}

#undef YES
#undef NO
