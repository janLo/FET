/***************************************************************************
                          addtimeconstraintform.cpp  -  description
                             -------------------
    begin                : Wed Apr 23 2003
    copyright            : (C) 2003 by Lalescu Liviu
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

#include "addtimeconstraintform.h"
#include "addconstraintactivitypreferredtimesform.h"
#include "timeconstraint.h"

#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>

#define yesNo(x)	((x)==0?QObject::tr("no"):QObject::tr("yes"))

AddTimeConstraintForm::AddTimeConstraintForm()
{
	updatePeriodGroupBox();
	updateStudentsComboBox();
	updateTeachersComboBox();
	updateActivitiesComboBox();

	maxHoursSpinBox->setMinValue(0);
	maxHoursSpinBox->setMaxValue(0);
	minHoursSpinBox->setMinValue(0);
	minHoursSpinBox->setMaxValue(0);
}

AddTimeConstraintForm::~AddTimeConstraintForm()
{
}

void AddTimeConstraintForm::updateTeachersComboBox(){
	teachersComboBox->clear();
	for(Teacher* tch=gt.rules.teachersList.first(); tch; tch=gt.rules.teachersList.next())
		teachersComboBox->insertItem(tch->name);

	teacherChanged(teachersComboBox->currentText());
}

void AddTimeConstraintForm::updateStudentsComboBox(){
	studentsComboBox->clear();
	for(StudentsYear* sty=gt.rules.yearsList.first(); sty; sty=gt.rules.yearsList.next()){
		studentsComboBox->insertItem(sty->name);
		for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
			studentsComboBox->insertItem(stg->name);
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next())
				studentsComboBox->insertItem(sts->name);
		}
	}

	studentsChanged(studentsComboBox->currentText());
}

void AddTimeConstraintForm::updateActivitiesComboBox(){
	activitiesComboBox->clear();
	for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next())
		activitiesComboBox->insertItem(act->getDescription(gt.rules));

	activityChanged();
}

void AddTimeConstraintForm::updatePeriodGroupBox(){
	startHourComboBox->clear();
	for(int i=0; i<=gt.rules.nHoursPerDay; i++)
		startHourComboBox->insertItem(gt.rules.hoursOfTheDay[i]);

	endHourComboBox->clear();
	for(int i=0; i<=gt.rules.nHoursPerDay; i++)
		endHourComboBox->insertItem(gt.rules.hoursOfTheDay[i]);

	dayComboBox->clear();
	for(int i=0; i<gt.rules.nDaysPerWeek; i++)
		dayComboBox->insertItem(gt.rules.daysOfTheWeek[i]);
}

void AddTimeConstraintForm::updatePeriodGroupBox2(){
	startHourComboBox->clear();
	startHourComboBox->insertItem(QObject::tr("Any"));
	for(int i=0; i<=gt.rules.nHoursPerDay; i++)
		startHourComboBox->insertItem(gt.rules.hoursOfTheDay[i]);

	/*endHourComboBox->clear();
	endHourComboBox->insertItem(QObject::tr("Any"));
	for(int i=0; i<=gt.rules.nHoursPerDay; i++)
		endHourComboBox->insertItem(gt.rules.hoursOfTheDay[i]);*/

	dayComboBox->clear();
	dayComboBox->insertItem(QObject::tr("Any"));
	for(int i=0; i<gt.rules.nDaysPerWeek; i++)
		dayComboBox->insertItem(gt.rules.daysOfTheWeek[i]);
}

void AddTimeConstraintForm::updateHoursBox()
{
	maxHoursTextLabel->setText("max");
	maxHoursSpinBox->setMinValue(0);
	maxHoursSpinBox->setMaxValue(gt.rules.nHoursPerDay);
	maxHoursSpinBox->setValue(0);
}

void AddTimeConstraintForm::updateHoursBox2()
{
	maxHoursTextLabel->setText("max (-1 for none)");
	maxHoursSpinBox->setMinValue(-1);
	maxHoursSpinBox->setMaxValue(gt.rules.nHoursPerDay);
	maxHoursSpinBox->setValue(-1);
	minHoursTextLabel->setText("min (-1 for none)");
	minHoursSpinBox->setMinValue(-1);
	minHoursSpinBox->setMaxValue(gt.rules.nHoursPerDay);
	minHoursSpinBox->setValue(-1);
}

