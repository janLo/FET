/***************************************************************************
                          timetableviewstudentsform.cpp  -  description
                             -------------------
    begin                : Tue Apr 22 2003
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

#include "timetableviewstudentsform.h"
#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fetmainform.h"
#include "fet.h"

#include <qcombobox.h>
#include <qmessagebox.h>
#include <qgroupbox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qtable.h>
#include <qapplication.h>
#include <qlistbox.h>
#include <qlabel.h>
#include <qtable.h>
#include <qtextedit.h>
#include <qstring.h>

extern bool students_schedule_ready;
extern bool teachers_schedule_ready;

extern bool simulation_running;

extern TimeChromosome best_time_chromosome;

TimetableViewStudentsForm::TimetableViewStudentsForm()
{
	yearsListBox->clear();
	for(StudentsYear* sty=gt.rules.yearsList.first(); sty; sty=gt.rules.yearsList.next())
		yearsListBox->insertItem(sty->name);
	yearChanged(yearsListBox->currentText());
}

TimetableViewStudentsForm::~TimetableViewStudentsForm()
{
}

void TimetableViewStudentsForm::yearChanged(const QString &yearName)
{
	if(yearName==NULL)
		return;

	int yearIndex=gt.rules.searchYear(yearName);
	if(yearIndex<0)
		return;

	groupsListBox->clear();
	StudentsYear* sty=gt.rules.yearsList.at(yearIndex);
	for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next())
		groupsListBox->insertItem(stg->name);

	groupChanged(groupsListBox->currentText());
}

void TimetableViewStudentsForm::groupChanged(const QString &groupName)
{
	int yearIndex=gt.rules.searchYear(yearsListBox->currentText());
	if(yearIndex<0)
		return;

	int groupIndex=gt.rules.searchGroup(yearsListBox->currentText(), groupName);
	if(groupIndex<0)
		return;

	subgroupsListBox->clear();
	
	StudentsYear* sty=gt.rules.yearsList.at(yearIndex);
	StudentsGroup* stg=sty->groupsList.at(groupIndex);
	for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next())
		subgroupsListBox->insertItem(sts->name);
}

void TimetableViewStudentsForm::subgroupChanged(const QString &subgroupName)
{
	if(subgroupName!="")
		;	

	updateStudentsTimetableTable();
}

void TimetableViewStudentsForm::updateStudentsTimetableTable(){
	assert(students_schedule_ready && teachers_schedule_ready);

	QString s;
	QString yearname;
	QString groupname;
	QString subgroupname;

	if(yearsListBox->currentText()==NULL)
		return;
	if(groupsListBox->currentText()==NULL)
		return;
	if(subgroupsListBox->currentText()==NULL)
		return;

	yearname = yearsListBox->currentText();
	groupname = groupsListBox->currentText();
	subgroupname = subgroupsListBox->currentText();

	s = QObject::tr("Students");
	s += ": ";
	s += yearname;
	s += ", ";
	s += groupname;
	s += ", ";
	s += subgroupname;

	classNameTextLabel->setText(s);

	assert(gt.rules.initialized);
	studentsTimetableTable->setNumRows(gt.rules.nHoursPerDay+1);
	studentsTimetableTable->setNumCols(gt.rules.nDaysPerWeek+1);
	for(int j=0; j<gt.rules.nDaysPerWeek; j++)
		studentsTimetableTable->setText(0, j+1, gt.rules.daysOfTheWeek[j]);
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		studentsTimetableTable->setText(i+1, 0, gt.rules.hoursOfTheDay[i]);

	StudentsSubgroup* sts=(StudentsSubgroup*)gt.rules.searchStudentsSet(subgroupname);
	assert(sts);
	int i;
	for(i=0; i<gt.rules.nInternalSubgroups; i++)
		if(gt.rules.internalSubgroupsList[i]==sts)
			break;
	assert(i<gt.rules.nInternalSubgroups);
	for(int j=0; j<gt.rules.nHoursPerDay; j++){
		for(int k=0; k<gt.rules.nDaysPerWeek; k++){
			s="";
			int ai=students_timetable_week1[i][k][j]; //activity index
			//Activity* act=gt.rules.activitiesList.at(ai);
			if(ai!=UNALLOCATED_ACTIVITY){
				Activity* act=&gt.rules.internalActivitiesList[ai];
				assert(act!=NULL);
				s+=act->subjectName + " " + act->subjectTagName;
			}
			ai=students_timetable_week2[i][k][j]; //activity index
			//act=gt.rules.activitiesList.at(ai);
			if(ai!=UNALLOCATED_ACTIVITY){
				Activity* act=&gt.rules.internalActivitiesList[ai];
				assert(act!=NULL);
				s += " / " + act->subjectName + " " + act->subjectTagName;
			}
			studentsTimetableTable->setText(j+1, k+1, s);
		}
	}
	for(int i=0; i<=gt.rules.nHoursPerDay; i++)
		studentsTimetableTable->adjustRow(i); //added in version 3_9_16, on 16 Oct. 2004
}

void TimetableViewStudentsForm::detailActivity(int row, int col)
{
	assert(students_schedule_ready && teachers_schedule_ready);

	QString s;
	QString yearname;
	QString groupname;
	QString subgroupname;

	if(yearsListBox->currentText()==NULL)
		return;
	if(groupsListBox->currentText()==NULL)
		return;
	if(subgroupsListBox->currentText()==NULL)
		return;

	yearname = yearsListBox->currentText();
	groupname = groupsListBox->currentText();
	subgroupname = subgroupsListBox->currentText();

	StudentsSubgroup* sts=(StudentsSubgroup*)gt.rules.searchStudentsSet(subgroupname);
	assert(sts);
	int i;
	for(i=0; i<gt.rules.nInternalSubgroups; i++)
		if(gt.rules.internalSubgroupsList[i]==sts)
			break;
	assert(i<gt.rules.nInternalSubgroups);
	int j=row-1;
	int k=col-1;
	s="";
	if(j>=0 && k>=0){
		int ai=students_timetable_week1[i][k][j]; //activity index
		//Activity* act=gt.rules.activitiesList.at(ai);
		if(ai!=UNALLOCATED_ACTIVITY){
			Activity* act=&gt.rules.internalActivitiesList[ai];
			assert(act!=NULL);
			s+=act->getDetailedDescriptionWithConstraints(gt.rules);
		}
		ai=students_timetable_week2[i][k][j]; //activity index
		//act=gt.rules.activitiesList.at(ai);
		if(ai!=UNALLOCATED_ACTIVITY){
			Activity* act=&gt.rules.internalActivitiesList[ai];
			assert(act!=NULL);
			s+="/\n";
			s+=act->getDetailedDescriptionWithConstraints(gt.rules);
		}
	}
	detailsTextEdit->setText(s);
}

void TimetableViewStudentsForm::lock()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	//find subgroup index
	assert(students_schedule_ready && teachers_schedule_ready);

	QString yearname;
	QString groupname;
	QString subgroupname;

	if(yearsListBox->currentText()==NULL)
		return;
	if(groupsListBox->currentText()==NULL)
		return;
	if(subgroupsListBox->currentText()==NULL)
		return;

	yearname = yearsListBox->currentText();
	groupname = groupsListBox->currentText();
	subgroupname = subgroupsListBox->currentText();

	TimeChromosome* tc=&best_time_chromosome;

	StudentsSubgroup* sts=(StudentsSubgroup*)gt.rules.searchStudentsSet(subgroupname);
	assert(sts);
	int i;
	for(i=0; i<gt.rules.nInternalSubgroups; i++)
		if(gt.rules.internalSubgroupsList[i]==sts)
			break;
	assert(i<gt.rules.nInternalSubgroups);

	//lock selected activities
	for(int j=0; j<gt.rules.nHoursPerDay; j++){
		for(int k=0; k<gt.rules.nDaysPerWeek; k++){
			if(studentsTimetableTable->isSelected(j+1, k+1)){
				int ai=students_timetable_week1[i][k][j];
				if(ai!=UNALLOCATED_ACTIVITY){
					int time=tc->times[ai];
					int hour=time/gt.rules.nDaysPerWeek;
					int day=time%gt.rules.nDaysPerWeek;
					//Activity* act=gt.rules.activitiesList.at(ai);
					Activity* act=&gt.rules.internalActivitiesList[ai];
					ConstraintActivityPreferredTime* ctr=new ConstraintActivityPreferredTime(1, true, act->id, day, hour);
					bool t=gt.rules.addTimeConstraint(ctr);
					if(t)
						QMessageBox::information(this, QObject::tr("FET information"), 
						 QObject::tr("Added the following constraint:\n"+ctr->getDetailedDescription(gt.rules)));
					else{
						QMessageBox::warning(this, QObject::tr("FET information"), 
						 QObject::tr("Constraint\n%1 NOT added - duplicate").arg(ctr->getDetailedDescription(gt.rules)));
						delete ctr;
					}
				}
				
				ai=students_timetable_week2[i][k][j];
				if(ai!=UNALLOCATED_ACTIVITY){
					int time=tc->times[ai];
					int hour=time/gt.rules.nDaysPerWeek;
					int day=time%gt.rules.nDaysPerWeek;
					//Activity* act=gt.rules.activitiesList.at(ai);
					Activity* act=&gt.rules.internalActivitiesList[ai];
					ConstraintActivityPreferredTime* ctr=new ConstraintActivityPreferredTime(1, true, act->id, day, hour);
					bool t=gt.rules.addTimeConstraint(ctr);
					if(t)
						QMessageBox::information(this, QObject::tr("FET information"), 
						 QObject::tr("Added the following constraint:\n"+ctr->getDetailedDescription(gt.rules)));
					else{
						QMessageBox::warning(this, QObject::tr("FET information"), 
						 QObject::tr("Constraint\n%1 NOT added - duplicate").arg(ctr->getDetailedDescription(gt.rules)));
						delete ctr;
					}
				}
			}
		}
	}
}
