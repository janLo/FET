/***************************************************************************
                          modifyconstraintactivitypreferredtimeslotsform.cpp  -  description
                             ------------------
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

#include "modifyconstraintactivitypreferredtimeslotsform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <q3table.h>

#include <QDesktopWidget>

#define YES	(QObject::tr("Allowed", "Please keep translation short"))
#define NO	(QObject::tr("Not allowed", "Please keep translation short"))

ModifyConstraintActivityPreferredTimeSlotsForm::ModifyConstraintActivityPreferredTimeSlotsForm(ConstraintActivityPreferredTimeSlots* ctr)
{
	//setWindowFlags(Qt::Window);
	/*setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	QDesktopWidget* desktop=QApplication::desktop();
	int xx=desktop->width()/2 - frameGeometry().width()/2;
	int yy=desktop->height()/2 - frameGeometry().height()/2;
	move(xx, yy);	*/
	centerWidgetOnScreen(this);
	
	this->_ctr=ctr;
	
	weightLineEdit->setText(QString::number(ctr->weightPercentage));
	//compulsoryCheckBox->setChecked(ctr->compulsory);

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
	
	updateActivitiesComboBox();

	preferredTimesTable->setNumRows(gt.rules.nHoursPerDay);
	preferredTimesTable->setNumCols(gt.rules.nDaysPerWeek);

	for(int j=0; j<gt.rules.nDaysPerWeek; j++)
		//preferredTimesTable->setText(0, j+1, gt.rules.daysOfTheWeek[j]);
		preferredTimesTable->horizontalHeader()->setLabel(j, gt.rules.daysOfTheWeek[j]);
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		//preferredTimesTable->setText(i+1, 0, gt.rules.hoursOfTheDay[i]);
		preferredTimesTable->verticalHeader()->setLabel(i, gt.rules.hoursOfTheDay[i]);
		
	bool currentMatrix[MAX_HOURS_PER_DAY][MAX_DAYS_PER_WEEK];
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++)
			currentMatrix[i][j]=false;
	for(int k=0; k<ctr->p_nPreferredTimeSlots; k++){
		if(ctr->p_days[k]==-1 || ctr->p_hours[k]==-1)
			assert(0);
		int i=ctr->p_hours[k];
		int j=ctr->p_days[k];
		currentMatrix[i][j]=true;
	}

	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++)
			if(!currentMatrix[i][j])
				preferredTimesTable->setText(i, j, NO);
			else
				preferredTimesTable->setText(i, j, YES);
}

ModifyConstraintActivityPreferredTimeSlotsForm::~ModifyConstraintActivityPreferredTimeSlotsForm()
{
}

void ModifyConstraintActivityPreferredTimeSlotsForm::setAllSlotsAllowed()
{
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++)
			preferredTimesTable->setText(i, j, YES);
}

void ModifyConstraintActivityPreferredTimeSlotsForm::setAllSlotsNotAllowed()
{
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++)
			preferredTimesTable->setText(i, j, NO);
}

bool ModifyConstraintActivityPreferredTimeSlotsForm::filterOk(Activity* act)
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
	if(sbtn!="" && sbtn!=act->activityTagName)
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

void ModifyConstraintActivityPreferredTimeSlotsForm::filterChanged()
{
	this->updateActivitiesComboBox();
}

void ModifyConstraintActivityPreferredTimeSlotsForm::tableClicked(int row, int col, int button, const QPoint& mousePos)
{
	Q_UNUSED(button);
	Q_UNUSED(mousePos);
	//if(&button!=NULL && &mousePos!=NULL)
	//	;

	//row--; col--;
	if(row>=0 && row<gt.rules.nHoursPerDay && col>=0 && col<gt.rules.nDaysPerWeek){
		QString s=preferredTimesTable->text(row, col);
		if(s==YES)
			s=NO;
		else{
			assert(s==NO);
			s=YES;
		}
		preferredTimesTable->setText(row, col, s);
	}
}

void ModifyConstraintActivityPreferredTimeSlotsForm::updateActivitiesComboBox(){
	int i=0, j=-1;
	activitiesComboBox->clear();
	activitiesList.clear();
	for(int k=0; k<gt.rules.activitiesList.size(); k++){
		Activity* act=gt.rules.activitiesList[k];
		if(filterOk(act)){
			activitiesComboBox->insertItem(act->getDescription(gt.rules));
			this->activitiesList.append(act->id);
			if(act->id==this->_ctr->p_activityId)
				j=i;
		
			i++;
		}
	}
	//assert(j>=0); only first time
	activitiesComboBox->setCurrentItem(j);
}

void ModifyConstraintActivityPreferredTimeSlotsForm::cancel()
{
	this->close();
}

void ModifyConstraintActivityPreferredTimeSlotsForm::ok()
{
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

	int i=activitiesComboBox->currentItem();
	assert(i<activitiesList.size());
	if(i<0 || activitiesComboBox->count()<=0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid activity"));
		return;
	}
	int id=activitiesList.at(i);

/*	
	foreach(TimeConstraint* tc, gt.rules.timeConstraintsList)
		if(tc->type==CONSTRAINT_ACTIVITY_PREFERRED_TIMES){
			ConstraintActivityPreferredTimes* c=(ConstraintActivityPreferredTimes*) tc;
			if(c->activityId==id && c!=this->_ctr){
				QMessageBox::warning(this, QObject::tr("FET information"),
				 QObject::tr("This activity id has other constraint of this type attached\n"
				 "Please remove the other constraints of type activity preferred times\n"
				 "referring to this activity before proceeding"));
				return;
			}
		}
*/

	int days[MAX_N_CONSTRAINT_ACTIVITY_PREFERRED_TIME_SLOTS];
	int hours[MAX_N_CONSTRAINT_ACTIVITY_PREFERRED_TIME_SLOTS];
	int n=0;
	for(int j=0; j<gt.rules.nDaysPerWeek; j++)
		for(i=0; i<gt.rules.nHoursPerDay; i++)
			if(preferredTimesTable->text(i, j)==YES){
				if(n>=MAX_N_CONSTRAINT_ACTIVITY_PREFERRED_TIME_SLOTS){
					QString s=QObject::tr("Not enough slots (too many \"Yes\" values).");
					s+="\n";
					s+=QObject::tr("Please increase the variable MAX_N_CONSTRAINT_ACTIVITY_PREFERRED_TIME_SLOTS");
					s+="\n";
					s+=QObject::tr("Currently, it is %1").arg(MAX_N_CONSTRAINT_ACTIVITY_PREFERRED_TIME_SLOTS);
					QMessageBox::warning(this, QObject::tr("FET information"), s);
					
					return;
				}
				
				days[n]=j;
				hours[n]=i;
				n++;
			}

	if(n<=0){
		int t=QMessageBox::question(this, tr("FET question"),
		 tr("Warning: 0 slots selected. Are you sure?"),
		 QMessageBox::Yes, QMessageBox::Cancel);
						 
		if(t==QMessageBox::Cancel)
				return;
	}

	this->_ctr->weightPercentage=weight;
	//this->_ctr->compulsory=compulsory;
	this->_ctr->p_activityId=id;
	this->_ctr->p_nPreferredTimeSlots=n;
	for(int i=0; i<n; i++){
		this->_ctr->p_days[i]=days[i];
		this->_ctr->p_hours[i]=hours[i];
	}

	gt.rules.internalStructureComputed=false;
	
	this->close();
}

#undef YES
#undef NO