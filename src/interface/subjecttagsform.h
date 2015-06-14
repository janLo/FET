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
#ifndef SUBJECTTAGSFORM_H
#define SUBJECTTAGSFORM_H

#include "subjecttagsform_template.h"

class SubjectTagsForm : public SubjectTagsForm_template
{
public:
	SubjectTagsForm();

	~SubjectTagsForm();

	void addSubjectTag();
	void removeSubjectTag();
	void renameSubjectTag();
	void sortSubjectTags();
	
	void subjectTagChanged(int index);
	
	void activateSubjectTag();
	void deactivateSubjectTag();
};

#endif
