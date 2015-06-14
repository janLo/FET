//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Lalescu Liviu <Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)>, (C) 2005
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
