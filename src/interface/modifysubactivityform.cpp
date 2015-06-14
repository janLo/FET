/***************************************************************************
                          modifysubactivityform.cpp  -  description
                             -------------------
    begin                : Feb 9, 2005
    copyright            : (C) 2005 by Lalescu Liviu
    email                : Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "modifysubactivityform.h"
#include "teacher.h"
#include "subject.h"
#include "studentsset.h"

#include <qlabel.h>
#include <qtabwidget.h>

#include <QDesktopWidget>

ModifySubactivityForm::ModifySubactivityForm(int id, int activityGroupId)
{
    setupUi(this);

    connect(subjectsComboBox, SIGNAL(activated(QString)), this /*ModifySubactivityForm_template*/, SLOT(subjectChanged(QString)));
    connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(okPushButton, SIGNAL(clicked()), this, SLOT(ok()));
    connect(clearTeacherPushButton, SIGNAL(clicked()), this, SLOT(clearTeachers()));
    connect(clearStudentsPushButton, SIGNAL(clicked()), this, SLOT(clearStudents()));
    connect(allTeachersListBox, SIGNAL(selected(QString)), this, SLOT(addTeacher()));
    connect(selectedTeachersListBox, SIGNAL(selected(QString)), this, SLOT(removeTeacher()));
    connect(allStudentsListBox, SIGNAL(selected(QString)), this, SLOT(addStudents()));
    connect(selectedStudentsListBox, SIGNAL(selected(QString)), this, SLOT(removeStudents()));
    connect(clearActivityTagPushButton, SIGNAL(clicked()), this, SLOT(clearActivityTags()));
    connect(allActivityTagsListBox, SIGNAL(selected(QString)), this, SLOT(addActivityTag()));
    connect(selectedActivityTagsListBox, SIGNAL(selected(QString)), this, SLOT(removeActivityTag()));
    connect(showYearsCheckBox, SIGNAL(toggled(bool)), this, SLOT(showYearsChanged()));
    connect(showGroupsCheckBox, SIGNAL(toggled(bool)), this, SLOT(showGroupsChanged()));
    connect(showSubgroupsCheckBox, SIGNAL(toggled(bool)), this, SLOT(showSubgroupsChanged()));


	//setWindowFlags(Qt::Window);
	/*setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	QDesktopWidget* desktop=QApplication::desktop();
	int xx=desktop->width()/2 - frameGeometry().width()/2;
	int yy=desktop->height()/2 - frameGeometry().height()/2;
	move(xx, yy);*/
	centerWidgetOnScreen(this);

	QSize tmp3=subjectsComboBox->minimumSizeHint();
	Q_UNUSED(tmp3);
	
	int splitIndex=0;
	int nSplit=0;
	bool found=false;
	
	this->_id=id;
	this->_activityGroupId=activityGroupId;
	for(int i=0; i<gt.rules.activitiesList.size(); i++){
		Activity* act=gt.rules.activitiesList[i];
		if(act->activityGroupId==this->_activityGroupId && act->id==this->_id){
			this->_activity=act;
			splitIndex++;
			nSplit++;
			found=true;
		}
		else if(this->_activityGroupId!=0 && act->activityGroupId!=0 && act->activityGroupId==this->_activityGroupId){
			if(!found)
				splitIndex++;
			nSplit++;
		}
	}
	
	//splitIndexGroupBox->setTitle(tr("Split index = %1").arg(splitIndex));
	//durationActiveGroupBox->setTitle(tr("Current subactivity has split index %1 out of %2").arg(splitIndex).arg(nSplit));
	crtTextLabel->setText(tr("Current subactivity has split index %1 out of %2").arg(splitIndex).arg(nSplit)+
	 "\n"+tr("The current data applies only to this subactivity"));

	/*splitSpinBox->setMinValue(nSplit);
	splitSpinBox->setMaxValue(nSplit);	
	splitSpinBox->setValue(nSplit);	*/
			
	this->_teachers=this->_activity->teachersNames;
	this->_subject = this->_activity->subjectName;
	this->_activityTags = this->_activity->activityTagsNames;
	this->_students=this->_activity->studentsNames;
	
	durationSpinBox->setValue(this->_activity->duration);
	activeCheckBox->setChecked(this->_activity->active);

	nStudentsSpinBox->setMinValue(-1);
	nStudentsSpinBox->setMaxValue(MAX_ROOM_CAPACITY);
	nStudentsSpinBox->setValue(-1);
				
	if(this->_activity->computeNTotalStudents==false)
		nStudentsSpinBox->setValue(this->_activity->nTotalStudents);
	
	updateStudentsListBox();
	updateTeachersListBox();
	updateSubjectsComboBox();
	updateActivityTagsListBox();

	selectedStudentsListBox->clear();
	for(QStringList::Iterator it=this->_students.begin(); it!=this->_students.end(); it++)
		selectedStudentsListBox->insertItem(*it);
	
	okPushButton->setDefault(true);
	okPushButton->setFocus();
}

