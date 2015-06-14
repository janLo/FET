//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Liviu Lalescu <liviu@lalescu.ro>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "genetictimetable_defs.h"
#include "fet.h"
#include "fetmainform.h"
#include "buildingsform.h"
#include "addbuildingform.h"
#include "modifybuildingform.h"

#include <qlistbox.h>
#include <qinputdialog.h>

BuildingsForm::BuildingsForm()
 : BuildingsForm_template()
{
	buildingsListBox->clear();
	for(Building* bu=gt.rules.buildingsList.first(); bu; bu=gt.rules.buildingsList.next())
		buildingsListBox->insertItem(bu->name);
}

BuildingsForm::~BuildingsForm()
{
}

void BuildingsForm::addBuilding()
{
	AddBuildingForm* addBuildingForm=new AddBuildingForm();
	addBuildingForm->exec();

	buildingsListBox->clear();
	for(Building* bu=gt.rules.buildingsList.first(); bu; bu=gt.rules.buildingsList.next())
		buildingsListBox->insertItem(bu->name);
		
	buildingsListBox->setCurrentItem(buildingsListBox->count()-1);
}

void BuildingsForm::removeBuilding()
{
	if(buildingsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected building"));
		return;
	}
	
	uint t=buildingsListBox->currentItem();

	QString text=buildingsListBox->currentText();
	int building_ID=gt.rules.searchBuilding(text);
	if(building_ID<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected building"));
		return;
	}

	if(QMessageBox::warning( this, QObject::tr("FET"),
		QObject::tr("Are you sure you want to delete this building and all related constraints?\n"),
		QObject::tr("Yes"), QObject::tr("No"), 0, 0, 1 ) == 1)
		return;

	bool tmp=gt.rules.removeBuilding(text);
	if(tmp)
		buildingsListBox->removeItem(buildingsListBox->currentItem());
	this->show();
	
	if(t>=buildingsListBox->count())
		t=buildingsListBox->count()-1;
	buildingsListBox->setCurrentItem(t);
}

void BuildingsForm::modifyBuilding()
{
	if(buildingsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected building"));
		return;
	}
	
	uint t=buildingsListBox->currentItem();

	QString text=buildingsListBox->currentText();
	int building_ID=gt.rules.searchBuilding(text);
	if(building_ID<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected building"));
		return;
	}

	ModifyBuildingForm* modifyBuildingForm=new ModifyBuildingForm(text);
	modifyBuildingForm->exec();

	buildingsListBox->clear();
	for(Building* bu=gt.rules.buildingsList.first(); bu; bu=gt.rules.buildingsList.next())
		buildingsListBox->insertItem(bu->name);

	buildingsListBox->setCurrentItem(t);
}

void BuildingsForm::sortBuildings()
{
	gt.rules.sortBuildingsAlphabetically();

	buildingsListBox->clear();
	for(Building* bu=gt.rules.buildingsList.first(); bu; bu=gt.rules.buildingsList.next())
		buildingsListBox->insertItem(bu->name);
}

void BuildingsForm::buildingChanged(int index)
{
	if(index<0){
		currentBuildingTextEdit->setText(QObject::tr("Invalid building"));
		return;
	}

	QString s;
	Building* building=gt.rules.buildingsList.at(index);

	assert(building!=NULL);
	s=building->getDetailedDescriptionWithConstraints(gt.rules);
	currentBuildingTextEdit->setText(s);
}
