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
#include "fetmainform.h"
#include "timetableallocatehoursform.h"
#include "timetableviewstudentsform.h"
#include "timetableviewteachersform.h"
#include "timetableshowconflictstimeform.h"
#include "timetableallocateroomsform.h"
#include "timetableviewroomsform.h"
#include "timetableshowconflictsspaceform.h"
#include "daysform.h"
#include "hoursform.h"
#include "subjectsform.h"
#include "teachersform.h"
#include "yearsform.h"
#include "groupsform.h"
#include "subgroupsform.h"
#include "activitiesform.h"
#include "equipmentsform.h"
#include "roomsform.h"
#include "timeconstraintsform.h"
#include "spaceconstraintsform.h"
#include "helpaboutform.h"
#include "populationnumberform.h"
#include "fet.h"
#include "constraint2activitiesconsecutiveform.h"
#include "constraint2activitiesgroupedform.h"
#include "constraintactivitiespreferredtimesform.h"
#include "constraintactivitiessamestartingtimeform.h"

#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qstring.h>
#include <qdir.h>

bool students_schedule_ready;
bool teachers_schedule_ready;
bool rooms_schedule_ready;

FetMainForm *fetMainForm_pointer;

QString timeConflictsString; //the string that contains a log of the broken time constraints
QString spaceConflictsString; //the string that contains a log of the broken space constraints

QTranslator* ptranslator;

extern QApplication* pqapplication;

FetMainForm::FetMainForm() : FetMainForm_template()
{
	ptranslator=NULL;

	fetMainForm_pointer=this;

	//new data
	if(gt.rules.initialized)
		gt.rules.kill();
	gt.rules.init();

	bool tmp=gt.rules.addTimeConstraint(new ConstraintBasicCompulsoryTime(1.0));
	assert(tmp);
	tmp=gt.rules.addSpaceConstraint(new ConstraintBasicCompulsorySpace(1.0));
	assert(tmp);

	students_schedule_ready=0;
	teachers_schedule_ready=0;
}

FetMainForm::~FetMainForm()
{
	if(QMessageBox::information( this, QObject::tr("FET - exiting"),
	 QObject::tr("File not saved - do you want to save it?"),
	 QObject::tr("&Yes"), QObject::tr("&No"), 0 , 1 ) == 0)
		this->fileSave();

	fetMainForm_pointer=NULL;

	INPUT_FILENAME_XML = "";
}

void FetMainForm::fileExit()
{
	close();
}

void FetMainForm::fileNew()
{
	INPUT_FILENAME_XML="";

	int confirm=0;
	switch( QMessageBox::information( this, QObject::tr("FET application"),
	 QObject::tr("Are you sure you want to load new data (rules) ?"),
	 QObject::tr("&Yes"), QObject::tr("&No"), 0 , 1 ) ) {
	case 0: // Yes
		confirm=1;
		break;
	case 1: // No
		confirm=0;
		break;
	}

	if(confirm){
		if(gt.rules.initialized)
			gt.rules.kill();
		gt.rules.init();

		bool tmp=gt.rules.addTimeConstraint(new ConstraintBasicCompulsoryTime(1.0));
		assert(tmp);
		tmp=gt.rules.addSpaceConstraint(new ConstraintBasicCompulsorySpace(1.0));
		assert(tmp);

		students_schedule_ready=0;
		teachers_schedule_ready=0;
	}
}

void FetMainForm::fileOpen()
{
	int confirm=1;

	if(confirm){
		QString s = QFileDialog::getOpenFileName(WORKING_DIRECTORY, QObject::tr("FET xml files (*.xml)"),
			this, QObject::tr("open file dialog"), QObject::tr("Choose a file"));
		if(s.isNull())
			return;
		
		if(gt.rules.read(s)){
			students_schedule_ready=0;
			teachers_schedule_ready=0;

			INPUT_FILENAME_XML = s;
		}
		else{
			QMessageBox::information(this, QObject::tr("FET info"), QObject::tr("Invalid file"), QObject::tr("&OK"));
		}
		//get the directory
		int tmp=s.findRev("/");
		WORKING_DIRECTORY=s.left(tmp+1);
	}
}

