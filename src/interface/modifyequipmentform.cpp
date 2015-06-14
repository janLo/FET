/***************************************************************************
                          modifyequipmentform.cpp  -  description
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

#include "modifyequipmentform.h"

#include <qlineedit.h>

ModifyEquipmentForm::ModifyEquipmentForm(const QString& initialEquipmentName)
{
	this->_initialEquipmentName=initialEquipmentName;
	nameLineEdit->setText(initialEquipmentName);
	nameLineEdit->selectAll();
	nameLineEdit->setFocus();
}

ModifyEquipmentForm::~ModifyEquipmentForm()
{
}

void ModifyEquipmentForm::ok()
{
	if(nameLineEdit->text().isEmpty()){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Incorrect name"));
		return;
	}
	
	if(this->_initialEquipmentName!=nameLineEdit->text() && gt.rules.searchEquipment(nameLineEdit->text())>=0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Name existing - please choose another"));
		return;
	}
	
	bool t=gt.rules.modifyEquipment(this->_initialEquipmentName, nameLineEdit->text());
	assert(t);	

	this->close();
}

void ModifyEquipmentForm::cancel()
{
	this->close();
}
