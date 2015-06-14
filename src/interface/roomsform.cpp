//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Liviu Lalescu <liviu@lalescu.ro>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "genetictimetable_defs.h"
#include "fet.h"
#include "fetmainform.h"
#include "roomsform.h"
#include "addroomform.h"
#include "modifyroomform.h"
#include "roomsequipmentsform.h"

#include <qlistbox.h>
#include <qinputdialog.h>

RoomsForm::RoomsForm()
 : RoomsForm_template()
{
	visibleRoomsList.setAutoDelete(false);
	
	typesComboBox->insertItem("");
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next()){
		int i;
		for(i=0; i<typesComboBox->count(); i++)
			if(typesComboBox->text(i)==rm->type)
				break;
		if(i==typesComboBox->count())
			typesComboBox->insertItem(rm->type);
	}
	typesComboBox->setCurrentItem(0);

	buildingsComboBox->insertItem("");
	for(Building* bu=gt.rules.buildingsList.first(); bu; bu=gt.rules.buildingsList.next())
		buildingsComboBox->insertItem(bu->name);
	buildingsComboBox->setCurrentItem(0);
	
	this->filterChanged();
}


RoomsForm::~RoomsForm()
{
}

bool RoomsForm::filterOk(Room* rm)
{
	QString tn=typesComboBox->currentText();
	QString bu=buildingsComboBox->currentText();
	
	bool ok=true;
	
	if(tn!="" && tn!=rm->type)
		ok=false;
		
	if(bu!="" && bu!=rm->building)
		ok=false;
		
	return ok;
}

void RoomsForm::filterChanged()
{
	QString s;
	roomsListBox->clear();
	assert(visibleRoomsList.autoDelete()==false);
	visibleRoomsList.clear();
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next())
		if(this->filterOk(rm)){
			s=rm->getDescription();
			visibleRoomsList.append(rm);
			roomsListBox->insertItem(s);
		}
	roomChanged(roomsListBox->currentItem());
}

void RoomsForm::addRoom()
{
	int ind=roomsListBox->currentItem();

	AddRoomForm* addRoomForm=new AddRoomForm();
	addRoomForm->exec();
	
	typesComboBox->clear();
	typesComboBox->insertItem("");
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next()){
		int i;
		for(i=0; i<typesComboBox->count(); i++)
			if(typesComboBox->text(i)==rm->type)
				break;
		if(i==typesComboBox->count())
			typesComboBox->insertItem(rm->type);
	}
	typesComboBox->setCurrentItem(0);

	filterChanged();
	
	roomsListBox->setCurrentItem(ind);
}

void RoomsForm::removeRoom()
{
	int ind=roomsListBox->currentItem();
	if(ind<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected room"));
		return;
	}
	
	Room* rm=visibleRoomsList.at(ind);
	assert(rm!=NULL);

	if(QMessageBox::warning( this, QObject::tr("FET"),
		QObject::tr("Are you sure you want to delete this room and all related constraints?\n"),
		QObject::tr("Yes"), QObject::tr("No"), 0, 0, 1 ) == 1)
		return;

	bool tmp=gt.rules.removeRoom(rm->name);
	assert(tmp);

	filterChanged();
	
	if((uint)(ind)>=roomsListBox->count())
		ind=roomsListBox->count()-1;
	roomsListBox->setCurrentItem(ind);
}

void RoomsForm::roomChanged(int index)
{
	if(index<0){
		currentRoomTextEdit->setText(QObject::tr("Invalid room"));
		return;
	}

	QString s;
	Room* room=visibleRoomsList.at(index);

	assert(room!=NULL);
	s=room->getDetailedDescriptionWithConstraints(gt.rules);
	currentRoomTextEdit->setText(s);
}

void RoomsForm::roomsEquipments()
{
	int ci=roomsListBox->currentItem();

	RoomsEquipmentsForm* roomsEquipmentsForm=new RoomsEquipmentsForm();
	roomsEquipmentsForm->exec();

	filterChanged();
	
	roomsListBox->setCurrentItem(ci);
}

void RoomsForm::sortRooms()
{
	gt.rules.sortRoomsAlphabetically();

	roomsListBox->clear();
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next())
		roomsListBox->insertItem(rm->name);
}

void RoomsForm::modifyRoom()
{
	int ci=roomsListBox->currentItem();
	if(ci<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected room"));
		return;
	}
	
	Room* rm=visibleRoomsList.at(ci);
	ModifyRoomForm* modifyRoomForm=new ModifyRoomForm(rm->name, rm->type, rm->building, rm->capacity);
	modifyRoomForm->exec();

	typesComboBox->clear();
	typesComboBox->insertItem("");
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next()){
		int i;
		for(i=0; i<typesComboBox->count(); i++)
			if(typesComboBox->text(i)==rm->type)
				break;
		if(i==typesComboBox->count())
			typesComboBox->insertItem(rm->type);
	}
	typesComboBox->setCurrentItem(0);

	filterChanged();
	
	roomsListBox->setCurrentItem(ci);
}
