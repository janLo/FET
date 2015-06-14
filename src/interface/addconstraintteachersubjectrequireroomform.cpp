/***************************************************************************
                          addconstraintteachersubjectrequireroomform.cpp  -  description
                             -------------------
    begin                : Sat 2 July 2005
    copyright            : (C) 2005 by Liviu Lalescu
    email                : Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "addconstraintteachersubjectrequireroomform.h"
#include "spaceconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

AddConstraintTeacherSubjectRequireRoomForm::AddConstraintTeacherSubjectRequireRoomForm()
{
	updateTeachersComboBox();
	updateSubjectsComboBox();
	updateRoomsComboBox();
}

AddConstraintTeacherSubjectRequireRoomForm::~AddConstraintTeacherSubjectRequireRoomForm()
{
}

void AddConstraintTeacherSubjectRequireRoomForm::updateTeachersComboBox()
{
	teachersComboBox->clear();
	for(Teacher* tc=gt.rules.teachersList.first(); tc; tc=gt.rules.teachersList.next())
		teachersComboBox->insertItem(tc->name);
}

void AddConstraintTeacherSubjectRequireRoomForm::updateSubjectsComboBox()
{
	subjectsComboBox->clear();
	for(Subject* sb=gt.rules.subjectsList.first(); sb; sb=gt.rules.subjectsList.next())
		subjectsComboBox->insertItem(sb->name);
}

void AddConstraintTeacherSubjectRequireRoomForm::updateRoomsComboBox()
{
	roomsComboBox->clear();
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next())
		roomsComboBox->insertItem(rm->name);
}

void AddConstraintTeacherSubjectRequireRoomForm::addConstraint()
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

	int i=teachersComboBox->currentItem();
	if(i<0 || teachersComboBox->count()<=0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid teacher"));
		return;
	}
	QString teacher=teachersComboBox->currentText();

	i=subjectsComboBox->currentItem();
	if(i<0 || subjectsComboBox->count()<=0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid subject"));
		return;
	}
	QString subject=subjectsComboBox->currentText();

	i=roomsComboBox->currentItem();
	if(i<0 || roomsComboBox->count()<=0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid room"));
		return;
	}
	QString room=roomsComboBox->currentText();

	ctr=new ConstraintTeacherSubjectRequireRoom(weight, compulsory, teacher, subject, room);

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
