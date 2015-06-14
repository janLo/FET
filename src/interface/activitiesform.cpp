/***************************************************************************
                          activitiesform.cpp  -  description
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

#include "genetictimetable_defs.h"
#include "fet.h"
#include "genetictimetable.h"

#include "activitiesform.h"
#include "addactivityform.h"
#include "modifyactivityform.h"

#include <qstring.h>
#include <qlistbox.h>
#include <qmessagebox.h>
#include <qtextedit.h>

ActivitiesForm::ActivitiesForm()
{
	visibleActivitiesList.setAutoDelete(false);

	teachersComboBox->insertItem("");
	for(Teacher* tch=gt.rules.teachersList.first(); tch; tch=gt.rules.teachersList.next())
		teachersComboBox->insertItem(tch->name);
	teachersComboBox->setCurrentItem(0);

	subjectsComboBox->insertItem("");
	for(Subject* sb=gt.rules.subjectsList.first(); sb; sb=gt.rules.subjectsList.next())
		subjectsComboBox->insertItem(sb->name);
	subjectsComboBox->setCurrentItem(0);

	studentsComboBox->insertItem("");
	for(StudentsYear* sty=gt.rules.yearsList.first(); sty; sty=gt.rules.yearsList.next()){
		studentsComboBox->insertItem(sty->name);
		for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
			studentsComboBox->insertItem(stg->name);
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next())
				studentsComboBox->insertItem(sts->name);
		}
	}
	studentsComboBox->setCurrentItem(0);

	this->filterChanged();
}

ActivitiesForm::~ActivitiesForm()
{
}

bool ActivitiesForm::filterOk(Activity* act)
{
	QString tn=teachersComboBox->currentText();
	QString stn=studentsComboBox->currentText();
	QString sbn=subjectsComboBox->currentText();
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

void ActivitiesForm::filterChanged()
{
	QString s;
	activitiesListBox->clear();
	assert(visibleActivitiesList.autoDelete()==false);
	visibleActivitiesList.clear();
	for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next())
		if(this->filterOk(act)){
			s=act->getDescription(gt.rules);
			visibleActivitiesList.append(act);
			activitiesListBox->insertItem(s);
		}
	activityChanged(activitiesListBox->currentItem());
}

void ActivitiesForm::addActivity()
{
	int ind=activitiesListBox->currentItem();

	AddActivityForm *addActivityForm=new AddActivityForm();
	addActivityForm->exec();

	//rebuild the activities list box
	filterChanged();
	
	activitiesListBox->setCurrentItem(ind);
}

void ActivitiesForm::removeActivity()
{
	int ind=activitiesListBox->currentItem();
	if(ind<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected activity"));
		return;
	}

	Activity* act=visibleActivitiesList.at(ind);
	assert(act!=NULL);

	QString s;
	if(!act->isSplit())
		s=QObject::tr("Removing activity:");
	else
		s=QObject::tr("Removing sub-activity:");
	s+="\n";
	s+=act->getDetailedDescription(gt.rules);
	if(act->isSplit())
		s+=QObject::tr("There will also be removed all the\nsub-activities from the same split activity");

	switch( QMessageBox::warning( this, QObject::tr("FET warning"),
	s, QObject::tr("OK"), QObject::tr("Cancel"), 0, 0, 1 ) ){
	case 0: // The user clicked the OK button or pressed Enter
		gt.rules.removeActivity(act->id, act->activityGroupId);
		filterChanged();
		break;
	case 1: // The user clicked the Cancel or pressed Escape
		break;
	}
	
	if((uint)(ind) >= activitiesListBox->count())
		ind = activitiesListBox->count()-1;
	activitiesListBox->setCurrentItem(ind);
}

void ActivitiesForm::modifyActivity()
{
	int ind=activitiesListBox->currentItem();
	if(ind<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected activity"));
		return;
	}

	Activity* act=visibleActivitiesList.at(ind);
	assert(act!=NULL);
	
	ModifyActivityForm* modifyActivityForm=new ModifyActivityForm(act->id, act->activityGroupId);
	modifyActivityForm->exec();

	filterChanged();
	
	activitiesListBox->setCurrentItem(ind);
}

void ActivitiesForm::activityChanged(int index)
{
	if(index<0){
		currentActivityTextEdit->setText(QObject::tr("Invalid activity"));
		return;
	}

	QString s;
	Activity* act=visibleActivitiesList.at(index);

	assert(act!=NULL);
	s=act->getDetailedDescription(gt.rules);
	currentActivityTextEdit->setText(s);
}