ModifySubactivityForm::~ModifySubactivityForm()
{
}

void ModifySubactivityForm::updateTeachersListBox()
{
	allTeachersListBox->clear();
	for(int i=0; i<gt.rules.teachersList.size(); i++){
		Teacher* tch=gt.rules.teachersList[i];
		allTeachersListBox->insertItem(tch->name);
	}
		
	selectedTeachersListBox->clear();
	for(QStringList::Iterator it=this->_teachers.begin(); it!=this->_teachers.end(); it++)
		selectedTeachersListBox->insertItem(*it);
		
	subactivityChanged();
}

void ModifySubactivityForm::updateSubjectsComboBox()
{
	int i=0, j=-1;
	subjectsComboBox->clear();
	for(int k=0; k<gt.rules.subjectsList.size(); k++, i++){
		Subject* sbj=gt.rules.subjectsList[k];
		subjectsComboBox->insertItem(sbj->name);
		if(sbj->name==this->_subject)
			j=i;
	}
	assert(j!=-1);
	subjectsComboBox->setCurrentItem(j);

	subjectChanged("");
}

void ModifySubactivityForm::updateActivityTagsListBox()
{
/*	int i=0, j=-1;
	activityTagsComboBox->clear();
	activityTagsComboBox->insertItem("");
	if(this->_activityTag=="")
		j=i;
	i++;
	for(int k=0; k<gt.rules.activityTagsList.size(); k++, i++){
		ActivityTag* sbt=gt.rules.activityTagsList[k];
		activityTagsComboBox->insertItem(sbt->name);
		if(sbt->name==this->_activityTag)
			j=i;
	}
	assert(j!=-1);
	activityTagsComboBox->setCurrentItem(j);

	activityTagChanged("");*/
	
	allActivityTagsListBox->clear();
	for(int i=0; i<gt.rules.activityTagsList.size(); i++){
		ActivityTag* at=gt.rules.activityTagsList[i];
		allActivityTagsListBox->insertItem(at->name);
	}
		
	selectedActivityTagsListBox->clear();
	for(QStringList::Iterator it=this->_activityTags.begin(); it!=this->_activityTags.end(); it++)
		selectedActivityTagsListBox->insertItem(*it);
		
	subactivityChanged();
}

void ModifySubactivityForm::showYearsChanged()
{
	updateStudentsListBox();
}

void ModifySubactivityForm::showGroupsChanged()
{
	updateStudentsListBox();
}

void ModifySubactivityForm::showSubgroupsChanged()
{
	updateStudentsListBox();
}

void ModifySubactivityForm::updateStudentsListBox()
{
	const int INDENT=2;

	bool showYears=showYearsCheckBox->isChecked();
	bool showGroups=showGroupsCheckBox->isChecked();
	bool showSubgroups=showSubgroupsCheckBox->isChecked();

	allStudentsListBox->clear();
	canonicalStudentsSetsNames.clear();
	for(int i=0; i<gt.rules.yearsList.size(); i++){
		StudentsYear* sty=gt.rules.yearsList[i];
		if(showYears){
			allStudentsListBox->insertItem(sty->name);
			canonicalStudentsSetsNames.append(sty->name);
		}
		for(int j=0; j<sty->groupsList.size(); j++){
			StudentsGroup* stg=sty->groupsList[j];
			if(showGroups){
				QString begin=QString("");
				QString end=QString("");
				begin=QString(INDENT, ' ');
				if(LANGUAGE_STYLE_RIGHT_TO_LEFT==true)
					end=QString(INDENT, ' ');
				allStudentsListBox->insertItem(begin+stg->name+end);
				canonicalStudentsSetsNames.append(stg->name);
			}
			for(int k=0; k<stg->subgroupsList.size(); k++){
				StudentsSubgroup* sts=stg->subgroupsList[k];
				if(showSubgroups){
					QString begin=QString("");
					QString end=QString("");
					begin=QString(2*INDENT, ' ');
					if(LANGUAGE_STYLE_RIGHT_TO_LEFT==true)
						end=QString(2*INDENT, ' ');
					allStudentsListBox->insertItem(begin+sts->name+end);
					canonicalStudentsSetsNames.append(sts->name);
				}
			}
		}
	}

	/*allStudentsListBox->clear();
	for(int i=0; i<gt.rules.yearsList.size(); i++){
		StudentsYear* sty=gt.rules.yearsList[i];
		allStudentsListBox->insertItem(sty->name);
		for(int j=0; j<sty->groupsList.size(); j++){
			StudentsGroup* stg=sty->groupsList[j];
			allStudentsListBox->insertItem(stg->name);
			for(int k=0; k<stg->subgroupsList.size(); k++){
				StudentsSubgroup* sts=stg->subgroupsList[k];
				allStudentsListBox->insertItem(sts->name);
			}
		}
	}
	
	selectedStudentsListBox->clear();
	for(QStringList::Iterator it=this->_students.begin(); it!=this->_students.end(); it++)
		selectedStudentsListBox->insertItem(*it);*/

	subactivityChanged();
}

