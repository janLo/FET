//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Lalescu Liviu <liviu@lalescu.ro>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fet.h"

#include "populationnumberform.h"

#include <qslider.h>
#include <qlabel.h>

PopulationNumberForm::PopulationNumberForm()
 : PopulationNumberForm_template()
{
	populationNumberSlider->setMaxValue(MAX_POPULATION_SIZE);
	populationNumberSlider->setValue(population_number);
	populationNumberSlider->setMinValue(1);

	QString s=QObject::tr("Population number (power of search)");
	s+="\n";
	s+=QString::number(population_number);
	populationNumberTextLabel->setText(s);
}


PopulationNumberForm::~PopulationNumberForm()
{
}

void PopulationNumberForm::populationNumberChanged()
{
	population_number=populationNumberSlider->value();

	QString s=QObject::tr("Population number (power of search)");
	s+="\n";
	s+=QString::number(population_number);
	populationNumberTextLabel->setText(s);
}
