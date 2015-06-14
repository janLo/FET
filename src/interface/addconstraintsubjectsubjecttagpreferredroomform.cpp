/***************************************************************************
                          addconstraintsubjectsubjecttagpreferredroomform.cpp  -  description
                             -------------------
    begin                : 8 Apr 2005
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

#include "addconstraintsubjectsubjecttagpreferredroomform.h"
#include "spaceconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>

AddConstraintSubjectSubjectTagPreferredRoomForm::AddConstraintSubjectSubjectTagPreferredRoomForm()
{
	updateSubjectsComboBox();
	updateSubjectTagsComboBox();
	updateRoomsComboBox();
}

AddConstraintSubjectSubjectTagPreferredRoomForm::~AddConstraintSubjectSubjectTagPreferredRoomForm()
{
}

void AddConstraintSubjectSubjectTagPreferredRoomForm::updateSubjectsComboBox()
{
	subjectsComboBox->clear();
	for(Subject* sb=gt.rules.subjectsList.first(); sb; sb=gt.rules.subjectsList.next())
		subjectsComboBox->insertItem(sb->name);
}

void AddConstraintSubjectSubjectTagPreferredRoomForm::updateSubjectTagsComboBox()
{
	subjectTagsComboBox->clear();
	for(SubjectTag* st=gt.rules.subjectTagsList.first(); st; st=gt.rules.subjectTagsList.next())
		subjectTagsComboBox->insertItem(st->name);
}

void AddConstraintSubjectSubjectTagPreferredRoomForm::updateRoomsComboBox()
{
	roomsComboBox->clear();
	for(Room* rm=gt.rules.roomsList.first(); rm; rm=gt.rules.roomsList.next())
		roomsComboBox->insertItem(rm->name);
}

void AddConstraintSubjectSubjectTagPreferredRoomForm::addConstraint()
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

	i=subjectTagsComboBox->currentItem();
	if(i<0 || subjectTagsComboBox->count()<=0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid subject tag"));
		return;
	}
	QString subjectTag=subjectTagsComboBox->currentText();

	i=roomsComboBox->currentItem();
	if(i<0 || roomsComboBox->count()<=0){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid room"));
		return;
	}
	QString room=roomsComboBox->currentText();

	ctr=new ConstraintSubjectSubjectTagPreferredRoom(weight, compulsory, subject, subjectTag, room);

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
