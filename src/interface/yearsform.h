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
	void yearChanged();
};

#endif
