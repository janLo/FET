/***************************************************************************
                          modifyconstraintteachermingapsbetweebuildingchangesform.cpp  -  description
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

#include <QMessageBox>

#include <cstdio>

#include "modifyconstraintteachermingapsbetweenbuildingchangesform.h"
#include "spaceconstraint.h"

ModifyConstraintTeacherMinGapsBetweenBuildingChangesForm::ModifyConstraintTeacherMinGapsBetweenBuildingChangesForm(ConstraintTeacherMinGapsBetweenBuildingChanges* ctr)
{
    setupUi(this);

//    connect(weightLineEdit, SIGNAL(textChanged(QString)), this /*ModifyConstraintTeacherMinGapsBetweenBuildingChangesForm_template*/, SLOT(constraintChanged()));
    connect(okPushButton, SIGNAL(clicked()), this /*ModifyConstraintTeacherMinGapsBetweenBuildingChangesForm_template*/, SLOT(ok()));
    connect(cancelPushButton, SIGNAL(clicked()), this /*ModifyConstraintTeacherMinGapsBetweenBuildingChangesForm_template*/, SLOT(cancel()));
//    connect(teachersComboBox, SIGNAL(activated(QString)), this /*ModifyConstraintTeacherMinGapsBetweenBuildingChangesForm_template*/, SLOT(constraintChanged()));
//    connect(minGapsSpinBox, SIGNAL(valueChanged(int)), this /*ModifyConstraintTeacherMinGapsBetweenBuildingChangesForm_template*/, SLOT(constraintChanged()));


	//setWindowFlags(Qt::Window);
	/*setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	QDesktopWidget* desktop=QApplication::desktop();
	int xx=desktop->width()/2 - frameGeometry().width()/2;
	int yy=desktop->height()/2 - frameGeometry().height()/2;
	move(xx, yy);*/
	centerWidgetOnScreen(this);

	QSize tmp1=teachersComboBox->minimumSizeHint();
	Q_UNUSED(tmp1);
		
	this->_ctr=ctr;
	
	weightLineEdit->setText(QString::number(ctr->weightPercentage));
	
	updateTeachersComboBox();

	minGapsSpinBox->setMinValue(1);
	minGapsSpinBox->setMaxValue(gt.rules.nHoursPerDay);
	minGapsSpinBox->setValue(ctr->minGapsBetweenBuildingChanges);	
		
	constraintChanged();
}

ModifyConstraintTeacherMinGapsBetweenBuildingChangesForm::~ModifyConstraintTeacherMinGapsBetweenBuildingChangesForm()
{
}

void ModifyConstraintTeacherMinGapsBetweenBuildingChangesForm::updateTeachersComboBox()
{
	teachersComboBox->clear();
	int i=0, j=-1;
	for(int k=0; k<gt.rules.teachersList.size(); k++, i++){
		Teacher* tch=gt.rules.teachersList[k];
		teachersComboBox->insertItem(tch->name);
		if(tch->name==this->_ctr->teacherName)
			j=i;
	}
	assert(j>=0);
	teachersComboBox->setCurrentItem(j);

	constraintChanged();
}

void ModifyConstraintTeacherMinGapsBetweenBuildingChangesForm::constraintChanged()
{/*
	QString s;
	s+=tr("Current constraint:");
	s+="\n";

	double weight;
	QString tmp=weightLineEdit->text();
	sscanf(tmp, "%lf", &weight);
	s+=tr(QString("Weight (percentage)=%1\%").arg(weight));
	s+="\n";

	s+=tr("Teacher min gaps between building changes");
	s+="\n";
	s+=tr("Teacher=%1").arg(teachersComboBox->currentText());
	s+="\n";
	
	s+=tr("Min gaps between building changes=%1").arg(minGapsSpinBox->value());
	s+="\n";

	currentConstraintTextEdit->setText(s);*/
}

void ModifyConstraintTeacherMinGapsBetweenBuildingChangesForm::ok()
{
	double weight;
	QString tmp=weightLineEdit->text();
	sscanf(tmp, "%lf", &weight);
	if(weight<100.0 || weight>100.0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage). It has to be 100"));
		return;
	}

	QString teacher_name=teachersComboBox->currentText();
	int t=gt.rules.searchTeacher(teacher_name);
	if(t==-1){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid teacher"));
		return;
	}

	this->_ctr->weightPercentage=weight;
	this->_ctr->teacherName=teacher_name;
	this->_ctr->minGapsBetweenBuildingChanges=minGapsSpinBox->value();

	gt.rules.internalStructureComputed=false;
	
	this->close();
}

void ModifyConstraintTeacherMinGapsBetweenBuildingChangesForm::cancel()
{
	this->close();
}
