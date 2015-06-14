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
#ifndef HOURSFORM_H
#define HOURSFORM_H

#include "hoursform_template.h"

class HoursForm : public HoursForm_template
{
public:
	HoursForm();

	~HoursForm();

	void hoursChanged();
	void ok();
	void cancel();
};

#endif
