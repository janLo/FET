/***************************************************************************
                          modifyconstraintmaxbuildingchangesperdayforstudentsform.cpp  -  description
                             -------------------
    begin                : July 11, 2005
    copyright            : (C) 2005 by Lalescu Liviu
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

#include "modifyconstraintmaxbuildingchangesperdayforstudentsform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>

#include <QDesktopWidget>

#define yesNo(x)	((x)==0?QObject::tr("no"):QObject::tr("yes"))

ModifyConstraintMaxBuildingChangesPerDayForStudentsForm::ModifyConstraintMaxBuildingChangesPerDayForStudentsForm(ConstraintMaxBuildingChangesPerDayForStudents* ctr)
{
	//setWindowFlags(Qt::Window);
	setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	QDesktopWidget* desktop=QApplication::desktop();
	int xx=desktop->width()/2 - frameGeometry().width()/2;
	int yy=desktop->height()/2 - frameGeometry().height()/2;
	move(xx, yy);

	this->_ctr=ctr;
	
	compulsoryCheckBox->setChecked(ctr->compulsory);
	weightLineEdit->setText(QString::number(ctr->weight));	
	maxBuildingChangesSpinBox->setValue(ctr->maxBuildingChanges);
}

ModifyConstraintMaxBuildingChangesPerDayForStudentsForm::~ModifyConstraintMaxBuildingChangesPerDayForStudentsForm()
{
}

void ModifyConstraintMaxBuildingChangesPerDayForStudentsForm::constraintChanged()
{
	QString s;
	s+=QObject::tr("Current constraint:");
	s+="\n";
	s+=QObject::tr("Max building changes per day for students");
	s+="\n";

	double weight;
	QString tmp=weightLineEdit->text();
	sscanf(tmp, "%lf", &weight);
	s+=QObject::tr("Weight=%1").arg(weight);
	s+="\n";

	bool compulsory=false;
	if(compulsoryCheckBox->isChecked())
		compulsory=true;
	s+=QObject::tr("Compulsory=%1").arg(yesNo(compulsory));
	s+="\n";
	
	s+=QObject::tr("Max building changes=%1").arg(maxBuildingChangesSpinBox->value());
	s+="\n";

	currentConstraintTextEdit->setText(s);
}

void ModifyConstraintMaxBuildingChangesPerDayForStudentsForm::ok()
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

	this->_ctr->weight=weight;
	this->_ctr->compulsory=compulsory;
	this->_ctr->maxBuildingChanges=maxBuildingChangesSpinBox->value();
	
	gt.rules.internalStructureComputed=false;

	this->close();
}

void ModifyConstraintMaxBuildingChangesPerDayForStudentsForm::cancel()
{
	this->close();
}
