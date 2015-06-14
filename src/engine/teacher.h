//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Liviu Lalescu <Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)>, (C) 2003
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
class Rules;

class TeachersList: public QPtrList<Teacher>
{
	int compareItems(QPtrCollection::Item,QPtrCollection::Item);
};

/**
@author Liviu Lalescu
*/
class Teacher
{
public:
	QString name;

	Teacher();
	~Teacher();

	QString getXmlDescription();
	QString getDetailedDescription();
	QString getDetailedDescriptionWithConstraints(Rules& r);
};

#endif
