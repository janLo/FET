/***************************************************************************
                          modifyconstraintteacherrequiresroomform.cpp  -  description
                             -------------------
    begin                : 13 Feb 2005
    copyright            : (C) 2005 by Liviu Lalescu
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

#include "modifyconstraintteacherrequiresroomform.h"
#include "spaceconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

ModifyConstraintTeacherRequiresRoomForm::ModifyConstraintTeacherRequiresRoomForm(ConstraintTeacherRequiresRoom* ctr)
{
	this->_ctr=ctr;
	
	compulsoryCheckBox->setChecked(ctr->compulsory);
	weightLineEdit->setText(QString::number(ctr->weight));

	updateTeachersComboBox();
	updateRoomsComboBox();
}

ModifyConstraintTeacherRequiresRoomForm::~ModifyConstraintTeacherRequiresRoomForm()
{
}

void ModifyConstraintTeacherRequiresRoomForm::updateTeachersComboBox()
{
	teachersComboBox->clear();
	int i=0, j=-1;
	for(Teacher* tc=gt.rules.teachersList.first(); tc; tc=gt.rules.teachersList.next()){
		teachersComboBox->insertItem(tc->name);
		if(tc->name==this->_ctr->teacherName)
			j=i;
		i++;
	}
	assert(j>=0);
	teachersComboBox->setCurrentItem(j);
}

void ModifyConstraintTeacherRequiresRoomForm::updateRoomsComboBox()
{
	roomsComboBox->clear();
	int i=0, j=-1;
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next()){
		roomsComboBox->insertItem(rm->name);
		if(rm->name==this->_ctr->roomName)
			j=i;
		i++;
	}
	assert(j>=0);
	roomsComboBox->setCurrentItem(j);
}

void ModifyConstraintTeacherRequiresRoomForm::cancel()
{
	this->close();
}

void ModifyConstraintTeacherRequiresRoomForm::ok()
{
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

	this->_ctr->teacherName=teacher;
	this->_ctr->weight=weight;
	this->_ctr->compulsory=compulsory;
	this->_ctr->roomName=room;
	
	gt.rules.internalStructureComputed=false;
	
	this->close();
}
