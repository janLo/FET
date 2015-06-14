/***************************************************************************
                          addconstraintactivitypreferredroomform.cpp  -  description
                             -------------------
    begin                : 7 Feb 2005
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

#include "addconstraintactivitypreferredroomform.h"
#include "spaceconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

AddConstraintActivityPreferredRoomForm::AddConstraintActivityPreferredRoomForm()
{
	updateActivitiesComboBox();
	updateRoomsComboBox();
}

AddConstraintActivityPreferredRoomForm::~AddConstraintActivityPreferredRoomForm()
{
}

void AddConstraintActivityPreferredRoomForm::updateActivitiesComboBox()
{
	activitiesComboBox->clear();
	for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next())
		activitiesComboBox->insertItem(act->getDescription(gt.rules));
}

void AddConstraintActivityPreferredRoomForm::updateRoomsComboBox()
{
	roomsComboBox->clear();
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next())
		roomsComboBox->insertItem(rm->name);
}

void AddConstraintActivityPreferredRoomForm::addConstraint()
{
	SpaceConstraint *ctr=NULL;

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

	ctr=new ConstraintActivityPreferredRoom(weight, compulsory, act->id, room);

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
