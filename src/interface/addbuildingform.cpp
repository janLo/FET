/***************************************************************************
                          addbuildingform.cpp  -  description
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

#include "addbuildingform.h"

#include <qlineedit.h>

AddBuildingForm::AddBuildingForm()
{
}

AddBuildingForm::~AddBuildingForm()
{
}

void AddBuildingForm::addBuilding()
{
	if(nameLineEdit->text().isEmpty()){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Incorrect name"));
		return;
	}
	Building* bu=new Building();
	bu->name=nameLineEdit->text();
	if(!gt.rules.addBuilding(bu)){
		QMessageBox::information(this, QObject::tr("Building insertion dialog"),
			QObject::tr("Could not insert item. Must be a duplicate"));
		delete bu;
	}
	else{
		QMessageBox::information(this, QObject::tr("Building insertion dialog"),
			QObject::tr("Building added"));
	}

	nameLineEdit->selectAll();
	nameLineEdit->setFocus();
}
