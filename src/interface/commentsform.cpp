//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Lalescu Liviu <liviu@lalescu.ro>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fet.h"

#include "commentsform.h"

#include <qtextedit.h>

extern GeneticTimetable gt;

CommentsForm::CommentsForm()
 : CommentsForm_template()
{
	commentsTextEdit->setText(gt.rules.comments);
}

CommentsForm::~CommentsForm()
{
}

void CommentsForm::ok()
{
	gt.rules.comments=commentsTextEdit->text();

	this->close();
}

void CommentsForm::cancel()
{
	this->close();
}
