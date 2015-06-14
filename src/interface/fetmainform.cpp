//
//
// C++ Implementation: $MODULE$
//
// Description:
//
//
// Author: Lalescu Liviu <Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)>, (C) 2003
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
#include "timetableviewstudentswithroomsform.h"
#include "timetableviewteacherswithroomsform.h"
#include "timetableviewstudentswithrooms2form.h"
#include "timetableviewteacherswithrooms2form.h"
#include "timetableshowconflictstimeform.h"
#include "timetableallocateroomsform.h"
#include "timetableallocatehoursroomsform.h"
#include "timetableviewroomsform.h"
#include "timetableshowconflictsspaceform.h"
#include "timetableviewrooms2form.h"
#include "timetableshowconflictstimespaceform.h"
#include "timetableexporticalteachersform.h"
#include "timetableexporticalstudentsform.h"
#include "timetableexporticalteacherswithrooms1form.h"
#include "timetableexporticalstudentswithrooms1form.h"
#include "timetableexporticalteacherswithrooms2form.h"
#include "timetableexporticalstudentswithrooms2form.h"
#include "institutionnameform.h"
#include "commentsform.h"
#include "daysform.h"
#include "hoursform.h"
#include "subjectsform.h"
#include "subjecttagsform.h"
#include "teachersform.h"
#include "yearsform.h"
#include "groupsform.h"
#include "subgroupsform.h"
#include "activitiesform.h"
#include "equipmentsform.h"
#include "buildingsform.h"
#include "roomsform.h"
#include "alltimeconstraintsform.h"
#include "allspaceconstraintsform.h"
#include "helpaboutform.h"
#include "helpfaqform.h"
#include "helptimeconstraintsform.h"
#include "helptimeconstraintssubtagsform.h"
#include "populationnumberform.h"
#include "fet.h"
#include "constraint2activitiesconsecutiveform.h"
#include "constraint2activitiesorderedform.h"
#include "constraint2activitiesgroupedform.h"
#include "constraintactivitiespreferredtimesform.h"
#include "constraintactivitiessamestartingtimeform.h"
#include "constraintactivitiessamestartinghourform.h"
#include "constraintteachernotavailableform.h"
#include "constraintbasiccompulsorytimeform.h"
#include "constraintbasiccompulsoryspaceform.h"
#include "constraintteacherrequiresroomform.h"
#include "constraintteachersubjectrequireroomform.h"
#include "constraintroomnotavailableform.h"
#include "constraintactivitypreferredroomform.h"
#include "constraintminimizenumberofroomsforstudentsform.h"
#include "constraintminimizenumberofroomsforteachersform.h"
#include "constraintroomtypenotallowedsubjectsform.h"
#include "constraintsubjectrequiresequipmentsform.h"
#include "constraintsubjectsubjecttagrequireequipmentsform.h"
#include "constraintstudentssetnotavailableform.h"
#include "constraintbreakform.h"
#include "constraintteachermaxdaysperweekform.h"
#include "constraintteachersmaxhourscontinuouslyform.h"
#include "constraintteachersmaxhoursdailyform.h"
#include "constraintteacherssubgroupsmaxhoursdailyform.h"
#include "constraintactivitypreferredtimeform.h"
#include "constraintstudentssetnogapsform.h"
#include "constraintstudentsnogapsform.h"
#include "constraintteachersnogapsform.h"
#include "constraintstudentsearlyform.h"
#include "constraintstudentssetintervalmaxdaysperweekform.h"
#include "constraintstudentssetnhoursdailyform.h"
#include "constraintstudentsnhoursdailyform.h"
#include "constraintactivityendsdayform.h"
#include "constraintactivitiesnotoverlappingform.h"
#include "constraintminndaysbetweenactivitiesform.h"
#include "constraintactivitypreferredtimesform.h"
#include "constraintteacherssubjecttagsmaxhourscontinuouslyform.h"
#include "constraintteacherssubjecttagmaxhourscontinuouslyform.h"
#include "constraintactivitiessameroomform.h"
#include "constraintactivitypreferredroomsform.h"
#include "constraintsubjectpreferredroomform.h"
#include "constraintsubjectsubjecttagpreferredroomform.h"
#include "constraintsubjectpreferredroomsform.h"
#include "constraintsubjectsubjecttagpreferredroomsform.h"
#include "constraintmaxbuildingchangesperdayforteachersform.h"
#include "constraintmaxbuildingchangesperdayforstudentsform.h"
#include "constraintmaxroomchangesperdayforteachersform.h"
#include "constraintmaxroomchangesperdayforstudentsform.h"

#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qstring.h>
#include <qdir.h>
#include <qpopupmenu.h>

bool simulation_running; //true if the user started an allocation of the timetable

bool students_schedule_ready;
bool teachers_schedule_ready;
bool rooms_schedule_ready;
bool students_schedule_ready2;
bool teachers_schedule_ready2;
bool rooms_schedule_ready2;

