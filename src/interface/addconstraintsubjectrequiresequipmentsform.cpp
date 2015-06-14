/***************************************************************************
                          addconstraintsubjectrequiresequipmentsform.cpp  -  description
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

#include "addconstraintsubjectrequiresequipmentsform.h"
#include "spaceconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

AddConstraintSubjectRequiresEquipmentsForm::AddConstraintSubjectRequiresEquipmentsForm()
{
	updateSubjectsComboBox();
	updateEquipmentsListBox();
	requiredEquipmentsListBox->clear();
}

AddConstraintSubjectRequiresEquipmentsForm::~AddConstraintSubjectRequiresEquipmentsForm()
{
}

void AddConstraintSubjectRequiresEquipmentsForm::updateSubjectsComboBox()
{
	subjectsComboBox->clear();
	for(Subject* sb=gt.rules.subjectsList.first(); sb; sb=gt.rules.subjectsList.next())
		subjectsComboBox->insertItem(sb->name);
}

void AddConstraintSubjectRequiresEquipmentsForm::updateEquipmentsListBox()
{
	allEquipmentsListBox->clear();
	for(Equipment* eq=gt.rules.equipmentsList.first(); eq; eq=gt.rules.equipmentsList.next())
		allEquipmentsListBox->insertItem(eq->name);
}

void AddConstraintSubjectRequiresEquipmentsForm::addConstraint()
{
	SpaceConstraint *ctr=NULL;

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

	int i=subjectsComboBox->currentItem();
	if(i<0 || subjectsComboBox->count()<=0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid subject"));
		return;
	}
	QString subject=subjectsComboBox->currentText();

	if(requiredEquipmentsListBox->count()==0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Empty list of required equipments"));
		return;
	}

	ctr=new ConstraintSubjectRequiresEquipments(weight, compulsory, subject);

	for(uint i=0; i<requiredEquipmentsListBox->count(); i++)
		((ConstraintSubjectRequiresEquipments*)ctr)->addRequiredEquipment(requiredEquipmentsListBox->text(i));

	bool tmp2=gt.rules.addSpaceConstraint(ctr);
	if(tmp2){
		QString s=QObject::tr("Constraint added:");
		s+="\n";
		s+=ctr->getDetailedDescription(gt.rules);
		QMessageBox::information(this, QObject::tr("FET information"), s);
	}
	else{
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Constraint NOT added - error ?"));
		delete ctr;
	}
}

void AddConstraintSubjectRequiresEquipmentsForm::addEquipment()
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

void AddConstraintSubjectRequiresEquipmentsForm::removeEquipment()
{
	if(requiredEquipmentsListBox->currentItem()<0 || requiredEquipmentsListBox->count()<=0)
		return;
	requiredEquipmentsListBox->removeItem(requiredEquipmentsListBox->currentItem());
}