void ModifySubactivityForm::addTeacher()
{
	if(allTeachersListBox->currentItem()<0 || (uint)(allTeachersListBox->currentItem())>=allTeachersListBox->count())
		return;
	
	for(uint i=0; i<selectedTeachersListBox->count(); i++)
		if(selectedTeachersListBox->text(i)==allTeachersListBox->currentText())
			return;
			
	selectedTeachersListBox->insertItem(allTeachersListBox->currentText());

	subactivityChanged();
}

void ModifySubactivityForm::removeTeacher()
{
	if(selectedTeachersListBox->count()<=0 || selectedTeachersListBox->currentItem()<0 ||
	 (uint)(selectedTeachersListBox->currentItem())>=selectedTeachersListBox->count())
		return;
		
	selectedTeachersListBox->removeItem(selectedTeachersListBox->currentItem());

	subactivityChanged();
}

void ModifySubactivityForm::addActivityTag()
{
	if(allActivityTagsListBox->currentItem()<0 || (uint)(allActivityTagsListBox->currentItem())>=allActivityTagsListBox->count())
		return;
	
	for(uint i=0; i<selectedActivityTagsListBox->count(); i++)
		if(selectedActivityTagsListBox->text(i)==allActivityTagsListBox->currentText())
			return;
			
	selectedActivityTagsListBox->insertItem(allActivityTagsListBox->currentText());

	subactivityChanged();
}

void ModifySubactivityForm::removeActivityTag()
{
	if(selectedActivityTagsListBox->count()<=0 || selectedActivityTagsListBox->currentItem()<0 ||
	 (uint)(selectedActivityTagsListBox->currentItem())>=selectedActivityTagsListBox->count())
		return;
		
	selectedActivityTagsListBox->removeItem(selectedActivityTagsListBox->currentItem());

	subactivityChanged();
}

void ModifySubactivityForm::addStudents()
{
	if(allStudentsListBox->currentItem()<0 || (uint)(allStudentsListBox->currentItem())>=allStudentsListBox->count())
		return;
	
	assert(canonicalStudentsSetsNames.count()==(int)(allStudentsListBox->count()));
	QString sn=canonicalStudentsSetsNames.at(allStudentsListBox->currentItem());
	
	for(uint i=0; i<selectedStudentsListBox->count(); i++)
		if(selectedStudentsListBox->text(i)==sn)
			return;
			
	selectedStudentsListBox->insertItem(sn);
	
	subactivityChanged();
}

void ModifySubactivityForm::removeStudents()
{
	if(selectedStudentsListBox->count()<=0 || selectedStudentsListBox->currentItem()<0 ||
	 (uint)(selectedStudentsListBox->currentItem())>=selectedStudentsListBox->count())
		return;
		
	selectedStudentsListBox->removeItem(selectedStudentsListBox->currentItem());

	subactivityChanged();
}

void ModifySubactivityForm::subjectChanged(const QString& dummy)
{
	Q_UNUSED(dummy);
	//if(dummy=="")
	//	;

	subactivityChanged();
}

void ModifySubactivityForm::subactivityChanged()
{
/*	QString s;
	//s+=tr("Current activity:");
	//s+="\n";
	
	if(selectedTeachersListBox->count()==0){
		if(splitSpinBox->value()==1)
			s+=tr("No teachers for this activity\n");
		else
			s+=tr("No teachers for these activities\n");
	}
	else
		for(uint i=0; i<selectedTeachersListBox->count(); i++){
			s+=tr("Teacher=%1").arg(selectedTeachersListBox->text(i));
			s+="\n";
		}

	s+=tr("Subject=%1").arg(subjectsComboBox->currentText());
	s+="\n";
	if(activityTagsComboBox->currentText()!=""){
		s+=tr("Activity tag=%1").arg(activityTagsComboBox->currentText());
		s+="\n";
	}
	if(selectedStudentsListBox->count()==0){
		if(splitSpinBox->value()==1)
			s+=tr("No students for this activity\n");
		else
			s+=tr("No students for these activities\n");
	}
	else
		for(uint i=0; i<selectedStudentsListBox->count(); i++){
			s+=tr("Students=%1").arg(selectedStudentsListBox->text(i));
			s+="\n";
		}
	
	if(nStudentsSpinBox->value()==-1){
		s+=tr("Number of students: computed from corresponding students sets");
		s+="\n";	
	}
	else{
		s+=tr("Number of students=%1").arg(nStudentsSpinBox->value());
		s+="\n";	
	}

	if(splitSpinBox->value()==1){
		s+=tr("Duration=%1").arg(dur(0)->value());
		s+="\n";
		
		if(activ(0)->isChecked()){
			s+=tr("Active activity");
			s+="\n";
		}
		else{
			s+=tr("Non-active activity");
			s+="\n";
		}
	}
	else{
		s+=tr("This larger activity is split into %1 smaller activities per week").arg(splitSpinBox->value());
		s+="\n";
		s+="\n";

		for(int i=0; i<splitSpinBox->value(); i++){
			s+=tr("Component %1:").arg(i+1);
			s+="\n";
			s+=tr("Duration=%1").arg(dur(i)->value());
			s+="\n";
			if(activ(i)->isChecked()){
				s+=tr("Active activity");
				s+="\n";
			}
			else{
				s+=tr("Non-active activity");
				s+="\n";
			}
			s+="\n";
		}
	}

	currentActivityTextEdit->setText(s);
	*/
}

