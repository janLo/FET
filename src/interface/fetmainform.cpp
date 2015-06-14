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
#include "roomsform.h"
#include "alltimeconstraintsform.h"
#include "allspaceconstraintsform.h"
#include "helpaboutform.h"
#include "helpfaqform.h"
#include "populationnumberform.h"
#include "fet.h"
#include "constraint2activitiesconsecutiveform.h"
#include "constraint2activitiesgroupedform.h"
#include "constraintactivitiespreferredtimesform.h"
#include "constraintactivitiessamestartingtimeform.h"
#include "constraintactivitiessamestartinghourform.h"
#include "constraintteachernotavailableform.h"
#include "constraintbasiccompulsorytimeform.h"
#include "constraintbasiccompulsoryspaceform.h"
#include "constraintteacherrequiresroomform.h"
#include "constraintroomnotavailableform.h"
#include "constraintactivitypreferredroomform.h"
#include "constraintminimizenumberofroomsforstudentsform.h"
#include "constraintroomtypenotallowedsubjectsform.h"
#include "constraintsubjectrequiresequipmentsform.h"
#include "constraintsubjectsubjecttagrequireequipmentsform.h"
#include "constraintstudentssetnotavailableform.h"
#include "constraintbreakform.h"
#include "constraintteachermaxdaysperweekform.h"
#include "constraintteachersnomorethanxhourscontinuouslyform.h"
#include "constraintteachersnomorethanxhoursdailyform.h"
#include "constraintteacherssubgroupsnomorethanxhoursdailyform.h"
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
#include "constraintactivitiessameroomform.h"
#include "constraintactivitypreferredroomsform.h"

#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qstring.h>
#include <qdir.h>

bool students_schedule_ready;
bool teachers_schedule_ready;
bool rooms_schedule_ready;
bool students_schedule_ready2;
bool teachers_schedule_ready2;
bool rooms_schedule_ready2;

FetMainForm *fetMainForm_pointer;

QString timeConflictsString; //the string that contains a log of the broken time constraints
QString spaceConflictsString; //the string that contains a log of the broken space constraints
QString timeSpaceConflictsString; //the string that contains a log of the broken time&space constraints

QTranslator* ptranslator;

extern QApplication* pqapplication;

FetMainForm::FetMainForm() : FetMainForm_template()
{
	ptranslator=NULL;

	fetMainForm_pointer=this;
	
	if(FET_LANGUAGE=="FR")
		this->languageFrench();
	else if(FET_LANGUAGE=="RO")
		this->languageRomanian();
	else if(FET_LANGUAGE=="CA")
		this->languageCatalan();

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

		students_schedule_ready=false;
		teachers_schedule_ready=false;
		students_schedule_ready2=false;
		teachers_schedule_ready2=false;
		rooms_schedule_ready2=false;
	}
}

void FetMainForm::fileOpen()
{
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
		WORKING_DIRECTORY, QObject::tr("FET xml files (*.fet);;All files (*)"),
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
	InstitutionNameForm* institutionNameForm=new InstitutionNameForm();
	institutionNameForm->exec();
}

void FetMainForm::dataComments()
{
	CommentsForm* commentsForm=new CommentsForm();
	commentsForm->exec();
}

void FetMainForm::dataDays()
{
	DaysForm* daysForm=new DaysForm();
	daysForm->exec();
}

void FetMainForm::dataHours()
{
	HoursForm* hoursForm=new HoursForm();
	hoursForm->exec();
}

void FetMainForm::dataTeachers()
{
	TeachersForm* teachersForm=new TeachersForm();
	teachersForm->exec();
}

void FetMainForm::dataSubjects()
{
	SubjectsForm* subjectsForm=new SubjectsForm();
	subjectsForm->exec();
}

void FetMainForm::dataSubjectTags()
{
	SubjectTagsForm* subjectTagsForm=new SubjectTagsForm();
	subjectTagsForm->exec();
}

void FetMainForm::dataYears()
{
	YearsForm* yearsForm=new YearsForm();
	yearsForm->exec();
}

