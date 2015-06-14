//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Liviu Lalescu <liviu@lalescu.ro>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "genetictimetable_defs.h"
#include "fet.h"
#include "fetmainform.h"
#include "equipmentsform.h"
#include "addequipmentform.h"
#include "modifyequipmentform.h"

#include <qlistbox.h>
#include <qinputdialog.h>

EquipmentsForm::EquipmentsForm()
 : EquipmentsForm_template()
{
	equipmentsListBox->clear();
	for(Equipment* eq=gt.rules.equipmentsList.first(); eq; eq=gt.rules.equipmentsList.next())
		equipmentsListBox->insertItem(eq->name);
}


EquipmentsForm::~EquipmentsForm()
{
}

void EquipmentsForm::addEquipment()
{
	AddEquipmentForm* addEquipmentForm=new AddEquipmentForm();
	addEquipmentForm->exec();

	equipmentsListBox->clear();
	for(Equipment* eq=gt.rules.equipmentsList.first(); eq; eq=gt.rules.equipmentsList.next())
		equipmentsListBox->insertItem(eq->name);
		
	equipmentsListBox->setCurrentItem(equipmentsListBox->count()-1);
}

void EquipmentsForm::removeEquipment()
{
	if(equipmentsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected equipment"));
		return;
	}
	
	uint t=equipmentsListBox->currentItem();

	QString text=equipmentsListBox->currentText();
	int equipment_ID=gt.rules.searchEquipment(text);
	if(equipment_ID<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected equipment"));
		return;
	}

	if(QMessageBox::warning( this, QObject::tr("FET"),
		QObject::tr("Are you sure you want to delete this equipment and all related constraints?\n"),
		QObject::tr("Yes"), QObject::tr("No"), 0, 0, 1 ) == 1)
		return;

	bool tmp=gt.rules.removeEquipment(text);
	if(tmp)
		equipmentsListBox->removeItem(equipmentsListBox->currentItem());
	this->show();
	
	if(t>=equipmentsListBox->count())
		t=equipmentsListBox->count()-1;
	equipmentsListBox->setCurrentItem(t);
}

void EquipmentsForm::modifyEquipment()
{
	if(equipmentsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected equipment"));
		return;
	}
	
	uint t=equipmentsListBox->currentItem();

	QString text=equipmentsListBox->currentText();
	int equipment_ID=gt.rules.searchEquipment(text);
	if(equipment_ID<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected equipment"));
		return;
	}

	ModifyEquipmentForm* modifyEquipmentForm=new ModifyEquipmentForm(text);
	modifyEquipmentForm->exec();

	equipmentsListBox->clear();
	for(Equipment* eq=gt.rules.equipmentsList.first(); eq; eq=gt.rules.equipmentsList.next())
		equipmentsListBox->insertItem(eq->name);

	equipmentsListBox->setCurrentItem(t);
}

void EquipmentsForm::sortEquipments()
{
	gt.rules.sortEquipmentsAlphabetically();

	equipmentsListBox->clear();
	for(Equipment* eq=gt.rules.equipmentsList.first(); eq; eq=gt.rules.equipmentsList.next())
		equipmentsListBox->insertItem(eq->name);
}

void EquipmentsForm::equipmentChanged(int index)
{
	if(index<0){
		currentEquipmentTextEdit->setText(QObject::tr("Invalid equipment"));
		return;
	}

	QString s;
	Equipment* equipment=gt.rules.equipmentsList.at(index);

	assert(equipment!=NULL);
	s=equipment->getDetailedDescription();
	currentEquipmentTextEdit->setText(s);
}
