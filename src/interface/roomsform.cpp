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
	roomsListBox->clear();
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next())
		roomsListBox->insertItem(rm->name);
}


RoomsForm::~RoomsForm()
{
}

void RoomsForm::addRoom()
{
	AddRoomForm* addRoomForm=new AddRoomForm();
	addRoomForm->exec();

	roomsListBox->clear();
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next())
		roomsListBox->insertItem(rm->name);
}

void RoomsForm::removeRoom()
{
	if(roomsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected room"));
		return;
	}

	QString text=roomsListBox->currentText();
	int room_ID=gt.rules.searchRoom(text);
	if(room_ID<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected room"));
		return;
	}

	if(QMessageBox::warning( this, QObject::tr("FET"),
		QObject::tr("Are you sure you want to delete this room and all related constraints?\n"),
		QObject::tr("Yes"), QObject::tr("No"), 0, 0, 1 ) == 1)
		return;

	bool tmp=gt.rules.removeRoom(text);
	if(tmp)
		roomsListBox->removeItem(roomsListBox->currentItem());
	this->show();
}

void RoomsForm::roomChanged(int index)
{
	if(index<0){
		currentRoomTextEdit->setText(QObject::tr("Invalid room"));
		return;
	}

	QString s;
	Room* room=gt.rules.roomsList.at(index);

	assert(room!=NULL);
	s=room->getDetailedDescription();
	currentRoomTextEdit->setText(s);
}

void RoomsForm::roomsEquipments()
{
	RoomsEquipmentsForm* roomsEquipmentsForm=new RoomsEquipmentsForm();
	roomsEquipmentsForm->exec();

	roomsListBox->clear();
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next())
		roomsListBox->insertItem(rm->name);
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
	if(roomsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected room"));
		return;
	}

	QString text=roomsListBox->currentText();
	int room_ID=gt.rules.searchRoom(text);
	if(room_ID<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected room"));
		return;
	}
	
	Room* rm=gt.rules.roomsList.at(room_ID);
	ModifyRoomForm* modifyRoomForm=new ModifyRoomForm(rm->name, rm->type, rm->capacity);
	modifyRoomForm->exec();

	roomsListBox->clear();
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next())
		roomsListBox->insertItem(rm->name);
	
	roomsListBox->setCurrentItem(ci);
}
