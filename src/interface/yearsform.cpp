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

#include "addstudentsyearform.h"
#include "modifystudentsyearform.h"
#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fet.h"
#include "yearsform.h"
#include "fetmainform.h"
#include "studentsset.h"

#include <qlistbox.h>
#include <qinputdialog.h>
#include <qstring.h>

YearsForm::YearsForm()
 : YearsForm_template()
{
	yearsListBox->clear();
	for(StudentsYear* year=gt.rules.yearsList.first(); year; year=gt.rules.yearsList.next())
		yearsListBox->insertItem(year->name);
}


YearsForm::~YearsForm()
{
}

void YearsForm::addYear()
{
	AddStudentsYearForm* addStudentsYearForm=new AddStudentsYearForm();
	addStudentsYearForm->exec();

	yearsListBox->clear();
	for(StudentsYear* year=gt.rules.yearsList.first(); year; year=gt.rules.yearsList.next())
		yearsListBox->insertItem(year->name);
}

void YearsForm::removeYear()
{
	if(yearsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected year"));
		return;
	}
	QString yearName=yearsListBox->currentText();
	int yearIndex=gt.rules.searchYear(yearName);
	assert(yearIndex>=0);

	if(QMessageBox::warning( this, QObject::tr("FET"),
		QObject::tr("Are you sure you want to delete this year and all related groups, subgroups, activities and constraints?\n"),
		QObject::tr("Yes"), QObject::tr("No"), 0, 0, 1 ) == 1)
		return;

	bool tmp=gt.rules.removeYear(yearName);
	assert(tmp);
	if(tmp)
		yearsListBox->removeItem(yearsListBox->currentItem());
	this->show();
}

void YearsForm::yearChanged()
{
	if(yearsListBox->currentItem()<0)
		return;
	StudentsYear* sty=gt.rules.yearsList.at(yearsListBox->currentItem());
	detailsTextEdit->setText(sty->getDetailedDescriptionWithConstraints(gt.rules));
}

void YearsForm::sortYears()
{
	gt.rules.sortYearsAlphabetically();

	yearsListBox->clear();
	for(StudentsYear* year=gt.rules.yearsList.first(); year; year=gt.rules.yearsList.next())
		yearsListBox->insertItem(year->name);
}

void YearsForm::modifyYear()
{
	int ci=yearsListBox->currentItem();
	if(yearsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected year"));
		return;
	}
	QString yearName=yearsListBox->currentText();
	int numberOfStudents=gt.rules.searchStudentsSet(yearName)->numberOfStudents;

	ModifyStudentsYearForm* modifyStudentsYearForm=new ModifyStudentsYearForm(yearName, numberOfStudents);
	modifyStudentsYearForm->exec();

	yearsListBox->clear();
	for(StudentsYear* year=gt.rules.yearsList.first(); year; year=gt.rules.yearsList.next())
		yearsListBox->insertItem(year->name);

	yearsListBox->setCurrentItem(ci);
}

void YearsForm::activateStudents()
{
	if(yearsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected year"));
		return;
	}
	
	QString yearName=yearsListBox->currentText();
	int count=gt.rules.activateStudents(yearName);
	QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Activated a number of %1 activities").arg(count));
}

void YearsForm::deactivateStudents()
{
	if(yearsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected year"));
		return;
	}
	
	QString yearName=yearsListBox->currentText();
	int count=gt.rules.deactivateStudents(yearName);
	QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("De-activated a number of %1 activities").arg(count));
}
