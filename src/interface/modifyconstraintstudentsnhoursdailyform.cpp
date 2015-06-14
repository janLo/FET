/***************************************************************************
                          modifyconstraintstudentsnhoursdailyform.cpp  -  description
                             -------------------
    begin                : Feb 11, 2005
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

#include "modifyconstraintstudentsnhoursdailyform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>

#include <QDesktopWidget>

#define yesNo(x)	((x)==0?QObject::tr("no"):QObject::tr("yes"))

ModifyConstraintStudentsNHoursDailyForm::ModifyConstraintStudentsNHoursDailyForm(ConstraintStudentsNHoursDaily* ctr)
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
	
	maxHoursSpinBox->setMinValue(-1);
	maxHoursSpinBox->setMaxValue(gt.rules.nHoursPerDay);
	maxHoursSpinBox->setValue(ctr->maxHoursDaily);
	minHoursSpinBox->setMinValue(-1);
	minHoursSpinBox->setMaxValue(gt.rules.nHoursPerDay);
	minHoursSpinBox->setValue(ctr->minHoursDaily);
}

ModifyConstraintStudentsNHoursDailyForm::~ModifyConstraintStudentsNHoursDailyForm()
{
}

void ModifyConstraintStudentsNHoursDailyForm::constraintChanged()
{
	QString s;
	s+=QObject::tr("Current constraint:");
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

	s+=QObject::tr("Students recommended n hours daily");
	s+="\n";

	s+=QObject::tr("Min hours:%1").arg(minHoursSpinBox->value());
	s+="\n";
	s+=QObject::tr("Max hours:%1").arg(maxHoursSpinBox->value());
	s+="\n";

	currentConstraintTextEdit->setText(s);
}

void ModifyConstraintStudentsNHoursDailyForm::ok()
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

	if(minHoursSpinBox->value()==-1 && maxHoursSpinBox->value()==-1){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Please specify at least a min or max value"));
		return;
	}

	this->_ctr->weight=weight;
	this->_ctr->compulsory=compulsory;
	this->_ctr->minHoursDaily=minHoursSpinBox->value();
	this->_ctr->maxHoursDaily=maxHoursSpinBox->value();

	gt.rules.internalStructureComputed=false;
	
	this->close();
}

void ModifyConstraintStudentsNHoursDailyForm::cancel()
{
	this->close();
}