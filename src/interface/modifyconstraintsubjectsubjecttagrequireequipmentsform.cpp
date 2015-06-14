/***************************************************************************
                          modifyconstraintsubjectsubjecttagrequireequipmentsform.cpp  -  description
                             -------------------
    begin                : Feb 22, 2005
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

#include "modifyconstraintsubjectsubjecttagrequireequipmentsform.h"
#include "spaceconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

ModifyConstraintSubjectSubjectTagRequireEquipmentsForm::ModifyConstraintSubjectSubjectTagRequireEquipmentsForm(ConstraintSubjectSubjectTagRequireEquipments* ctr)
{
	this->_ctr=ctr;
	
	compulsoryCheckBox->setChecked(ctr->compulsory);
	weightLineEdit->setText(QString::number(ctr->weight));

	updateSubjectsComboBox();
	updateSubjectTagsComboBox();
	updateEquipmentsListBox();
	
	requiredEquipmentsListBox->clear();
	for(QStringList::Iterator it=ctr->equipmentsNames.begin(); it!=ctr->equipmentsNames.end(); it++)
		requiredEquipmentsListBox->insertItem(*it);
}

ModifyConstraintSubjectSubjectTagRequireEquipmentsForm::~ModifyConstraintSubjectSubjectTagRequireEquipmentsForm()
{
}

void ModifyConstraintSubjectSubjectTagRequireEquipmentsForm::updateSubjectsComboBox()
{
	int i=0, j=-1;
	subjectsComboBox->clear();
	for(Subject* sb=gt.rules.subjectsList.first(); sb; sb=gt.rules.subjectsList.next()){
		subjectsComboBox->insertItem(sb->name);
		if(sb->name==this->_ctr->subjectName)
			j=i;
		i++;
	}
	assert(j>=0);
	subjectsComboBox->setCurrentItem(j);
}

void ModifyConstraintSubjectSubjectTagRequireEquipmentsForm::updateSubjectTagsComboBox()
{
	int i=0, j=-1;
	subjectTagsComboBox->clear();
	for(SubjectTag* st=gt.rules.subjectTagsList.first(); st; st=gt.rules.subjectTagsList.next()){
		subjectTagsComboBox->insertItem(st->name);
		if(st->name==this->_ctr->subjectTagName)
			j=i;
		i++;
	}
	assert(j>=0);
	subjectTagsComboBox->setCurrentItem(j);
}

void ModifyConstraintSubjectSubjectTagRequireEquipmentsForm::updateEquipmentsListBox()
{
	allEquipmentsListBox->clear();
	for(Equipment* eq=gt.rules.equipmentsList.first(); eq; eq=gt.rules.equipmentsList.next())
		allEquipmentsListBox->insertItem(eq->name);
}

void ModifyConstraintSubjectSubjectTagRequireEquipmentsForm::cancel()
{
	this->close();
}

void ModifyConstraintSubjectSubjectTagRequireEquipmentsForm::ok()
{
	double weight;
	QString tmp=weightLineEdit->text();
	sscanf(tmp, "%lf", &weight);
	if(weight<=0.0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid weight"));
		return;
	}

	bool compulsory=false;
	if(compulsoryCheckBox->isChecked())
		compulsory=true;

	int i=subjectsComboBox->currentItem();
	if(i<0 || subjectsComboBox->count()<=0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid subject"));
		return;
	}
	QString subject=subjectsComboBox->currentText();

	i=subjectTagsComboBox->currentItem();
	if(i<0 || subjectTagsComboBox->count()<=0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid subject tag"));
		return;
	}
	QString subjectTag=subjectTagsComboBox->currentText();

	if(requiredEquipmentsListBox->count()==0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Empty list of required equipments"));
		return;
	}

	this->_ctr->weight=weight;
	this->_ctr->compulsory=compulsory;
	this->_ctr->subjectName=subject;
	this->_ctr->subjectTagName=subjectTag;

	this->_ctr->equipmentsNames.clear();
	for(uint i=0; i<requiredEquipmentsListBox->count(); i++)
		this->_ctr->addRequiredEquipment(requiredEquipmentsListBox->text(i));

	gt.rules.internalStructureComputed=false;
	
	this->close();
}

void ModifyConstraintSubjectSubjectTagRequireEquipmentsForm::addEquipment()
{
	if(allEquipmentsListBox->currentItem()<0)
		return;
	QString equip=allEquipmentsListBox->currentText();
	assert(equip!="");
	uint i;
	//duplicate?
	for(i=0; i<requiredEquipmentsListBox->count(); i++)
		if(equip==requiredEquipmentsListBox->text(i))
			break;
	if(i<requiredEquipmentsListBox->count())
		return;
	requiredEquipmentsListBox->insertItem(equip);
}

void ModifyConstraintSubjectSubjectTagRequireEquipmentsForm::removeEquipment()
{
	if(requiredEquipmentsListBox->currentItem()<0 || requiredEquipmentsListBox->count()<=0)
		return;
	requiredEquipmentsListBox->removeItem(requiredEquipmentsListBox->currentItem());
}
