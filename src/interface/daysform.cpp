//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Lalescu Liviu <Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fet.h"

#include "daysform.h"

#include <qspinbox.h>
#include <qlineedit.h>
#include <qmessagebox.h>

#include <QDesktopWidget>

extern GeneticTimetable gt;

QLineEdit* daysNames[14];
int nDays;

DaysForm::DaysForm()
 : DaysForm_template()
{
	//setWindowFlags(Qt::Window);
	setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	QDesktopWidget* desktop=QApplication::desktop();
	int xx=desktop->width()/2 - frameGeometry().width()/2;
	int yy=desktop->height()/2 - frameGeometry().height()/2;
	move(xx, yy);

	nDays=gt.rules.nDaysPerWeek;
	daysNames[0]=day1LineEdit;
	daysNames[1]=day2LineEdit;
	daysNames[2]=day3LineEdit;
	daysNames[3]=day4LineEdit;
	daysNames[4]=day5LineEdit;
	daysNames[5]=day6LineEdit;
	daysNames[6]=day7LineEdit;
	daysNames[7]=day8LineEdit;
	daysNames[8]=day9LineEdit;
	daysNames[9]=day10LineEdit;
	daysNames[10]=day11LineEdit;
	daysNames[11]=day12LineEdit;
	daysNames[12]=day13LineEdit;
	daysNames[13]=day14LineEdit;

	daysSpinBox->setMinValue(1);
	daysSpinBox->setMaxValue(14/*MAX_DAYS_PER_WEEK*/);
	daysSpinBox->setValue(gt.rules.nDaysPerWeek);

	for(int i=0; i<14; i++)
		if(i<nDays){
			daysNames[i]->setEnabled(true);
			daysNames[i]->setText(gt.rules.daysOfTheWeek[i]);
		}
		else
			daysNames[i]->setDisabled(true);
}


DaysForm::~DaysForm()
{
}

void DaysForm::daysChanged()
{
	nDays=daysSpinBox->value();
	assert(nDays <= MAX_DAYS_PER_WEEK);
	for(int i=0; i<14; i++)
		if(i<nDays)
			daysNames[i]->setEnabled(true);
		else
			daysNames[i]->setDisabled(true);
}

void DaysForm::ok()
{
	for(int i=0; i<nDays; i++)
		if(daysNames[i]->text()==""){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Empty names not allowed\n"));
			return;
		}
	for(int i=0; i<nDays; i++)
		for(int j=i+1; j<nDays; j++)
			if(daysNames[i]->text()==daysNames[j]->text()){
				QMessageBox::warning(this, QObject::tr("FET information"),
					QObject::tr("Duplicates not allowed\n"));
				return;
			}

	QMessageBox::information(this, QObject::tr("FET information"),
		QObject::tr("Please note that FET will NOT take care\n"
		"of old constraints using erased days\n"
		"(only renamed days will be handled correctly)\n"));

	gt.rules.nDaysPerWeek=nDays;
	for(int i=0; i<nDays; i++)
		gt.rules.daysOfTheWeek[i]=daysNames[i]->text();

	this->close();
}

void DaysForm::cancel()
{
	this->close();
}