SpaceChromosome best_space_chromosome;
TimeChromosome best_time_chromosome;
TimeSpaceChromosome best_time_space_chromosome;

FetMainForm* fetMainForm_pointer;

QString timeConflictsString; //the string that contains a log of the broken time constraints
QString spaceConflictsString; //the string that contains a log of the broken space constraints
QString timeSpaceConflictsString; //the string that contains a log of the broken time&space constraints

QTranslator* ptranslator;

extern QApplication* pqapplication;

FetMainForm::FetMainForm() : FetMainForm_template()
{
	ptranslator=NULL;

	fetMainForm_pointer=this;
	
	if(FET_LANGUAGE=="en_GB")
		this->languageEnglish();
	else if(FET_LANGUAGE=="fr")
		this->languageFrench();
	else if(FET_LANGUAGE=="ro")
		this->languageRomanian();
	else if(FET_LANGUAGE=="ca")
		this->languageCatalan();
	else if(FET_LANGUAGE=="ms")
		this->languageMalay();
	else if(FET_LANGUAGE=="pl")
		this->languagePolish();
	else if(FET_LANGUAGE=="tr")
		this->languageTurkish();
	else if(FET_LANGUAGE=="nl")
		this->languageDutch();

	//new data
	if(gt.rules.initialized)
		gt.rules.kill();
	gt.rules.init();

	bool tmp=gt.rules.addTimeConstraint(new ConstraintBasicCompulsoryTime(1.0));
	assert(tmp);
	tmp=gt.rules.addSpaceConstraint(new ConstraintBasicCompulsorySpace(1.0));
	assert(tmp);

	students_schedule_ready=false;
	teachers_schedule_ready=false;
	rooms_schedule_ready=false;
	students_schedule_ready2=false;
	teachers_schedule_ready2=false;
	rooms_schedule_ready2=false;
	
	languageMenu->setCheckable(true);
}

FetMainForm::~FetMainForm()
{
	if(QMessageBox::information( this, QObject::tr("FET - exiting"),
	 QObject::tr("File not saved - do you want to save it?"),
	 QObject::tr("&Yes"), QObject::tr("&No"), QString::null, 0 , 1 ) == 0)
		this->fileSave();

	fetMainForm_pointer=NULL;

	INPUT_FILENAME_XML = "";
}

void FetMainForm::fileExit()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	close();
}

void FetMainForm::fileNew()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

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

		students_schedule_ready=false;
		teachers_schedule_ready=false;
		students_schedule_ready2=false;
		teachers_schedule_ready2=false;
		rooms_schedule_ready2=false;
	}
}

void FetMainForm::fileOpen()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	int confirm=1;

	if(confirm){
		QString s = QFileDialog::getOpenFileName(WORKING_DIRECTORY, QObject::tr("FET xml files (*.fet);;Old FET xml files (*.xml);;All files (*)"),
			this, QObject::tr("open file dialog"), QObject::tr("Choose a file"));
		if(s.isNull())
			return;
		
		if(gt.rules.read(s)){
			students_schedule_ready=false;
			teachers_schedule_ready=false;
			students_schedule_ready2=false;
			teachers_schedule_ready2=false;
			rooms_schedule_ready2=false;

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
		INPUT_FILENAME_XML, QObject::tr("FET xml files (*.fet);;All files (*)"),
		this, QObject::tr("Save file dialog"), QObject::tr("Choose a filename to save under" ));
	if(s==NULL)
		return;
		
	if(s.right(4)!=".fet")
		s+=".fet";

	int tmp=s.findRev("/");
	WORKING_DIRECTORY=s.left(tmp+1);

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

void FetMainForm::dataInstitutionName()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	InstitutionNameForm* institutionNameForm=new InstitutionNameForm();
	institutionNameForm->exec();
}

void FetMainForm::dataComments()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	CommentsForm* commentsForm=new CommentsForm();
	commentsForm->exec();
}

void FetMainForm::dataDays()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	DaysForm* daysForm=new DaysForm();
	daysForm->exec();
}

void FetMainForm::dataHours()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	HoursForm* hoursForm=new HoursForm();
	hoursForm->exec();
}

void FetMainForm::dataTeachers()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	TeachersForm* teachersForm=new TeachersForm();
	teachersForm->exec();
}

void FetMainForm::dataSubjects()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	SubjectsForm* subjectsForm=new SubjectsForm();
	subjectsForm->exec();
}

void FetMainForm::dataSubjectTags()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	SubjectTagsForm* subjectTagsForm=new SubjectTagsForm();
	subjectTagsForm->exec();
}

void FetMainForm::dataYears()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	YearsForm* yearsForm=new YearsForm();
	yearsForm->exec();
}

void FetMainForm::dataGroups()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	GroupsForm* groupsForm=new GroupsForm();
	groupsForm->exec();
}

