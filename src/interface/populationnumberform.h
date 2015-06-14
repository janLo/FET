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
#ifndef POPULATIONNUMBERFORM_H
#define POPULATIONNUMBERFORM_H

#include "populationnumberform_template.h"

class PopulationNumberForm : public PopulationNumberForm_template
{
	int p_n;
	
public:
	PopulationNumberForm();

	~PopulationNumberForm();

	void populationNumberChanged();
	
	void ok();
	void cancel();
};

#endif