void AddTimeConstraintForm::updateHoursBox3()
{
	minHoursTextLabel->setText("min days");
	minHoursSpinBox->setMinValue(1);
	minHoursSpinBox->setMaxValue(gt.rules.nDaysPerWeek-1);
	minHoursSpinBox->setValue(1);
}

void AddTimeConstraintForm::updateHoursBox4()
{
	maxHoursTextLabel->setText("max");
	maxHoursSpinBox->setMinValue(0);
	maxHoursSpinBox->setMaxValue(gt.rules.nDaysPerWeek);
	maxHoursSpinBox->setValue(0);
}

void AddTimeConstraintForm::teacherChanged(const QString& dummy)
{
	if(dummy=="")
		;
	
	constraintChanged();
}

void AddTimeConstraintForm::studentsChanged(const QString& dummy)
{
	if(dummy=="")
		;
	
	constraintChanged();
}

void AddTimeConstraintForm::activityChanged()
{
	constraintChanged();
}

void AddTimeConstraintForm::constraintChanged()
{
	QString s;
	s+=QObject::tr("Current constraint");
	s+=":\n";

	double weight;
	QString tmp=weightLineEdit->text();
	sscanf(tmp, "%lf", &weight);
	s+=QObject::tr(QString("Weight=%1").arg(weight));
	s+="\n";

	bool compulsory=false;
	if(compulsoryCheckBox->isChecked())
		compulsory=true;
	s+=QObject::tr(QString("Compulsory=%1").arg(yesNo(compulsory)));
	s+="\n";

	if(cTeachersNoMoreThanXHoursContinuouslyRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setDisabled(true);
		studentsComboBox->setDisabled(true);
		periodGroupBox->setDisabled(true);
		maxHoursSpinBox->setEnabled(true);
		maxHoursTextLabel->setEnabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);

		s+=QObject::tr(QString("Teachers must not have more than %1 hours continuously").arg(maxHoursSpinBox->value()));
		s+="\n";

		currentConstraintTextEdit->setText(s);
		return;
	}
	else if(cTeacherMaxDaysPerWeekRadioButton->isChecked()){
		teacherTextLabel->setEnabled(true);
		teachersComboBox->setEnabled(true);
		studentsTextLabel->setDisabled(true);
		studentsComboBox->setDisabled(true);
		periodGroupBox->setDisabled(true);
		dayComboBox->setDisabled(true);
		dayTextLabel->setDisabled(true);
		maxHoursSpinBox->setEnabled(true);
		maxHoursTextLabel->setEnabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);
		endHourComboBox->setEnabled(true);
		endHourTextLabel->setEnabled(true);

		s+=QObject::tr("Teacher max days per week");
		s+="\n";
		s+=QObject::tr(QString("Teacher=%1").arg(teachersComboBox->currentText()));
		s+="\n";
		s+=QObject::tr(QString("Maximum number of days=%1").arg(maxHoursSpinBox->value()));
		s+="\n";

		currentConstraintTextEdit->setText(s);

		return;
	}
	else if(cTeachersNoMoreThanXHoursDailyRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setDisabled(true);
		studentsComboBox->setDisabled(true);
		periodGroupBox->setDisabled(true);
		maxHoursSpinBox->setEnabled(true);
		maxHoursTextLabel->setEnabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);

		s+=QObject::tr(QString("Teachers must not have more than %1 hours daily").arg(maxHoursSpinBox->value()));
		s+="\n";

		currentConstraintTextEdit->setText(s);
		return;
	}
	else if(cTeachersSubgroupsNoMoreThanXHoursDailyRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setDisabled(true);
		studentsComboBox->setDisabled(true);
		periodGroupBox->setDisabled(true);
		maxHoursSpinBox->setEnabled(true);
		maxHoursTextLabel->setEnabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);

		s+=QObject::tr(QString("Teachers must not teach to any subgroup more than %1 hours per day").arg(maxHoursSpinBox->value()));
		s+="\n";
		s+=QObject::tr("Please be careful: this restriction seems to slow down very much the automatic generation of the timetable");
		s+="\n";

		currentConstraintTextEdit->setText(s);
		return;
	}
	/*else if(cActivitiesSameStartingTimeRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setDisabled(true);
		studentsComboBox->setDisabled(true);
		periodGroupBox->setDisabled(true);
		maxHoursSpinBox->setDisabled(true);
		maxHoursTextLabel->setDisabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setEnabled(true);
		cActivitiesLineEdit->setEnabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);

		s+=QObject::tr(QString("The activities with id's: %1 must be scheduled at the same time").arg(cActivitiesLineEdit->text()));
		s+=cActivitiesLineEdit->text();
		s+="\n";

		currentConstraintTextEdit->setText(s);
		return;
	}*/
	else if(cActivitiesNotOverlappingRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setDisabled(true);
		studentsComboBox->setDisabled(true);
		periodGroupBox->setDisabled(true);
		maxHoursSpinBox->setDisabled(true);
		maxHoursTextLabel->setDisabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setEnabled(true);
		cActivitiesLineEdit->setEnabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);

		s+=QObject::tr(QString("The activities with id's: %1 must not overlap").arg(cActivitiesLineEdit->text()));
		s+="\n";

		currentConstraintTextEdit->setText(s);
		return;
	}
	else if(cMinNDaysBetweenActivitiesRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setDisabled(true);
		studentsComboBox->setDisabled(true);
		periodGroupBox->setDisabled(true);
		maxHoursSpinBox->setDisabled(true);
		maxHoursTextLabel->setDisabled(true);
		minHoursSpinBox->setEnabled(true);
		minHoursTextLabel->setEnabled(true);
		cActivitiesTextLabel->setEnabled(true);
		cActivitiesLineEdit->setEnabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);

		s+=QObject::tr(QString("The distance between each pair of activities in the set %1 must be at least %2 days").arg(cActivitiesLineEdit->text()).arg(minHoursSpinBox->value()));
		s+="\n";

		currentConstraintTextEdit->setText(s);
		return;
	}
	else if(cTeacherNotAvailableRadioButton->isChecked()){
		teacherTextLabel->setEnabled(true);
		teachersComboBox->setEnabled(true);
		studentsTextLabel->setDisabled(true);
		studentsComboBox->setDisabled(true);
		periodGroupBox->setEnabled(true);
		dayComboBox->setEnabled(true);
		dayTextLabel->setEnabled(true);
		maxHoursSpinBox->setDisabled(true);
		maxHoursTextLabel->setDisabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);
		endHourComboBox->setEnabled(true);
		endHourTextLabel->setEnabled(true);

		s+=QObject::tr("Teacher not available");
		s+="\n";
		s+=QObject::tr(QString("Teacher=%1").arg(teachersComboBox->currentText()));
		s+="\n";
	}
	else if(cActivityPreferredTimeRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setDisabled(true);
		studentsComboBox->setDisabled(true);
		periodGroupBox->setEnabled(true);
		dayComboBox->setEnabled(true);
		dayTextLabel->setEnabled(true);
		maxHoursSpinBox->setDisabled(true);
		maxHoursTextLabel->setDisabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setEnabled(true);
		activitiesTextLabel->setEnabled(true);
		endHourComboBox->setDisabled(true);
		endHourTextLabel->setDisabled(true);

		s+=QObject::tr("Activity preferred time");
		s+="\n";
		if(activitiesComboBox->currentItem()>=0 && gt.rules.activitiesList.count()>=1){
			//messageFunctionQt(QString::number(activitiesComboBox->currentItem()));
			Activity* act=gt.rules.activitiesList.at(activitiesComboBox->currentItem());
			s+=QObject::tr(QString("Activity id=%1").arg(act->id));
			s+="\n";
		}
	}
	else if(cActivityEndsDayRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setDisabled(true);
		studentsComboBox->setDisabled(true);
		periodGroupBox->setDisabled(true);
		dayComboBox->setDisabled(true);
		dayTextLabel->setDisabled(true);
		maxHoursSpinBox->setDisabled(true);
		maxHoursTextLabel->setDisabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setEnabled(true);
		activitiesTextLabel->setEnabled(true);
		endHourComboBox->setDisabled(true);
		endHourTextLabel->setDisabled(true);

		s+=QObject::tr("Activity ends day (must be the last activity of the day)");
		s+="\n";
		if(activitiesComboBox->currentItem()>=0 && gt.rules.activitiesList.count()>=1){
			//messageFunctionQt(QString::number(activitiesComboBox->currentItem()));
			Activity* act=gt.rules.activitiesList.at(activitiesComboBox->currentItem());
			s+=QObject::tr(QString("Activity id=%1").arg(act->id));
			s+="\n";
		}
	}
	else if(cBreakRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setDisabled(true);
		studentsComboBox->setDisabled(true);
		periodGroupBox->setEnabled(true);
		dayComboBox->setEnabled(true);
		dayTextLabel->setEnabled(true);
		maxHoursSpinBox->setDisabled(true);
		maxHoursTextLabel->setDisabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);
		endHourComboBox->setEnabled(true);
		endHourTextLabel->setEnabled(true);

		s+=QObject::tr("Break period");
		s+="\n";
	}
	else if(cStudentsSetNotAvailableRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setEnabled(true);
		studentsComboBox->setEnabled(true);
		periodGroupBox->setEnabled(true);
		dayComboBox->setEnabled(true);
		dayTextLabel->setEnabled(true);
		maxHoursSpinBox->setDisabled(true);
		maxHoursTextLabel->setDisabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);
		endHourComboBox->setEnabled(true);
		endHourTextLabel->setEnabled(true);

		s+=QObject::tr("Students set not available");
		s+="\n";
		s+=QObject::tr(QString("Students set=%1").arg(studentsComboBox->currentText()));
		s+="\n";
	}
	else if(cStudentsSetIntervalMaxDaysPerWeekRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setEnabled(true);
		studentsComboBox->setEnabled(true);
		periodGroupBox->setEnabled(true);
		dayComboBox->setDisabled(true);
		dayTextLabel->setDisabled(true);
		maxHoursSpinBox->setEnabled(true);
		maxHoursTextLabel->setEnabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);
		endHourComboBox->setEnabled(true);
		endHourTextLabel->setEnabled(true);

		s+=QObject::tr("Students set interval max days per week");
		s+="\n";
		s+=QObject::tr(QString("Students set=%1").arg(studentsComboBox->currentText()));
		s+="\n";
		s+=QObject::tr(QString("Maximum number of intervals=%1").arg(maxHoursSpinBox->value()));
		s+="\n";
	}
	else if(cTeachersNoGapsRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setDisabled(true);
		studentsComboBox->setDisabled(true);
		periodGroupBox->setDisabled(true);
		maxHoursSpinBox->setDisabled(true);
		maxHoursTextLabel->setDisabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);

		s+=QObject::tr("Teachers' gaps to be avoided");
		s+="\n";

		currentConstraintTextEdit->setText(s);
		return;
	}
	else if(cStudentsNoGapsRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setDisabled(true);
		studentsComboBox->setDisabled(true);
		periodGroupBox->setDisabled(true);
		maxHoursSpinBox->setDisabled(true);
		maxHoursTextLabel->setDisabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);

		s+=QObject::tr("Students' gaps to be avoided for all students");
		s+="\n";

		currentConstraintTextEdit->setText(s);
		return;
	}
	else if(cStudentsSetNoGapsRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setEnabled(true);
		studentsComboBox->setEnabled(true);
		periodGroupBox->setDisabled(true);
		maxHoursSpinBox->setDisabled(true);
		maxHoursTextLabel->setDisabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);

		s+=QObject::tr(QString("Students' gaps to be avoided for set %1").arg(studentsComboBox->currentText()));
		s+="\n";

		currentConstraintTextEdit->setText(s);
		return;
	}
	else if(cStudentsEarlyRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setDisabled(true);
		studentsComboBox->setDisabled(true);
		periodGroupBox->setDisabled(true);
		maxHoursSpinBox->setDisabled(true);
		maxHoursTextLabel->setDisabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);

		s+=QObject::tr("Students must begin their courses as early as possible");
		s+="\n";

		currentConstraintTextEdit->setText(s);
		return;
	}
	else if(cStudentsNHoursDailyRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setDisabled(true);
		studentsComboBox->setDisabled(true);
		periodGroupBox->setDisabled(true);
		maxHoursSpinBox->setEnabled(true);
		maxHoursTextLabel->setEnabled(true);
		minHoursSpinBox->setEnabled(true);
		minHoursTextLabel->setEnabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);

		s+=QObject::tr("The limits of daily hours for all students");
		s+="\n";
		if(maxHoursSpinBox->value()>=0){
			s+=QObject::tr(QString("Maximum hours daily=%1").arg(maxHoursSpinBox->value()));
			s+="\n";
		}
		if(minHoursSpinBox->value()>=0){
			s+=QObject::tr(QString("Minimum hours daily=%1").arg(minHoursSpinBox->value()));
			s+="\n";
		}

		currentConstraintTextEdit->setText(s);
		return;
	}
	else if(cStudentsSetNHoursDailyRadioButton->isChecked()){
		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setEnabled(true);
		studentsComboBox->setEnabled(true);
		periodGroupBox->setDisabled(true);
		maxHoursSpinBox->setEnabled(true);
		maxHoursTextLabel->setEnabled(true);
		minHoursSpinBox->setEnabled(true);
		minHoursTextLabel->setEnabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setEnabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);

		s+=QObject::tr("Student set must have the number of recommended hours daily");
		s+="\n";
		s+=QObject::tr(QString("Students set=%1").arg(studentsComboBox->currentText()));
		s+="\n";
		if(maxHoursSpinBox->value()>=0){
			s+=QObject::tr(QString("Maximum hours daily=%1").arg(maxHoursSpinBox->value()));
			s+="\n";
		}
		if(minHoursSpinBox->value()>=0){
			s+=QObject::tr(QString("Minimum hours daily=%1").arg(minHoursSpinBox->value()));
			s+="\n";
		}

		currentConstraintTextEdit->setText(s);
		return;
	}
	else{
		s+=QObject::tr("Invalid type");
		s+="\n";

		teacherTextLabel->setDisabled(true);
		teachersComboBox->setDisabled(true);
		studentsTextLabel->setDisabled(true);
		studentsComboBox->setDisabled(true);
		periodGroupBox->setDisabled(true);
		maxHoursSpinBox->setDisabled(true);
		maxHoursTextLabel->setDisabled(true);
		minHoursSpinBox->setDisabled(true);
		minHoursTextLabel->setDisabled(true);
		cActivitiesTextLabel->setDisabled(true);
		cActivitiesLineEdit->setDisabled(true);
		compulsoryCheckBox->setDisabled(true);
		activitiesComboBox->setDisabled(true);
		activitiesTextLabel->setDisabled(true);

		currentConstraintTextEdit->setText(s);

		return;
	}

	if(dayComboBox->isEnabled()==true){
		int day=dayComboBox->currentItem();
		if(day<0 || day>gt.rules.nDaysPerWeek){
			s+=QObject::tr("Invalid day");
			s+="\n";
		}
		else{
			s+=QObject::tr(QString("Day:%1").arg(dayComboBox->currentText()));
			s+="\n";
		}
	}

	if(startHourComboBox->isEnabled()==true){
		int startHour=startHourComboBox->currentItem();
		if(startHour<0 || startHour>gt.rules.nHoursPerDay){
			s+=QObject::tr("Invalid start hour");
			s+="\n";
		}
		else{
			s+=QObject::tr(QString("Start hour:%1").arg(startHourComboBox->currentText()));
			s+="\n";
		}
	}
	if(endHourComboBox->isEnabled()==true){
		int endHour=endHourComboBox->currentItem();
		if(endHour<0 || endHour>gt.rules.nHoursPerDay){
			s+=QObject::tr("Invalid end hour");
			s+="\n";
		}
		else{
			s+=QObject::tr(QString("End hour:%1").arg(endHourComboBox->currentText()));
			s+="\n";
		}
	}

	currentConstraintTextEdit->setText(s);
}

