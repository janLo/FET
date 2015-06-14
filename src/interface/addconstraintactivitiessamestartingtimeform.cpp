/***************************************************************************
                          addconstraintactivitiessamestartingtimeform.cpp  -  description
                             -------------------
    begin                : Wed June 23 2004
    copyright            : (C) 2004 by Lalescu Liviu
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

#include "addconstraintactivitiessamestartingtimeform.h"
#include "spaceconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <q3table.h>

#include <QDesktopWidget>

AddConstraintActivitiesSameStartingTimeForm::AddConstraintActivitiesSameStartingTimeForm()
{
	//setWindowFlags(Qt::Window);
	setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	QDesktopWidget* desktop=QApplication::desktop();
	int xx=desktop->width()/2 - frameGeometry().width()/2;
	int yy=desktop->height()/2 - frameGeometry().height()/2;
	move(xx, yy);
						
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

	subjectTagsComboBox->insertItem("");
	for(int i=0; i<gt.rules.subjectTagsList.size(); i++){
		SubjectTag* st=gt.rules.subjectTagsList[i];
		subjectTagsComboBox->insertItem(st->name);
	}
	subjectTagsComboBox->setCurrentItem(0);

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
	
	simultaneousActivitiesListBox->clear();
	this->simultaneousActivitiesList.clear();

	updateActivitiesListBox();
}

AddConstraintActivitiesSameStartingTimeForm::~AddConstraintActivitiesSameStartingTimeForm()
{
}

bool AddConstraintActivitiesSameStartingTimeForm::filterOk(Activity* act)
{
	QString tn=teachersComboBox->currentText();
	QString stn=studentsComboBox->currentText();
	QString sbn=subjectsComboBox->currentText();
	QString sbtn=subjectTagsComboBox->currentText();
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
		
	//subject tag
	if(sbtn!="" && sbtn!=act->subjectTagName)
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

void AddConstraintActivitiesSameStartingTimeForm::filterChanged()
{
	this->updateActivitiesListBox();
}

void AddConstraintActivitiesSameStartingTimeForm::updateActivitiesListBox()
{
	activitiesListBox->clear();
	//simultaneousActivitiesListBox->clear();

	this->activitiesList.clear();
	//this->simultaneousActivitiesList.clear();

	if(blockCheckBox->isChecked())
		//show only non-split activities and split activities which are the representatives
		for(int i=0; i<gt.rules.activitiesList.size(); i++){
			Activity* ac=gt.rules.activitiesList[i];
			if(filterOk(ac)){
				if(ac->activityGroupId==0){
					activitiesListBox->insertItem(ac->getDescription(gt.rules));
					this->activitiesList.append(ac->id);
				}
				else if(ac->id==ac->activityGroupId){
					activitiesListBox->insertItem(ac->getDescription(gt.rules));
					this->activitiesList.append(ac->id);
				}
			}
		}
	else
		for(int i=0; i<gt.rules.activitiesList.size(); i++){
			Activity* ac=gt.rules.activitiesList[i];
			if(filterOk(ac)){
				activitiesListBox->insertItem(ac->getDescription(gt.rules));
				this->activitiesList.append(ac->id);
			}
		}
}

void AddConstraintActivitiesSameStartingTimeForm::blockChanged()
{
	simultaneousActivitiesListBox->clear();
	this->simultaneousActivitiesList.clear();

	this->updateActivitiesListBox();
}

void AddConstraintActivitiesSameStartingTimeForm::addConstraint()
{
	TimeConstraint *ctr=NULL;

	double weight;
	QString tmp=weightLineEdit->text();
	sscanf(tmp, "%lf", &weight);
	if(weight<0.0 || weight>100.0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid weight (percentage)"));
		return;
	}

	/*bool compulsory=false;
	if(compulsoryCheckBox->isChecked())
		compulsory=true;*/

	if(this->simultaneousActivitiesList.count()==0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Empty list of simultaneous activities"));
		return;
	}
	if(this->simultaneousActivitiesList.count()==1){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Only one selected activity - impossible"));
		return;
	}
	if(this->simultaneousActivitiesList.size()>=MAX_CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Too many activities - please report error\n(CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME too little)"));
		return;
	}
	