void FetMainForm::dataGroups()
{
	GroupsForm* groupsForm=new GroupsForm();
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

void FetMainForm::dataAllTimeConstraints()
{
	AllTimeConstraintsForm* allTimeConstraintsForm=new AllTimeConstraintsForm();
	allTimeConstraintsForm->exec();
}

void FetMainForm::dataAllSpaceConstraints()
{
	AllSpaceConstraintsForm* allSpaceConstraintsForm=new AllSpaceConstraintsForm();
	allSpaceConstraintsForm->exec();
}

void FetMainForm::dataTimeConstraints2ActivitiesConsecutive()
{
	Constraint2ActivitiesConsecutiveForm* constraint2ActivitiesConsecutiveForm=new Constraint2ActivitiesConsecutiveForm();
	constraint2ActivitiesConsecutiveForm->exec();
}

void FetMainForm::dataTimeConstraints2ActivitiesGrouped()
{
	Constraint2ActivitiesGroupedForm* constraint2ActivitiesGroupedForm=new Constraint2ActivitiesGroupedForm();
	constraint2ActivitiesGroupedForm->exec();
}

void FetMainForm::dataTimeConstraintsActivitiesPreferredTimes()
{
	ConstraintActivitiesPreferredTimesForm* constraintActivitiesPreferredTimesForm=new ConstraintActivitiesPreferredTimesForm();
	constraintActivitiesPreferredTimesForm->exec();
}

void FetMainForm::dataTimeConstraintsActivitiesSameStartingTime()
{
	ConstraintActivitiesSameStartingTimeForm* constraintActivitiesSameStartingTimeForm=new ConstraintActivitiesSameStartingTimeForm();
	constraintActivitiesSameStartingTimeForm->exec();
}

void FetMainForm::dataTimeConstraintsActivitiesSameStartingHour()
{
	ConstraintActivitiesSameStartingHourForm* constraintActivitiesSameStartingHourForm=new ConstraintActivitiesSameStartingHourForm();
	constraintActivitiesSameStartingHourForm->exec();
}

void FetMainForm::dataTimeConstraintsTeacherNotAvailable()
{
	ConstraintTeacherNotAvailableForm* constraintTeacherNotAvailableForm=new ConstraintTeacherNotAvailableForm();
	constraintTeacherNotAvailableForm->exec();
}

void FetMainForm::dataTimeConstraintsBasicCompulsoryTime()
{
	ConstraintBasicCompulsoryTimeForm* constraintBasicCompulsoryTimeForm=new ConstraintBasicCompulsoryTimeForm();
	constraintBasicCompulsoryTimeForm->exec();
}

void FetMainForm::dataSpaceConstraintsBasicCompulsorySpace()
{
	ConstraintBasicCompulsorySpaceForm* constraintBasicCompulsorySpaceForm=new ConstraintBasicCompulsorySpaceForm();
	constraintBasicCompulsorySpaceForm->exec();
}

void FetMainForm::dataSpaceConstraintsTeacherRequiresRoom()
{
	ConstraintTeacherRequiresRoomForm* constraintTeacherRequiresRoomForm=new ConstraintTeacherRequiresRoomForm();
	constraintTeacherRequiresRoomForm->exec();
}

void FetMainForm::dataSpaceConstraintsRoomNotAvailable()
{
	ConstraintRoomNotAvailableForm* constraintRoomNotAvailableForm=new ConstraintRoomNotAvailableForm();
	constraintRoomNotAvailableForm->exec();
}

void FetMainForm::dataSpaceConstraintsActivityPreferredRoom()
{
	ConstraintActivityPreferredRoomForm* constraintActivityPreferredRoomForm=new ConstraintActivityPreferredRoomForm();
	constraintActivityPreferredRoomForm->exec();
}

void FetMainForm::dataSpaceConstraintsMinimizeNumberOfRoomsForStudents()
{
	ConstraintMinimizeNumberOfRoomsForStudentsForm* constraintMinimizeNumberOfRoomsForStudentsForm=new ConstraintMinimizeNumberOfRoomsForStudentsForm();
	constraintMinimizeNumberOfRoomsForStudentsForm->exec();
}

void FetMainForm::dataSpaceConstraintsRoomTypeNotAllowedSubjects()
{
	ConstraintRoomTypeNotAllowedSubjectsForm* constraintRoomTypeNotAllowedSubjectsForm=new ConstraintRoomTypeNotAllowedSubjectsForm();
	constraintRoomTypeNotAllowedSubjectsForm->exec();
}

void FetMainForm::dataSpaceConstraintsSubjectRequiresEquipments()
{
	ConstraintSubjectRequiresEquipmentsForm* constraintSubjectRequiresEquipmentsForm=new ConstraintSubjectRequiresEquipmentsForm();
	constraintSubjectRequiresEquipmentsForm->exec();
}

void FetMainForm::dataSpaceConstraintsSubjectSubjectTagRequireEquipments()
{
	ConstraintSubjectSubjectTagRequireEquipmentsForm* constraintSubjectSubjectTagRequireEquipmentsForm=new ConstraintSubjectSubjectTagRequireEquipmentsForm();
	constraintSubjectSubjectTagRequireEquipmentsForm->exec();
}

void FetMainForm::dataSpaceConstraintsActivitiesSameRoom()
{
	ConstraintActivitiesSameRoomForm* constraintActivitiesSameRoomForm=new ConstraintActivitiesSameRoomForm();
	constraintActivitiesSameRoomForm->exec();
}

void FetMainForm::dataSpaceConstraintsActivityPreferredRooms()
{
	ConstraintActivityPreferredRoomsForm* constraintActivityPreferredRoomsForm=new ConstraintActivityPreferredRoomsForm();
	constraintActivityPreferredRoomsForm->exec();
}

void FetMainForm::dataTimeConstraintsStudentsSetNotAvailable()
{
	ConstraintStudentsSetNotAvailableForm* constraintStudentsSetNotAvailableForm=new ConstraintStudentsSetNotAvailableForm();
	constraintStudentsSetNotAvailableForm->exec();
}

void FetMainForm::dataTimeConstraintsBreak()
{
	ConstraintBreakForm* constraintBreakForm=new ConstraintBreakForm();
	constraintBreakForm->exec();
}

void FetMainForm::dataTimeConstraintsTeacherMaxDaysPerWeek()
{
	ConstraintTeacherMaxDaysPerWeekForm* constraintTeacherMaxDaysPerWeekForm=new ConstraintTeacherMaxDaysPerWeekForm();
	constraintTeacherMaxDaysPerWeekForm->exec();
}

void FetMainForm::dataTimeConstraintsTeachersNoMoreThanXHoursContinuously()
{
	ConstraintTeachersNoMoreThanXHoursContinuouslyForm* constraintTeachersNoMoreThanXHoursContinuouslyForm=new ConstraintTeachersNoMoreThanXHoursContinuouslyForm();
	constraintTeachersNoMoreThanXHoursContinuouslyForm->exec();
}

void FetMainForm::dataTimeConstraintsTeachersNoMoreThanXHoursDaily()
{
	ConstraintTeachersNoMoreThanXHoursDailyForm* constraintTeachersNoMoreThanXHoursDailyForm=new ConstraintTeachersNoMoreThanXHoursDailyForm();
	constraintTeachersNoMoreThanXHoursDailyForm->exec();
}

void FetMainForm::dataTimeConstraintsTeachersSubgroupsNoMoreThanXHoursDaily()
{
	ConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm* constraintTeachersSubgroupsNoMoreThanXHoursDailyForm=new ConstraintTeachersSubgroupsNoMoreThanXHoursDailyForm();
	constraintTeachersSubgroupsNoMoreThanXHoursDailyForm->exec();
}

void FetMainForm::dataTimeConstraintsActivityPreferredTime()
{
	ConstraintActivityPreferredTimeForm* constraintActivityPreferredTimeForm=new ConstraintActivityPreferredTimeForm();
	constraintActivityPreferredTimeForm->exec();
}

void FetMainForm::dataTimeConstraintsStudentsSetNoGaps()
{
	ConstraintStudentsSetNoGapsForm* constraintStudentsSetNoGapsForm=new ConstraintStudentsSetNoGapsForm();
	constraintStudentsSetNoGapsForm->exec();
}

void FetMainForm::dataTimeConstraintsStudentsNoGaps()
{
	ConstraintStudentsNoGapsForm* constraintStudentsNoGapsForm=new ConstraintStudentsNoGapsForm();
	constraintStudentsNoGapsForm->exec();
}

void FetMainForm::dataTimeConstraintsTeachersNoGaps()
{
	ConstraintTeachersNoGapsForm* constraintTeachersNoGapsForm=new ConstraintTeachersNoGapsForm();
	constraintTeachersNoGapsForm->exec();
}

void FetMainForm::dataTimeConstraintsStudentsEarly()
{
	ConstraintStudentsEarlyForm* constraintStudentsEarlyForm=new ConstraintStudentsEarlyForm();
	constraintStudentsEarlyForm->exec();
}

void FetMainForm::dataTimeConstraintsStudentsSetIntervalMaxDaysPerWeek()
{
	ConstraintStudentsSetIntervalMaxDaysPerWeekForm* constraintStudentsSetIntervalMaxDaysPerWeekForm=new ConstraintStudentsSetIntervalMaxDaysPerWeekForm();
	constraintStudentsSetIntervalMaxDaysPerWeekForm->exec();
}

void FetMainForm::dataTimeConstraintsStudentsSetNHoursDaily()
{
	ConstraintStudentsSetNHoursDailyForm* constraintStudentsSetNHoursDailyForm=new ConstraintStudentsSetNHoursDailyForm();
	constraintStudentsSetNHoursDailyForm->exec();
}

void FetMainForm::dataTimeConstraintsStudentsNHoursDaily()
{
	ConstraintStudentsNHoursDailyForm* constraintStudentsNHoursDailyForm=new ConstraintStudentsNHoursDailyForm();
	constraintStudentsNHoursDailyForm->exec();
}

void FetMainForm::dataTimeConstraintsActivityEndsDay()
{
	ConstraintActivityEndsDayForm* constraintActivityEndsDayForm=new ConstraintActivityEndsDayForm();
	constraintActivityEndsDayForm->exec();
}

void FetMainForm::dataTimeConstraintsActivitiesNotOverlapping()
{
	ConstraintActivitiesNotOverlappingForm* constraintActivitiesNotOverlappingForm=new ConstraintActivitiesNotOverlappingForm();
	constraintActivitiesNotOverlappingForm->exec();
}

void FetMainForm::dataTimeConstraintsMinNDaysBetweenActivities()
{
	ConstraintMinNDaysBetweenActivitiesForm* constraintMinNDaysBetweenActivitiesForm=new ConstraintMinNDaysBetweenActivitiesForm();
	constraintMinNDaysBetweenActivitiesForm->exec();
}

void FetMainForm::dataTimeConstraintsActivityPreferredTimes()
{
	ConstraintActivityPreferredTimesForm* constraintActivityPreferredTimesForm=new ConstraintActivityPreferredTimesForm();
	constraintActivityPreferredTimesForm->exec();
}

void FetMainForm::helpAbout()
{
	HelpAboutForm *helpAboutForm=new HelpAboutForm();
	helpAboutForm->show();
}

void FetMainForm::helpFAQ()
{
	HelpFaqForm *helpFaqForm=new HelpFaqForm();
	helpFaqForm->show();
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
	if(gt.rules.activitiesList.count()<2 || gt.rules.roomsList.count()<=0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Please input at least two activities and a room before allocating hours and rooms"));
		return;
	}
	TimetableAllocateHoursRoomsForm *timetableAllocateHoursRoomsForm=new TimetableAllocateHoursRoomsForm();
	timetableAllocateHoursRoomsForm->show();
}

void FetMainForm::timetableExport()
{
	QMessageBox::information(this, QObject::tr("FET information"), 
	  QObject::tr("Please enter the directory \"%1\"\n"
	  "and you will find the exported .xml and .html files.\n"
	  "You can translate the .xml files into .tex or .html\n"
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
	
	FET_LANGUAGE="EN";
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
	
	FET_LANGUAGE="RO";
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
	
	FET_LANGUAGE="FR";
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
	
	FET_LANGUAGE="CA";
}

void FetMainForm::parametersPopulationNumber()
{
	PopulationNumberForm* populationNumberForm=new PopulationNumberForm();
	populationNumberForm->exec();
}