void FetMainForm::dataSubgroups()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	SubgroupsForm* subgroupsForm=new SubgroupsForm();
	subgroupsForm->exec();
}

void FetMainForm::dataActivities()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ActivitiesForm* activitiesForm=new ActivitiesForm();
	activitiesForm->exec();
}

void FetMainForm::dataEquipments()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	EquipmentsForm* equipmentsForm=new EquipmentsForm();
	equipmentsForm->exec();
}

void FetMainForm::dataBuildings()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	BuildingsForm* buildingsForm=new BuildingsForm();
	buildingsForm->exec();
}

void FetMainForm::dataRooms()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	RoomsForm* roomsForm=new RoomsForm();
	roomsForm->exec();
}

void FetMainForm::dataAllTimeConstraints()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	AllTimeConstraintsForm* allTimeConstraintsForm=new AllTimeConstraintsForm();
	allTimeConstraintsForm->exec();
}

void FetMainForm::dataAllSpaceConstraints()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	AllSpaceConstraintsForm* allSpaceConstraintsForm=new AllSpaceConstraintsForm();
	allSpaceConstraintsForm->exec();
}

void FetMainForm::dataTimeConstraints2ActivitiesConsecutive()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	Constraint2ActivitiesConsecutiveForm* constraint2ActivitiesConsecutiveForm=new Constraint2ActivitiesConsecutiveForm();
	constraint2ActivitiesConsecutiveForm->exec();
}

void FetMainForm::dataTimeConstraints2ActivitiesOrdered()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	Constraint2ActivitiesOrderedForm* constraint2ActivitiesOrderedForm=new Constraint2ActivitiesOrderedForm();
	constraint2ActivitiesOrderedForm->exec();
}

void FetMainForm::dataTimeConstraints2ActivitiesGrouped()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	Constraint2ActivitiesGroupedForm* constraint2ActivitiesGroupedForm=new Constraint2ActivitiesGroupedForm();
	constraint2ActivitiesGroupedForm->exec();
}

void FetMainForm::dataTimeConstraintsActivitiesPreferredTimes()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintActivitiesPreferredTimesForm* constraintActivitiesPreferredTimesForm=new ConstraintActivitiesPreferredTimesForm();
	constraintActivitiesPreferredTimesForm->exec();
}

void FetMainForm::dataTimeConstraintsActivitiesSameStartingTime()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintActivitiesSameStartingTimeForm* constraintActivitiesSameStartingTimeForm=new ConstraintActivitiesSameStartingTimeForm();
	constraintActivitiesSameStartingTimeForm->exec();
}

void FetMainForm::dataTimeConstraintsActivitiesSameStartingHour()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintActivitiesSameStartingHourForm* constraintActivitiesSameStartingHourForm=new ConstraintActivitiesSameStartingHourForm();
	constraintActivitiesSameStartingHourForm->exec();
}

void FetMainForm::dataTimeConstraintsTeacherNotAvailable()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintTeacherNotAvailableForm* constraintTeacherNotAvailableForm=new ConstraintTeacherNotAvailableForm();
	constraintTeacherNotAvailableForm->exec();
}

void FetMainForm::dataTimeConstraintsBasicCompulsoryTime()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintBasicCompulsoryTimeForm* constraintBasicCompulsoryTimeForm=new ConstraintBasicCompulsoryTimeForm();
	constraintBasicCompulsoryTimeForm->exec();
}

void FetMainForm::dataSpaceConstraintsBasicCompulsorySpace()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintBasicCompulsorySpaceForm* constraintBasicCompulsorySpaceForm=new ConstraintBasicCompulsorySpaceForm();
	constraintBasicCompulsorySpaceForm->exec();
}

void FetMainForm::dataSpaceConstraintsTeacherRequiresRoom()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintTeacherRequiresRoomForm* constraintTeacherRequiresRoomForm=new ConstraintTeacherRequiresRoomForm();
	constraintTeacherRequiresRoomForm->exec();
}

void FetMainForm::dataSpaceConstraintsTeacherSubjectRequireRoom()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintTeacherSubjectRequireRoomForm* constraintTeacherSubjectRequireRoomForm=new ConstraintTeacherSubjectRequireRoomForm();
	constraintTeacherSubjectRequireRoomForm->exec();
}

void FetMainForm::dataSpaceConstraintsRoomNotAvailable()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintRoomNotAvailableForm* constraintRoomNotAvailableForm=new ConstraintRoomNotAvailableForm();
	constraintRoomNotAvailableForm->exec();
}

void FetMainForm::dataSpaceConstraintsActivityPreferredRoom()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintActivityPreferredRoomForm* constraintActivityPreferredRoomForm=new ConstraintActivityPreferredRoomForm();
	constraintActivityPreferredRoomForm->exec();
}

void FetMainForm::dataSpaceConstraintsMinimizeNumberOfRoomsForStudents()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintMinimizeNumberOfRoomsForStudentsForm* constraintMinimizeNumberOfRoomsForStudentsForm=new ConstraintMinimizeNumberOfRoomsForStudentsForm();
	constraintMinimizeNumberOfRoomsForStudentsForm->exec();
}

