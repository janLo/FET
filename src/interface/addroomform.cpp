/***************************************************************************
                          addroomform.cpp  -  description
                             -------------------
    begin                : Sun Jan 1 2004
    copyright            : (C) 2004 by Lalescu Liviu
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

#include "addroomform.h"

#include <qlineedit.h>

AddRoomForm::AddRoomForm()
{
}

AddRoomForm::~AddRoomForm()
{
}

void AddRoomForm::addRoom()
{
	if(nameLineEdit->text().isEmpty()){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Incorrect name"));
		return;
	}
	if(typeLineEdit->text().isEmpty()){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Incorrect type"));
		return;
	}
	Room* rm=new Room();
	rm->name=nameLineEdit->text();
	rm->type=typeLineEdit->text();
	rm->capacity=capacitySpinBox->value();
	if(!gt.rules.addRoom(rm)){
		QMessageBox::information(this, QObject::tr("Room insertion dialog"),
			QObject::tr("Could not insert item. Must be a duplicate"));
		delete rm;
	}
	else{
		QMessageBox::information(this, QObject::tr("Room insertion dialog"),
			QObject::tr("Room added"));
	}

	nameLineEdit->selectAll();
	nameLineEdit->setFocus();
}
