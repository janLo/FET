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

#include "institutionnameform.h"

#include <qlineedit.h>

extern GeneticTimetable gt;

InstitutionNameForm::InstitutionNameForm()
 : InstitutionNameForm_template()
{
	institutionNameLineEdit->setText(gt.rules.institutionName);
}

InstitutionNameForm::~InstitutionNameForm()
{
}

void InstitutionNameForm::ok()
{
	gt.rules.institutionName=institutionNameLineEdit->text();

	this->close();
}

void InstitutionNameForm::cancel()
{
	this->close();
}