void FetMainForm::dataSpaceConstraintsMinimizeNumberOfRoomsForTeachers()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintMinimizeNumberOfRoomsForTeachersForm* form=new ConstraintMinimizeNumberOfRoomsForTeachersForm();
	form->exec();
}

void FetMainForm::dataSpaceConstraintsRoomTypeNotAllowedSubjects()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintRoomTypeNotAllowedSubjectsForm* constraintRoomTypeNotAllowedSubjectsForm=new ConstraintRoomTypeNotAllowedSubjectsForm();
	constraintRoomTypeNotAllowedSubjectsForm->exec();
}

void FetMainForm::dataSpaceConstraintsSubjectRequiresEquipments()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintSubjectRequiresEquipmentsForm* constraintSubjectRequiresEquipmentsForm=new ConstraintSubjectRequiresEquipmentsForm();
	constraintSubjectRequiresEquipmentsForm->exec();
}

void FetMainForm::dataSpaceConstraintsSubjectSubjectTagRequireEquipments()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintSubjectSubjectTagRequireEquipmentsForm* constraintSubjectSubjectTagRequireEquipmentsForm=new ConstraintSubjectSubjectTagRequireEquipmentsForm();
	constraintSubjectSubjectTagRequireEquipmentsForm->exec();
}

void FetMainForm::dataSpaceConstraintsActivitiesSameRoom()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintActivitiesSameRoomForm* constraintActivitiesSameRoomForm=new ConstraintActivitiesSameRoomForm();
	constraintActivitiesSameRoomForm->exec();
}

void FetMainForm::dataSpaceConstraintsActivityPreferredRooms()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintActivityPreferredRoomsForm* constraintActivityPreferredRoomsForm=new ConstraintActivityPreferredRoomsForm();
	constraintActivityPreferredRoomsForm->exec();
}

void FetMainForm::dataSpaceConstraintsSubjectPreferredRoom()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintSubjectPreferredRoomForm* constraintSubjectPreferredRoomForm=new ConstraintSubjectPreferredRoomForm();
	constraintSubjectPreferredRoomForm->exec();
}

void FetMainForm::dataSpaceConstraintsSubjectSubjectTagPreferredRoom()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintSubjectSubjectTagPreferredRoomForm* constraintSubjectSubjectTagPreferredRoomForm=new ConstraintSubjectSubjectTagPreferredRoomForm();
	constraintSubjectSubjectTagPreferredRoomForm->exec();
}

void FetMainForm::dataSpaceConstraintsSubjectPreferredRooms()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintSubjectPreferredRoomsForm* constraintSubjectPreferredRoomsForm=new ConstraintSubjectPreferredRoomsForm();
	constraintSubjectPreferredRoomsForm->exec();
}

void FetMainForm::dataSpaceConstraintsSubjectSubjectTagPreferredRooms()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintSubjectSubjectTagPreferredRoomsForm* constraintSubjectSubjectTagPreferredRoomsForm=new ConstraintSubjectSubjectTagPreferredRoomsForm();
	constraintSubjectSubjectTagPreferredRoomsForm->exec();
}

void FetMainForm::dataSpaceConstraintsMaxBuildingChangesPerDayForTeachers()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintMaxBuildingChangesPerDayForTeachersForm* form=new ConstraintMaxBuildingChangesPerDayForTeachersForm();
	form->exec();
}

void FetMainForm::dataSpaceConstraintsMaxBuildingChangesPerDayForStudents()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintMaxBuildingChangesPerDayForStudentsForm* form=new ConstraintMaxBuildingChangesPerDayForStudentsForm();
	form->exec();
}

void FetMainForm::dataSpaceConstraintsMaxRoomChangesPerDayForTeachers()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintMaxRoomChangesPerDayForTeachersForm* form=new ConstraintMaxRoomChangesPerDayForTeachersForm();
	form->exec();
}

void FetMainForm::dataSpaceConstraintsMaxRoomChangesPerDayForStudents()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintMaxRoomChangesPerDayForStudentsForm* form=new ConstraintMaxRoomChangesPerDayForStudentsForm();
	form->exec();
}

void FetMainForm::dataTimeConstraintsStudentsSetNotAvailable()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintStudentsSetNotAvailableForm* constraintStudentsSetNotAvailableForm=new ConstraintStudentsSetNotAvailableForm();
	constraintStudentsSetNotAvailableForm->exec();
}

void FetMainForm::dataTimeConstraintsBreak()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintBreakForm* constraintBreakForm=new ConstraintBreakForm();
	constraintBreakForm->exec();
}

void FetMainForm::dataTimeConstraintsTeacherMaxDaysPerWeek()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintTeacherMaxDaysPerWeekForm* constraintTeacherMaxDaysPerWeekForm=new ConstraintTeacherMaxDaysPerWeekForm();
	constraintTeacherMaxDaysPerWeekForm->exec();
}

