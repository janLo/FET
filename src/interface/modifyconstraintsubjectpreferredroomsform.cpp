/***************************************************************************
                          modifyconstraintsubjectpreferredroomsform.cpp  -  description
                             -------------------
    begin                : April 8, 2005
    copyright            : (C) 2005 by Lalescu Liviu
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

#include "modifyconstraintsubjectpreferredroomsform.h"
#include "spaceconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

ModifyConstraintSubjectPreferredRoomsForm::ModifyConstraintSubjectPreferredRoomsForm(ConstraintSubjectPreferredRooms* ctr)
{
	updateRoomsListBox();
	
	int i=0, j=-1;
	for(Subject* sb=gt.rules.subjectsList.first(); sb; sb=gt.rules.subjectsList.next()){
		subjectsComboBox->insertItem(sb->name);
		if(ctr->subjectName==sb->name){
			assert(j==-1);
			j=i;
		}
		i++;
	}
	assert(j>=0);
	subjectsComboBox->setCurrentItem(j);
	
	this->_ctr=ctr;
	
	weightLineEdit->setText(QString::number(ctr->weight));
	compulsoryCheckBox->setChecked(ctr->compulsory);
	
	for(QStringList::Iterator it=ctr->roomsNames.begin(); it!=ctr->roomsNames.end(); it++)
		selectedRoomsListBox->insertItem(*it);
}

ModifyConstraintSubjectPreferredRoomsForm::~ModifyConstraintSubjectPreferredRoomsForm()
{
}

void ModifyConstraintSubjectPreferredRoomsForm::updateRoomsListBox()
{
	roomsListBox->clear();
	selectedRoomsListBox->clear();

	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next())
		roomsListBox->insertItem(rm->name);
}

void ModifyConstraintSubjectPreferredRoomsForm::ok()
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

	if(selectedRoomsListBox->count()==0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Empty list of selected rooms"));
		return;
	}
	if(selectedRoomsListBox->count()==1){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Only one selected room"));
		return;
	}
	if(selectedRoomsListBox->count()>(uint)(MAX_CONSTRAINT_SUBJECT_PREFERRED_ROOMS)){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Please report error to the author\nMAX_CONSTRAINT_SUBJECT_PREFERRED_ROOM must be increased (you have too many selected rooms)"));
		return;
	}
	
	if(subjectsComboBox->currentItem()<0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid selected subject"));
		return;
	}
	QString subject=subjectsComboBox->currentText();
	
	QStringList roomsList;
	for(uint i=0; i<selectedRoomsListBox->count(); i++)
		roomsList.append(selectedRoomsListBox->text(i));
	
	this->_ctr->weight=weight;
	this->_ctr->compulsory=compulsory;
	this->_ctr->subjectName=subject;
	this->_ctr->roomsNames=roomsList;
	
	gt.rules.internalStructureComputed=false;
	
	this->close();
}

void ModifyConstraintSubjectPreferredRoomsForm::cancel()
{
	this->close();
}

void ModifyConstraintSubjectPreferredRoomsForm::addRoom()
{
	if(roomsListBox->currentItem()<0)
		return;
	QString rmName=roomsListBox->currentText();
	assert(rmName!="");
	uint i;
	//duplicate?
	for(i=0; i<selectedRoomsListBox->count(); i++)
		if(rmName==selectedRoomsListBox->text(i))
			break;
	if(i<selectedRoomsListBox->count())
		return;
	selectedRoomsListBox->insertItem(rmName);
}

void ModifyConstraintSubjectPreferredRoomsForm::removeRoom()
{
	if(selectedRoomsListBox->currentItem()<0 || selectedRoomsListBox->count()<=0)
		return;		
	selectedRoomsListBox->removeItem(selectedRoomsListBox->currentItem());
}
