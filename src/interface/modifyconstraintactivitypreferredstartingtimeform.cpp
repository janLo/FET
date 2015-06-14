/***************************************************************************
                          modifyconstraintactivitypreferredstartingtimeform.cpp  -  description
                             -------------------
    begin                : Feb 11, 2005
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

#include "modifyconstraintactivitypreferredstartingtimeform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>

#include <QDesktopWidget>

#include "lockunlock.h"

ModifyConstraintActivityPreferredStartingTimeForm::ModifyConstraintActivityPreferredStartingTimeForm(ConstraintActivityPreferredStartingTime* ctr)
{
    setupUi(this);

//    connect(weightLineEdit, SIGNAL(textChanged(QString)), this /*ModifyConstraintActivityPreferredStartingTimeForm_template*/, SLOT(constraintChanged()));
    connect(okPushButton, SIGNAL(clicked()), this /*ModifyConstraintActivityPreferredStartingTimeForm_template*/, SLOT(ok()));
    connect(cancelPushButton, SIGNAL(clicked()), this /*ModifyConstraintActivityPreferredStartingTimeForm_template*/, SLOT(cancel()));
//    connect(activitiesComboBox, SIGNAL(activated(QString)), this /*ModifyConstraintActivityPreferredStartingTimeForm_template*/, SLOT(constraintChanged()));
//    connect(dayComboBox, SIGNAL(activated(QString)), this /*ModifyConstraintActivityPreferredStartingTimeForm_template*/, SLOT(constraintChanged()));
//    connect(startHourComboBox, SIGNAL(activated(QString)), this /*ModifyConstraintActivityPreferredStartingTimeForm_template*/, SLOT(constraintChanged()));
    connect(teachersComboBox, SIGNAL(activated(QString)), this /*ModifyConstraintActivityPreferredStartingTimeForm_template*/, SLOT(filterChanged()));
    connect(studentsComboBox, SIGNAL(activated(QString)), this /*ModifyConstraintActivityPreferredStartingTimeForm_template*/, SLOT(filterChanged()));
    connect(subjectsComboBox, SIGNAL(activated(QString)), this /*ModifyConstraintActivityPreferredStartingTimeForm_template*/, SLOT(filterChanged()));
    connect(activityTagsComboBox, SIGNAL(activated(QString)), this /*ModifyConstraintActivityPreferredStartingTimeForm_template*/, SLOT(filterChanged()));
//    connect(permLockedCheckBox, SIGNAL(toggled(bool)), this /*ModifyConstraintActivityPreferredStartingTimeForm_template*/, SLOT(constraintChanged()));


	//setWindowFlags(Qt::Window);
	/*setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	QDesktopWidget* desktop=QApplication::desktop();
	int xx=desktop->width()/2 - frameGeometry().width()/2;
	int yy=desktop->height()/2 - frameGeometry().height()/2;
	move(xx, yy);*/
	centerWidgetOnScreen(this);

	QSize tmp1=teachersComboBox->minimumSizeHint();
	Q_UNUSED(tmp1);
	QSize tmp2=studentsComboBox->minimumSizeHint();
	Q_UNUSED(tmp2);
	QSize tmp3=subjectsComboBox->minimumSizeHint();
	Q_UNUSED(tmp3);
	QSize tmp4=activityTagsComboBox->minimumSizeHint();
	Q_UNUSED(tmp4);

	QSize tmp5=activitiesComboBox->minimumSizeHint();
	Q_UNUSED(tmp5);
	QSize tmp6=dayComboBox->minimumSizeHint();
	Q_UNUSED(tmp6);
	QSize tmp7=startHourComboBox->minimumSizeHint();
	Q_UNUSED(tmp7);
	
	activitiesComboBox->setMaximumWidth(maxRecommendedWidth(this));
	
	//permTextLabel->setWordWrap(true);
	
	this->_ctr=ctr;
	
	//compulsoryCheckBox->setChecked(ctr->compulsory);
	weightLineEdit->setText(QString::number(ctr->weightPercentage));

	teachersComboBox->insertItem("");
	for(int i=0; i<gt.rules.teachersList.size(); i++){
		Teacher* tch=gt.rules.teachersList[i];
		teachersComboBox->insertItem(tch->name);
	}
	teachersComboBox->setCurrentItem(0);

	subjectsComboBox->insertItem("");
	for(int i=0; i<gt.rules.subjectsList.size(); i++){
		Subject* sb=gt.rules.subjectsList[i];
		subjectsComboBox->insertItem(sb->name);
	}
	subjectsComboBox->setCurrentItem(0);

	activityTagsComboBox->insertItem("");
	for(int i=0; i<gt.rules.activityTagsList.size(); i++){
		ActivityTag* st=gt.rules.activityTagsList[i];
		activityTagsComboBox->insertItem(st->name);
	}
	activityTagsComboBox->setCurrentItem(0);

	studentsComboBox->insertItem("");
	for(int i=0; i<gt.rules.yearsList.size(); i++){
		StudentsYear* sty=gt.rules.yearsList[i];
		studentsComboBox->insertItem(sty->name);
		for(int j=0; j<sty->groupsList.size(); j++){
			StudentsGroup* stg=sty->groupsList[j];
			studentsComboBox->insertItem(stg->name);
			for(int k=0; k<stg->subgroupsList.size(); k++){
				StudentsSubgroup* sts=stg->subgroupsList[k];
				studentsComboBox->insertItem(sts->name);
			}
		}
	}
	studentsComboBox->setCurrentItem(0);
	
	updatePeriodGroupBox();
	updateActivitiesComboBox();

	dayComboBox->setCurrentItem(ctr->day);
	startHourComboBox->setCurrentItem(ctr->hour);
	
	permLockedCheckBox->setChecked(this->_ctr->permanentlyLocked);
	
	constraintChanged();
}

