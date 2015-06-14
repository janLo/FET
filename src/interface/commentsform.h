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
#ifndef COMMENTSFORM_H
#define COMMENTSFORM_H

#include "commentsform_template.h"

class CommentsForm : public CommentsForm_template
{
public:
	CommentsForm();

	~CommentsForm();

	void ok();
	void cancel();
};

#endif