void AddTimeConstraintForm::addCurrentConstraint()
{
	TimeConstraint *ctr=NULL;

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

	if(cTeachersNoGapsRadioButton->isChecked()){
		ctr=new ConstraintTeachersNoGaps(weight, compulsory);

		bool tmp=gt.rules.addTimeConstraint(ctr);
		if(tmp)
			QMessageBox::information(this, QObject::tr("FET information"),
				QObject::tr("Constraint added"));
		else{
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Constraint NOT added - duplicate?"));
			delete ctr;
		}
		return;
	}
	else if(cStudentsNoGapsRadioButton->isChecked()){
		ctr=new ConstraintStudentsNoGaps(weight, compulsory);

		bool tmp=gt.rules.addTimeConstraint(ctr);
		if(tmp)
			QMessageBox::information(this, QObject::tr("FET information"),
				QObject::tr("Constraint added"));
		else{
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Constraint NOT added - duplicate?"));
			delete ctr;
		}
		return;
	}
	else if(cStudentsSetNoGapsRadioButton->isChecked()){
		if(studentsComboBox->currentItem()<0){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Invalid students"));
			return;
		}
		ctr=new ConstraintStudentsSetNoGaps(weight, compulsory, studentsComboBox->currentText());

		bool tmp=gt.rules.addTimeConstraint(ctr);
		if(tmp)
			QMessageBox::information(this, QObject::tr("FET information"),
				QObject::tr("Constraint added"));
		else{
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Constraint NOT added - duplicate?"));
			delete ctr;
		}
		return;
	}
	else if(cStudentsEarlyRadioButton->isChecked()){
		ctr=new ConstraintStudentsEarly(weight, compulsory);

		bool tmp=gt.rules.addTimeConstraint(ctr);
		if(tmp)
			QMessageBox::information(this, QObject::tr("FET information"),
				QObject::tr("Constraint added"));
		else{
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Constraint NOT added - duplicate?"));
			delete ctr;
		}
		return;
	}
	else if(cStudentsNHoursDailyRadioButton->isChecked()){
		if(maxHoursSpinBox->value()==0 || maxHoursSpinBox->value()<-1 || maxHoursSpinBox->value()>gt.rules.nHoursPerDay){
			QMessageBox::warning(this, QObject::tr("FET information"), 
				QObject::tr("Invalid maximum hours per day"));
			return;
		}
		if(maxHoursSpinBox->value()==0 || minHoursSpinBox->value()<-1 || minHoursSpinBox->value()>gt.rules.nHoursPerDay){
			QMessageBox::warning(this, QObject::tr("FET information"), 
				QObject::tr("Invalid minimum hours per day"));
			return;
		}
		if(minHoursSpinBox->value()==-1 && maxHoursSpinBox->value()==-1){
			QMessageBox::warning(this, QObject::tr("FET information"), 
				QObject::tr("You must specify at lease a miminum or maximum limit"));
			return;
		}

		ctr=new ConstraintStudentsNHoursDaily(weight, compulsory, maxHoursSpinBox->value(), minHoursSpinBox->value());

		bool tmp=gt.rules.addTimeConstraint(ctr);
		if(tmp)
			QMessageBox::information(this, QObject::tr("FET information"),
				QObject::tr("Constraint added"));
		else{
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Constraint NOT added - duplicate?"));
			delete ctr;
		}
		return;
	}
	else if(cStudentsSetNHoursDailyRadioButton->isChecked()){
		if(maxHoursSpinBox->value()<-1 || maxHoursSpinBox->value()>gt.rules.nHoursPerDay){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Invalid maximum hours per day"));
			return;
		}
		if(minHoursSpinBox->value()<-1 || minHoursSpinBox->value()>gt.rules.nHoursPerDay){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Invalid minimum hours per day"));
			return;
		}
		if(minHoursSpinBox->value()==-1 && maxHoursSpinBox->value()==-1){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("You must specify at lease a miminum or maximum limit"));
			return;
		}

		ctr=new ConstraintStudentsSetNHoursDaily(weight, compulsory, maxHoursSpinBox->value(), minHoursSpinBox->value(), studentsComboBox->currentText());

		bool tmp=gt.rules.addTimeConstraint(ctr);
		if(tmp)
			QMessageBox::information(this, QObject::tr("FET information"),
				QObject::tr("Constraint added"));
		else{
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Constraint NOT added - duplicate?"));
			delete ctr;
		}
		return;
	}

	else if(cTeachersNoMoreThanXHoursContinuouslyRadioButton->isChecked()){
		if(maxHoursSpinBox->value()<=0){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Invalid max hours continuously (x)"));
			return;
		}

		ctr=new ConstraintTeachersNoMoreThanXHoursContinuously(weight, compulsory, maxHoursSpinBox->value());

		bool tmp=gt.rules.addTimeConstraint(ctr);
		if(tmp)
			QMessageBox::information(this, QObject::tr("FET information"),
				QObject::tr("Constraint added"));
		else{
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Constraint NOT added - duplicate?"));
			delete ctr;
		}
		return;
	}
	else if(cTeacherMaxDaysPerWeekRadioButton->isChecked()){
		QString teacher_name=teachersComboBox->currentText();
		assert(gt.rules.searchTeacher(teacher_name) >= 0 );

		if(maxHoursSpinBox->value()<=0){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Invalid max days"));
			return;
		}

		ctr=new ConstraintTeacherMaxDaysPerWeek(weight, compulsory, maxHoursSpinBox->value(), teacher_name);

		bool tmp=gt.rules.addTimeConstraint(ctr);
		if(tmp)
			QMessageBox::information(this, QObject::tr("FET information"),
				QObject::tr("Constraint added"));
		else{
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Constraint NOT added - duplicate?"));
			delete ctr;
		}
		return;
	}
	else if(cTeachersNoMoreThanXHoursDailyRadioButton->isChecked()){
		if(maxHoursSpinBox->value()<=0){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Invalid max hours daily (x)"));
			return;
		}

		ctr=new ConstraintTeachersNoMoreThanXHoursDaily(weight, compulsory, maxHoursSpinBox->value());

		bool tmp=gt.rules.addTimeConstraint(ctr);
		if(tmp)
			QMessageBox::information(this, QObject::tr("FET information"),
				QObject::tr("Constraint added"));
		else{
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Constraint NOT added - duplicate?"));
			delete ctr;
		}
		return;
	}
	else if(cTeachersSubgroupsNoMoreThanXHoursDailyRadioButton->isChecked()){
		if(maxHoursSpinBox->value()<=0){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Invalid max hours daily (x)"));
			return;
		}

		ctr=new ConstraintTeachersSubgroupsNoMoreThanXHoursDaily(weight, compulsory, maxHoursSpinBox->value());

		bool tmp=gt.rules.addTimeConstraint(ctr);
		if(tmp)
			QMessageBox::information(this, QObject::tr("FET information"),
				QObject::tr("Constraint added. Please be very careful: "
				"this constraint dramatically slows down the automatic generation "
				"of the timetable"));
		else{
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Constraint NOT added - duplicate?"));
			delete ctr;
		}
		return;
	}
	else if(cActivitiesNotOverlappingRadioButton->isChecked()){
		int act_id[MAX_CONSTRAINT_ACTIVITIES_NOT_OVERLAPPING];
		int n_act=sscanf(cActivitiesLineEdit->text(),
			"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			act_id, act_id+1, act_id+2, act_id+3, act_id+4,
			act_id+5, act_id+6, act_id+7, act_id+8, act_id+9,
			act_id+10, act_id+11, act_id+12, act_id+13, act_id+14,
			act_id+15, act_id+16, act_id+17, act_id+18, act_id+19);

		if(n_act<2){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Less than two activities not same time"));
			return;
		}

		ctr=new ConstraintActivitiesNotOverlapping(weight, compulsory, n_act, act_id);

		bool tmp=gt.rules.addTimeConstraint(ctr);
		if(tmp)
			QMessageBox::information(this, QObject::tr("FET information"),
				QObject::tr("Constraint added"));
		else{
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Constraint NOT added - duplicate?"));
			delete ctr;
		}
		return;
	}
	else if(cMinNDaysBetweenActivitiesRadioButton->isChecked()){
		int act_id[MAX_CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES];
		int n_act=sscanf(cActivitiesLineEdit->text(),
			"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			act_id, act_id+1, act_id+2, act_id+3, act_id+4,
			act_id+5, act_id+6, act_id+7, act_id+8, act_id+9,
			act_id+10, act_id+11, act_id+12, act_id+13, act_id+14,
			act_id+15, act_id+16, act_id+17, act_id+18, act_id+19);

		if(n_act<2){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Less than two activities"));
			return;
		}

		ctr=new ConstraintMinNDaysBetweenActivities(weight, compulsory, n_act, act_id, minHoursSpinBox->value());

		bool tmp=gt.rules.addTimeConstraint(ctr);
		if(tmp)
			QMessageBox::information(this, QObject::tr("FET information"),
				QObject::tr("Constraint added"));
		else{
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Constraint NOT added - duplicate?"));
			delete ctr;
		}
		return;
	}

	int day=dayComboBox->currentItem();
	if(day<0 || day>gt.rules.nDaysPerWeek){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid day"));
		return;
	}
	int startHour=startHourComboBox->currentItem();
	if(startHour<0 || startHour>gt.rules.nHoursPerDay){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid start hour"));
		return;
	}
	int endHour=-1;
	if(endHourComboBox->isEnabled()==true){
		endHour=endHourComboBox->currentItem();
		if(endHour<0 || endHour>gt.rules.nHoursPerDay){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Invalid end hour"));
			return;
		}
		if(endHour<=startHour){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("End hour <= start hour - impossible"));
			return;
		}
	}

	if(cTeacherNotAvailableRadioButton->isChecked()){
		QString teacher_name=teachersComboBox->currentText();
		int teacher_ID=gt.rules.searchTeacher(teacher_name);
		if(teacher_ID<0){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Invalid teacher"));
			return;
		}

		ctr=new ConstraintTeacherNotAvailable(weight, compulsory, teacher_name, day, startHour, endHour);
	}

	else if(cActivityPreferredTimeRadioButton->isChecked()){
		int i=activitiesComboBox->currentItem();
		if(i<0){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Invalid activity"));
			return;
		}
		if(day-1==-1 && startHour-1==-1){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Please specify at least a day or an hour"));
			return;
		}
		Activity* act=gt.rules.activitiesList.at(i);

		ctr=new ConstraintActivityPreferredTime(weight, compulsory, act->id, day-1, startHour-1);
	}

	else if(cActivityEndsDayRadioButton->isChecked()){
		int i=activitiesComboBox->currentItem();
		if(i<0){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Invalid activity"));
			return;
		}
		Activity* act=gt.rules.activitiesList.at(i);

		ctr=new ConstraintActivityEndsDay(weight, compulsory, act->id);
	}

	else if(cBreakRadioButton->isChecked()){
		ctr=new ConstraintBreak(weight, compulsory, day, startHour, endHour);
	}

	else if(cStudentsSetNotAvailableRadioButton->isChecked()){
		QString students_name=studentsComboBox->currentText();
		assert(gt.rules.searchStudentsSet(students_name) != NULL);

		ctr=new ConstraintStudentsSetNotAvailable(weight, compulsory, students_name, day, startHour, endHour);
	}
	else if(cStudentsSetIntervalMaxDaysPerWeekRadioButton->isChecked()){
		QString students_name=studentsComboBox->currentText();
		assert(gt.rules.searchStudentsSet(students_name) != NULL);

		if(maxHoursSpinBox->value()<=0){
			QMessageBox::warning(this, QObject::tr("FET information"),
				QObject::tr("Invalid max intervals"));
			return;
		}

		ctr=new ConstraintStudentsSetIntervalMaxDaysPerWeek(weight, compulsory, students_name, startHour, endHour, maxHoursSpinBox->value());
	}
	else{
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Invalid type"));
		return;
	}

	bool tmp2=gt.rules.addTimeConstraint(ctr);
	if(tmp2)
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Constraint added"));
	else{
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("Constraint NOT added - duplicate?"));
		delete ctr;
	}
}

