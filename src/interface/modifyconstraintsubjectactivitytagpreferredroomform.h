/***************************************************************************
                          modifyconstraintsubjectactivitytagpreferredroomform.h  -  description
                             -------------------
    begin                : Aug 18, 2007
    copyright            : (C) 2007 by Lalescu Liviu
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

#ifndef MODIFYCONSTRAINTSUBJECTACTIVITYTAGPREFERREDROOMFORM_H
#define MODIFYCONSTRAINTSUBJECTACTIVITYTAGPREFERREDROOMFORM_H

#include "ui_modifyconstraintsubjectactivitytagpreferredroomform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

#include <q3combobox.h>
#include <q3listbox.h>
#include <qmessagebox.h>
#include <q3groupbox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <q3textedit.h>

class ModifyConstraintSubjectActivityTagPreferredRoomForm : public QDialog, Ui::ModifyConstraintSubjectActivityTagPreferredRoomForm_template  {
	Q_OBJECT
	
	ConstraintSubjectActivityTagPreferredRoom* _ctr;
public:
	ModifyConstraintSubjectActivityTagPreferredRoomForm(ConstraintSubjectActivityTagPreferredRoom* ctr);
	~ModifyConstraintSubjectActivityTagPreferredRoomForm();

	void updateSubjectsComboBox();
	void updateActivityTagsComboBox();
	void updateRoomsComboBox();

public slots:
	void ok();
	void cancel();
};

#endif
