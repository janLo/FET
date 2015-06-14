/***************************************************************************
                          activitiesform.h  -  description
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

#ifndef ACTIVITIESFORM_H
#define ACTIVITIESFORM_H

#include "activitiesform_template.h"

class ActivitiesForm : public ActivitiesForm_template  {
public:
	ActivitiesList visibleActivitiesList;

	ActivitiesForm();
	~ActivitiesForm();

	void addActivity();
	void removeActivity();
	void activityChanged(int index);
	void teacherChanged(const QString& dummy);
	void studentsChanged(const QString& dummy);
	void filterChanged();
	bool filterOk(Activity* act);
};

#endif
