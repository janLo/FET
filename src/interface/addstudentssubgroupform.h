/***************************************************************************
                          addstudentssubgroupform.h  -  description
                             -------------------
    begin                : Sat Jan 24 2004
    copyright            : (C) 2004 by Lalescu Liviu
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

#ifndef ADDSTUDENTSSUBGROUPFORM_H
#define ADDSTUDENTSSUBGROUPFORM_H

#include "ui_addstudentssubgroupform_template.h"

#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

//#include "fetmainform.h"

#include <qspinbox.h>
#include <qlineedit.h>
#include <q3textedit.h>


class AddStudentsSubgroupForm : public QDialog, Ui::AddStudentsSubgroupForm_template {
	Q_OBJECT
public:
	AddStudentsSubgroupForm(const QString& yearName, const QString& groupName);
	~AddStudentsSubgroupForm();

public slots:
	void addStudentsSubgroup();
};

#endif
