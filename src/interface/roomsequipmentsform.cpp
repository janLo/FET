/***************************************************************************
                          roomsequipmentsform.cpp  -  description
                             -------------------
    begin                : Thu Jan 29 2004
    copyright            : (C) 2004 by Lalescu Liviu
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

#include "roomsequipmentsform.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

RoomsEquipmentsForm::RoomsEquipmentsForm()
{
	updateRoomsComboBox();
	updateAllEquipmentsListBox();
	if(gt.rules.roomsList.count()==0)
		roomChanged(-1);
	else
		roomChanged(0);
}

RoomsEquipmentsForm::~RoomsEquipmentsForm()
{
}

void RoomsEquipmentsForm::roomChanged(int i)
{
	roomEquipmentsListBox->clear();

	if(i<0)
		return;
	else{
		Room* rm=gt.rules.roomsList.at(i);
		for(QStringList::Iterator it=rm->equipments.begin(); it!=rm->equipments.end(); it++){
			roomEquipmentsListBox->insertItem(*it);
		}
	}
}

void RoomsEquipmentsForm::updateRoomsComboBox()
{
	roomsComboBox->clear();
	for(Room* room=gt.rules.roomsList.first(); room; room=gt.rules.roomsList.next())
		roomsComboBox->insertItem(room->name);
}

void RoomsEquipmentsForm::updateAllEquipmentsListBox()
{
	allEquipmentsListBox->clear();
	for(Equipment* eq=gt.rules.equipmentsList.first(); eq; eq=gt.rules.equipmentsList.next())
		allEquipmentsListBox->insertItem(eq->name);
}

void RoomsEquipmentsForm::addEquipment()
{
	if(allEquipmentsListBox->currentItem()<0)
		return;
	QString equip=allEquipmentsListBox->currentText();
	assert(equip!="");
	uint i;
	//duplicate?
	for(i=0; i<roomEquipmentsListBox->count(); i++)
		if(equip==roomEquipmentsListBox->text(i))
			break;
	if(i<roomEquipmentsListBox->count())
		return;
	roomEquipmentsListBox->insertItem(equip);

	if(roomsComboBox->currentItem()>=0){
		Room* rm=gt.rules.roomsList.at(roomsComboBox->currentItem());
		rm->addEquipment(equip);
	}
}

void RoomsEquipmentsForm::removeEquipment()
{
	if(roomEquipmentsListBox->currentItem()<0 || roomEquipmentsListBox->count()<=0)
		return;
	QString equip=roomEquipmentsListBox->currentText();
	roomEquipmentsListBox->removeItem(roomEquipmentsListBox->currentItem());

	if(roomsComboBox->currentItem()>=0){
		Room* rm=gt.rules.roomsList.at(roomsComboBox->currentItem());
		rm->removeEquipment(equip);
	}
}
