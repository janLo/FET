//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Lalescu Liviu <liviu@lalescu.ro>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "addstudentssubgroupform.h"
#include "modifystudentssubgroupform.h"
#include "subgroupsform.h"
#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fet.h"
#include "fetmainform.h"

#include <qlistbox.h>
#include <qinputdialog.h>


SubgroupsForm::SubgroupsForm()
 : SubgroupsForm_template()
{
	yearsListBox->clear();
	for(StudentsYear* year=gt.rules.yearsList.first(); year; year=gt.rules.yearsList.next())
		yearsListBox->insertItem(year->name);

	yearChanged(yearsListBox->currentText());
}

SubgroupsForm::~SubgroupsForm()
{
}

void SubgroupsForm::addSubgroup()
{
	if(yearsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected year"));
		return;
	}
	QString yearName=yearsListBox->currentText();
	int yearIndex=gt.rules.searchYear(yearName);
	assert(yearIndex>=0);

	if(groupsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected group"));
		return;
	}
	QString groupName=groupsListBox->currentText();
	int groupIndex=gt.rules.searchGroup(yearName, groupName);
	assert(groupIndex>=0);

	AddStudentsSubgroupForm* addStudentsSubgroupForm=new AddStudentsSubgroupForm();
	addStudentsSubgroupForm->yearNameLineEdit->setText(yearName);
	addStudentsSubgroupForm->groupNameLineEdit->setText(groupName);
	addStudentsSubgroupForm->exec();

	groupChanged(groupsListBox->currentText());
}

void SubgroupsForm::removeSubgroup()
{
	if(yearsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected year"));
		return;
	}
	QString yearName=yearsListBox->currentText();
	int yearIndex=gt.rules.searchYear(yearName);
	assert(yearIndex>=0);

	if(groupsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected group"));
		return;
	}
	QString groupName=groupsListBox->currentText();
	int groupIndex=gt.rules.searchGroup(yearName, groupName);
	assert(groupIndex>=0);

	if(subgroupsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected subgroup"));
		return;
	}
	QString subgroupName=subgroupsListBox->currentText();
	int subgroupIndex=gt.rules.searchSubgroup(yearName, groupName, subgroupName);
	if(subgroupIndex<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected subgroup"));
		return;
	}

	if(QMessageBox::warning( this, QObject::tr("FET"),
		QObject::tr("Are you sure you want to delete this subgroup and all related activities and constraints?\n"),
		QObject::tr("Yes"), QObject::tr("No"), 0, 0, 1 ) == 1)
		return;

	bool tmp=gt.rules.removeSubgroup(yearName, groupName, subgroupName);
	assert(tmp);
	if(tmp)
		subgroupsListBox->removeItem(subgroupsListBox->currentItem());
	this->show();
}

void SubgroupsForm::yearChanged(const QString &yearName)
{
	int yearIndex=gt.rules.searchYear(yearName);
	if(yearIndex<0)
		return;
	StudentsYear* sty=gt.rules.yearsList.at(yearIndex);

	groupsListBox->clear();
	StudentsGroup* stg;
	for(stg=sty->groupsList.first(); stg; stg=sty->groupsList.next())
		groupsListBox->insertItem(stg->name);

	groupChanged(groupsListBox->currentText());
}

void SubgroupsForm::groupChanged(const QString &groupName)
{
	QString yearName=yearsListBox->currentText();
	int yearIndex=gt.rules.searchYear(yearName);
	if(yearIndex<0)
		return;
	int groupIndex=gt.rules.searchGroup(yearName, groupName);
	if(groupIndex<0)
		return;

	StudentsYear* sty=gt.rules.yearsList.at(yearIndex);
	StudentsGroup* stg=sty->groupsList.at(groupIndex);
	StudentsSubgroup* sts;
	subgroupsListBox->clear();
	for(sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next())
		subgroupsListBox->insertItem(sts->name);
}

void SubgroupsForm::subgroupChanged(const QString &subgroupName)
{
	StudentsSet* ss=gt.rules.searchStudentsSet(subgroupName);
	if(ss==NULL)
		return;
	StudentsSubgroup* s=(StudentsSubgroup*)ss;
	subgroupTextEdit->setText(s->getDetailedDescription());
}

void SubgroupsForm::sortSubgroups()
{
	if(yearsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected year"));
		return;
	}
	QString yearName=yearsListBox->currentText();
	int yearIndex=gt.rules.searchYear(yearName);
	assert(yearIndex>=0);

	if(groupsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected group"));
		return;
	}
	QString groupName=groupsListBox->currentText();
	int groupIndex=gt.rules.searchGroup(yearName, groupName);
	assert(groupIndex>=0);
	
	gt.rules.sortSubgroupsAlphabetically(yearName, groupName);
	
	groupChanged(groupName);
}

void SubgroupsForm::modifySubgroup()
{
	if(yearsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected year"));
		return;
	}
	QString yearName=yearsListBox->currentText();
	int yearIndex=gt.rules.searchYear(yearName);
	assert(yearIndex>=0);

	if(groupsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected group"));
		return;
	}
	QString groupName=groupsListBox->currentText();
	int groupIndex=gt.rules.searchGroup(yearName, groupName);
	assert(groupIndex>=0);

	int ci=subgroupsListBox->currentItem();
	if(subgroupsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected subgroup"));
		return;
	}
	QString subgroupName=subgroupsListBox->currentText();
	int subgroupIndex=gt.rules.searchSubgroup(yearName, groupName, subgroupName);
	if(subgroupIndex<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected subgroup"));
		return;
	}
	
	int numberOfStudents=gt.rules.searchStudentsSet(subgroupName)->numberOfStudents;
	
	ModifyStudentsSubgroupForm* modifyStudentsSubgroupForm=new ModifyStudentsSubgroupForm(yearName, groupName, subgroupName, numberOfStudents);
	modifyStudentsSubgroupForm->exec();

	groupChanged(groupName);
	
	subgroupsListBox->setCurrentItem(ci);
}
