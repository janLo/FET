//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Lalescu Liviu <Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef YEARSFORM_H
#define YEARSFORM_H

#include "yearsform_template.h"

class YearsForm : public YearsForm_template
{
public:
	YearsForm();
	~YearsForm();

	void addYear();
	void removeYear();
	void modifyYear();
	void sortYears();
	void yearChanged();
	
	void activateStudents();
	void deactivateStudents();
	
	void divideYear();
};

#endif
