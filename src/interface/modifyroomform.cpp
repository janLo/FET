/***************************************************************************
                          modifyroomform.cpp  -  description
                             -------------------
    begin                : Feb 12, 2005
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

#include "modifyroomform.h"

#include <qlineedit.h>

ModifyRoomForm::ModifyRoomForm(const QString& initialRoomName, const QString& initialRoomType, int initialRoomCapacity)
{
	this->_initialRoomName=initialRoomName;
	this->_initialRoomType=initialRoomType;
	this->_initialRoomCapacity=initialRoomCapacity;
	capacitySpinBox->setValue(initialRoomCapacity);
	nameLineEdit->setText(initialRoomName);
	nameLineEdit->selectAll();
	nameLineEdit->setFocus();
	typeLineEdit->setText(initialRoomType);	
}

ModifyRoomForm::~ModifyRoomForm()
{
}

void ModifyRoomForm::cancel()
{
	this->close();
}

void ModifyRoomForm::ok()
{
	if(nameLineEdit->text().isEmpty()){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Incorrect name"));
		return;
	}
	if(typeLineEdit->text().isEmpty()){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Incorrect type"));
		return;
	}
	if(this->_initialRoomName!=nameLineEdit->text() && gt.rules.searchRoom(nameLineEdit->text())>=0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Name existing - please choose another"));
		return;
	}
	
	bool t=gt.rules.modifyRoom(this->_initialRoomName, nameLineEdit->text(), typeLineEdit->text(), capacitySpinBox->value());
	assert(t);
	
	this->close();
}
