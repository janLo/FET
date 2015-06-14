/***************************************************************************
                          timetableviewstudentswithroomsform.h  -  description
                             -------------------
    begin                : 6 Feb 2005
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

#ifndef TIMETABLEVIEWSTUDENTSWITHROOMSFORM_H
#define TIMETABLEVIEWSTUDENTSWITHROOMSFORM_H

#include "timetableviewstudentswithroomsform_template.h"

class TimetableViewStudentsWithRoomsForm : public TimetableViewStudentsWithRoomsForm_template  {
public: 
	TimetableViewStudentsWithRoomsForm();
	~TimetableViewStudentsWithRoomsForm();

	void yearChanged(const QString& yearName);
	void groupChanged(const QString& groupName);
	void subgroupChanged(const QString& subgroupName);

	void updateStudentsTimetableTable();
	
	void detailActivity(int row, int col);
};

#endif
