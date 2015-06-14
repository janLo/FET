/***************************************************************************
                          timetableviewroomsform.cpp  -  description
                             -------------------
    begin                : Wed May 14 2003
    copyright            : (C) 2003 by Lalescu Liviu
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

#include "timetableviewroomsform.h"
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

extern bool students_schedule_ready;
extern bool teachers_schedule_ready;
extern bool rooms_schedule_ready;

extern bool simulation_running;

extern TimeChromosome best_time_chromosome;
extern SpaceChromosome best_space_chromosome;

TimetableViewRoomsForm::TimetableViewRoomsForm()
{
	roomsListBox->clear();
	for(int i=0; i<gt.rules.nInternalRooms; i++)
		roomsListBox->insertItem(gt.rules.internalRoomsList[i]->name);
	roomChanged(roomsListBox->currentText());
}

TimetableViewRoomsForm::~TimetableViewRoomsForm()
{
}

void TimetableViewRoomsForm::roomChanged(const QString &roomName)
{
	if(roomName==NULL)
		return;

	int roomIndex=gt.rules.searchRoom(roomName);
	if(roomIndex<0)
		return;

	updateRoomsTimetableTable();
}

void TimetableViewRoomsForm::updateRoomsTimetableTable(){
	assert(students_schedule_ready && teachers_schedule_ready);
	assert(rooms_schedule_ready);

	QString s;
	QString roomName;

	if(roomsListBox->currentItem() < 0)
		return;

	roomName = roomsListBox->currentText();
	s = roomName;
	roomNameTextLabel->setText(s);

	assert(gt.rules.initialized);
	roomsTimetableTable->setNumRows(gt.rules.nHoursPerDay+1);
	roomsTimetableTable->setNumCols(gt.rules.nDaysPerWeek+1);
	for(int j=0; j<gt.rules.nDaysPerWeek; j++)
		roomsTimetableTable->setText(0, j+1, gt.rules.daysOfTheWeek[j]);
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		roomsTimetableTable->setText(i+1, 0, gt.rules.hoursOfTheDay[i]);

	int roomIndex=gt.rules.searchRoom(roomName);
	assert(roomIndex>=0);
	for(int j=0; j<gt.rules.nHoursPerDay; j++){
		for(int k=0; k<gt.rules.nDaysPerWeek; k++){
			s = "";
			int ai=rooms_timetable_week1[roomIndex][k][j]; //activity index
			//Activity* act=gt.rules.activitiesList.at(ai);
			if(ai!=UNALLOCATED_ACTIVITY){
				Activity* act=&gt.rules.internalActivitiesList[ai];
				assert(act!=NULL);
				s += act->subjectName + " " + act->subjectTagName;
			}
			ai=rooms_timetable_week2[roomIndex][k][j]; //activity index
			//act=gt.rules.activitiesList.at(ai);
			if(ai!=UNALLOCATED_ACTIVITY){
				Activity* act=&gt.rules.internalActivitiesList[ai];
				assert(act!=NULL);
				s += "/" + act->subjectName + " " + act->subjectTagName;
			}
			roomsTimetableTable->setText(j+1, k+1, s);
		}
	}
	for(int i=0; i<=gt.rules.nHoursPerDay; i++)
		roomsTimetableTable->adjustRow(i); //added in version 3_12_20
}

void TimetableViewRoomsForm::detailActivity(int row, int col){
	assert(students_schedule_ready && teachers_schedule_ready);
	assert(rooms_schedule_ready);

	QString s;
	QString roomName;

	if(roomsListBox->currentText()==NULL)
		return;

	roomName = roomsListBox->currentText();
	s = roomName;
	roomNameTextLabel->setText(s);

	int roomIndex=gt.rules.searchRoom(roomName);
	assert(roomIndex>=0);
	int j=row-1;
	int k=col-1;
	s = "";
	if(j>=0 && k>=0){
		int ai=rooms_timetable_week1[roomIndex][k][j]; //activity index
		//Activity* act=gt.rules.activitiesList.at(ai);
		if(ai!=UNALLOCATED_ACTIVITY){
			Activity* act=&gt.rules.internalActivitiesList[ai];
			assert(act!=NULL);
			s += act->getDetailedDescriptionWithConstraints(gt.rules);
		}
		ai=rooms_timetable_week2[roomIndex][k][j]; //activity index
		//act=gt.rules.activitiesList.at(ai);
		if(ai!=UNALLOCATED_ACTIVITY){
			Activity* act=&gt.rules.internalActivitiesList[ai];
			assert(act!=NULL);
			s += "/\n";
			s += act->getDetailedDescriptionWithConstraints(gt.rules);
		}
	}
	detailsTextEdit->setText(s);
}

void TimetableViewRoomsForm::lock()
{
	this->lock(true, true);
}

void TimetableViewRoomsForm::lockTime()
{
	this->lock(true, false);
}

void TimetableViewRoomsForm::lockSpace()
{
	this->lock(false, true);
}

void TimetableViewRoomsForm::lock(bool lockTime, bool lockSpace)
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	//find room index
	QString roomName;
	if(roomsListBox->currentText()==NULL)
		return;
	roomName = roomsListBox->currentText();
	int i=gt.rules.searchRoom(roomName);

	assert(rooms_schedule_ready);
	SpaceChromosome* c=&best_space_chromosome;
	TimeChromosome* tc=&best_time_chromosome;

	//lock selected activities
	for(int j=0; j<gt.rules.nHoursPerDay; j++){
		for(int k=0; k<gt.rules.nDaysPerWeek; k++){
			if(roomsTimetableTable->isSelected(j+1, k+1)){
				int ai=rooms_timetable_week1[i][k][j];
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
				
				ai=rooms_timetable_week2[i][k][j];
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
