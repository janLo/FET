/***************************************************************************
                          addactivityform.cpp  -  description
                             -------------------
    begin                : Wed Apr 23 2003
    copyright            : (C) 2003 by Lalescu Liviu
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

#include "addactivityform.h"
#include "teacher.h"
#include "subject.h"
#include "studentsset.h"

#include <qlabel.h>
#include <qtabwidget.h>

#define subTab(i)	subactivitiesTabWidget->page(i)
#define prefDay(i)	(i==0?preferredDay1ComboBox:				\
			(i==1?preferredDay2ComboBox:				\
			(i==2?preferredDay3ComboBox:				\
			(i==3?preferredDay4ComboBox:				\
			(i==4?preferredDay5ComboBox:				\
			(i==5?preferredDay6ComboBox:				\
			(i==6?preferredDay7ComboBox:				\
			(preferredDay8ComboBox))))))))
#define prefHour(i)	(i==0?preferredHour1ComboBox:				\
			(i==1?preferredHour2ComboBox:				\
			(i==2?preferredHour3ComboBox:				\
			(i==3?preferredHour4ComboBox:				\
			(i==4?preferredHour5ComboBox:				\
			(i==5?preferredHour6ComboBox:				\
			(i==6?preferredHour7ComboBox:				\
			(preferredHour8ComboBox))))))))
#define par(i)		(i==0?parity1CheckBox:					\
			(i==1?parity2CheckBox:					\
			(i==2?parity3CheckBox:					\
			(i==3?parity4CheckBox:					\
			(i==4?parity5CheckBox:					\
			(i==5?parity6CheckBox:					\
			(i==6?parity7CheckBox:					\
			(parity8CheckBox))))))))
#define dur(i)		(i==0?duration1SpinBox:					\
			(i==1?duration2SpinBox:					\
			(i==2?duration3SpinBox:					\
			(i==3?duration4SpinBox:					\
			(i==4?duration5SpinBox:					\
			(i==5?duration6SpinBox:					\
			(i==6?duration7SpinBox:					\
			(duration1SpinBox))))))))

AddActivityForm::AddActivityForm()
{
	updatePreferredDaysComboBox();
	updatePreferredHoursComboBox();
	updateStudentsComboBox();
	updateTeachersComboBox();
	updateSubjectsComboBox();

	minDayDistanceSpinBox->setMaxValue(gt.rules.nDaysPerWeek-1);
	minDayDistanceSpinBox->setMinValue(0);
	minDayDistanceSpinBox->setValue(0);

	int nSplit=splitSpinBox->value();
	for(int i=0; i<8; i++)
		if(i<nSplit)
			subTab(i)->setEnabled(true);
		else
			subTab(i)->setDisabled(true);
}

AddActivityForm::~AddActivityForm()
{
}

void AddActivityForm::updateTeachersComboBox()
{
	teachers1ComboBox->clear();
	for(Teacher* tch=gt.rules.teachersList.first(); tch; tch=gt.rules.teachersList.next())
		teachers1ComboBox->insertItem(tch->name);
	teachers1ComboBox->setCurrentItem(0);

	teachers2ComboBox->clear();
	teachers2ComboBox->insertItem("");
	for(Teacher* tch=gt.rules.teachersList.first(); tch; tch=gt.rules.teachersList.next())
		teachers2ComboBox->insertItem(tch->name);
	teachers2ComboBox->setCurrentItem(0);

	teachers3ComboBox->clear();
	teachers3ComboBox->insertItem("");
	for(Teacher* tch=gt.rules.teachersList.first(); tch; tch=gt.rules.teachersList.next())
		teachers3ComboBox->insertItem(tch->name);
	teachers3ComboBox->setCurrentItem(0);

	teacherChanged("");
}

void AddActivityForm::updateSubjectsComboBox()
{
	subjectsComboBox->clear();
	for(Subject* sbj=gt.rules.subjectsList.first(); sbj; sbj=gt.rules.subjectsList.next())
		subjectsComboBox->insertItem(sbj->name);

	subjectChanged(subjectsComboBox->currentText());
}

void AddActivityForm::showYearsChanged()
{
	updateStudentsComboBox();
}

void AddActivityForm::showGroupsChanged()
{
	updateStudentsComboBox();
}

void AddActivityForm::showSubgroupsChanged()
{
	updateStudentsComboBox();
}

void AddActivityForm::updateStudentsComboBox()
{
	bool showYears=yearsCheckBox->isChecked();	
	bool showGroups=groupsCheckBox->isChecked();	
	bool showSubgroups=subgroupsCheckBox->isChecked();	

	students1ComboBox->clear();
	for(StudentsYear* sty=gt.rules.yearsList.first(); sty; sty=gt.rules.yearsList.next()){
		if(showYears)
			students1ComboBox->insertItem(sty->name);
		for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
			if(showGroups)
				students1ComboBox->insertItem(stg->name);
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next())
				if(showSubgroups)
					students1ComboBox->insertItem(sts->name);
		}
	}
	students1ComboBox->setCurrentItem(0);

	students2ComboBox->clear();
	students2ComboBox->insertItem("");
	for(StudentsYear* sty=gt.rules.yearsList.first(); sty; sty=gt.rules.yearsList.next()){
		if(showYears)
			students2ComboBox->insertItem(sty->name);
		for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
			if(showGroups)
				students2ComboBox->insertItem(stg->name);
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next())
				if(showSubgroups)
					students2ComboBox->insertItem(sts->name);
		}
	}
	students2ComboBox->setCurrentItem(0);

	students3ComboBox->clear();
	students3ComboBox->insertItem("");
	for(StudentsYear* sty=gt.rules.yearsList.first(); sty; sty=gt.rules.yearsList.next()){
		if(showYears)
			students3ComboBox->insertItem(sty->name);
		for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
			if(showGroups)
				students3ComboBox->insertItem(stg->name);
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next())
				if(showSubgroups)
					students3ComboBox->insertItem(sts->name);
		}
	}
	students3ComboBox->setCurrentItem(0);

	students4ComboBox->clear();
	students4ComboBox->insertItem("");
	for(StudentsYear* sty=gt.rules.yearsList.first(); sty; sty=gt.rules.yearsList.next()){
		if(showYears)
			students4ComboBox->insertItem(sty->name);
		for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
			if(showGroups)
				students4ComboBox->insertItem(stg->name);
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next())
				if(showSubgroups)
					students4ComboBox->insertItem(sts->name);
		}
	}
	students4ComboBox->setCurrentItem(0);

	studentsChanged("");
}

void AddActivityForm::updatePreferredDaysComboBox()
{
	for(int j=0; j<8; j++){
		prefDay(j)->clear();
		prefDay(j)->insertItem(QObject::tr("Any"));
		for(int i=0; i<gt.rules.nDaysPerWeek; i++)
			prefDay(j)->insertItem(gt.rules.daysOfTheWeek[i]);
	}
}

void AddActivityForm::updatePreferredHoursComboBox()
{
	for(int j=0; j<8; j++){
		prefHour(j)->clear();
		prefHour(j)->insertItem(QObject::tr("Any"));
		for(int i=0; i<gt.rules.nHoursPerDay; i++)
			prefHour(j)->insertItem(gt.rules.hoursOfTheDay[i]);
	}
}

void AddActivityForm::teacherChanged(const QString& dummy)
{
	if(dummy=="")
		;

	activityChanged();
}

void AddActivityForm::subjectChanged(const QString& dummy)
{
	if(dummy=="")
		;

	activityChanged();
}

void AddActivityForm::studentsChanged(const QString& dummy)
{
	if(dummy=="")
		;

	activityChanged();
}

void AddActivityForm::splitChanged()
{
	int nSplit=splitSpinBox->value();

	minDayDistanceTextLabel->setEnabled(nSplit>=2);
	minDayDistanceSpinBox->setEnabled(nSplit>=2);

	for(int i=0; i<8; i++)
		if(i<nSplit)
			subTab(i)->setEnabled(true);
		else
			subTab(i)->setDisabled(true);

	activityChanged();
}

void AddActivityForm::activityChanged()
{
	QString s;
	s+=QObject::tr("Current activity:");s+="\n";
	s+=QObject::tr(QString("Teacher=%1").arg(teachers1ComboBox->currentText()));s+="\n";
	if(teachers2ComboBox->currentItem()!=0){
		s+=QObject::tr(QString("Teacher=%1").arg(teachers2ComboBox->currentText()));
		s+="\n";
	}
	if(teachers3ComboBox->currentItem()!=0){
		s+=QObject::tr(QString("Teacher=%1").arg(teachers3ComboBox->currentText()));
		s+="\n";
	}
	s+=QObject::tr(QString("Subject=%1").arg(subjectsComboBox->currentText()));s+="\n";
	s+=QObject::tr(QString("Students=%1").arg(students1ComboBox->currentText()));s+="\n";
	if(students2ComboBox->currentItem()!=0){
		s+=QObject::tr(QString("Students=%1").arg(students2ComboBox->currentText()));
		s+="\n";
	}
	if(students3ComboBox->currentItem()!=0){
		s+=QObject::tr(QString("Students=%1").arg(students3ComboBox->currentText()));
		s+="\n";
	}
	if(students4ComboBox->currentItem()!=0){
		s+=QObject::tr(QString("Students=%1").arg(students4ComboBox->currentText()));
		s+="\n";
	}

	if(splitSpinBox->value()==1){
		s+=QObject::tr(QString("Duration=%1").arg(dur(0)->value()));
		s+="\n";
		if(par(0)->isChecked()){
			s+=QObject::tr("Bi-weekly activity");
			s+="\n";
		}
		if(prefDay(0)->currentItem()>0){
			s+=QObject::tr("Preferred day=%1").arg(prefDay(0)->currentText());
			s+="\n";
		}
		if(prefHour(0)->currentItem()>0){
			s+=QObject::tr("Preferred hour=%1").arg(prefHour(0)->currentText());
			s+="\n";
		}
	}
	else{
		s+=QObject::tr(QString("This activity will be split into %1 lessons per week").arg(splitSpinBox->value()));
		s+="\n";
		if(minDayDistanceSpinBox->value()>0){
			s+=QObject::tr("The distance between any pair of subactivities must be at least %1 days").arg(minDayDistanceSpinBox->value());
			s+="\n";
		}
		s+="\n";

		for(int i=0; i<splitSpinBox->value(); i++){
			s+=QObject::tr("Componenent %1:").arg(i+1);
			s+="\n";
			s+=QObject::tr(QString("Duration=%1").arg(dur(i)->value()));
			s+="\n";
			if(par(i)->isChecked()){
				s+=QObject::tr("Bi-weekly activity");
				s+="\n";
			}
			if(prefDay(i)->currentItem()>0){
				s+=QObject::tr("Preferred day=%1").arg(prefDay(i)->currentText());
				s+="\n";
			}
			if(prefHour(i)->currentItem()>0){
				s+=QObject::tr("Preferred hour=%1").arg(prefHour(i)->currentText());
				s+="\n";
			}
			s+="\n";
		}
	}

	currentActivityTextEdit->setText(s);
}

void AddActivityForm::addActivity()
{
	//teachers
	QStringList teachers_names;
	if(teachers1ComboBox->currentItem()<0 || teachers1ComboBox->currentText()==""){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid teacher (1)"));
		return;
	}
	else{
		assert(gt.rules.searchTeacher(teachers1ComboBox->currentText())>=0);
		teachers_names.append(teachers1ComboBox->currentText());
	}
	if(teachers2ComboBox->currentItem()>=1){
		assert(gt.rules.searchTeacher(teachers2ComboBox->currentText())>=0);
		teachers_names.append(teachers2ComboBox->currentText());
	}
	if(teachers3ComboBox->currentItem()>=1){
		assert(gt.rules.searchTeacher(teachers3ComboBox->currentText())>=0);
		teachers_names.append(teachers3ComboBox->currentText());
	}

	//subject
	QString subject_name=subjectsComboBox->currentText();
	int subject_index=gt.rules.searchSubject(subject_name);
	if(subject_index<0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid subject"));
		return;
	}

	//students
	QStringList students_names;
	if(students1ComboBox->currentItem()<0 || students1ComboBox->currentText()==""){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid students set (1)"));
		return;
	}
	else{
		assert(gt.rules.searchStudentsSet(students1ComboBox->currentText())!=NULL);
		students_names.append(students1ComboBox->currentText());
	}
	if(students2ComboBox->currentItem()>=1){
		assert(gt.rules.searchStudentsSet(students2ComboBox->currentText())!=NULL);
		students_names.append(students2ComboBox->currentText());
	}
	if(students3ComboBox->currentItem()>=1){
		assert(gt.rules.searchStudentsSet(students3ComboBox->currentText())!=NULL);
		students_names.append(students3ComboBox->currentText());
	}
	if(students4ComboBox->currentItem()>=1){
		assert(gt.rules.searchStudentsSet(students4ComboBox->currentText())!=NULL);
		students_names.append(students4ComboBox->currentText());
	}

	if(splitSpinBox->value()==1){ //indivisible activity
		int duration=duration1SpinBox->value();
		if(duration<0){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Invalid duration"));
			return;
		}
		int parity=PARITY_WEEKLY;
		if(parity1CheckBox->isChecked())
			parity=PARITY_BIWEEKLY;

		int preferred_day=preferredDay1ComboBox->currentItem()-1;
		if(preferred_day<-1 || preferred_day>=gt.rules.nDaysPerWeek){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Invalid preferred day"));
			return;
		}
		int preferred_hour=preferredHour1ComboBox->currentItem()-1;
		if(preferred_hour<-1 || preferred_hour>=gt.rules.nHoursPerDay){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Invalid preferred hour"));
			return;
		}

		int activityid=0; //We set the id of this newly added activity = (the largest existing id + 1)
		for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next())
			if(act->id > activityid)
				activityid = act->id;
		activityid++;
		Activity a(gt.rules, activityid, 0, teachers_names, subject_name, students_names,
			duration, duration, parity);

		bool already_existing=false;
		for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next())
			if((*act)==a)
				already_existing=true;

		if(already_existing){
			int t=QMessageBox::question(this, QObject::tr("FET question"), 
				QObject::tr("This activity already exists. Insert it again?"),
				QObject::tr("Yes"),QObject::tr("No"));
			assert(t==0 || t==1);
			if(t==1) //no pressed
				return;
		}

		bool tmp=gt.rules.addSimpleActivity(activityid, 0, teachers_names, subject_name,
			students_names,	duration, duration, parity, preferred_day, preferred_hour);
		if(tmp)
			QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Activity added"));
		else
			QMessageBox::critical(this, QObject::tr("FET information"), QObject::tr("Activity NOT added - please report error"));
	}
	else{ //split activity
		int totalduration;
		int durations[MAX_CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES];
		int parities[MAX_CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES];
		int preferred_days[MAX_CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES];
		int preferred_hours[MAX_CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES];
		int nsplit=splitSpinBox->value();

		totalduration=0;
		for(int i=0; i<nsplit; i++){
			durations[i]=dur(i)->value();
			parities[i]=PARITY_WEEKLY;
			if(par(i)->isChecked())
				parities[i]=PARITY_BIWEEKLY;
			preferred_days[i]=prefDay(i)->currentItem()-1;
			preferred_hours[i]=prefHour(i)->currentItem()-1;

			totalduration+=durations[i];
		}

		//the group id of this split activity and the id of the first partial activity
		//it is the maximum already existing id + 1
		int firstactivityid=0;
		for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next())
			if(act->id > firstactivityid)
				firstactivityid = act->id;
		firstactivityid++;

		int minD=minDayDistanceSpinBox->value();
		bool tmp=gt.rules.addSplitActivity(firstactivityid, firstactivityid,
			teachers_names, subject_name, students_names,
			nsplit, totalduration, durations,
			parities, minD, preferred_days, preferred_hours);
		if(tmp)
			QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Split activity added"));
		else
			QMessageBox::critical(this, QObject::tr("FET information"), QObject::tr("Split activity NOT added - error???"));
	}
}

#undef prefDay
#undef prefHour
#undef subTab