void ModifySubactivityForm::cancel()
{
	this->reject();
}

void ModifySubactivityForm::ok()
{
	//teachers
	QStringList teachers_names;
	if(selectedTeachersListBox->count()<=0){
		int t=QMessageBox::question(this, tr("FET question"),
		 tr("Do you really want to have the subactivity without teacher(s)?"),
		 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		if(t==QMessageBox::No)
			return;
	}
	/*else if(selectedTeachersListBox->count()>(uint)(MAX_TEACHERS_PER_ACTIVITY)){
		QMessageBox::warning(this, tr("FET information"),
			tr("Too many teachers for an activity. The current maximum is %1.\n"
			"If you really need more teachers per activity, please talk to the author").
			arg(MAX_TEACHERS_PER_ACTIVITY));
		return;
	}*/
	else{
		for(uint i=0; i<selectedTeachersListBox->count(); i++){
			assert(gt.rules.searchTeacher(selectedTeachersListBox->text(i))>=0);
			teachers_names.append(selectedTeachersListBox->text(i));
		}
	}

	//subject
	QString subject_name=subjectsComboBox->currentText();
	int subject_index=gt.rules.searchSubject(subject_name);
	if(subject_index<0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid subject"));
		return;
	}

	//activity tag
	QStringList activity_tags_names;
	/*
	int activity_tag_index=gt.rules.searchActivityTag(activity_tag_name);
	if(activity_tag_index<0 && activity_tag_name!=""){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid activity tag"));
		return;
	}*/
	for(uint i=0; i<selectedActivityTagsListBox->count(); i++){
		assert(gt.rules.searchActivityTag(selectedActivityTagsListBox->text(i))>=0);
		activity_tags_names.append(selectedActivityTagsListBox->text(i));
	}

	//students
	QStringList students_names;
	if(selectedStudentsListBox->count()<=0){
		int t=QMessageBox::question(this, tr("FET question"),
		 tr("Do you really want to have the subactivity without student set(s)?"),
		  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		if(t==QMessageBox::No)
			return;
	}
	else{
		for(uint i=0; i<selectedStudentsListBox->count(); i++){
			assert(gt.rules.searchStudentsSet(selectedStudentsListBox->text(i))!=NULL);
			students_names.append(selectedStudentsListBox->text(i));
		}
	}

	int total_number_of_students=0;
	for(QStringList::Iterator it=students_names.begin(); it!=students_names.end(); it++){
		StudentsSet* ss=gt.rules.searchStudentsSet(*it);
		total_number_of_students+=ss->numberOfStudents;
	}

	if(nStudentsSpinBox->value()==-1){
		gt.rules.modifySubactivity(this->_id, this->_activityGroupId, teachers_names, subject_name,
		 activity_tags_names,students_names, durationSpinBox->value(), activeCheckBox->isChecked(),
		 (nStudentsSpinBox->value()==-1), total_number_of_students);
	}
	else{
		gt.rules.modifySubactivity(this->_id, this->_activityGroupId, teachers_names, subject_name,
		 activity_tags_names,students_names, durationSpinBox->value(), activeCheckBox->isChecked(),
		 (nStudentsSpinBox->value()==-1), nStudentsSpinBox->value());
	}
	
	this->accept();
}

void ModifySubactivityForm::clearTeachers()
{
	selectedTeachersListBox->clear();
	subactivityChanged();
}

void ModifySubactivityForm::clearStudents()
{
	selectedStudentsListBox->clear();
	subactivityChanged();
}

void ModifySubactivityForm::clearActivityTags()
{
	selectedActivityTagsListBox->clear();
	subactivityChanged();
}