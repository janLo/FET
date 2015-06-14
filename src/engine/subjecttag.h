//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Liviu Lalescu <liviu@lalescu.ro>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SUBJECTTAG_H
#define SUBJECTTAG_H

#include "genetictimetable_defs.h"

#include <qstring.h>
#include <qptrlist.h>

class SubjectTag;

class SubjectTagsList: public QPtrList<SubjectTag>
{
	int compareItems(QPtrCollection::Item, QPtrCollection::Item);
};

/**
This class represents a subject tag

@author Liviu Lalescu
*/
class SubjectTag{
public:
	QString name;

	SubjectTag();
	~SubjectTag();

	QString getXMLDescription();
};

#endif
