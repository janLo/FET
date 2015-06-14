/***************************************************************************
                          timeconstraintsform.cpp  -  description
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

#include "timeconstraintsform.h"
#include "addtimeconstraintform.h"

TimeConstraintsForm::TimeConstraintsForm()
{
	this->visibleConstraintsList.setAutoDelete(false);
	this->filterChanged();
}

TimeConstraintsForm::~TimeConstraintsForm()
{
}

bool TimeConstraintsForm::filterOk(TimeConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_BASIC_COMPULSORY_TIME)
		return basicCompulsoryTimeCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_TEACHER_NOT_AVAILABLE)
		return teacherNotAvailableCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_STUDENTS_SET_NOT_AVAILABLE)
		return studentsSetNotAvailableCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_TEACHERS_NO_MORE_THAN_X_HOURS_CONTINUOUSLY)
		return teachersNoMoreThanXHoursContinuouslyCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_TEACHERS_NO_MORE_THAN_X_HOURS_DAILY)
		return teachersNoMoreThanXHoursDailyCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_TEACHERS_SUBGROUPS_NO_MORE_THAN_X_HOURS_DAILY)
		return teachersSubgroupsNoMoreThanXHoursDailyCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_TEACHER_MAX_DAYS_PER_WEEK)
		return teacherMaxDaysPerWeekCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_TIME)
		return activityPreferredTimeCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES)
		return minNDaysBetweenActivitiesCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_ACTIVITIES_NOT_OVERLAPPING)
		return activitiesNotOverlappingCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_BREAK)
		return breakCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_TEACHERS_NO_GAPS)
		return teachersNoGapsCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_STUDENTS_NO_GAPS)
		return studentsNoGapsCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_STUDENTS_SET_NO_GAPS)
		return studentsSetNoGapsCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_STUDENTS_EARLY)
		return studentsEarlyCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_STUDENTS_N_HOURS_DAILY)
		return studentsNHoursDailyCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_STUDENTS_SET_N_HOURS_DAILY)
		return studentsSetNHoursDailyCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_TIMES)
		return activityPreferredTimesCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_ACTIVITY_ENDS_DAY)
		return activityEndsDayCheckBox->isChecked();
	else if(ctr->type==CONSTRAINT_STUDENTS_SET_INTERVAL_MAX_DAYS_PER_WEEK)
		return studentsSetIntervalMaxDaysPerWeekCheckBox->isChecked();
	else
		return false;
}

void TimeConstraintsForm::filterChanged()
{
	this->visibleConstraintsList.clear();
	constraintsListBox->clear();
	for(TimeConstraint* ctr=gt.rules.timeConstraintsList.first(); ctr; ctr=gt.rules.timeConstraintsList.next())
		if(filterOk(ctr)){
			QString s;
			s=ctr->getDescription(gt.rules);
			visibleConstraintsList.append(ctr);
			constraintsListBox->insertItem(s);
		}

	constraintsListBox->setCurrentItem(0);
	this->constraintChanged(constraintsListBox->currentItem());
}

void TimeConstraintsForm::constraintChanged(int index)
{
	if(index<0)
		return;
	QString s;
	assert(index<this->visibleConstraintsList.count());
	TimeConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=NULL);
	s=ctr->getDetailedDescription(gt.rules);
	currentConstraintTextEdit->setText(s);
}

void TimeConstraintsForm::addConstraint()
{
	AddTimeConstraintForm *addTimeConstraintForm=new AddTimeConstraintForm();
	addTimeConstraintForm->exec();

	filterChanged();
}

void TimeConstraintsForm::removeConstraint()
{
	int i=constraintsListBox->currentItem();
	if(i<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected constraint"));
		return;
	}
	TimeConstraint* ctr=this->visibleConstraintsList.at(i);
	if(ctr->type==CONSTRAINT_BASIC_COMPULSORY_TIME){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("You cannot erase the basic time constraints"));
		return;
	}
	QString s;
	s=QObject::tr("Removing constraint:\n");
	s+=ctr->getDetailedDescription(gt.rules);
	s+=QObject::tr("\nAre you sure?");

	switch( QMessageBox::warning( this, QObject::tr("FET warning"),
		s, QObject::tr("OK"), QObject::tr("Cancel"), 0, 0, 1 ) ){
	case 0: // The user clicked the OK again button or pressed Enter
		gt.rules.removeTimeConstraint(ctr);
		filterChanged();
		break;
	case 1: // The user clicked the Cancel or pressed Escape
		break;
	}
}