ModifyConstraintActivityPreferredStartingTimeForm::~ModifyConstraintActivityPreferredStartingTimeForm()
{
}

bool ModifyConstraintActivityPreferredStartingTimeForm::filterOk(Activity* act)
{
	QString tn=teachersComboBox->currentText();
	QString stn=studentsComboBox->currentText();
	QString sbn=subjectsComboBox->currentText();
	QString sbtn=activityTagsComboBox->currentText();
	int ok=true;

	//teacher
	if(tn!=""){
		bool ok2=false;
		for(QStringList::Iterator it=act->teachersNames.begin(); it!=act->teachersNames.end(); it++)
			if(*it == tn){
				ok2=true;
				break;
			}
		if(!ok2)
			ok=false;
	}

	//subject
	if(sbn!="" && sbn!=act->subjectName)
		ok=false;
		
	//activity tag
//	if(sbtn!="" && sbtn!=act->activityTagName)
	if(sbtn!="" && !act->activityTagsNames.contains(sbtn))
		ok=false;
		
	//students
	if(stn!=""){
		bool ok2=false;
		for(QStringList::Iterator it=act->studentsNames.begin(); it!=act->studentsNames.end(); it++)
			if(*it == stn){
				ok2=true;
				break;
			}
		if(!ok2)
			ok=false;
	}
	
	return ok;
}

void ModifyConstraintActivityPreferredStartingTimeForm::filterChanged()
{
	this->updateActivitiesComboBox();
}

void ModifyConstraintActivityPreferredStartingTimeForm::updateActivitiesComboBox(){
	activitiesComboBox->clear();
	activitiesList.clear();
	int i=0, j=-1;
	for(int k=0; k<gt.rules.activitiesList.size(); k++){
		Activity* act=gt.rules.activitiesList[k];
		if(filterOk(act)){
			activitiesComboBox->insertItem(act->getDescription(gt.rules));
			this->activitiesList.append(act->id);
			if(act->id==this->_ctr->activityId)
				j=i;
				
			i++;
		}
	}
	//assert(j>=0); only first time
	activitiesComboBox->setCurrentItem(j);

	constraintChanged();
}

void ModifyConstraintActivityPreferredStartingTimeForm::updatePeriodGroupBox(){
	startHourComboBox->clear();
	//startHourComboBox->insertItem(tr("Any"));
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		startHourComboBox->insertItem(gt.rules.hoursOfTheDay[i]);

	dayComboBox->clear();
	//dayComboBox->insertItem(tr("Any"));
	for(int i=0; i<gt.rules.nDaysPerWeek; i++)
		dayComboBox->insertItem(gt.rules.daysOfTheWeek[i]);
}

