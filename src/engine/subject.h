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
#ifndef SUBJECT_H
#define SUBJECT_H

#include "genetictimetable_defs.h"

#include <qstring.h>
#include <qptrlist.h>

class Subject;

typedef QPtrList<Subject> SubjectsList;

/**
This class represents a subject

@author Liviu Lalescu
*/
class Subject{
public:
	QString name;

	Subject();
	~Subject();

	QString getXMLDescription();
};

#endif
