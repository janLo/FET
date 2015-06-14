/***************************************************************************
                          addconstraintteacheractivitytagmaxhoursdailyform.cpp  -  description
                             -------------------
    begin                : 2009
    copyright            : (C) 2009 by Lalescu Liviu
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

#include "longtextmessagebox.h"

#include "addconstraintteacheractivitytagmaxhoursdailyform.h"
#include "timeconstraint.h"

AddConstraintTeacherActivityTagMaxHoursDailyForm::AddConstraintTeacherActivityTagMaxHoursDailyForm()
{
    setupUi(this);

//    connect(weightLineEdit, SIGNAL(textChanged(QString)), this /*AddConstraintTeacherActivityTagMaxHoursContinuouslyForm_template*/, SLOT(constraintChanged()));
    connect(addConstraintPushButton, SIGNAL(clicked()), this /*AddConstraintTeacherActivityTagMaxHoursContinuouslyForm_template*/, SLOT(addCurrentConstraint()));
    connect(closePushButton, SIGNAL(clicked()), this /*AddConstraintTeacherActivityTagMaxHoursContinuouslyForm_template*/, SLOT(close()));
//    connect(maxHoursSpinBox, SIGNAL(valueChanged(int)), this /*AddConstraintTeacherActivityTagMaxHoursContinuouslyForm_template*/, SLOT(constraintChanged()));
//    connect(teachersComboBox, SIGNAL(activated(QString)), this /*AddConstraintTeacherActivityTagMaxHoursContinuouslyForm_template*/, SLOT(constraintChanged()));
//    connect(activityTagsComboBox, SIGNAL(activated(QString)), this /*AddConstraintTeacherActivityTagMaxHoursContinuouslyForm_template*/, SLOT(constraintChanged()));


	//setWindowFlags(Qt::Window);
	/*setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	QDesktopWidget* desktop=QApplication::desktop();
	int xx=desktop->width()/2 - frameGeometry().width()/2;
	int yy=desktop->height()/2 - frameGeometry().height()/2;
	move(xx, yy);*/
	centerWidgetOnScreen(this);

	QSize tmp1=teachersComboBox->minimumSizeHint();
	Q_UNUSED(tmp1);
	QSize tmp4=activityTagsComboBox->minimumSizeHint();
	Q_UNUSED(tmp4);
	
	updateMaxHoursSpinBox();

	teachersComboBox->clear();
	for(int i=0; i<gt.rules.teachersList.size(); i++){
		Teacher* tch=gt.rules.teachersList[i];
		teachersComboBox->insertItem(tch->name);
	}
	
	activityTagsComboBox->clear();
	foreach(ActivityTag* at, gt.rules.activityTagsList)
		activityTagsComboBox->insertItem(at->name);

	constraintChanged();
}

AddConstraintTeacherActivityTagMaxHoursDailyForm::~AddConstraintTeacherActivityTagMaxHoursDailyForm()
{
}

void AddConstraintTeacherActivityTagMaxHoursDailyForm::updateMaxHoursSpinBox(){
	maxHoursSpinBox->setMinValue(1);
	maxHoursSpinBox->setMaxValue(gt.rules.nHoursPerDay);
	maxHoursSpinBox->setValue(gt.rules.nHoursPerDay);
}

void AddConstraintTeacherActivityTagMaxHoursDailyForm::constraintChanged()
{
	//nothing
}

void AddConstraintTeacherActivityTagMaxHoursDailyForm::addCurrentConstraint()
{
	TimeConstraint *ctr=NULL;

	double weight;
	QString tmp=weightLineEdit->text();
	sscanf(tmp, "%lf", &weight);
	if(weight<0.0 || weight>100.0){
		QMessageBox::warning(this, tr("FET warning"),
			tr("Invalid weight (percentage)"));
		return;
	}

	QString teacher_name=teachersComboBox->currentText();
	int teacher_ID=gt.rules.searchTeacher(teacher_name);
	if(teacher_ID<0){
		QMessageBox::warning(this, tr("FET warning"),
			tr("Invalid teacher"));
		return;
	}

	QString activityTagName=activityTagsComboBox->currentText();
	int activityTagIndex=gt.rules.searchActivityTag(activityTagName);
	if(activityTagIndex<0){
		QMessageBox::warning(this, tr("FET warning"),
			tr("Invalid activity tag"));
		return;
	}

	int max_hours=maxHoursSpinBox->value();

	ctr=new ConstraintTeacherActivityTagMaxHoursDaily(weight, max_hours, teacher_name, activityTagName);

	bool tmp2=gt.rules.addTimeConstraint(ctr);
	if(tmp2)
		LongTextMessageBox::information(this, tr("FET information"),
			tr("Constraint added:")+"\n\n"+ctr->getDetailedDescription(gt.rules));
	else{
		QMessageBox::warning(this, tr("FET information"),
			tr("Constraint NOT added - please report error"));
		delete ctr;
	}
}
