/***************************************************************************
                          modifyconstraintminimizenumberofroomsforstudentsform.cpp  -  description
                             -------------------
    begin                : 13 Feb. 2005
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

#include "modifyconstraintminimizenumberofroomsforstudentsform.h"
#include "spaceconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

ModifyConstraintMinimizeNumberOfRoomsForStudentsForm::ModifyConstraintMinimizeNumberOfRoomsForStudentsForm(ConstraintMinimizeNumberOfRoomsForStudents* ctr)
{
	this->_ctr=ctr;
	
	compulsoryCheckBox->setChecked(ctr->compulsory);
	weightLineEdit->setText(QString::number(ctr->weight));
}

ModifyConstraintMinimizeNumberOfRoomsForStudentsForm::~ModifyConstraintMinimizeNumberOfRoomsForStudentsForm()
{
}

void ModifyConstraintMinimizeNumberOfRoomsForStudentsForm::cancel()
{
	this->close();
}

void ModifyConstraintMinimizeNumberOfRoomsForStudentsForm::ok()
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

	this->_ctr->weight=weight;
	this->_ctr->compulsory=compulsory;

	gt.rules.internalStructureComputed=false;
	
	this->close();
}
