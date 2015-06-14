//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Lalescu Liviu <liviu@lalescu.ro>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef INSTITUTIONNAMEFORM_H
#define INSTITUTIONNAMEFORM_H

#include "institutionnameform_template.h"

class InstitutionNameForm : public InstitutionNameForm_template
{
public:
	InstitutionNameForm();

	~InstitutionNameForm();

	void ok();
	void cancel();
};

#endif