void FetMainForm::fileSaveAs()
{
	QString s = QFileDialog::getSaveFileName(
		WORKING_DIRECTORY, QObject::tr("FET xml file (*.xml)"),
		this, QObject::tr("Save file dialog"), QObject::tr("Choose a filename to save under" ));
	if(s==NULL)
		return;

	if(QFile::exists(s))
		if(QMessageBox::information( this, QObject::tr("FET"),
		 QObject::tr("File exists - are you sure you want to overwrite existing file?"),
		 QObject::tr("&Yes"), QObject::tr("&No"), 0 , 1 ) == 1)
		 	return;

	INPUT_FILENAME_XML = s;
	gt.rules.write(INPUT_FILENAME_XML);
}

void FetMainForm::fileSave()
{
	if(INPUT_FILENAME_XML == "")
		fileSaveAs();
	else
		gt.rules.write(INPUT_FILENAME_XML);
}

void FetMainForm::dataDays()
{
	DaysForm *daysForm=new DaysForm();
	daysForm->exec();
}

void FetMainForm::dataHours()
{
	HoursForm *hoursForm=new HoursForm();
	hoursForm->exec();
}

void FetMainForm::dataTeachers()
{
	TeachersForm *teachersForm=new TeachersForm();
	teachersForm->exec();
}

void FetMainForm::dataSubjects()
{
	SubjectsForm *subjectsForm=new SubjectsForm();
	subjectsForm->exec();
}

void FetMainForm::dataYears()
{
	YearsForm *yearsForm=new YearsForm();
	yearsForm->exec();
}

void FetMainForm::dataGroups()
{
	GroupsForm *groupsForm=new GroupsForm();
	groupsForm->exec();
}

void FetMainForm::dataSubgroups()
{
	SubgroupsForm* subgroupsForm=new SubgroupsForm();
	subgroupsForm->exec();
}

void FetMainForm::dataActivities()
{
	ActivitiesForm* activitiesForm=new ActivitiesForm();
	activitiesForm->exec();
}

void FetMainForm::dataEquipments()
{
	EquipmentsForm* equipmentsForm=new EquipmentsForm();
	equipmentsForm->exec();
}

void FetMainForm::dataRooms()
{
	RoomsForm* roomsForm=new RoomsForm();
	roomsForm->exec();
}

void FetMainForm::dataTimeConstraints()
{
	TimeConstraintsForm* timeConstraintsForm=new TimeConstraintsForm();
	timeConstraintsForm->exec();
}

void FetMainForm::dataSpaceConstraints()
{
	SpaceConstraintsForm* spaceConstraintsForm=new SpaceConstraintsForm();
	spaceConstraintsForm->exec();
}

void FetMainForm::dataTimeConstraints22ActivitiesConsecutive()
{
	Constraint2ActivitiesConsecutiveForm* constraint2ActivitiesConsecutiveForm=new Constraint2ActivitiesConsecutiveForm();
	constraint2ActivitiesConsecutiveForm->exec();
}

void FetMainForm::dataTimeConstraints22ActivitiesGrouped()
{
	Constraint2ActivitiesGroupedForm* constraint2ActivitiesGroupedForm=new Constraint2ActivitiesGroupedForm();
	constraint2ActivitiesGroupedForm->exec();
}

void FetMainForm::dataTimeConstraints2ActivitiesPreferredTimes()
{
	ConstraintActivitiesPreferredTimesForm* constraintActivitiesPreferredTimesForm=new ConstraintActivitiesPreferredTimesForm();
	constraintActivitiesPreferredTimesForm->exec();
}

void FetMainForm::dataTimeConstraints2ActivitiesSameStartingTime()
{
	ConstraintActivitiesSameStartingTimeForm* constraintActivitiesSameStartingTimeForm=new ConstraintActivitiesSameStartingTimeForm();
	constraintActivitiesSameStartingTimeForm->exec();
}

void FetMainForm::helpAbout()
{
	HelpAboutForm *helpAboutForm=new HelpAboutForm();
	helpAboutForm->show();
}

void FetMainForm::helpFAQ()
{
	QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please read the file doc/FAQ"));
}

