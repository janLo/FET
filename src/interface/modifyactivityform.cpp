/***************************************************************************
                          modifyactivityform.cpp  -  description
                             -------------------
    begin                : Feb 9, 2005
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

#include "modifyactivityform.h"
#include "teacher.h"
#include "subject.h"
#include "studentsset.h"

#include <qlabel.h>
#include <qtabwidget.h>

#define subTab(i)	subactivitiesTabWidget->page(i)
#define par(i)		(i==0?parity1CheckBox:			\
			(i==1?parity2CheckBox:					\
			(i==2?parity3CheckBox:					\
			(i==3?parity4CheckBox:					\
			(i==4?parity5CheckBox:					\
			(i==5?parity6CheckBox:					\
			(i==6?parity7CheckBox:					\
			(parity8CheckBox))))))))
#define dur(i)		(i==0?duration1SpinBox:			\
			(i==1?duration2SpinBox:					\
			(i==2?duration3SpinBox:					\
			(i==3?duration4SpinBox:					\
			(i==4?duration5SpinBox:					\
			(i==5?duration6SpinBox:					\
			(i==6?duration7SpinBox:					\
			(duration1SpinBox))))))))

ModifyActivityForm::ModifyActivityForm(int id, int activityGroupId)
{
	this->_id=id;
	this->_activityGroupId=activityGroupId;
	for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next())
		if(act->activityGroupId==this->_activityGroupId && act->id==this->_id)
			this->_activity=act;
			
	QStringList::Iterator it = this->_activity->teachersNames.begin();
	assert(it != this->_activity->teachersNames.end());
	this->_teacher1=*it;
	if(it!=this->_activity->teachersNames.end())
		it++;
	if(it!=this->_activity->teachersNames.end())
		this->_teacher2=*it;
	else
		this->_teacher2="";
	if(it!=this->_activity->teachersNames.end())
		it++;
	if(it!=this->_activity->teachersNames.end())
		this->_teacher3=*it;
	else
		this->_teacher3="";
		
	this->_subject = this->_activity->subjectName;
	this->_subjectTag = this->_activity->subjectTagName;

	it = this->_activity->studentsNames.begin();
	assert(it != this->_activity->studentsNames.end());
	this->_students1=*it;
	if(it!=this->_activity->studentsNames.end())
		it++;
	if(it!=this->_activity->studentsNames.end())
		this->_students2=*it;
	else
		this->_students2="";
	if(it!=this->_activity->studentsNames.end())
		it++;
	if(it!=this->_activity->studentsNames.end())
		this->_students3=*it;
	else
		this->_students3="";
	if(it!=this->_activity->studentsNames.end())
		it++;
	if(it!=this->_activity->studentsNames.end())
		this->_students4=*it;
	else
		this->_students4="";
	
	int nSplit;
	
	if(this->_activityGroupId!=0){
		nSplit=0;
		for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next())
			if(act->activityGroupId==this->_activityGroupId){
				par(nSplit)->setChecked(act->parity==PARITY_BIWEEKLY);
				dur(nSplit)->setValue(act->duration);
				nSplit++;
			}
	}
	else{
		nSplit=1;
		par(0)->setChecked(this->_activity->parity==PARITY_BIWEEKLY);
		dur(0)->setValue(this->_activity->duration);
	}

	splitSpinBox->setMinValue(nSplit);
	splitSpinBox->setMaxValue(nSplit);	
	splitSpinBox->setValue(nSplit);	
	
	updateStudentsComboBox();
	updateTeachersComboBox();
	updateSubjectsComboBox();
	updateSubjectTagsComboBox();

	for(int i=0; i<8; i++)
		if(i<nSplit)
			subTab(i)->setEnabled(true);
		else
			subTab(i)->setDisabled(true);
}

ModifyActivityForm::~ModifyActivityForm()
{
}

void ModifyActivityForm::updateTeachersComboBox()
{
	int i, j;

	i=0;
	j=-1;
	teachers1ComboBox->clear();
	for(Teacher* tch=gt.rules.teachersList.first(); tch; tch=gt.rules.teachersList.next(), i++){
		teachers1ComboBox->insertItem(tch->name);
		if(tch->name==this->_teacher1)
			j=i;
	}
	assert(j!=-1);
	teachers1ComboBox->setCurrentItem(j);

	i=1;
	j=-1;
	teachers2ComboBox->clear();
	teachers2ComboBox->insertItem("");
	if(this->_teacher2=="")
		j=0;
	for(Teacher* tch=gt.rules.teachersList.first(); tch; tch=gt.rules.teachersList.next(), i++){
		teachers2ComboBox->insertItem(tch->name);
		if(tch->name==this->_teacher2)
			j=i;
	}
	assert(j!=-1);
	teachers2ComboBox->setCurrentItem(j);

	i=1;
	j=-1;
	teachers3ComboBox->clear();
	teachers3ComboBox->insertItem("");
	if(this->_teacher3=="")
		j=0;
	for(Teacher* tch=gt.rules.teachersList.first(); tch; tch=gt.rules.teachersList.next(), i++){
		teachers3ComboBox->insertItem(tch->name);
		if(tch->name==this->_teacher3)
			j=i;
	}
	assert(j!=-1);
	teachers3ComboBox->setCurrentItem(j);

	teacherChanged("");
}

void ModifyActivityForm::updateSubjectsComboBox()
{
	int i=0, j=-1;
	subjectsComboBox->clear();
	for(Subject* sbj=gt.rules.subjectsList.first(); sbj; sbj=gt.rules.subjectsList.next(), i++){
		subjectsComboBox->insertItem(sbj->name);
		if(sbj->name==this->_subject)
			j=i;
	}
	assert(j!=-1);
	subjectsComboBox->setCurrentItem(j);

	subjectChanged("");
}

void ModifyActivityForm::updateSubjectTagsComboBox()
{
	int i=0, j=-1;
	subjectTagsComboBox->clear();
	subjectTagsComboBox->insertItem("");
	if(this->_subjectTag=="")
		j=i;
	i++;
	for(SubjectTag* sbt=gt.rules.subjectTagsList.first(); sbt; sbt=gt.rules.subjectTagsList.next(), i++){
		subjectTagsComboBox->insertItem(sbt->name);
		if(sbt->name==this->_subjectTag)
			j=i;
	}
	assert(j!=-1);
	subjectTagsComboBox->setCurrentItem(j);

	subjectTagChanged("");
}

void ModifyActivityForm::showYearsChanged()
{
	updateStudentsComboBox();
}

void ModifyActivityForm::showGroupsChanged()
{
	updateStudentsComboBox();
}

void ModifyActivityForm::showSubgroupsChanged()
{
	updateStudentsComboBox();
}

void ModifyActivityForm::updateStudentsComboBox()
{
	int i, j;
	
	i=0;
	j=0;
	students1ComboBox->clear();
	for(StudentsYear* sty=gt.rules.yearsList.first(); sty; sty=gt.rules.yearsList.next()){
		students1ComboBox->insertItem(sty->name);
		i++;
		if(sty->name==this->_students1)
			j=i;
		for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
			students1ComboBox->insertItem(stg->name);
			i++;
			if(stg->name==this->_students1)
				j=i;
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next()){
				students1ComboBox->insertItem(sts->name);
				i++;
				if(sts->name==this->_students1)
					j=i;
			}
		}
	}
	assert(j!=0);
	students1ComboBox->setCurrentItem(j-1);

	i=0;
	j=0;
	students2ComboBox->clear();
	students2ComboBox->insertItem("");
	i++;
	if(this->_students2=="")
		j=i;
	for(StudentsYear* sty=gt.rules.yearsList.first(); sty; sty=gt.rules.yearsList.next()){
		students2ComboBox->insertItem(sty->name);
		i++;
		if(sty->name==this->_students2)
			j=i;
		for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
			students2ComboBox->insertItem(stg->name);
			i++;
			if(stg->name==this->_students2)
				j=i;
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next()){
				students2ComboBox->insertItem(sts->name);
				i++;
				if(sts->name==this->_students2)
					j=i;
			}
		}
	}
	assert(j!=0);
	students2ComboBox->setCurrentItem(j-1);

	i=0;
	j=0;
	students3ComboBox->clear();
	students3ComboBox->insertItem("");
	i++;
	if(this->_students3=="")
		j=i;
	for(StudentsYear* sty=gt.rules.yearsList.first(); sty; sty=gt.rules.yearsList.next()){
		students3ComboBox->insertItem(sty->name);
		i++;
		if(sty->name==this->_students3)
			j=i;
		for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
			students3ComboBox->insertItem(stg->name);
			i++;
			if(stg->name==this->_students3)
				j=i;
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next()){
				students3ComboBox->insertItem(sts->name);
				i++;
				if(sts->name==this->_students3)
					j=i;
			}
		}
	}
	assert(j!=0);
	students3ComboBox->setCurrentItem(j-1);

	i=0;
	j=0;
	students4ComboBox->clear();
	students4ComboBox->insertItem("");
	i++;
	if(this->_students4=="")
		j=i;
	for(StudentsYear* sty=gt.rules.yearsList.first(); sty; sty=gt.rules.yearsList.next()){
		students4ComboBox->insertItem(sty->name);
		i++;
		if(sty->name==this->_students4)
			j=i;
		for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
			students4ComboBox->insertItem(stg->name);
			i++;
			if(stg->name==this->_students4)
				j=i;
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next()){
				students4ComboBox->insertItem(sts->name);
				i++;
				if(sts->name==this->_students4)
					j=i;
			}
		}
	}
	assert(j!=0);
	students4ComboBox->setCurrentItem(j-1);

	studentsChanged("");
}

void ModifyActivityForm::teacherChanged(const QString& dummy)
{
	if(dummy=="")
		;

	activityChanged();
}

void ModifyActivityForm::subjectChanged(const QString& dummy)
{
	if(dummy=="")
		;

	activityChanged();
}

void ModifyActivityForm::subjectTagChanged(const QString& dummy)
{
	if(dummy=="")
		;

	activityChanged();
}

void ModifyActivityForm::studentsChanged(const QString& dummy)
{
	if(dummy=="")
		;

	activityChanged();
}

void ModifyActivityForm::activityChanged()
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
	if(subjectTagsComboBox->currentText()!=""){
		s+=QObject::tr(QString("Subject tag=%1").arg(subjectTagsComboBox->currentText()));
		s+="\n";
	}
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
	}
	else{
		s+=QObject::tr(QString("This activity will be split into %1 lessons per week").arg(splitSpinBox->value()));
		s+="\n";
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
			s+="\n";
		}
	}

	currentActivityTextEdit->setText(s);
}

void ModifyActivityForm::cancel()
{
	this->close();
}

void ModifyActivityForm::ok()
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

	//subject tag
	QString subject_tag_name=subjectTagsComboBox->currentText();
	int subject_tag_index=gt.rules.searchSubjectTag(subject_tag_name);
	if(subject_tag_index<0 && subject_tag_name!=""){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid subject tag"));
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
	
	int total_number_of_students=0;
	for(QStringList::Iterator it=students_names.begin(); it!=students_names.end(); it++){
		StudentsSet* ss=gt.rules.searchStudentsSet(*it);
		total_number_of_students+=ss->numberOfStudents;
	}

	int totalduration;
	int durations[8];
	int parities[8];
	int nsplit=splitSpinBox->value();

	totalduration=0;
	for(int i=0; i<nsplit; i++){
		durations[i]=dur(i)->value();
		parities[i]=PARITY_WEEKLY;
		if(par(i)->isChecked())
			parities[i]=PARITY_BIWEEKLY;

		totalduration+=durations[i];
	}

	gt.rules.modifyActivity(this->_id, this->_activityGroupId, teachers_names, subject_name,
	 subject_tag_name,students_names, total_number_of_students, nsplit, totalduration, durations, parities);
	
	this->close();
}

#undef prefDay
#undef prefHour
#undef subTab
