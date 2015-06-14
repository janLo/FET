//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Liviu Lalescu <Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "timetable_defs.h"
#include "fet.h"
#include "buildingsform.h"
#include "addbuildingform.h"
#include "modifybuildingform.h"

#include <q3listbox.h>
#include <qinputdialog.h>

#include <QDesktopWidget>

#include <QMessageBox>

BuildingsForm::BuildingsForm()
 : BuildingsForm_template()
{
	//setWindowFlags(Qt::Window);
	/*setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	QDesktopWidget* desktop=QApplication::desktop();
	int xx=desktop->width()/2 - frameGeometry().width()/2;
	int yy=desktop->height()/2 - frameGeometry().height()/2;
	move(xx, yy);*/
	centerWidgetOnScreen(this);
	
	this->filterChanged();
}


BuildingsForm::~BuildingsForm()
{
}

bool BuildingsForm::filterOk(Building* bu)
{
	Q_UNUSED(bu);

	bool ok=true;

	return ok;
}

void BuildingsForm::filterChanged()
{
	QString s;
	buildingsListBox->clear();
	visibleBuildingsList.clear();
	for(int i=0; i<gt.rules.buildingsList.size(); i++){
		Building* bu=gt.rules.buildingsList[i];
		if(this->filterOk(bu)){
			s=bu->getDescription();
			visibleBuildingsList.append(bu);
			buildingsListBox->insertItem(s);
		}
	}
	buildingChanged(buildingsListBox->currentItem());
}

void BuildingsForm::addBuilding()
{
	int ind=buildingsListBox->currentItem();

	AddBuildingForm* addBuildingForm=new AddBuildingForm();
	addBuildingForm->exec();
	
	filterChanged();
	
	buildingsListBox->setCurrentItem(ind);
}

void BuildingsForm::removeBuilding()
{
	int ind=buildingsListBox->currentItem();
	if(ind<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected building"));
		return;
	}
	
	Building* bu=visibleBuildingsList.at(ind);
	assert(bu!=NULL);

	if(QMessageBox::warning( this, QObject::tr("FET"),
		QObject::tr("Are you sure you want to delete this building?\n"),
		QObject::tr("Yes"), QObject::tr("No"), 0, 0, 1 ) == 1)
		return;
		
	bool tmp=gt.rules.removeBuilding(bu->name);
	assert(tmp);

	filterChanged();
	
	if((uint)(ind)>=buildingsListBox->count())
		ind=buildingsListBox->count()-1;
	buildingsListBox->setCurrentItem(ind);
}

void BuildingsForm::buildingChanged(int index)
{
	if(index<0){
		currentBuildingTextEdit->setText(QObject::tr("Invalid building"));
		return;
	}

	QString s;
	Building* building=visibleBuildingsList.at(index);

	assert(building!=NULL);
	s=building->getDetailedDescriptionWithConstraints(gt.rules);
	currentBuildingTextEdit->setText(s);
}

void BuildingsForm::sortBuildings()
{
	gt.rules.sortBuildingsAlphabetically();

	filterChanged();
}

void BuildingsForm::modifyBuilding()
{
	int ci=buildingsListBox->currentItem();
	if(ci<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected building"));
		return;
	}
	
	Building* bu=visibleBuildingsList.at(ci);
	ModifyBuildingForm* form=new ModifyBuildingForm(bu->name);
	form->exec();

	filterChanged();
	
	buildingsListBox->setCurrentItem(ci);
}