/***************************************************************************
                          timetableviewteacherswithrooms2form.cpp  -  description
                             -------------------
    begin                : Feb 14, 2005
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

#include "timetableviewteacherswithrooms2form.h"
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
#include <qtextedit.h>
#include <qstring.h>
#include <qlistbox.h>
#include <qlabel.h>
#include <qtable.h>

extern bool students_schedule_ready2;
extern bool teachers_schedule_ready2;
extern bool rooms_schedule_ready2;

extern bool simulation_running;

extern TimeSpaceChromosome best_time_space_chromosome;

TimetableViewTeachersWithRooms2Form::TimetableViewTeachersWithRooms2Form()
{
	teachersListBox->clear();
	for(int i=0; i<gt.rules.nInternalTeachers; i++)
		teachersListBox->insertItem(gt.rules.internalTeachersList[i]->name);
	teacherChanged(teachersListBox->currentText());
}

TimetableViewTeachersWithRooms2Form::~TimetableViewTeachersWithRooms2Form()
{
}

void TimetableViewTeachersWithRooms2Form::teacherChanged(const QString &teacherName)
{
	if(teacherName==NULL)
		return;

	int teacherId=gt.rules.searchTeacher(teacherName);
	if(teacherId<0)
		return;

	updateTeachersTimetableTable();
}

void TimetableViewTeachersWithRooms2Form::updateTeachersTimetableTable(){
	assert(students_schedule_ready2 && teachers_schedule_ready2 && rooms_schedule_ready2);

	QString s;
	QString teachername;

	if(teachersListBox->currentText()==NULL)
		return;

	teachername = teachersListBox->currentText();

	s = teachername;
	teacherNameTextLabel->setText(s);

	assert(gt.rules.initialized);
	teachersTimetableTable->setNumRows(gt.rules.nHoursPerDay+1);
	teachersTimetableTable->setNumCols(gt.rules.nDaysPerWeek+1);
	for(int j=0; j<gt.rules.nDaysPerWeek; j++)
		teachersTimetableTable->setText(0, j+1, gt.rules.daysOfTheWeek[j]);
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		teachersTimetableTable->setText(i+1, 0, gt.rules.hoursOfTheDay[i]);
		
	assert(rooms_schedule_ready2);
	SpaceChromosome* c=(SpaceChromosome*)(&best_time_space_chromosome);

	int teacher=gt.rules.searchTeacher(teachername);
	if(teacher<0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("The teacher's timetable cannot be printed,\nbecause the teacher is invalid"));
	}
	else{
		for(int j=0; j<gt.rules.nHoursPerDay; j++){
			for(int k=0; k<gt.rules.nDaysPerWeek; k++){
				s = "";
				int ai=teachers_timetable_week1[teacher][k][j]; //activity index
				//Activity* act=gt.rules.activitiesList.at(ai);
				if(ai!=UNALLOCATED_ACTIVITY){
					Activity* act=&gt.rules.internalActivitiesList[ai];
					assert(act!=NULL);
					s += act->subjectName+" "+act->subjectTagName+"\n";
					for(QStringList::Iterator it=act->studentsNames.begin(); it!=act->studentsNames.end(); it++)
						s += (*it) + " ";
						
					s+="\n";
					int ri=c->rooms[ai];
					QString roomName;
					if(ri==UNALLOCATED_SPACE)
						roomName="UNALLOCATED_SPACE";
					else
						roomName=gt.rules.internalRoomsList[ri]->name;
					s+=roomName;
				}
				ai=teachers_timetable_week2[teacher][k][j]; //activity index
				//act=gt.rules.activitiesList.at(ai);
				if(ai!=UNALLOCATED_ACTIVITY){
					Activity* act=&gt.rules.internalActivitiesList[ai];
					assert(act!=NULL);
					if(!act->studentsNames.isEmpty())
						s += "\n/\n";
					s += act->subjectName+" "+act->subjectTagName+"\n";
					for(QStringList::Iterator it=act->studentsNames.begin(); it!=act->studentsNames.end(); it++)
						s += (*it) + " ";

					s+="\n";
					int ri=c->rooms[ai];
					QString roomName;
					if(ri==UNALLOCATED_SPACE)
						roomName="UNALLOCATED_SPACE";
					else
						roomName=gt.rules.internalRoomsList[ri]->name;
					s+=roomName;
				}
				teachersTimetableTable->setText(j+1, k+1, s);
			}
		for(int i=0; i<=gt.rules.nHoursPerDay; i++) //added in version 3_9_16, on 16 Oct. 2004
			teachersTimetableTable->adjustRow(i);
		}
	}
}

void TimetableViewTeachersWithRooms2Form::detailActivity(int row, int col){
	assert(students_schedule_ready2 && teachers_schedule_ready2 && rooms_schedule_ready2);

	QString s;
	QString teachername;

	if(teachersListBox->currentText()==NULL)
		return;

	teachername = teachersListBox->currentText();

	s = teachername;

	teacherNameTextLabel->setText(s);

	assert(rooms_schedule_ready2);
	SpaceChromosome* c=(SpaceChromosome*)(&best_time_space_chromosome);

	int teacher=gt.rules.searchTeacher(teachername);
	if(teacher<0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("The teacher is invalid"));
	}
	else{
		int j=row-1;
		int k=col-1;
		s = "";
		if(j>=0 && k>=0){
			int ai=teachers_timetable_week1[teacher][k][j]; //activity index
			//Activity* act=gt.rules.activitiesList.at(ai);
			if(ai!=UNALLOCATED_ACTIVITY){
				Activity* act=&gt.rules.internalActivitiesList[ai];
				assert(act!=NULL);
				s += act->getDetailedDescriptionWithConstraints(gt.rules);
				
				int ri=c->rooms[ai]; //room index;
				QString roomName;
				if(ri==UNALLOCATED_SPACE)
					roomName="UNALLOCATED_SPACE";
				else
					roomName=gt.rules.internalRoomsList[ri]->name;
				s+=QObject::tr("Room=%1").arg(roomName);
				s+="\n";
			}
			ai=teachers_timetable_week2[teacher][k][j]; //activity index
			//act=gt.rules.activitiesList.at(ai);
			if(ai!=UNALLOCATED_ACTIVITY){
				Activity* act=&gt.rules.internalActivitiesList[ai];
				assert(act!=NULL);
				s += "/\n";
				s += act->getDetailedDescriptionWithConstraints(gt.rules);

				int ri=c->rooms[ai]; //room index;
				QString roomName;
				if(ri==UNALLOCATED_SPACE)
					roomName="UNALLOCATED_SPACE";
				else
					roomName=gt.rules.internalRoomsList[ri]->name;
				s+=QObject::tr("Room=%1").arg(roomName);
				s+="\n";

			}
		}
		detailsTextEdit->setText(s);
	}
}

void TimetableViewTeachersWithRooms2Form::lockTime()
{
	this->lock(true, false);
}

void TimetableViewTeachersWithRooms2Form::lockSpace()
{
	this->lock(false, true);
}

void TimetableViewTeachersWithRooms2Form::lock()
{
	this->lock(true, true);
}

void TimetableViewTeachersWithRooms2Form::lock(bool lockTime, bool lockSpace)
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	//find teacher index
	QString teachername;

	if(teachersListBox->currentText()==NULL)
		return;

	teachername = teachersListBox->currentText();
	int i=gt.rules.searchTeacher(teachername);

	assert(rooms_schedule_ready2);
	SpaceChromosome* c=(SpaceChromosome*)(&best_time_space_chromosome);
	TimeChromosome* tc=(TimeChromosome*)(&best_time_space_chromosome);

	//lock selected activities
	for(int j=0; j<gt.rules.nHoursPerDay; j++){
		for(int k=0; k<gt.rules.nDaysPerWeek; k++){
			if(teachersTimetableTable->isSelected(j+1, k+1)){
				int ai=teachers_timetable_week1[i][k][j];
				if(ai!=UNALLOCATED_ACTIVITY){
					int time=tc->times[ai];
					int hour=time/gt.rules.nDaysPerWeek;
					int day=time%gt.rules.nDaysPerWeek;
					//Activity* act=gt.rules.activitiesList.at(ai);
					Activity* act=&gt.rules.internalActivitiesList[ai];
					if(lockTime){
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
					
					int ri=c->rooms[ai];
					if(ri!=UNALLOCATED_SPACE && lockSpace){
						ConstraintActivityPreferredRoom* ctr=new ConstraintActivityPreferredRoom(1, true, act->id, (gt.rules.internalRoomsList[ri])->name);
						bool t=gt.rules.addSpaceConstraint(ctr);
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
				
				ai=teachers_timetable_week2[i][k][j];
				if(ai!=UNALLOCATED_ACTIVITY){
					int time=tc->times[ai];
					int hour=time/gt.rules.nDaysPerWeek;
					int day=time%gt.rules.nDaysPerWeek;
					//Activity* act=gt.rules.activitiesList.at(ai);
					Activity* act=&gt.rules.internalActivitiesList[ai];
					if(lockTime){
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
					
					int ri=c->rooms[ai];
					if(ri!=UNALLOCATED_SPACE && lockSpace){
						ConstraintActivityPreferredRoom* ctr=new ConstraintActivityPreferredRoom(1, true, act->id, (gt.rules.internalRoomsList[ri])->name);
						bool t=gt.rules.addSpaceConstraint(ctr);
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
}
