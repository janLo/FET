//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Liviu Lalescu <liviu@lalescu.ro>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TEACHER_H
#define TEACHER_H

#include "genetictimetable_defs.h"

#include <qstring.h>
#include <qptrlist.h>

class Teacher;

typedef QPtrList<Teacher> TeachersList;

/**
@author Liviu Lalescu
*/
class Teacher{
public:
	QString name;

	Teacher();
	~Teacher();

	QString getXMLDescription();
};

#endif
