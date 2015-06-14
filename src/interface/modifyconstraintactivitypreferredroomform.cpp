/***************************************************************************
                          modifyconstraintactivitypreferredroomform.cpp  -  description
                             -------------------
    begin                : 13 Feb 2005
    copyright            : (C) 2005 by Liviu Lalescu
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

#include "modifyconstraintactivitypreferredroomform.h"
#include "spaceconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

ModifyConstraintActivityPreferredRoomForm::ModifyConstraintActivityPreferredRoomForm(ConstraintActivityPreferredRoom* ctr)
{
	this->_ctr=ctr;
	
	compulsoryCheckBox->setChecked(ctr->compulsory);
	weightLineEdit->setText(QString::number(ctr->weight));

	updateActivitiesComboBox();
	updateRoomsComboBox();
}

ModifyConstraintActivityPreferredRoomForm::~ModifyConstraintActivityPreferredRoomForm()
{
}

void ModifyConstraintActivityPreferredRoomForm::updateActivitiesComboBox()
{
	int i=0, j=-1;
	activitiesComboBox->clear();
	for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next()){
		activitiesComboBox->insertItem(act->getDescription(gt.rules));
		if(act->id==this->_ctr->activityId)
			j=i;
		i++;
	}
	assert(j>=0);
	activitiesComboBox->setCurrentItem(j);
}

void ModifyConstraintActivityPreferredRoomForm::updateRoomsComboBox()
{
	int i=0, j=-1;
	roomsComboBox->clear();
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next()){
		roomsComboBox->insertItem(rm->name);
		if(rm->name==this->_ctr->roomName)
			j=i;
		i++;
	}
	assert(j>=0);
	roomsComboBox->setCurrentItem(j);
}

void ModifyConstraintActivityPreferredRoomForm::cancel()
{
	this->close();
}

void ModifyConstraintActivityPreferredRoomForm::ok()
{
	double weight;
	QString tmp=weightLineEdit->text();
	sscanf(tmp, "%lf", &weight);
	if(weight<0.0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid weight"));
		return;
	}

	bool compulsory=false;
	if(compulsoryCheckBox->isChecked())
		compulsory=true;

	int i=activitiesComboBox->currentItem();
	if(i<0 || activitiesComboBox->count()<=0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid activity"));
		return;
	}
	Activity* act=gt.rules.activitiesList.at(i);

	i=roomsComboBox->currentItem();
	if(i<0 || roomsComboBox->count()<=0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid room"));
		return;
	}
	QString room=roomsComboBox->currentText();

	this->_ctr->weight=weight;
	this->_ctr->compulsory=compulsory;
	this->_ctr->roomName=room;
	this->_ctr->activityId=act->id;

	gt.rules.internalStructureComputed=false;
	
	this->close();
}
