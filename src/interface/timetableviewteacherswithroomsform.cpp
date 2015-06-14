/***************************************************************************
                          timetableviewteacherswithroomsform.cpp  -  description
                             -------------------
    begin                : Feb 6 2005
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

#include "timetableviewteacherswithroomsform.h"
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

extern bool students_schedule_ready, teachers_schedule_ready, rooms_schedule_ready;

TimetableViewTeachersWithRoomsForm::TimetableViewTeachersWithRoomsForm()
{
	teachersListBox->clear();
	for(int i=0; i<gt.rules.nInternalTeachers; i++)
		teachersListBox->insertItem(gt.rules.internalTeachersList[i]->name);
	teacherChanged(teachersListBox->currentText());
}

TimetableViewTeachersWithRoomsForm::~TimetableViewTeachersWithRoomsForm()
{
}

void TimetableViewTeachersWithRoomsForm::teacherChanged(const QString &teacherName)
{
	if(teacherName==NULL)
		return;

	int teacherId=gt.rules.searchTeacher(teacherName);
	if(teacherId<0)
		return;

	updateTeachersTimetableTable();
}

void TimetableViewTeachersWithRoomsForm::updateTeachersTimetableTable(){
	assert(students_schedule_ready && teachers_schedule_ready && rooms_schedule_ready);

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
		
	assert(rooms_schedule_ready);
	SpaceChromosome& c=gt.spacePopulation.bestChromosome(gt.rules);

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
				Activity* act=gt.rules.activitiesList.at(ai);
				if(ai!=UNALLOCATED_ACTIVITY){
					assert(act!=NULL);
					s += act->subjectName+" "+act->subjectTagName+"\n"; //added in version 3_9_16, on 16 Oct. 2004; suggested by Nicholas Robinson
					for(QStringList::Iterator it=act->studentsNames.begin(); it!=act->studentsNames.end(); it++)
						s += (*it) + " ";
						
					s+="\n";
					int ri=c.rooms[ai];
					QString roomName;
					if(ri==UNALLOCATED_SPACE)
						roomName="UNALLOCATED_SPACE";
					else
						roomName=gt.rules.roomsList.at(ri)->name;
					s+=roomName;
				}
				ai=teachers_timetable_week2[teacher][k][j]; //activity index
				act=gt.rules.activitiesList.at(ai);
				if(ai!=UNALLOCATED_ACTIVITY && !act->studentsNames.isEmpty())
					s += "\n/\n";
				if(ai!=UNALLOCATED_ACTIVITY){
					assert(act!=NULL);
					s += act->subjectName+" "+act->subjectTagName+"\n"; //added in version 3_9_16, on 16 Oct. 2004; suggested by Nicholas Robinson
					for(QStringList::Iterator it=act->studentsNames.begin(); it!=act->studentsNames.end(); it++)
						s += (*it) + " ";

					s+="\n";
					int ri=c.rooms[ai];
					QString roomName;
					if(ri==UNALLOCATED_SPACE)
						roomName="UNALLOCATED_SPACE";
					else
						roomName=gt.rules.roomsList.at(ri)->name;
					s+=roomName;
				}
				teachersTimetableTable->setText(j+1, k+1, s);
			}
		for(int i=0; i<=gt.rules.nHoursPerDay; i++) //added in version 3_9_16, on 16 Oct. 2004
			teachersTimetableTable->adjustRow(i);
		}
	}
}

void TimetableViewTeachersWithRoomsForm::detailActivity(int row, int col){
	assert(students_schedule_ready && teachers_schedule_ready && rooms_schedule_ready);

	QString s;
	QString teachername;

	if(teachersListBox->currentText()==NULL)
		return;

	teachername = teachersListBox->currentText();

	s = teachername;

	teacherNameTextLabel->setText(s);

	assert(rooms_schedule_ready);
	SpaceChromosome& c=gt.spacePopulation.bestChromosome(gt.rules);

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
			Activity* act=gt.rules.activitiesList.at(ai);
			if(ai!=UNALLOCATED_ACTIVITY){
				assert(act!=NULL);
				s += act->getDetailedDescription(gt.rules);
				
				int ri=c.rooms[ai]; //room index;
				QString roomName;
				if(ri==UNALLOCATED_SPACE)
					roomName="UNALLOCATED_SPACE";
				else
					roomName=gt.rules.roomsList.at(ri)->name;
				s+=QObject::tr("Room=%1").arg(roomName);
				s+="\n";
			}
			ai=teachers_timetable_week2[teacher][k][j]; //activity index
			act=gt.rules.activitiesList.at(ai);
			if(ai!=UNALLOCATED_ACTIVITY){
				assert(act!=NULL);
				s += "/\n";
				s += act->getDetailedDescription(gt.rules);

				int ri=c.rooms[ai]; //room index;
				QString roomName;
				if(ri==UNALLOCATED_SPACE)
					roomName="UNALLOCATED_SPACE";
				else
					roomName=gt.rules.roomsList.at(ri)->name;
				s+=QObject::tr("Room=%1").arg(roomName);
				s+="\n";

			}
		}
		detailsTextEdit->setText(s);
	}
}