void FetMainForm::dataTimeConstraintsTeachersMaxHoursContinuously()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintTeachersMaxHoursContinuouslyForm* constraintTeachersMaxHoursContinuouslyForm=new ConstraintTeachersMaxHoursContinuouslyForm();
	constraintTeachersMaxHoursContinuouslyForm->exec();
}

void FetMainForm::dataTimeConstraintsTeachersMaxHoursDaily()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintTeachersMaxHoursDailyForm* constraintTeachersMaxHoursDailyForm=new ConstraintTeachersMaxHoursDailyForm();
	constraintTeachersMaxHoursDailyForm->exec();
}

void FetMainForm::dataTimeConstraintsTeachersSubgroupsMaxHoursDaily()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintTeachersSubgroupsMaxHoursDailyForm* constraintTeachersSubgroupsMaxHoursDailyForm=new ConstraintTeachersSubgroupsMaxHoursDailyForm();
	constraintTeachersSubgroupsMaxHoursDailyForm->exec();
}

void FetMainForm::dataTimeConstraintsActivityPreferredTime()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintActivityPreferredTimeForm* constraintActivityPreferredTimeForm=new ConstraintActivityPreferredTimeForm();
	constraintActivityPreferredTimeForm->exec();
}

void FetMainForm::dataTimeConstraintsStudentsSetNoGaps()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintStudentsSetNoGapsForm* constraintStudentsSetNoGapsForm=new ConstraintStudentsSetNoGapsForm();
	constraintStudentsSetNoGapsForm->exec();
}

void FetMainForm::dataTimeConstraintsStudentsNoGaps()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintStudentsNoGapsForm* constraintStudentsNoGapsForm=new ConstraintStudentsNoGapsForm();
	constraintStudentsNoGapsForm->exec();
}

void FetMainForm::dataTimeConstraintsTeachersNoGaps()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintTeachersNoGapsForm* constraintTeachersNoGapsForm=new ConstraintTeachersNoGapsForm();
	constraintTeachersNoGapsForm->exec();
}

void FetMainForm::dataTimeConstraintsStudentsEarly()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintStudentsEarlyForm* constraintStudentsEarlyForm=new ConstraintStudentsEarlyForm();
	constraintStudentsEarlyForm->exec();
}

void FetMainForm::dataTimeConstraintsStudentsSetIntervalMaxDaysPerWeek()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintStudentsSetIntervalMaxDaysPerWeekForm* constraintStudentsSetIntervalMaxDaysPerWeekForm=new ConstraintStudentsSetIntervalMaxDaysPerWeekForm();
	constraintStudentsSetIntervalMaxDaysPerWeekForm->exec();
}

void FetMainForm::dataTimeConstraintsStudentsSetNHoursDaily()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintStudentsSetNHoursDailyForm* constraintStudentsSetNHoursDailyForm=new ConstraintStudentsSetNHoursDailyForm();
	constraintStudentsSetNHoursDailyForm->exec();
}

void FetMainForm::dataTimeConstraintsStudentsNHoursDaily()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintStudentsNHoursDailyForm* constraintStudentsNHoursDailyForm=new ConstraintStudentsNHoursDailyForm();
	constraintStudentsNHoursDailyForm->exec();
}

void FetMainForm::dataTimeConstraintsActivityEndsDay()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintActivityEndsDayForm* constraintActivityEndsDayForm=new ConstraintActivityEndsDayForm();
	constraintActivityEndsDayForm->exec();
}

void FetMainForm::dataTimeConstraintsActivitiesNotOverlapping()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintActivitiesNotOverlappingForm* constraintActivitiesNotOverlappingForm=new ConstraintActivitiesNotOverlappingForm();
	constraintActivitiesNotOverlappingForm->exec();
}

void FetMainForm::dataTimeConstraintsMinNDaysBetweenActivities()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintMinNDaysBetweenActivitiesForm* constraintMinNDaysBetweenActivitiesForm=new ConstraintMinNDaysBetweenActivitiesForm();
	constraintMinNDaysBetweenActivitiesForm->exec();
}

void FetMainForm::dataTimeConstraintsActivityPreferredTimes()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintActivityPreferredTimesForm* constraintActivityPreferredTimesForm=new ConstraintActivityPreferredTimesForm();
	constraintActivityPreferredTimesForm->exec();
}

void FetMainForm::dataTimeConstraintsTeachersSubjectTagsMaxHoursContinuously()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintTeachersSubjectTagsMaxHoursContinuouslyForm* constraintTeachersSubjectTagsMaxHoursContinuouslyForm=new ConstraintTeachersSubjectTagsMaxHoursContinuouslyForm();
	constraintTeachersSubjectTagsMaxHoursContinuouslyForm->exec();
}

