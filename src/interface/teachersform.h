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
#ifndef TEACHERSFORM_H
#define TEACHERSFORM_H

#include "teachersform_template.h"

class TeachersForm : public TeachersForm_template
{
public:
	TeachersForm();

	~TeachersForm();

	void addTeacher();
	void removeTeacher();
	void renameTeacher();
	void sortTeachers();
	
	void teacherChanged(int index);
	
	void activateTeacher();
	void deactivateTeacher();
};

#endif
