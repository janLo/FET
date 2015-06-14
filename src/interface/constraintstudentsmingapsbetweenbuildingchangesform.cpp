/***************************************************************************
                          constraintstudentsmingapsbetweenbuildingchangesform.cpp  -  description
                             -------------------
    begin                : Feb 10, 2005
    copyright            : (C) 2005 by Lalescu Liviu
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

#include "constraintstudentsmingapsbetweenbuildingchangesform.h"
#include "addconstraintstudentsmingapsbetweenbuildingchangesform.h"
#include "modifyconstraintstudentsmingapsbetweenbuildingchangesform.h"

#include <QDesktopWidget>

ConstraintStudentsMinGapsBetweenBuildingChangesForm::ConstraintStudentsMinGapsBetweenBuildingChangesForm()
{
	//setWindowFlags(Qt::Window);
	setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	QDesktopWidget* desktop=QApplication::desktop();
	int xx=desktop->width()/2 - frameGeometry().width()/2;
	int yy=desktop->height()/2 - frameGeometry().height()/2;
	move(xx, yy);

	this->filterChanged();
}

ConstraintStudentsMinGapsBetweenBuildingChangesForm::~ConstraintStudentsMinGapsBetweenBuildingChangesForm()
{
}

bool ConstraintStudentsMinGapsBetweenBuildingChangesForm::filterOk(SpaceConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_STUDENTS_MIN_GAPS_BETWEEN_BUILDING_CHANGES){
		return true;
	}
	else
		return false;
}

void ConstraintStudentsMinGapsBetweenBuildingChangesForm::filterChanged()
{
	this->visibleConstraintsList.clear();
	constraintsListBox->clear();
	for(int i=0; i<gt.rules.spaceConstraintsList.size(); i++){
		SpaceConstraint* ctr=gt.rules.spaceConstraintsList[i];
		if(filterOk(ctr)){
			visibleConstraintsList.append(ctr);
			constraintsListBox->insertItem(ctr->getDescription(gt.rules));
		}
	}
}

void ConstraintStudentsMinGapsBetweenBuildingChangesForm::constraintChanged(int index)
{
	if(index<0)
		return;
	assert(index<this->visibleConstraintsList.size());
	SpaceConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=NULL);
	currentConstraintTextEdit->setText(ctr->getDetailedDescription(gt.rules));
}

void ConstraintStudentsMinGapsBetweenBuildingChangesForm::addConstraint()
{
	AddConstraintStudentsMinGapsBetweenBuildingChangesForm *form=new AddConstraintStudentsMinGapsBetweenBuildingChangesForm();
	form->exec();

	filterChanged();
	
	constraintsListBox->setCurrentItem(constraintsListBox->count()-1);
}

void ConstraintStudentsMinGapsBetweenBuildingChangesForm::modifyConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	SpaceConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintStudentsMinGapsBetweenBuildingChangesForm *form
	 = new ModifyConstraintStudentsMinGapsBetweenBuildingChangesForm((ConstraintStudentsMinGapsBetweenBuildingChanges*)ctr);
	form->exec();

	filterChanged();
	constraintsListBox->setCurrentItem(i);
}

void ConstraintStudentsMinGapsBetweenBuildingChangesForm::removeConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	SpaceConstraint* ctr=this->visibleConstraintsList.at(i);
	QString s;
	s=QObject::tr("Removing constraint:\n");
	s+=ctr->getDetailedDescription(gt.rules);
	s+=QObject::tr("\nAre you sure?");

	switch( QMessageBox::warning( this, QObject::tr("FET warning"),
		s, QObject::tr("OK"), QObject::tr("Cancel"), 0, 0, 1 ) ){
	case 0: // The user clicked the OK again button or pressed Enter
		gt.rules.removeSpaceConstraint(ctr);
		filterChanged();
		break;
	case 1: // The user clicked the Cancel or pressed Escape
		break;
	}
	
	if((uint)(i) >= constraintsListBox->count())
		i=constraintsListBox->count()-1;
	constraintsListBox->setCurrentItem(i);
}
