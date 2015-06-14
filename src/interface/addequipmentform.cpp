/***************************************************************************
                          addequipmentform.cpp  -  description
                             -------------------
    begin                : Thu Jan 29 2004
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

#include "addequipmentform.h"

#include <qlineedit.h>

AddEquipmentForm::AddEquipmentForm()
{
}

AddEquipmentForm::~AddEquipmentForm()
{
}

void AddEquipmentForm::addEquipment()
{
	if(nameLineEdit->text().isEmpty()){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Incorrect name"));
		return;
	}
	Equipment* eq=new Equipment();
	eq->name=nameLineEdit->text();
	if(!gt.rules.addEquipment(eq)){
		QMessageBox::information(this, QObject::tr("Equipment insertion dialog"),
			QObject::tr("Could not insert item. Must be a duplicate"));
		delete eq;
	}
	else{
		QMessageBox::information(this, QObject::tr("Equipment insertion dialog"),
			QObject::tr("Equipment added"));
	}

	nameLineEdit->selectAll();
	nameLineEdit->setFocus();
}
