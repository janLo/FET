//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Lalescu Liviu <liviu@lalescu.ro>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SUBJECTSFORM_H
#define SUBJECTSFORM_H

#include "subjectsform_template.h"

class SubjectsForm : public SubjectsForm_template
{
public:
	SubjectsForm();

	~SubjectsForm();

	void addSubject();
	void removeSubject();
	void renameSubject();
	void sortSubjects();
};

#endif
