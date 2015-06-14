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
#ifndef SUBGROUPSFORM_H
#define SUBGROUPSFORM_H

#include "subgroupsform_template.h"

class SubgroupsForm : public SubgroupsForm_template
{
public:
	SubgroupsForm();
	~SubgroupsForm();

	void addSubgroup();
	void removeSubgroup();
	void modifySubgroup();
	void sortSubgroups();
	void yearChanged(const QString &yearName);
	void groupChanged(const QString &groupName);
	void subgroupChanged(const QString &subgroupName);
};

#endif
