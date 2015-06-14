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
#ifndef BUILDINGSFORM_H
#define BUILDINGSFORM_H

#include "buildingsform_template.h"

/**
@author Liviu Lalescu
*/
class BuildingsForm : public BuildingsForm_template
{
public:
	BuildingsForm();

	~BuildingsForm();

	void addBuilding();

	void removeBuilding();

	void modifyBuilding();

	void sortBuildings();

	void buildingChanged(int index);
};

#endif