if(blockCheckBox->isChecked()){ //block constraints
	///////////phase 1 - how many constraints will be added?
	int nConstraints=0;
	QList<int>::iterator it;
	for(it=this->simultaneousActivitiesList.begin(); it!=this->simultaneousActivitiesList.end(); it++){
		int _id=(*it);
		int tmp=0; //tmp represents the number of sub-activities represented by the current (sub)activity

		for(int i=0; i<gt.rules.activitiesList.size(); i++){
			Activity* act=gt.rules.activitiesList[i];
			if(act->activityGroupId==0){
				if(act->id==_id){
					assert(tmp==0);
					tmp=1;
				}
			}
			else{
				if(act->id==_id){
					assert(act->activityGroupId==act->id);
					assert(tmp==0);
					tmp=1;
				}
				else if(act->activityGroupId==_id)
					tmp++;
			}
		}

		if(nConstraints==0){
			nConstraints=tmp;
		}
		else{
			if(tmp!=nConstraints){
				QString s=QObject::tr("Sub-activities do not correspond. Mistake:\n");
				s+=QObject::tr("1. First (sub)activity has id=%1 and represents %2 sub-activities\n")
					.arg(this->simultaneousActivitiesList.at(0))
					.arg(nConstraints);
				s+=QObject::tr("2. Current (sub)activity has id=%1 and represents %2 sub-activities\n")
					.arg(_id)
					.arg(tmp);
				QMessageBox::warning(this, QObject::tr("FET information"), s);
				return;				
			}
		}
	}
	
	/////////////phase 2 - compute the indices of all the (sub)activities
//#ifdef WIN32
//	int ids[10][MAX_CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME];
//#else
	int ids[nConstraints][this->simultaneousActivitiesList.count()];
//#endif
	int k;
	for(k=0, it=this->simultaneousActivitiesList.begin(); it!=this->simultaneousActivitiesList.end(); k++, it++){
		int _id=(*it);
		int tmp=0; //tmp represents the number of sub-activities represented by the current (sub)activity

		for(int i=0; i<gt.rules.activitiesList.size(); i++){
			Activity* act=gt.rules.activitiesList[i];
			if(act->activityGroupId==0){
				if(act->id==_id){
					assert(tmp==0);
					ids[tmp][k]=_id;
					tmp=1;
				}
			}
			else{
				if(act->id==_id){
					assert(act->activityGroupId==act->id);
					assert(tmp==0);
					ids[tmp][k]=_id;
					tmp=1;
				}
				else if(act->activityGroupId==_id){
					ids[tmp][k]=act->id;
					tmp++;
				}
			}
		}
	}
	
	////////////////phase 3 - add the constraints
	for(k=0; k<nConstraints; k++){
		ctr=new ConstraintActivitiesSameStartingTime(weight, /*compulsory,*/ this->simultaneousActivitiesList.count(), ids[k]);
		bool tmp2=gt.rules.addTimeConstraint(ctr);
		
		if(tmp2){
			QString s=QObject::tr("Constraint added:");
			s+="\n";
			s+=ctr->getDetailedDescription(gt.rules);
			QMessageBox::information(this, QObject::tr("FET information"), s);
		}
		else{
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Constraint NOT added - please report error"));
			delete ctr;
		}
	}
}
else{
	int ids[MAX_CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME];
	QList<int>::iterator it;
	int i;
	for(i=0, it=this->simultaneousActivitiesList.begin(); it!=this->simultaneousActivitiesList.end(); i++,it++){
		assert(i<MAX_CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME);
		ids[i]=*it;
	}
	ctr=new ConstraintActivitiesSameStartingTime(weight, /*compulsory,*/ this->simultaneousActivitiesList.count(), ids);

	bool tmp2=gt.rules.addTimeConstraint(ctr);
		
	if(tmp2){
		QString s=QObject::tr("Constraint added:");
		s+="\n";
		s+=ctr->getDetailedDescription(gt.rules);
		QMessageBox::information(this, QObject::tr("FET information"), s);
	}
	else{
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Constraint NOT added - please report error"));
		delete ctr;
	}
}
}

void AddConstraintActivitiesSameStartingTimeForm::addActivity()
{
	if(activitiesListBox->currentItem()<0)
		return;
	int tmp=activitiesListBox->currentItem();
	int _id=this->activitiesList.at(tmp);
	
	QString actName=activitiesListBox->currentText();
	assert(actName!="");
	uint i;
	//duplicate?
	for(i=0; i<simultaneousActivitiesListBox->count(); i++)
		if(actName==simultaneousActivitiesListBox->text(i))
			break;
	if(i<simultaneousActivitiesListBox->count())
		return;
	simultaneousActivitiesListBox->insertItem(actName);
	
	this->simultaneousActivitiesList.append(_id);
}

void AddConstraintActivitiesSameStartingTimeForm::removeActivity()
{
	if(simultaneousActivitiesListBox->currentItem()<0 || simultaneousActivitiesListBox->count()<=0)
		return;		
	int tmp=simultaneousActivitiesListBox->currentItem();
	
	simultaneousActivitiesListBox->removeItem(tmp);
	this->simultaneousActivitiesList.removeAt(tmp);
}

void AddConstraintActivitiesSameStartingTimeForm::clear()
{
	simultaneousActivitiesListBox->clear();
	simultaneousActivitiesList.clear();
}