void AddTimeConstraintForm::dayChanged()
{
	constraintChanged();
}

void AddTimeConstraintForm::startHourChanged()
{
	constraintChanged();
}

void AddTimeConstraintForm::endHourChanged()
{
	constraintChanged();
}

void AddTimeConstraintForm::typeChanged()
{
	if(cTeacherNotAvailableRadioButton->isChecked()){
		updatePeriodGroupBox();
	}
	else if(cActivityPreferredTimeRadioButton->isChecked()){
		updatePeriodGroupBox2();
	}
	else if(cBreakRadioButton->isChecked()){
		updatePeriodGroupBox();
	}
	else if(cStudentsSetNotAvailableRadioButton->isChecked()){
		updatePeriodGroupBox();
	}
	else if(cStudentsSetIntervalMaxDaysPerWeekRadioButton->isChecked()){
		updatePeriodGroupBox();
		updateHoursBox4();
	}
	else if(cTeacherMaxDaysPerWeekRadioButton->isChecked()){
		updatePeriodGroupBox();
		updateHoursBox4();
	}
	else if(cTeachersNoMoreThanXHoursContinuouslyRadioButton->isChecked()){
		updateHoursBox();
	}
	else if(cTeachersNoMoreThanXHoursDailyRadioButton->isChecked()){
		updateHoursBox();
	}
	else if(cTeachersSubgroupsNoMoreThanXHoursDailyRadioButton->isChecked()){
		updateHoursBox();
	}
	else if(cStudentsNHoursDailyRadioButton->isChecked()){
		updateHoursBox2();
	}
	else if(cStudentsSetNHoursDailyRadioButton->isChecked()){
		updateHoursBox2();
	}
	else if(cMinNDaysBetweenActivitiesRadioButton->isChecked()){
		updateHoursBox3();
	}

	constraintChanged();
}

void AddTimeConstraintForm::weightChanged()
{
	constraintChanged();
}

void AddTimeConstraintForm::maxHoursChanged()
{
	constraintChanged();
}

void AddTimeConstraintForm::minHoursChanged()
{
	constraintChanged();
}

void AddTimeConstraintForm::activitiesSameStartingTimeTextChanged()
{
	constraintChanged();
}

void AddTimeConstraintForm::addConstraintActivityPreferredTimes()
{
	AddConstraintActivityPreferredTimesForm* addConstraintActivityPreferredTimesForm=new AddConstraintActivityPreferredTimesForm();
	addConstraintActivityPreferredTimesForm->exec();
}
