/***************************************************************************
                          timetableviewrooms2form.cpp  -  description
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

#include "timetableviewrooms2form.h"
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

TimetableViewRooms2Form::TimetableViewRooms2Form()
{
	roomsListBox->clear();
	for(int i=0; i<gt.rules.nInternalRooms; i++)
		roomsListBox->insertItem(gt.rules.internalRoomsList[i]->name);
	roomChanged(roomsListBox->currentText());
}

TimetableViewRooms2Form::~TimetableViewRooms2Form()
{
}

void TimetableViewRooms2Form::roomChanged(const QString &roomName)
{
	if(roomName==NULL)
		return;

	int roomIndex=gt.rules.searchRoom(roomName);
	if(roomIndex<0)
		return;

	updateRoomsTimetableTable();
}

void TimetableViewRooms2Form::updateRoomsTimetableTable(){
	assert(students_schedule_ready2 && teachers_schedule_ready2);
	assert(rooms_schedule_ready2);

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
			Activity* act=gt.rules.activitiesList.at(ai);
			if(ai!=UNALLOCATED_ACTIVITY){
				assert(act!=NULL);
				s += act->subjectName + " " + act->subjectTagName;
			}
			ai=rooms_timetable_week2[roomIndex][k][j]; //activity index
			act=gt.rules.activitiesList.at(ai);
			if(ai!=UNALLOCATED_ACTIVITY){
				assert(act!=NULL);
				s += "/" + act->subjectName + " " + act->subjectTagName;
			}
			roomsTimetableTable->setText(j+1, k+1, s);
		}
	}
	for(int i=0; i<=gt.rules.nHoursPerDay; i++)
		roomsTimetableTable->adjustRow(i); //added in version 3_12_20
}

void TimetableViewRooms2Form::detailActivity(int row, int col){
	assert(students_schedule_ready2 && teachers_schedule_ready2);
	assert(rooms_schedule_ready2);

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
		Activity* act=gt.rules.activitiesList.at(ai);
		if(ai!=UNALLOCATED_ACTIVITY){
			assert(act!=NULL);
			s += act->getDetailedDescription(gt.rules);
		}
		ai=rooms_timetable_week2[roomIndex][k][j]; //activity index
		act=gt.rules.activitiesList.at(ai);
		if(ai!=UNALLOCATED_ACTIVITY){
			assert(act!=NULL);
			s += "/\n";
			s += act->getDetailedDescription(gt.rules);
		}
	}
	detailsTextEdit->setText(s);
}
