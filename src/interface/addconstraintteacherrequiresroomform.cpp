/***************************************************************************
                          addconstraintteacherrequiresroomform.cpp  -  description
                             -------------------
    begin                : Mon 13 Sept 2004
    copyright            : (C) 2004 by Liviu Lalescu
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

#include "addconstraintteacherrequiresroomform.h"
#include "spaceconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

AddConstraintTeacherRequiresRoomForm::AddConstraintTeacherRequiresRoomForm()
{
	updateTeachersComboBox();
	updateRoomsComboBox();
}

AddConstraintTeacherRequiresRoomForm::~AddConstraintTeacherRequiresRoomForm()
{
}

void AddConstraintTeacherRequiresRoomForm::updateTeachersComboBox()
{
	teachersComboBox->clear();
	for(Teacher* tc=gt.rules.teachersList.first(); tc; tc=gt.rules.teachersList.next())
		teachersComboBox->insertItem(tc->name);
}

void AddConstraintTeacherRequiresRoomForm::updateRoomsComboBox()
{
	roomsComboBox->clear();
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next())
		roomsComboBox->insertItem(rm->name);
}

void AddConstraintTeacherRequiresRoomForm::addConstraint()
{
	SpaceConstraint *ctr=NULL;

	double weight;
	QString tmp=weightLineEdit->text();
	sscanf(tmp, "%lf", &weight);
	if(weight<=0.0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid weight"));
		return;
	}

	bool compulsory=false;
	if(compulsoryCheckBox->isChecked())
		compulsory=true;

	int i=teachersComboBox->currentItem();
	if(i<0 || teachersComboBox->count()<=0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid teacher"));
		return;
	}
	QString teacher=teachersComboBox->currentText();

	i=roomsComboBox->currentItem();
	if(i<0 || roomsComboBox->count()<=0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid room"));
		return;
	}
	QString room=roomsComboBox->currentText();

	ctr=new ConstraintTeacherRequiresRoom(weight, compulsory, teacher, room);

	bool tmp2=gt.rules.addSpaceConstraint(ctr);
	if(tmp2){
		QString s=QObject::tr("Constraint added:");
		s+="\n";
		s+=ctr->getDetailedDescription(gt.rules);
		QMessageBox::information(this, QObject::tr("FET information"), s);
	}
	else{
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Constraint NOT added - error ?"));
		delete ctr;
	}
}
