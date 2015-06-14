/***************************************************************************
                          timetableviewroomsform.h  -  description
                             -------------------
    begin                : Wed May 14 2003
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

#ifndef TIMETABLEVIEWROOMSFORM_H
#define TIMETABLEVIEWROOMSFORM_H

#include "timetableviewroomsform_template.h"

class TimetableViewRoomsForm : public TimetableViewRoomsForm_template  {
public:
	TimetableViewRoomsForm();
	~TimetableViewRoomsForm();

	void roomChanged(const QString& roomName);

	void updateRoomsTimetableTable();

	void detailActivity(int row, int col);
};

#endif
