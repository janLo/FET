/***************************************************************************
                          modifybuildingform.cpp  -  description
                             -------------------
    begin                : July 9, 2005
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

#include "modifybuildingform.h"

#include <qlineedit.h>

ModifyBuildingForm::ModifyBuildingForm(const QString& initialBuildingName)
{
	this->_initialBuildingName=initialBuildingName;
	nameLineEdit->setText(initialBuildingName);
	nameLineEdit->selectAll();
	nameLineEdit->setFocus();
}

ModifyBuildingForm::~ModifyBuildingForm()
{
}

void ModifyBuildingForm::ok()
{
	if(nameLineEdit->text().isEmpty()){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Incorrect name"));
		return;
	}
	
	if(this->_initialBuildingName!=nameLineEdit->text() && gt.rules.searchBuilding(nameLineEdit->text())>=0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Name existing - please choose another"));
		return;
	}
	
	bool t=gt.rules.modifyBuilding(this->_initialBuildingName, nameLineEdit->text());
	assert(t);	

	this->close();
}

void ModifyBuildingForm::cancel()
{
	this->close();
}
