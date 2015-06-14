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
public:
	PopulationNumberForm();

	~PopulationNumberForm();

	void populationNumberChanged();
};

#endif