void FetMainForm::dataTimeConstraintsTeachersSubjectTagMaxHoursContinuously()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	ConstraintTeachersSubjectTagMaxHoursContinuouslyForm* constraintTeachersSubjectTagMaxHoursContinuouslyForm=new ConstraintTeachersSubjectTagMaxHoursContinuouslyForm();
	constraintTeachersSubjectTagMaxHoursContinuouslyForm->exec();
}

void FetMainForm::helpAbout()
{
	HelpAboutForm* helpAboutForm=new HelpAboutForm();
	helpAboutForm->show();
}

void FetMainForm::helpFAQ()
{
	HelpFaqForm* helpFaqForm=new HelpFaqForm();
	helpFaqForm->show();
}

void FetMainForm::helpTimeConstraints()
{
	HelpTimeConstraintsForm* helpTimeConstraintsForm=new HelpTimeConstraintsForm();
	helpTimeConstraintsForm->show();
}

void FetMainForm::helpTimeConstraintsSubtags()
{
	HelpTimeConstraintsSubtagsForm* helpTimeConstraintsSubtagsForm=new HelpTimeConstraintsSubtagsForm();
	helpTimeConstraintsSubtagsForm->show();
}

void FetMainForm::timetableAllocateHours()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	int count=0;
	for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next())
		if(act->active)
			count++;
	if(count<2){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please input at least two active activities before allocating hours"));
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

void FetMainForm::timetableViewStudentsWithRooms()
{
	if(!rooms_schedule_ready){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the rooms, firstly"));
		return;
	}

	TimetableViewStudentsWithRoomsForm *timetableViewStudentsWithRoomsForm=new TimetableViewStudentsWithRoomsForm();
	timetableViewStudentsWithRoomsForm->show();
}

void FetMainForm::timetableViewTeachersWithRooms()
{
	if(!rooms_schedule_ready){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the rooms, firstly"));
		return;
	}

	TimetableViewTeachersWithRoomsForm *timetableViewTeachersWithRoomsForm=new TimetableViewTeachersWithRoomsForm();
	timetableViewTeachersWithRoomsForm->show();
}

void FetMainForm::timetableViewStudentsWithRooms2()
{
	if(!rooms_schedule_ready2){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the hours and the rooms, firstly"));
		return;
	}

	TimetableViewStudentsWithRooms2Form *timetableViewStudentsWithRooms2Form=new TimetableViewStudentsWithRooms2Form();
	timetableViewStudentsWithRooms2Form->show();
}

void FetMainForm::timetableViewTeachersWithRooms2()
{
	if(!rooms_schedule_ready2){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the hours and the rooms, firstly"));
		return;
	}

	TimetableViewTeachersWithRooms2Form *timetableViewTeachersWithRooms2Form=new TimetableViewTeachersWithRooms2Form();
	timetableViewTeachersWithRooms2Form->show();
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
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	if(!gt.rules.internalStructureComputed){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the hours prior to allocating the rooms"));
		return;
	}
	if(! (students_schedule_ready && teachers_schedule_ready)){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the hours prior to allocating the rooms"));
		return;
	}
	if(gt.rules.roomsList.count()<=0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please input at least one room before allocating the rooms"));
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

void FetMainForm::timetableViewRooms2()
{
	if(!rooms_schedule_ready2){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the hours and the rooms, firstly"));
		return;
	}

	TimetableViewRooms2Form *timetableViewRooms2Form=new TimetableViewRooms2Form();
	timetableViewRooms2Form->show();
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

void FetMainForm::timetableShowConflictsTimeSpace()
{
	if(!rooms_schedule_ready2){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the hours and the rooms, firstly"));
		return;
	}

	TimetableShowConflictsTimeSpaceForm *timetableShowConflictsTimeSpaceForm=new TimetableShowConflictsTimeSpaceForm();
	timetableShowConflictsTimeSpaceForm->show();
}

void FetMainForm::timetableAllocateHoursRooms()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	int count=0;
	for(Activity* act=gt.rules.activitiesList.first(); act; act=gt.rules.activitiesList.next())
		if(act->active)
			count++;
	if(count<2 || gt.rules.roomsList.count()<=0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please input at least two active activities and a room before allocating hours and rooms"));
		return;
	}
	TimetableAllocateHoursRoomsForm *timetableAllocateHoursRoomsForm=new TimetableAllocateHoursRoomsForm();
	timetableAllocateHoursRoomsForm->show();
}

void FetMainForm::timetableExportXmlHtml()
{
	QMessageBox::information(this, QObject::tr("FET information"), 
	  QObject::tr("Results are exported to xml and html\n"
	  "automatically from the allocate menu.\n"
	  "Please enter the directory \"%1\"\n"
	  "and you will find the exported .xml and .html files.\n"
	  "You can translate the .xml files into .tex or .html\n"
	  "using the files \"fetxml2latex.py\" or \"students.xsl\", \n"
	  " \"teachers.xsl\" and \"rooms.xsl\" "
	  ).arg(OUTPUT_DIR));
}

void FetMainForm::timetableExportiCalTeachers()
{
	if(!(students_schedule_ready && teachers_schedule_ready)){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the hours, firstly"));
		return;
	}
	
	TimetableExportiCalTeachersForm* form=new TimetableExportiCalTeachersForm();
	form->exec();
}

void FetMainForm::timetableExportiCalStudents()
{
	if(!(students_schedule_ready && teachers_schedule_ready)){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the hours, firstly"));
		return;
	}
	
	TimetableExportiCalStudentsForm* form=new TimetableExportiCalStudentsForm();
	form->exec();
}

void FetMainForm::timetableExportiCalTeachersWithRooms1()
{
	if(!rooms_schedule_ready){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the rooms, firstly"));
		return;
	}
	
	TimetableExportiCalTeachersWithRooms1Form* form=new TimetableExportiCalTeachersWithRooms1Form();
	form->exec();
}

void FetMainForm::timetableExportiCalStudentsWithRooms1()
{
	if(!rooms_schedule_ready){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the rooms, firstly"));
		return;
	}

	TimetableExportiCalStudentsWithRooms1Form* form=new TimetableExportiCalStudentsWithRooms1Form();
	form->exec();
}

void FetMainForm::timetableExportiCalTeachersWithRooms2()
{
	if(!rooms_schedule_ready2){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the hours and the rooms, firstly"));
		return;
	}

	TimetableExportiCalTeachersWithRooms2Form* form=new TimetableExportiCalTeachersWithRooms2Form();
	form->exec();
}

void FetMainForm::timetableExportiCalStudentsWithRooms2()
{
	if(!rooms_schedule_ready2){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please allocate the hours and the rooms, firstly"));
		return;
	}

	TimetableExportiCalStudentsWithRooms2Form* form=new TimetableExportiCalStudentsWithRooms2Form();
	form->exec();
}

void FetMainForm::languageEnglish()
{
	if(ptranslator!=NULL){
		pqapplication->removeTranslator(ptranslator);
		delete ptranslator;
		ptranslator=NULL;
	}
	
	FET_LANGUAGE="en_GB";
	
	languageMenu->setItemChecked(languageMenu->idAt(0), true);
	languageMenu->setItemChecked(languageMenu->idAt(1), false);
	languageMenu->setItemChecked(languageMenu->idAt(2), false);
	languageMenu->setItemChecked(languageMenu->idAt(3), false);
	languageMenu->setItemChecked(languageMenu->idAt(4), false);
	languageMenu->setItemChecked(languageMenu->idAt(5), false);
	languageMenu->setItemChecked(languageMenu->idAt(6), false);
	languageMenu->setItemChecked(languageMenu->idAt(7), false);
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
	
	FET_LANGUAGE="ro";
	
	languageMenu->setItemChecked(languageMenu->idAt(0), false);
	languageMenu->setItemChecked(languageMenu->idAt(1), false);
	languageMenu->setItemChecked(languageMenu->idAt(2), false);
	languageMenu->setItemChecked(languageMenu->idAt(3), true);
	languageMenu->setItemChecked(languageMenu->idAt(4), false);
	languageMenu->setItemChecked(languageMenu->idAt(5), false);
	languageMenu->setItemChecked(languageMenu->idAt(6), false);
	languageMenu->setItemChecked(languageMenu->idAt(7), false);
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
	
	FET_LANGUAGE="fr";
	
	languageMenu->setItemChecked(languageMenu->idAt(0), false);
	languageMenu->setItemChecked(languageMenu->idAt(1), true);
	languageMenu->setItemChecked(languageMenu->idAt(2), false);
	languageMenu->setItemChecked(languageMenu->idAt(3), false);
	languageMenu->setItemChecked(languageMenu->idAt(4), false);
	languageMenu->setItemChecked(languageMenu->idAt(5), false);
	languageMenu->setItemChecked(languageMenu->idAt(6), false);
	languageMenu->setItemChecked(languageMenu->idAt(7), false);
}

void FetMainForm::languageCatalan()
{
	bool existing=true;
	if(ptranslator==NULL){
		existing=false;
		ptranslator=new QTranslator(0);
	}

	QDir d("/usr/share/fet/translations");
	if(d.exists())
		ptranslator->load("fet_ca", "/usr/share/fet/translations");
	else
		ptranslator->load("fet_ca", "translations");

	if(!existing)
		pqapplication->installTranslator(ptranslator);
	
	FET_LANGUAGE="ca";
	
	languageMenu->setItemChecked(languageMenu->idAt(0), false);
	languageMenu->setItemChecked(languageMenu->idAt(1), false);
	languageMenu->setItemChecked(languageMenu->idAt(2), true);
	languageMenu->setItemChecked(languageMenu->idAt(3), false);
	languageMenu->setItemChecked(languageMenu->idAt(4), false);
	languageMenu->setItemChecked(languageMenu->idAt(5), false);
	languageMenu->setItemChecked(languageMenu->idAt(6), false);
	languageMenu->setItemChecked(languageMenu->idAt(7), false);
}

void FetMainForm::languageMalay()
{
	bool existing=true;
	if(ptranslator==NULL){
		existing=false;
		ptranslator=new QTranslator(0);
	}

	QDir d("/usr/share/fet/translations");
	if(d.exists())
		ptranslator->load("fet_ms", "/usr/share/fet/translations");
	else
		ptranslator->load("fet_ms", "translations");

	if(!existing)
		pqapplication->installTranslator(ptranslator);
	
	FET_LANGUAGE="ms";
	
	languageMenu->setItemChecked(languageMenu->idAt(0), false);
	languageMenu->setItemChecked(languageMenu->idAt(1), false);
	languageMenu->setItemChecked(languageMenu->idAt(2), false);
	languageMenu->setItemChecked(languageMenu->idAt(3), false);
	languageMenu->setItemChecked(languageMenu->idAt(4), true);
	languageMenu->setItemChecked(languageMenu->idAt(5), false);
	languageMenu->setItemChecked(languageMenu->idAt(6), false);
	languageMenu->setItemChecked(languageMenu->idAt(7), false);
}

void FetMainForm::languagePolish()
{
	bool existing=true;
	if(ptranslator==NULL){
		existing=false;
		ptranslator=new QTranslator(0);
	}

	QDir d("/usr/share/fet/translations");
	if(d.exists())
		ptranslator->load("fet_pl", "/usr/share/fet/translations");
	else
		ptranslator->load("fet_pl", "translations");

	if(!existing)
		pqapplication->installTranslator(ptranslator);
	
	FET_LANGUAGE="pl";
	
	languageMenu->setItemChecked(languageMenu->idAt(0), false);
	languageMenu->setItemChecked(languageMenu->idAt(1), false);
	languageMenu->setItemChecked(languageMenu->idAt(2), false);
	languageMenu->setItemChecked(languageMenu->idAt(3), false);
	languageMenu->setItemChecked(languageMenu->idAt(4), false);
	languageMenu->setItemChecked(languageMenu->idAt(5), true);
	languageMenu->setItemChecked(languageMenu->idAt(6), false);
	languageMenu->setItemChecked(languageMenu->idAt(7), false);
}

void FetMainForm::languageTurkish()
{
	bool existing=true;
	if(ptranslator==NULL){
		existing=false;
		ptranslator=new QTranslator(0);
	}

	QDir d("/usr/share/fet/translations");
	if(d.exists())
		ptranslator->load("fet_tr", "/usr/share/fet/translations");
	else
		ptranslator->load("fet_tr", "translations");

	if(!existing)
		pqapplication->installTranslator(ptranslator);
	
	FET_LANGUAGE="tr";
	
	languageMenu->setItemChecked(languageMenu->idAt(0), false);
	languageMenu->setItemChecked(languageMenu->idAt(1), false);
	languageMenu->setItemChecked(languageMenu->idAt(2), false);
	languageMenu->setItemChecked(languageMenu->idAt(3), false);
	languageMenu->setItemChecked(languageMenu->idAt(4), false);
	languageMenu->setItemChecked(languageMenu->idAt(5), false);
	languageMenu->setItemChecked(languageMenu->idAt(6), true);
	languageMenu->setItemChecked(languageMenu->idAt(7), false);
}

void FetMainForm::languageDutch()
{
	bool existing=true;
	if(ptranslator==NULL){
		existing=false;
		ptranslator=new QTranslator(0);
	}

	QDir d("/usr/share/fet/translations");
	if(d.exists())
		ptranslator->load("fet_nl", "/usr/share/fet/translations");
	else
		ptranslator->load("fet_nl", "translations");

	if(!existing)
		pqapplication->installTranslator(ptranslator);
	
	FET_LANGUAGE="nl";
	
	languageMenu->setItemChecked(languageMenu->idAt(0), false);
	languageMenu->setItemChecked(languageMenu->idAt(1), false);
	languageMenu->setItemChecked(languageMenu->idAt(2), false);
	languageMenu->setItemChecked(languageMenu->idAt(3), false);
	languageMenu->setItemChecked(languageMenu->idAt(4), false);
	languageMenu->setItemChecked(languageMenu->idAt(5), false);
	languageMenu->setItemChecked(languageMenu->idAt(6), false);
	languageMenu->setItemChecked(languageMenu->idAt(7), true);
}

void FetMainForm::parametersPopulationNumber()
{
	if(simulation_running){
		QMessageBox::information(this, QObject::tr("FET information"),
			QObject::tr("Allocation in course.\nPlease stop simulation before this."));
		return;
	}

	PopulationNumberForm* populationNumberForm=new PopulationNumberForm();
	populationNumberForm->exec();
}