void FetMainForm::timetableAllocateHours()
{
	if(gt.rules.activitiesList.count()<2){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please input at least two activities before allocating hours"));
		return;
	}
	TimetableAllocateHoursForm *timetableAllocateHoursForm=new TimetableAllocateHoursForm();
	timetableAllocateHoursForm->show();
}

void FetMainForm::timetableViewStudents()
{
	if(!(students_schedule_ready && teachers_schedule_ready)){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the hours, firstly"));
		return;
	}

	TimetableViewStudentsForm *timetableViewStudentsForm=new TimetableViewStudentsForm();
	timetableViewStudentsForm->show();
}

void FetMainForm::timetableViewTeachers()
{
	if(!(students_schedule_ready && teachers_schedule_ready)){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the hours, firstly"));
		return;
	}

	TimetableViewTeachersForm *timetableViewTeachersForm=new TimetableViewTeachersForm();
	timetableViewTeachersForm->show();
}

void FetMainForm::timetableShowConflictsTime()
{
	if(!(students_schedule_ready && teachers_schedule_ready)){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the hours, firstly"));
		return;
	}

	TimetableShowConflictsTimeForm *timetableShowConflictsTimeForm=new TimetableShowConflictsTimeForm();
	timetableShowConflictsTimeForm->show();
}

void FetMainForm::timetableAllocateRooms()
{
	if(!gt.rules.internalStructureComputed){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the hours prior to allocating the rooms"));
		return;
	}
	if(! (students_schedule_ready && teachers_schedule_ready)){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the hours prior to allocating the rooms"));
		return;
	}
	if(gt.rules.roomsList.count()<=0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please input at least one room before that"));
		return;
	}
	TimetableAllocateRoomsForm *timetableAllocateRoomsForm=new TimetableAllocateRoomsForm();
	timetableAllocateRoomsForm->show();
}

void FetMainForm::timetableViewRooms()
{
	if(!rooms_schedule_ready){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the rooms, firstly"));
		return;
	}

	TimetableViewRoomsForm *timetableViewRoomsForm=new TimetableViewRoomsForm();
	timetableViewRoomsForm->show();
}

void FetMainForm::timetableShowConflictsSpace()
{
	if(!rooms_schedule_ready){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the rooms, firstly"));
		return;
	}

	TimetableShowConflictsSpaceForm *timetableShowConflictsSpaceForm=new TimetableShowConflictsSpaceForm();
	timetableShowConflictsSpaceForm->show();
}

void FetMainForm::timetableExport()
{
	QMessageBox::information(this, QObject::tr("FET information"), 
	  QObject::tr("Please enter the directory \"%1\" and you will\n"
	  "find the exported .xml files \"studentstimetable.xml\",\n"
	  " \"teacherstimetable.xml\" and \"roomstimetable.xml\".\n"
	  "You can translate these .xml files into .tex or .html\n"
	  "using the files \"fetxml2latex.py\" or \"students.xsl\", \n"
	  " \"teachers.xsl\" and \"rooms.xsl\" "
	  ).arg(OUTPUT_DIR));
}

void FetMainForm::languageEnglish()
{
	if(ptranslator!=NULL){
		pqapplication->removeTranslator(ptranslator);
		delete ptranslator;
		ptranslator=NULL;
	}
}

void FetMainForm::languageRomanian()
{
	bool existing=true;
	if(ptranslator==NULL){
		existing=false;
		ptranslator=new QTranslator(0);
	}
	
	QDir d("/usr/share/fet/translations");
	if(d.exists())
		ptranslator->load("fet_ro", "/usr/share/fet/translations");
	else
		ptranslator->load("fet_ro", "translations");
	if(!existing)
		pqapplication->installTranslator(ptranslator);
}

void FetMainForm::languageFrench()
{
	bool existing=true;
	if(ptranslator==NULL){
		existing=false;
		ptranslator=new QTranslator(0);
	}

	QDir d("/usr/share/fet/translations");
	if(d.exists())
		ptranslator->load("fet_fr", "/usr/share/fet/translations");
	else
		ptranslator->load("fet_fr", "translations");

	if(!existing)
		pqapplication->installTranslator(ptranslator);
}

void FetMainForm::parametersPopulationNumber()
{
	PopulationNumberForm* populationNumberForm=new PopulationNumberForm();
	populationNumberForm->exec();
}