void ModifyConstraintActivityPreferredStartingTimeForm::constraintChanged()
{/*
	QString s;
	s+=tr("Current constraint:");
	s+="\n";

	double weight;
	QString tmp=weightLineEdit->text();
	sscanf(tmp, "%lf", &weight);
	s+=tr("Weight (percentage)=%1\%").arg(weight);
	s+="\n";

	s+=tr("Activity preferred starting time");
	s+="\n";
	
	int id;
	int tmp2=activitiesComboBox->currentItem();
	assert(tmp2<gt.rules.activitiesList.size());
	assert(tmp2<activitiesList.size());
	if(tmp2<0){
		s+=tr("Invalid activity");
		s+="\n";
	}
	else{
		id=activitiesList.at(tmp2);
		s+=tr("Activity id=%1").arg(id);
		s+="\n";
	}

	int day=dayComboBox->currentItem();
	if(day<0 || day>=gt.rules.nDaysPerWeek){
		s+=tr("Invalid day");
		s+="\n";
	}
	else{
		s+=tr("Day:%1").arg(dayComboBox->currentText());
		s+="\n";
	}

	int startHour=startHourComboBox->currentItem();
	if(startHour<0 || startHour>=gt.rules.nHoursPerDay){
		s+=tr("Invalid start hour");
		s+="\n";
	}
	else{
		s+=tr("Start hour:%1").arg(startHourComboBox->currentText());
		s+="\n";
	}
	
	if(permLockedCheckBox->isChecked()){
		s+=tr("Permanently locked (cannot be unlocked from the 'Timetable' menu)");
	}
	else{
		s+=tr("Not permanently locked (can be unlocked from the 'Timetable' menu)");
	}
	s+="\n";

	currentConstraintTextEdit->setText(s);*/
}

void ModifyConstraintActivityPreferredStartingTimeForm::ok()
{
	double weight;
	QString tmp=weightLineEdit->text();
	sscanf(tmp, "%lf", &weight);
	if(weight<0.0 || weight>100.0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage)"));
		return;
	}

	/*bool compulsory=false;
	if(compulsoryCheckBox->isChecked())
		compulsory=true;*/

	int day=dayComboBox->currentItem();
	if(day<0 || day>=gt.rules.nDaysPerWeek){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid day"));
		return;
	}
	int startHour=startHourComboBox->currentItem();
	if(startHour<0 || startHour>=gt.rules.nHoursPerDay){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid start hour"));
		return;
	}

/*	if(startHour==0 && day==0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Please specify at least a day or an hour"));
		return;
	}*/
	
	int tmp2=activitiesComboBox->currentItem();
	assert(tmp2<gt.rules.activitiesList.size());
	assert(tmp2<activitiesList.size());
	if(tmp2<0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid activity"));
		return;
	}
	int id=activitiesList.at(tmp2);
	
/*	foreach(TimeConstraint* tc, gt.rules.timeConstraintsList)
		if(tc->type==CONSTRAINT_ACTIVITY_PREFERRED_STARTING_TIME){
			ConstraintActivityPreferredStartingTime* c=(ConstraintActivityPreferredStartingTime*) tc;
			if(c->activityId==id && c!=this->_ctr){
				QMessageBox::warning(this, tr("FET information"),
				 tr("This activity id has other constraint of this type attached\n"
				 "Please remove the other constraints of type activity preferred starting time\n"
				 "referring to this activity before proceeding"));
				return;
			}
		}*/

	this->_ctr->weightPercentage=weight;
	//this->_ctr->compulsory=compulsory;
	this->_ctr->day=day;
	this->_ctr->hour=startHour;
	this->_ctr->activityId=id;
	
	this->_ctr->permanentlyLocked=permLockedCheckBox->isChecked();
	
	gt.rules.internalStructureComputed=false;
	
	LockUnlock::computeLockedUnlockedActivitiesOnlyTime();
	LockUnlock::increaseCommunicationSpinBox();

	this->close();
}

void ModifyConstraintActivityPreferredStartingTimeForm::cancel()
{
	this->close();
}
