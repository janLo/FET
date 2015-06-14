/***************************************************************************
                          modifyconstraintactivitiessamestartinghourform.cpp  -  description
                             -------------------
    begin                : Feb 15, 2005
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

#include <QMessageBox>

#include <cstdio>

#include "modifyconstraintactivitiessamestartinghourform.h"
#include "spaceconstraint.h"

#include <QList>

ModifyConstraintActivitiesSameStartingHourForm::ModifyConstraintActivitiesSameStartingHourForm(ConstraintActivitiesSameStartingHour* ctr)
{
    setupUi(this);

    connect(cancelPushButton, SIGNAL(clicked()), this /*ModifyConstraintActivitiesSameStartingHourForm_template*/, SLOT(cancel()));
    connect(okPushButton, SIGNAL(clicked()), this /*ModifyConstraintActivitiesSameStartingHourForm_template*/, SLOT(ok()));
    connect(activitiesListBox, SIGNAL(selected(QString)), this /*ModifyConstraintActivitiesSameStartingHourForm_template*/, SLOT(addActivity()));
    connect(selectedActivitiesListBox, SIGNAL(selected(QString)), this /*ModifyConstraintActivitiesSameStartingHourForm_template*/, SLOT(removeActivity()));
    connect(teachersComboBox, SIGNAL(activated(QString)), this /*ModifyConstraintActivitiesSameStartingHourForm_template*/, SLOT(filterChanged()));
    connect(studentsComboBox, SIGNAL(activated(QString)), this /*ModifyConstraintActivitiesSameStartingHourForm_template*/, SLOT(filterChanged()));
    connect(subjectsComboBox, SIGNAL(activated(QString)), this /*ModifyConstraintActivitiesSameStartingHourForm_template*/, SLOT(filterChanged()));
    connect(activityTagsComboBox, SIGNAL(activated(QString)), this /*ModifyConstraintActivitiesSameStartingHourForm_template*/, SLOT(filterChanged()));

    connect(clearPushButton, SIGNAL(clicked()), this, SLOT(clear()));


	//setWindowFlags(Qt::Window);
	/*setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	QDesktopWidget* desktop=QApplication::desktop();
	int xx=desktop->width()/2 - frameGeometry().width()/2;
	int yy=desktop->height()/2 - frameGeometry().height()/2;
	move(xx, yy);*/
	centerWidgetOnScreen(this);
	
	this->_ctr=ctr;

	QSize tmp1=teachersComboBox->minimumSizeHint();
	Q_UNUSED(tmp1);
	QSize tmp2=studentsComboBox->minimumSizeHint();
	Q_UNUSED(tmp2);
	QSize tmp3=subjectsComboBox->minimumSizeHint();
	Q_UNUSED(tmp3);
	QSize tmp4=activityTagsComboBox->minimumSizeHint();
	Q_UNUSED(tmp4);
	

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
	



	updateActivitiesListBox();

	selectedActivitiesListBox->clear();
	this->selectedActivitiesList.clear();
	
	weightLineEdit->setText(QString::number(ctr->weightPercentage));
	//compulsoryCheckBox->setChecked(ctr->compulsory);
	
	for(int i=0; i<ctr->n_activities; i++){
		int actId=ctr->activitiesId[i];
		this->selectedActivitiesList.append(actId);
		Activity* act=NULL;
		for(int k=0; k<gt.rules.activitiesList.size(); k++){
			act=gt.rules.activitiesList[k];
			if(act->id==actId)
				break;
		}
		assert(act);
		this->selectedActivitiesListBox->insertItem(act->getDescription(gt.rules));
	}
}

ModifyConstraintActivitiesSameStartingHourForm::~ModifyConstraintActivitiesSameStartingHourForm()
{
}

bool ModifyConstraintActivitiesSameStartingHourForm::filterOk(Activity* act)
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

void ModifyConstraintActivitiesSameStartingHourForm::filterChanged()
{
	this->updateActivitiesListBox();
}

void ModifyConstraintActivitiesSameStartingHourForm::updateActivitiesListBox()
{
	activitiesListBox->clear();
	//selectedActivitiesListBox->clear();

	this->activitiesList.clear();
	//this->selectedActivitiesList.clear();

	for(int i=0; i<gt.rules.activitiesList.size(); i++){
		Activity* ac=gt.rules.activitiesList[i];
		 if(filterOk(ac)){
			activitiesListBox->insertItem(ac->getDescription(gt.rules));
			this->activitiesList.append(ac->id);
		}
	}
}

void ModifyConstraintActivitiesSameStartingHourForm::ok()
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

	if(this->selectedActivitiesList.size()==0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Empty list of selected activities"));
		return;
	}
	if(this->selectedActivitiesList.size()==1){
		QMessageBox::warning(this, tr("FET information"),
			tr("Only one selected activity"));
		return;
	}
	/*if(this->selectedActivitiesList.size()>MAX_CONSTRAINT_ACTIVITIES_SAME_STARTING_HOUR){
		QMessageBox::warning(this, tr("FET information"),
			tr("Please report error to the author\nMAX_CONSTRAINT_ACTIVITIES_SAME_STARTING_HOUR must be increased (you have too many activities)"));
		return;
	}*/
	
	int i;
	QList<int>::iterator it;
	this->_ctr->activitiesId.clear();
	for(i=0, it=this->selectedActivitiesList.begin(); it!=this->selectedActivitiesList.end(); it++, i++){
		//this->_ctr->activitiesId[i]=*it;
		this->_ctr->activitiesId.append(*it);
	}
	this->_ctr->n_activities=i;
		
	this->_ctr->weightPercentage=weight;
	//this->_ctr->compulsory=compulsory;
	
	gt.rules.internalStructureComputed=false;
	
	this->close();
}

void ModifyConstraintActivitiesSameStartingHourForm::cancel()
{
	this->close();
}

void ModifyConstraintActivitiesSameStartingHourForm::addActivity()
{
	if(activitiesListBox->currentItem()<0)
		return;
	int tmp=activitiesListBox->currentItem();
	int _id=this->activitiesList.at(tmp);
	
	QString actName=activitiesListBox->currentText();
	assert(actName!="");
	uint i;
	//duplicate?
	for(i=0; i<selectedActivitiesListBox->count(); i++)
		if(actName==selectedActivitiesListBox->text(i))
			break;
	if(i<selectedActivitiesListBox->count())
		return;
	selectedActivitiesListBox->insertItem(actName);
	
	this->selectedActivitiesList.append(_id);
}

void ModifyConstraintActivitiesSameStartingHourForm::removeActivity()
{
	if(selectedActivitiesListBox->currentItem()<0 || selectedActivitiesListBox->count()<=0)
		return;		
	int tmp=selectedActivitiesListBox->currentItem();
	
	selectedActivitiesListBox->removeItem(tmp);
	this->selectedActivitiesList.removeAt(tmp);
}

void ModifyConstraintActivitiesSameStartingHourForm::clear()
{
	selectedActivitiesListBox->clear();
	selectedActivitiesList.clear();
}

