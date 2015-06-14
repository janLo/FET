/***************************************************************************
                          addstudentsyearform.cpp  -  description
                             -------------------
    begin                : Sat Jan 24 2004
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

#include "addstudentsyearform.h"

#include <qlineedit.h>

AddStudentsYearForm::AddStudentsYearForm()
{
}

AddStudentsYearForm::~AddStudentsYearForm()
{
}

void AddStudentsYearForm::addStudentsYear()
{
	if(nameLineEdit->text().isEmpty()){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Incorrect name"));
		return;
	}
	StudentsYear* sy=new StudentsYear();
	sy->name=nameLineEdit->text();
	sy->numberOfStudents=numberSpinBox->value();

	if(gt.rules.searchYear(sy->name) >=0 ){
		QMessageBox::information( this, QObject::tr("Year insertion dialog"),
		QObject::tr("Could not insert item. Must be a duplicate"));
		delete sy;
	}
	else{
		bool tmp=gt.rules.addYear(sy);
		assert(tmp);

		QMessageBox::information(this, QObject::tr("Year insertion dialog"),
			QObject::tr("Year added"));
	}

	nameLineEdit->selectAll();
	nameLineEdit->setFocus();
}
