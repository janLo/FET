/***************************************************************************
                          modifyactivityform.h  -  description
                             -------------------
    begin                : Feb 9, 2005
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

#ifndef MODIFYACTIVITYFORM_H
#define MODIFYACTIVITYFORM_H

#include "modifyactivityform_template.h"

#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fet.h"

#include "fetmainform.h"

#include <qcombobox.h>
#include <qmessagebox.h>
#include <qgroupbox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qlistbox.h>


class ModifyActivityForm : public ModifyActivityForm_template  {
public:
	int _id;
	int _activityGroupId;
	Activity* _activity;
	QStringList _teachers;
	QString _subject;
	QString _subjectTag;
	QStringList _students;

	ModifyActivityForm(int id, int activityGroupId);
	~ModifyActivityForm();

	void addTeacher();
	void removeTeacher();
	void addStudents();
	void removeStudents();
	void updateStudentsListBox();
	void updateTeachersListBox();
	void updateSubjectsComboBox();
	void updateSubjectTagsComboBox();
	
	void clearTeachers();
	void clearStudents();

	void subjectChanged(const QString& dummy);
	void subjectTagChanged(const QString& dummy);
	void parityChanged();
	void durationChanged();
	void activityChanged();
	
	void showYearsChanged();
	void showGroupsChanged();
	void showSubgroupsChanged();

	void ok();
	void cancel();
};

#endif
