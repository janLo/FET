/*
File rules.cpp
*/

/*
Copyright 2002, 2003 Lalescu Liviu.

This file is part of FET.

FET is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

FET is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FET; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "genetictimetable_defs.h"
#include "rules.h"

#include <string.h>

#include <iostream>
using namespace std;

#include <qdom.h>
#include <qstring.h>
#include <qtranslator.h>
#include <qmessagebox.h>

void Rules::init() //initializes the rules (empty, but with default hours and days)
{
	this->subjectsList.setAutoDelete(true);
	this->subjectTagsList.setAutoDelete(true);
	this->yearsList.setAutoDelete(true);
	this->teachersList.setAutoDelete(true);
	this->activitiesList.setAutoDelete(true);
	this->timeConstraintsList.setAutoDelete(true);
	this->spaceConstraintsList.setAutoDelete(true);
	this->equipmentsList.setAutoDelete(true);
	this->roomsList.setAutoDelete(true);

	//defaults
	this->nHoursPerDay=12;
	this->hoursOfTheDay[0]="0";
	this->hoursOfTheDay[1]="1";
	this->hoursOfTheDay[2]="2";
	this->hoursOfTheDay[3]="3";
	this->hoursOfTheDay[4]="4";
	this->hoursOfTheDay[5]="5";
	this->hoursOfTheDay[6]="6";
	this->hoursOfTheDay[7]="7";
	this->hoursOfTheDay[8]="8";
	this->hoursOfTheDay[9]="9";
	this->hoursOfTheDay[10]="10";
	this->hoursOfTheDay[11]="11";
	this->hoursOfTheDay[12]="12";

	this->nDaysPerWeek=5;
	this->daysOfTheWeek[0] = QObject::tr("Monday");
	this->daysOfTheWeek[1] = QObject::tr("Tuesday");
	this->daysOfTheWeek[2] = QObject::tr("Wednesday");
	this->daysOfTheWeek[3] = QObject::tr("Thursday");
	this->daysOfTheWeek[4] = QObject::tr("Friday");
	
	this->institutionName=QObject::tr("Default institution");
	this->comments=QObject::tr("Default comments");

	this->initialized=true;
}

bool Rules::computeInternalStructure()
{
	//The order is important - firstly the teachers, subjects and students.
	//After that, the equipments.
	//After that, the rooms.
	//After that, the activities.
	//After that, the time constraints.
	//After that, initialization of the matrix "roomHasEquipment".
	//After that, the space constraints.

	int i;

	assert(this->initialized);

	//days and hours
	assert(this->nHoursPerDay>0);
	assert(this->nDaysPerWeek>0);
	this->nHoursPerWeek=this->nHoursPerDay*this->nDaysPerWeek;

	//teachers
	Teacher* tch;
	this->nInternalTeachers=this->teachersList.count();
	assert(this->nInternalTeachers<=MAX_TEACHERS);
	for(tch=this->teachersList.first(), i=0; tch; tch=this->teachersList.next(), i++)
		this->internalTeachersList[i]=tch;
	assert(i==this->nInternalTeachers);

	//subjects
	Subject* sbj;
	this->nInternalSubjects=this->subjectsList.count();
	assert(this->nInternalSubjects<=MAX_SUBJECTS);	
	for(sbj=this->subjectsList.first(), i=0; sbj; sbj=this->subjectsList.next(), i++)
		this->internalSubjectsList[i]=sbj;
	assert(i==this->nInternalSubjects);

	//students
	this->nInternalSubgroups=0;
	assert(this->yearsList.count()<=(uint)(MAX_YEARS));
	for(StudentsYear* sty=this->yearsList.first(); sty; sty=this->yearsList.next()){
		//if this year has no groups, insert something to simulate the whole year
		if(sty->groupsList.count()==0){
			StudentsGroup* tmpGroup = new StudentsGroup();
			tmpGroup->name = sty->name+" WHOLE YEAR";
			tmpGroup->numberOfStudents = sty->numberOfStudents;
			sty->groupsList.append(tmpGroup);
		}
		assert(sty->groupsList.count()<=(uint)(MAX_GROUPS_PER_YEAR));
		for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
			//if this group has no subgroups, insert something to simulate the whole group
			if(stg->subgroupsList.count()==0){
				StudentsSubgroup* tmpSubgroup = new StudentsSubgroup();
				tmpSubgroup->name = stg->name+" WHOLE GROUP";
				tmpSubgroup->numberOfStudents=stg->numberOfStudents;
				stg->subgroupsList.append(tmpSubgroup);
			}
			assert(stg->subgroupsList.count()<=(uint)(MAX_SUBGROUPS_PER_GROUP));
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next()){
				bool existing=false;
				for(int i=0; i<this->nInternalSubgroups; i++)
					if(this->internalSubgroupsList[i]->name==sts->name){
						existing=true;
						break;
					}
				if(!existing){
					assert(this->nInternalSubgroups<MAX_TOTAL_SUBGROUPS);
					this->internalSubgroupsList[this->nInternalSubgroups++]=sts;
				}
			}
		}
	}

	//equipments internal list
	this->nInternalEquipments=0;
	assert(this->equipmentsList.count()<=(uint)(MAX_EQUIPMENTS));
	for(Equipment* eq=this->equipmentsList.first(); eq; eq=this->equipmentsList.next())
		this->internalEquipmentsList[this->nInternalEquipments++]=eq;
	assert((uint)(this->nInternalEquipments)==this->equipmentsList.count());

	//rooms internal list
	this->nInternalRooms=0;
	assert(this->roomsList.count()<=(uint)(MAX_ROOMS));
	for(Room* rm=this->roomsList.first(); rm; rm=this->roomsList.next())
		this->internalRoomsList[this->nInternalRooms++]=rm;
	assert((uint)(this->nInternalRooms)==this->roomsList.count());

	//activities
	Activity* act;
	assert(this->activitiesList.count()<=(uint)(MAX_ACTIVITIES));
	for(act=this->activitiesList.first(); act; act=this->activitiesList.next())
		act->computeInternalStructure(*this);

	this->nInternalActivities=this->activitiesList.count();
	assert(this->nInternalActivities<=MAX_ACTIVITIES);
	for(act=this->activitiesList.first(), i=0; act; act=this->activitiesList.next(), i++)
		this->internalActivitiesList[i]=*act;

	//time constraints
	assert(this->timeConstraintsList.count()<=(uint)(MAX_TIME_CONSTRAINTS));
	TimeConstraint* tctr;
	for(tctr=this->timeConstraintsList.first(); tctr; tctr=this->timeConstraintsList.next())
		tctr->computeInternalStructure(*this);

	for(int i=0; i<this->nInternalActivities; i++){
		this->fixedDay[i]=-1;
		this->fixedHour[i]=-1;
		this->sameDay[i]=-1;
		this->sameHour[i]=-1;
	}
	//getting rid of compulsory preferred time-s, same starting hour & same starting time-s - 
	//these will be used separately to repair the chromosomes (it was practically
	//found that this is the best method).	
	for(tctr=this->timeConstraintsList.first(), i=0; tctr; tctr=this->timeConstraintsList.next()){
		if(tctr->type==CONSTRAINT_ACTIVITY_PREFERRED_TIME && tctr->compulsory==true){
			ConstraintActivityPreferredTime* c = (ConstraintActivityPreferredTime*)tctr;
			this->fixedDay[c->activityIndex] = c->day;
			this->fixedHour[c->activityIndex] = c->hour;
		}
		else if(tctr->type==CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME && tctr->compulsory==true){
			ConstraintActivitiesSameStartingTime* c=(ConstraintActivitiesSameStartingTime*)tctr;
			int ai1=c->_activities[0];
			for(int i=1; i<c->n_activities; i++){
				int ai2=c->_activities[i];
				this->sameDay[ai2]=ai1;
				this->sameHour[ai2]=ai1;
			}
		}
		else if(tctr->type==CONSTRAINT_ACTIVITIES_SAME_STARTING_HOUR && tctr->compulsory==true){
			ConstraintActivitiesSameStartingHour* c=(ConstraintActivitiesSameStartingHour*)tctr;
			int ai1=c->_activities[0];
			for(int i=1; i<c->n_activities; i++){
				int ai2=c->_activities[i];
				this->sameHour[ai2]=ai1;
			}
		}
		else{
			this->internalTimeConstraintsList[i++]=tctr;
		}
	}

	//all activities will depend on only one "head"
	for(int i=0; i<this->nInternalActivities; i++){
		for(int j=0; j<this->nInternalActivities; j++){
			int k=this->sameDay[j];
			if(k>=0 && this->sameDay[k]>=0)
				this->sameDay[j]=this->sameDay[k];
			k=this->sameHour[j];
			if(k>=0 && this->sameHour[k]>=0)
				this->sameHour[j]=this->sameHour[k];
		}
	}
	//make the head have the fixed day & hour
	for(int i=0; i<this->nInternalActivities; i++){
		if(this->fixedDay[i]>=0 && this->sameDay[i]>=0){
			bool t=this->fixedDay[this->sameDay[i]]==this->fixedDay[i] || this->fixedDay[this->sameDay[i]]==-1;
			if(!t){
				QMessageBox::warning(NULL, QObject::tr("FET information"), 
					 QObject::tr("Activity with id=%1 has fixed day on %2 and must be scheduled on the same\n"
					 " day with act. with id=%3, which is impossible. Please check your constraints").arg(this->internalActivitiesList[i].id)
					 .arg(this->fixedDay[i]).arg(this->internalActivitiesList[this->sameDay[i]].id));
				this->internalStructureComputed=false;
				return false;
			}

			assert(this->fixedDay[this->sameDay[i]]==this->fixedDay[i] || this->fixedDay[this->sameDay[i]]==-1);
			this->fixedDay[this->sameDay[i]]=this->fixedDay[i];
		}
		if(this->fixedHour[i]>=0 && this->sameHour[i]>=0){
			bool t=this->fixedHour[this->sameHour[i]]==this->fixedHour[i] || this->fixedHour[this->sameHour[i]]==-1;
			if(!t){
				QMessageBox::warning(NULL, QObject::tr("FET information"), 
					 QObject::tr("Activity with id=%1 has fixed hour on %2 and must be scheduled on the same\n"
					 " hour with act. with id=%3, which is impossible. Please check your constraints").arg(this->internalActivitiesList[i].id)
					 .arg(this->fixedHour[i]).arg(this->internalActivitiesList[this->sameHour[i]].id));
				this->internalStructureComputed=false;
				return false;
			}

			assert(this->fixedHour[this->sameHour[i]]==this->fixedHour[i] || this->fixedHour[this->sameHour[i]]==-1);
			this->fixedHour[this->sameHour[i]]=this->fixedHour[i];
		}
	}
	//make all have the fixed day & hour
	for(int i=0; i<this->nInternalActivities; i++){
		if(this->sameDay[i]>=0){
			assert(this->fixedDay[i]==this->fixedDay[this->sameDay[i]] || this->fixedDay[i]==-1);
			this->fixedDay[i]=this->fixedDay[this->sameDay[i]];
		}
		if(this->sameHour[i]>=0){
			assert(this->fixedHour[i]==this->fixedHour[this->sameHour[i]] || this->fixedHour[i]==-1);
			this->fixedHour[i]=this->fixedHour[this->sameHour[i]];
		}
	}
	
	this->nInternalTimeConstraints=i;
	assert(this->nInternalTimeConstraints<=MAX_TIME_CONSTRAINTS);
	
	//roomHasEquipment
	for(int rm=0; rm<this->nInternalRooms; rm++){
		for(int eq=0; eq<this->nInternalEquipments; eq++){
			this->roomHasEquipment[rm][eq] = 
			 (this->internalRoomsList[rm])->searchEquipment(this->internalEquipmentsList[eq]->name);
		}
	}

	//space constraints
	SpaceConstraint* sctr;
	assert(this->spaceConstraintsList.count()<=(uint)(MAX_SPACE_CONSTRAINTS));

	for(sctr=this->spaceConstraintsList.first(); sctr; sctr=this->spaceConstraintsList.next())
		sctr->computeInternalStructure(*this);
		
	for(int i=0; i<this->nInternalActivities; i++){
		this->fixedRoom[i]=-1;
		this->sameRoom[i]=-1;
	}

	for(sctr=this->spaceConstraintsList.first(), i=0; sctr; sctr=this->spaceConstraintsList.next()){
		if(sctr->type==CONSTRAINT_TEACHER_REQUIRES_ROOM && sctr->compulsory==true){
			//works by repairing
			ConstraintTeacherRequiresRoom* ctrr=(ConstraintTeacherRequiresRoom*) sctr;
			for(int i=0; i<ctrr->_nActivities; i++)
				this->fixedRoom[ctrr->_activities[i]]=ctrr->_room;
		}
		else if(sctr->type==CONSTRAINT_ACTIVITY_PREFERRED_ROOM && sctr->compulsory==true){
			//works by repairing
			ConstraintActivityPreferredRoom* capr=(ConstraintActivityPreferredRoom*) sctr;
			this->fixedRoom[capr->_activity]=capr->_room;
		}
		else if(sctr->type==CONSTRAINT_ACTIVITIES_SAME_ROOM && sctr->compulsory==true){
			ConstraintActivitiesSameRoom* c=(ConstraintActivitiesSameRoom*)sctr;
			int ai1=c->_activities[0];
			for(int i=1; i<c->n_activities; i++){
				int ai2=c->_activities[i];
				this->sameRoom[ai2]=ai1;
			}
		}
		else
			this->internalSpaceConstraintsList[i++]=sctr;
	}
	this->nInternalSpaceConstraints=i;
	assert(this->nInternalSpaceConstraints<=MAX_SPACE_CONSTRAINTS);

	//all activities will depend on only one "head"
	for(int i=0; i<this->nInternalActivities; i++){
		for(int j=0; j<this->nInternalActivities; j++){
			int k=this->sameRoom[j];
			if(k>=0 && this->sameRoom[k]>=0)
				this->sameRoom[j]=this->sameRoom[k];
		}
	}
	//make the head have the fixed room
	for(int i=0; i<this->nInternalActivities; i++){
		if(this->fixedRoom[i]>=0 && this->sameRoom[i]>=0){
			bool t=this->fixedRoom[this->sameRoom[i]]==this->fixedRoom[i] || this->fixedRoom[this->sameRoom[i]]==-1;
			if(!t){
				QMessageBox::warning(NULL, QObject::tr("FET information"), 
					 QObject::tr("Activity with id=%1 has fixed room in %2 and must be scheduled in the same\n"
					 " room with act. with id=%3, which is impossible. Please check your constraints").arg(this->internalActivitiesList[i].id)
					 .arg(this->fixedRoom[i]).arg(this->internalActivitiesList[this->sameRoom[i]].id));
				this->internalStructureComputed=false;
				return false;
			}

			assert(this->fixedRoom[this->sameRoom[i]]==this->fixedRoom[i] || this->fixedRoom[this->sameRoom[i]]==-1);
			this->fixedRoom[this->sameRoom[i]]=this->fixedRoom[i];
		}
	}
	//make all have the fixed room
	for(int i=0; i<this->nInternalActivities; i++){
		if(this->sameRoom[i]>=0){
			assert(this->fixedRoom[i]==this->fixedRoom[this->sameRoom[i]] || this->fixedRoom[i]==-1);
			this->fixedRoom[i]=this->fixedRoom[this->sameRoom[i]];
		}
	}

	//done.
	this->internalStructureComputed=true;
	
	return true;
}

void Rules::kill() //clears memory for the rules, destroyes them
{
	//Teachers
	assert(this->teachersList.autoDelete()==true);
	this->teachersList.clear();

	//Subjects
	assert(this->subjectsList.autoDelete()==true);
	this->subjectsList.clear();

	//Subject tags
	assert(this->subjectTagsList.autoDelete()==true);
	this->subjectTagsList.clear();

	//Years
	assert(this->yearsList.autoDelete()==true);
	this->yearsList.clear();

	//Activities
	assert(this->activitiesList.autoDelete()==true);
	this->activitiesList.clear();

	//Time constraints
	assert(this->timeConstraintsList.autoDelete()==true);
	this->timeConstraintsList.clear();

	//Space constraints
	assert(this->spaceConstraintsList.autoDelete()==true);
	this->spaceConstraintsList.clear();

	//Equipments
	assert(this->equipmentsList.autoDelete()==true);
	this->equipmentsList.clear();

	//Rooms
	assert(this->roomsList.autoDelete()==true);
	this->roomsList.clear();

	//done
	this->internalStructureComputed=false;
	this->initialized=false;
}

Rules::Rules()
{
	this->initialized=false;
}

Rules::~Rules()
{
	if(this->initialized)
		this->kill();
}

bool Rules::addTeacher(Teacher* teacher)
{
	for(Teacher* tch=this->teachersList.first(); tch; tch=this->teachersList.next())
		if(tch->name==teacher->name)
			return false;
	this->internalStructureComputed=false;
	this->teachersList.append(teacher);
	return true;
}

int Rules::searchTeacher(const QString& teacherName)
{
	int i;
	Teacher* tch;
	for(tch=this->teachersList.first(), i=0; tch; tch=this->teachersList.next(), i++)
		if(tch->name==teacherName)
			return i;
	return -1;
}

bool Rules::removeTeacher(const QString& teacherName)
{
	//TODO: improve this part
	for(Activity* act=this->activitiesList.first(); act; ){
		act->removeTeacher(teacherName);
		if(act->teachersNames.count()==0){
			this->removeActivity(act->id, act->activityGroupId);
			act=this->activitiesList.first(); //!!! please improve this!!!
			//(You have to be careful, there can be erased more activities here)
		}
		else
			act=this->activitiesList.next();
	}

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
		if(ctr->type==CONSTRAINT_TEACHER_NOT_AVAILABLE){
			ConstraintTeacherNotAvailable* crt_constraint=(ConstraintTeacherNotAvailable*)ctr;
			if(teacherName == crt_constraint->teacherName){
				this->removeTimeConstraint(ctr); //single constraint removal
				ctr=this->timeConstraintsList.current();
			}
			else{
				ctr=this->timeConstraintsList.next();
			}
		}
		else
			ctr=this->timeConstraintsList.next();
	}
	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
		if(ctr->type==CONSTRAINT_TEACHER_MAX_DAYS_PER_WEEK){
			ConstraintTeacherMaxDaysPerWeek* crt_constraint=(ConstraintTeacherMaxDaysPerWeek*)ctr;
			if(teacherName == crt_constraint->teacher){
				this->removeTimeConstraint(ctr); //single constraint removal
				ctr=this->timeConstraintsList.current();
			}
			else{
				ctr=this->timeConstraintsList.next();
			}
		}
		else
			ctr=this->timeConstraintsList.next();
	}
	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
		if(ctr->type==CONSTRAINT_ACTIVITIES_PREFERRED_TIMES){
			ConstraintActivitiesPreferredTimes* crt_constraint=(ConstraintActivitiesPreferredTimes*)ctr;
			if(teacherName == crt_constraint->teacherName){
				this->removeTimeConstraint(ctr); //single constraint removal
				ctr=this->timeConstraintsList.current();
			}
			else{
				ctr=this->timeConstraintsList.next();
			}
		}
		else
			ctr=this->timeConstraintsList.next();
	}
	
	for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; ){
		if(ctr->type==CONSTRAINT_TEACHER_REQUIRES_ROOM){
			ConstraintTeacherRequiresRoom* crt_constraint=(ConstraintTeacherRequiresRoom*)ctr;
			if(teacherName == crt_constraint->teacherName){
				this->removeSpaceConstraint(ctr); //single constraint removal
				ctr=this->spaceConstraintsList.current();
			}
			else{
				ctr=this->spaceConstraintsList.next();
			}
		}
		else
			ctr=this->spaceConstraintsList.next();
	}

	for(Teacher* tch=this->teachersList.first(); tch; tch=this->teachersList.next())
		if(tch->name==teacherName)
			break;
	this->teachersList.remove();

	this->internalStructureComputed=false;

	return true;
}

bool Rules::modifyTeacher(const QString& initialTeacherName, const QString& finalTeacherName)
{
	assert(this->searchTeacher(finalTeacherName)==-1);
	assert(this->searchTeacher(initialTeacherName)>=0);

	//TODO: improve this part
	for(Activity* act=this->activitiesList.first(); act; act=this->activitiesList.next())
		act->renameTeacher(initialTeacherName, finalTeacherName);

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ctr=this->timeConstraintsList.next()){
		if(ctr->type==CONSTRAINT_TEACHER_NOT_AVAILABLE){
			ConstraintTeacherNotAvailable* crt_constraint=(ConstraintTeacherNotAvailable*)ctr;
			if(initialTeacherName == crt_constraint->teacherName)
				crt_constraint->teacherName=finalTeacherName;
		}
	}

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ctr=this->timeConstraintsList.next()){
		if(ctr->type==CONSTRAINT_TEACHER_MAX_DAYS_PER_WEEK){
			ConstraintTeacherMaxDaysPerWeek* crt_constraint=(ConstraintTeacherMaxDaysPerWeek*)ctr;
			if(initialTeacherName == crt_constraint->teacher)
				crt_constraint->teacher=finalTeacherName;
		}
	}
	
	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ctr=this->timeConstraintsList.next()){
		if(ctr->type==CONSTRAINT_ACTIVITIES_PREFERRED_TIMES){
			ConstraintActivitiesPreferredTimes* crt_constraint=(ConstraintActivitiesPreferredTimes*)ctr;
			if(initialTeacherName == crt_constraint->teacherName)
				crt_constraint->teacherName=finalTeacherName;
		}
	}
	
	for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; ctr=this->spaceConstraintsList.next()){
		if(ctr->type==CONSTRAINT_TEACHER_REQUIRES_ROOM){
			ConstraintTeacherRequiresRoom* crt_constraint=(ConstraintTeacherRequiresRoom*)ctr;
			if(initialTeacherName == crt_constraint->teacherName)
				crt_constraint->teacherName=finalTeacherName;
		}
	}

	int t=0;
	for(Teacher* tch=this->teachersList.first(); tch; tch=this->teachersList.next())
		if(tch->name==initialTeacherName){
			tch->name=finalTeacherName;
			t++;
		}
	assert(t<=1);

	this->internalStructureComputed=false;

	if(t==0)
		return false;
	else
		return true;
}

void Rules::sortTeachersAlphabetically()
{
	this->teachersList.sort();

	this->internalStructureComputed=false;
}

bool Rules::addSubject(Subject* subject)
{
	for(Subject* sbj=this->subjectsList.first(); sbj; sbj=this->subjectsList.next())
		if(sbj->name==subject->name)
			return false;
	this->internalStructureComputed=false;
	this->subjectsList.append(subject);
	return true;
}

int Rules::searchSubject(const QString& subjectName)
{
	int i;
	Subject* subject;

	for(subject=this->subjectsList.first(), i=0; subject; subject=this->subjectsList.next())
		if(subject->name==subjectName)
			return i;
	return -1;
}

bool Rules::removeSubject(const QString& subjectName)
{
	//check the activities first
	//TODO: improve this part
	for(Activity* act=this->activitiesList.first(); act; ){
		if( act->subjectName == subjectName){
			this->removeActivity(act->id, act->activityGroupId);
			act=this->activitiesList.first(); //!!! please improve this!!!
			//(You have to be careful, there can be erased more activities here)
		}
		else
			act=this->activitiesList.next();
	}
	
	//delete the time constraints related to this subject
	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
		if(ctr->type==CONSTRAINT_ACTIVITIES_PREFERRED_TIMES){
			ConstraintActivitiesPreferredTimes* crt_constraint=(ConstraintActivitiesPreferredTimes*)ctr;
			if(subjectName == crt_constraint->subjectName){
				this->removeTimeConstraint(ctr); //single constraint removal
				ctr=this->timeConstraintsList.current();
			}
			else{
				ctr=this->timeConstraintsList.next();
			}
		}
		else
			ctr=this->timeConstraintsList.next();
	}

	//delete the space constraints related to this subject
	for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; ){
		if(ctr->type==CONSTRAINT_ROOM_TYPE_NOT_ALLOWED_SUBJECTS){
			ConstraintRoomTypeNotAllowedSubjects* crtas=(ConstraintRoomTypeNotAllowedSubjects*)ctr;
			QStringList::Iterator it;
			for(it=crtas->subjects.begin(); it!=crtas->subjects.end(); it++)
				if(*it==subjectName)
					break;
			if(it!=crtas->subjects.end()){
				int tmp=crtas->removeNotAllowedSubject(subjectName);
				assert(tmp==1);
			}

			//remove the constraint if subjects list empty
			if(crtas->subjects.isEmpty()){
				this->removeSpaceConstraint(ctr);
				ctr=this->spaceConstraintsList.current();
			}
			else
				ctr=this->spaceConstraintsList.next();
		}
		else if(ctr->type==CONSTRAINT_SUBJECT_REQUIRES_EQUIPMENTS){
			ConstraintSubjectRequiresEquipments* csre=(ConstraintSubjectRequiresEquipments*)ctr;

			if(csre->subjectName == subjectName){
				this->removeSpaceConstraint(ctr);
				ctr=this->spaceConstraintsList.current();
			}
			else
				ctr=this->spaceConstraintsList.next();
		}
		else if(ctr->type==CONSTRAINT_SUBJECT_SUBJECT_TAG_REQUIRE_EQUIPMENTS){
			ConstraintSubjectSubjectTagRequireEquipments* c=(ConstraintSubjectSubjectTagRequireEquipments*)ctr;

			if(c->subjectName == subjectName){
				this->removeSpaceConstraint(ctr);
				ctr=this->spaceConstraintsList.current();
			}
			else
				ctr=this->spaceConstraintsList.next();
		}
		else
			ctr=this->spaceConstraintsList.next();
	}

	//remove the subject from the list
	for(Subject* sbj=this->subjectsList.first(); sbj; sbj=this->subjectsList.next())
		if(sbj->name==subjectName)
			break;
	this->subjectsList.remove();

	this->internalStructureComputed=false;

	return true;
}

bool Rules::modifySubject(const QString& initialSubjectName, const QString& finalSubjectName)
{
	assert(this->searchSubject(finalSubjectName)==-1);
	assert(this->searchSubject(initialSubjectName)>=0);

	//check the activities first
	for(Activity* act=this->activitiesList.first(); act; act=this->activitiesList.next())
		if( act->subjectName == initialSubjectName)
			act->subjectName=finalSubjectName;
	
	//modify the time constraints related to this subject
	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ctr=this->timeConstraintsList.next()){
		if(ctr->type==CONSTRAINT_ACTIVITIES_PREFERRED_TIMES){
			ConstraintActivitiesPreferredTimes* crt_constraint=(ConstraintActivitiesPreferredTimes*)ctr;
			if(initialSubjectName == crt_constraint->subjectName)
				crt_constraint->subjectName=finalSubjectName;
		}
	}

	//modify the space constraints related to this subject
	for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; ctr=this->spaceConstraintsList.next()){
		if(ctr->type==CONSTRAINT_ROOM_TYPE_NOT_ALLOWED_SUBJECTS){
			ConstraintRoomTypeNotAllowedSubjects* crtas=(ConstraintRoomTypeNotAllowedSubjects*)ctr;
			QStringList::Iterator it;
			for(it=crtas->subjects.begin(); it!=crtas->subjects.end(); it++)
				if(*it==initialSubjectName){
					*it=finalSubjectName;
					break;
				}
		}
		else if(ctr->type==CONSTRAINT_SUBJECT_REQUIRES_EQUIPMENTS){
			ConstraintSubjectRequiresEquipments* csre=(ConstraintSubjectRequiresEquipments*)ctr;
			if(csre->subjectName == initialSubjectName)
				csre->subjectName=finalSubjectName;
		}
		else if(ctr->type==CONSTRAINT_SUBJECT_SUBJECT_TAG_REQUIRE_EQUIPMENTS){
			ConstraintSubjectSubjectTagRequireEquipments* c=(ConstraintSubjectSubjectTagRequireEquipments*)ctr;
			if(c->subjectName == initialSubjectName)
				c->subjectName=finalSubjectName;
		}
	}

	//rename the subject in the list
	int t=0;
	for(Subject* sbj=this->subjectsList.first(); sbj; sbj=this->subjectsList.next())
		if(sbj->name==initialSubjectName){
			t++;
			sbj->name=finalSubjectName;
		}
	assert(t<=1);

	this->internalStructureComputed=false;

	return true;
}

void Rules::sortSubjectsAlphabetically()
{
	this->subjectsList.sort();

	this->internalStructureComputed=false;
}

bool Rules::addSubjectTag(SubjectTag* subjectTag)
{
	for(SubjectTag* sbt=this->subjectTagsList.first(); sbt; sbt=this->subjectTagsList.next())
		if(sbt->name==subjectTag->name)
			return false;
	this->internalStructureComputed=false;
	this->subjectTagsList.append(subjectTag);
	return true;
}

int Rules::searchSubjectTag(const QString& subjectTagName)
{
	int i;
	SubjectTag* subjectTag;

	for(subjectTag=this->subjectTagsList.first(), i=0; subjectTag; subjectTag=this->subjectTagsList.next())
		if(subjectTag->name==subjectTagName)
			return i;
	return -1;
}

bool Rules::removeSubjectTag(const QString& subjectTagName)
{
	//check the activities first
	//TODO: improve this part
	for(Activity* act=this->activitiesList.first(); act; act=this->activitiesList.next())
		if( act->subjectTagName == subjectTagName)
			act->subjectTagName="";
			
	//delete the space constraints related to this subject
	for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; ){
		if(ctr->type==CONSTRAINT_SUBJECT_SUBJECT_TAG_REQUIRE_EQUIPMENTS){
			ConstraintSubjectSubjectTagRequireEquipments* c=(ConstraintSubjectSubjectTagRequireEquipments*)ctr;

			if(c->subjectTagName == subjectTagName){
				this->removeSpaceConstraint(ctr);
				ctr=this->spaceConstraintsList.current();
			}
			else
				ctr=this->spaceConstraintsList.next();
		}
		else
			ctr=this->spaceConstraintsList.next();
	}

	
	//remove the subject tag from the list
	for(SubjectTag* sbt=this->subjectTagsList.first(); sbt; sbt=this->subjectTagsList.next())
		if(sbt->name==subjectTagName)
			break;
	this->subjectTagsList.remove();

	this->internalStructureComputed=false;

	return true;
}

bool Rules::modifySubjectTag(const QString& initialSubjectTagName, const QString& finalSubjectTagName)
{
	assert(this->searchSubject(finalSubjectTagName)==-1);
	assert(this->searchSubject(initialSubjectTagName)>=0);

	//check the activities first
	for(Activity* act=this->activitiesList.first(); act; act=this->activitiesList.next())
		if( act->subjectTagName == initialSubjectTagName)
			act->subjectTagName=finalSubjectTagName;
	
	//modify the space constraints related to this subject
	for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; ctr=this->spaceConstraintsList.next())
		if(ctr->type==CONSTRAINT_SUBJECT_SUBJECT_TAG_REQUIRE_EQUIPMENTS){
			ConstraintSubjectSubjectTagRequireEquipments* c=(ConstraintSubjectSubjectTagRequireEquipments*)ctr;
			if(c->subjectTagName == initialSubjectTagName)
				c->subjectTagName=finalSubjectTagName;
		}

	//rename the subject tag in the list
	int t=0;
	for(SubjectTag* sbt=this->subjectTagsList.first(); sbt; sbt=this->subjectTagsList.next())
		if(sbt->name==initialSubjectTagName){
			t++;
			sbt->name=finalSubjectTagName;
		}
	assert(t<=1);

	this->internalStructureComputed=false;

	return true;
}

void Rules::sortSubjectTagsAlphabetically()
{
	this->subjectTagsList.sort();

	this->internalStructureComputed=false;
}

StudentsSet* Rules::searchStudentsSet(const QString& setName)
{
	for(StudentsYear* sty=this->yearsList.first(); sty; sty=this->yearsList.next()){
		if(sty->name==setName)
			return sty;
		for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
			if(stg->name==setName)
				return stg;
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next())
				if(sts->name==setName)
					return sts;
		}
	}
	return NULL;
}

bool Rules::addYear(StudentsYear* year)
{
	//already existing?
	if(this->searchStudentsSet(year->name)!=NULL)
		return false;
	this->yearsList.append(year);
	this->internalStructureComputed=false;
	return true;
}

bool Rules::removeYear(const QString& yearName)
{
	//TODO: improve this part
	for(Activity* act=this->activitiesList.first(); act; ){
		act->removeStudents(yearName);
		if(act->studentsNames.count()==0){
			this->removeActivity(act->id, act->activityGroupId);
			act=this->activitiesList.first(); //!!! please improve this!!!
			//(You have to be careful, there can be erased more activities here)
		}
		else
			act=this->activitiesList.next();
	}

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
		bool erased=false;
		if(ctr->type==CONSTRAINT_STUDENTS_SET_NOT_AVAILABLE){
			ConstraintStudentsSetNotAvailable* crt_constraint=(ConstraintStudentsSetNotAvailable*)ctr;
			if(yearName == crt_constraint->students){
				this->removeTimeConstraint(ctr);
				erased=true;
			}
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_N_HOURS_DAILY){
			ConstraintStudentsSetNHoursDaily* crt_constraint=(ConstraintStudentsSetNHoursDaily*)ctr;
			if(yearName == crt_constraint->students){
				this->removeTimeConstraint(ctr);
				erased=true;
			}
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_NO_GAPS){
			ConstraintStudentsSetNoGaps* crt_constraint=(ConstraintStudentsSetNoGaps*)ctr;
			if(yearName == crt_constraint->students){
				this->removeTimeConstraint(ctr);
				erased=true;
			}
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_INTERVAL_MAX_DAYS_PER_WEEK){
			ConstraintStudentsSetIntervalMaxDaysPerWeek* crt_constraint=(ConstraintStudentsSetIntervalMaxDaysPerWeek*)ctr;
			if(yearName == crt_constraint->students){
				this->removeTimeConstraint(ctr);
				erased=true;
			}
		}
		else if(ctr->type==CONSTRAINT_ACTIVITIES_PREFERRED_TIMES){
			ConstraintActivitiesPreferredTimes* crt_constraint=(ConstraintActivitiesPreferredTimes*)ctr;
			if(yearName == crt_constraint->studentsName){
				this->removeTimeConstraint(ctr);
				erased=true;
			}
		}

		if(!erased)
			ctr=this->timeConstraintsList.next();
		else
			ctr=this->timeConstraintsList.current();
	}

	StudentsYear* sty;
	for(sty=this->yearsList.first(); sty; sty=this->yearsList.next())
		if(sty->name==yearName)
			break;
	assert(sty);
	this->yearsList.remove();

	this->internalStructureComputed=false;
	return true;
}

int Rules::searchYear(const QString& yearName)
{
	int i;
	StudentsYear* sty;
	for(sty=this->yearsList.first(), i=0; sty; sty=this->yearsList.next(), i++){
		if(sty->name==yearName)
			return i;
	}
	return -1;
}

bool Rules::modifyYear(const QString& initialYearName, const QString& finalYearName, int finalNumberOfStudents)
{
	assert(searchYear(initialYearName)>=0);
	assert(searchStudentsSet(finalYearName)==NULL || initialYearName==finalYearName);

	//TODO: improve this part
	for(Activity* act=this->activitiesList.first(); act; act=this->activitiesList.next())
		act->renameStudents(initialYearName, finalYearName);

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ctr=this->timeConstraintsList.next()){
		if(ctr->type==CONSTRAINT_STUDENTS_SET_NOT_AVAILABLE){
			ConstraintStudentsSetNotAvailable* crt_constraint=(ConstraintStudentsSetNotAvailable*)ctr;
			if(initialYearName == crt_constraint->students)
				crt_constraint->students=finalYearName;
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_N_HOURS_DAILY){
			ConstraintStudentsSetNHoursDaily* crt_constraint=(ConstraintStudentsSetNHoursDaily*)ctr;
			if(initialYearName == crt_constraint->students)
				crt_constraint->students=finalYearName;
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_NO_GAPS){
			ConstraintStudentsSetNoGaps* crt_constraint=(ConstraintStudentsSetNoGaps*)ctr;
			if(initialYearName == crt_constraint->students)
				crt_constraint->students=finalYearName;
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_INTERVAL_MAX_DAYS_PER_WEEK){
			ConstraintStudentsSetIntervalMaxDaysPerWeek* crt_constraint=(ConstraintStudentsSetIntervalMaxDaysPerWeek*)ctr;
			if(initialYearName == crt_constraint->students)
				crt_constraint->students=finalYearName;
		}
		else if(ctr->type==CONSTRAINT_ACTIVITIES_PREFERRED_TIMES){
			ConstraintActivitiesPreferredTimes* crt_constraint=(ConstraintActivitiesPreferredTimes*)ctr;
			if(initialYearName == crt_constraint->studentsName)
				crt_constraint->studentsName=finalYearName;
		}
	}

	int t=0;
	for(StudentsYear* sty=this->yearsList.first(); sty; sty=this->yearsList.next())
		if(sty->name==initialYearName){
			sty->name=finalYearName;
			sty->numberOfStudents=finalNumberOfStudents;
			t++;
			
			for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
				if(stg->name.right(11)==" WHOLE YEAR" && stg->name.left(stg->name.length()-11)==initialYearName)
				 	this->modifyGroup(sty->name, stg->name, sty->name+" WHOLE YEAR", stg->numberOfStudents);
			}
		}
	assert(t<=1);

	this->internalStructureComputed=false;
	
	if(t==0)
		return false;
	else
		return true;
}

void Rules::sortYearsAlphabetically()
{
	this->yearsList.sort();

	this->internalStructureComputed=false;
}

bool Rules::addGroup(const QString& yearName, StudentsGroup* group)
{
	StudentsYear* sty;
	for(sty=this->yearsList.first(); sty; sty=this->yearsList.next())
		if(sty->name==yearName)
			break;
	assert(sty);
	for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next())
		if(stg->name==group->name)
			return false;
	sty->groupsList.append(group);

	this->internalStructureComputed=false;
	return true;
}

bool Rules::removeGroup(const QString& yearName, const QString& groupName)
{
	StudentsYear* sty;
	for(sty=this->yearsList.first(); sty; sty=this->yearsList.next())
		if(sty->name==yearName)
			break;
	assert(sty);
	StudentsGroup* stg;
	for(stg=sty->groupsList.first(); stg; stg=sty->groupsList.next())
		if(stg->name==groupName)
			break;
	assert(stg);
	sty->groupsList.take();

	this->internalStructureComputed=false;

	if(this->searchStudentsSet(stg->name)!=NULL)
		//group still exists
		return true;

	delete stg;

	//TODO: improve this part
	for(Activity* act=this->activitiesList.first(); act; ){
		act->removeStudents(groupName);
		if(act->studentsNames.count()==0){
			this->removeActivity(act->id, act->activityGroupId);
			act=this->activitiesList.first(); //!!! please improve this!!!
			//(You have to be careful, there can be erased more activities here)
		}
		else
			act=this->activitiesList.next();
	}

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
		bool erased=false;
		if(ctr->type==CONSTRAINT_STUDENTS_SET_NOT_AVAILABLE){
			ConstraintStudentsSetNotAvailable* crt_constraint=(ConstraintStudentsSetNotAvailable*)ctr;
			if(groupName == crt_constraint->students){
				this->removeTimeConstraint(ctr);
				erased=true;
			}
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_N_HOURS_DAILY){
			ConstraintStudentsSetNHoursDaily* crt_constraint=(ConstraintStudentsSetNHoursDaily*)ctr;
			if(groupName == crt_constraint->students){
				this->removeTimeConstraint(ctr);
				erased=true;
			}
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_NO_GAPS){
			ConstraintStudentsSetNoGaps* crt_constraint=(ConstraintStudentsSetNoGaps*)ctr;
			if(groupName == crt_constraint->students){
				this->removeTimeConstraint(ctr);
				erased=true;
			}
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_INTERVAL_MAX_DAYS_PER_WEEK){
			ConstraintStudentsSetIntervalMaxDaysPerWeek* crt_constraint=(ConstraintStudentsSetIntervalMaxDaysPerWeek*)ctr;
			if(groupName == crt_constraint->students){
				this->removeTimeConstraint(ctr);
				erased=true;
			}
		}
		else if(ctr->type==CONSTRAINT_ACTIVITIES_PREFERRED_TIMES){
			ConstraintActivitiesPreferredTimes* crt_constraint=(ConstraintActivitiesPreferredTimes*)ctr;
			if(groupName == crt_constraint->studentsName){
				this->removeTimeConstraint(ctr);
				erased=true;
			}
		}

		if(!erased)
			ctr=this->timeConstraintsList.next();
		else
			ctr=this->timeConstraintsList.current();
	}

	return true;
}

int Rules::searchGroup(const QString& yearName, const QString& groupName)
{
	StudentsYear* sty=this->yearsList.at(this->searchYear(yearName));
	assert(sty);
	
	StudentsGroup* stg;
	int i;
	for(stg=sty->groupsList.first(), i=0; stg; stg=sty->groupsList.next(), i++)
		if(stg->name==groupName)
			return i;
	return -1;
}

bool Rules::modifyGroup(const QString& yearName, const QString& initialGroupName, const QString& finalGroupName, int finalNumberOfStudents)
{
	assert(searchGroup(yearName, initialGroupName)>=0);
	assert(searchStudentsSet(finalGroupName)==NULL || initialGroupName==finalGroupName);

	StudentsYear* sty;
	for(sty=this->yearsList.first(); sty; sty=this->yearsList.next())
		if(sty->name==yearName)
			break;
	assert(sty);
	StudentsGroup* stg;
	for(stg=sty->groupsList.first(); stg; stg=sty->groupsList.next())
		if(stg->name==initialGroupName){
			stg->name=finalGroupName;
			stg->numberOfStudents=finalNumberOfStudents;

			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next()){
				if(sts->name.right(12)==" WHOLE GROUP" && sts->name.left(sts->name.length()-12)==initialGroupName)
				 	this->modifySubgroup(sty->name, stg->name, sts->name, stg->name+" WHOLE GROUP", sts->numberOfStudents);
			}

			break;
		}
	assert(stg);

	//TODO: improve this part
	for(Activity* act=this->activitiesList.first(); act; act=this->activitiesList.next())
		act->renameStudents(initialGroupName, finalGroupName);

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ctr=this->timeConstraintsList.next()){
		if(ctr->type==CONSTRAINT_STUDENTS_SET_NOT_AVAILABLE){
			ConstraintStudentsSetNotAvailable* crt_constraint=(ConstraintStudentsSetNotAvailable*)ctr;
			if(initialGroupName == crt_constraint->students)
				crt_constraint->students=finalGroupName;
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_N_HOURS_DAILY){
			ConstraintStudentsSetNHoursDaily* crt_constraint=(ConstraintStudentsSetNHoursDaily*)ctr;
			if(initialGroupName == crt_constraint->students)
				crt_constraint->students=finalGroupName;
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_NO_GAPS){
			ConstraintStudentsSetNoGaps* crt_constraint=(ConstraintStudentsSetNoGaps*)ctr;
			if(initialGroupName == crt_constraint->students)
				crt_constraint->students=finalGroupName;
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_INTERVAL_MAX_DAYS_PER_WEEK){
			ConstraintStudentsSetIntervalMaxDaysPerWeek* crt_constraint=(ConstraintStudentsSetIntervalMaxDaysPerWeek*)ctr;
			if(initialGroupName == crt_constraint->students)
				crt_constraint->students=finalGroupName;
		}
		else if(ctr->type==CONSTRAINT_ACTIVITIES_PREFERRED_TIMES){
			ConstraintActivitiesPreferredTimes* crt_constraint=(ConstraintActivitiesPreferredTimes*)ctr;
			if(initialGroupName == crt_constraint->studentsName)
				crt_constraint->studentsName=finalGroupName;
		}
	}

	this->internalStructureComputed=false;

	return true;
}

void Rules::sortGroupsAlphabetically(const QString& yearName)
{
	StudentsYear* sty;
	for(sty=this->yearsList.first(); sty; sty=this->yearsList.next())
		if(sty->name==yearName)
			break;
	assert(sty);
	
	sty->groupsList.sort();

	this->internalStructureComputed=false;
}

bool Rules::addSubgroup(const QString& yearName, const QString& groupName, StudentsSubgroup* subgroup)
{
	StudentsYear* sty;
	for(sty=this->yearsList.first(); sty; sty=this->yearsList.next())
		if(sty->name==yearName)
			break;
	assert(sty);

	StudentsGroup* stg;
	for(stg=sty->groupsList.first(); stg; stg=sty->groupsList.next())
		if(stg->name==groupName)
			break;
	assert(stg);

	for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next())
		if(sts->name==subgroup->name)
			return false;
	stg->subgroupsList.append(subgroup);

	this->internalStructureComputed=false;
	return true;
}

bool Rules::removeSubgroup(const QString& yearName, const QString& groupName, const QString& subgroupName)
{
	StudentsYear* sty;
	for(sty=this->yearsList.first(); sty; sty=this->yearsList.next())
		if(sty->name==yearName)
			break;
	assert(sty);

	StudentsGroup* stg;
	for(stg=sty->groupsList.first(); stg; stg=sty->groupsList.next())
		if(stg->name==groupName)
			break;
	assert(stg);

	StudentsSubgroup* sts;
	for(sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next())
		if(sts->name==subgroupName)
			break;
	assert(sts);
	stg->subgroupsList.take();
	if(this->searchStudentsSet(sts->name)!=NULL)
		//subgroup still exists, in other group
		return true;

	delete sts;

	this->internalStructureComputed=false;

	//TODO: improve this part
	for(Activity* act=this->activitiesList.first(); act; ){
		act->removeStudents(subgroupName);
		if(act->studentsNames.count()==0){
			this->removeActivity(act->id, act->activityGroupId);
			act=this->activitiesList.first(); //!!! please improve this!!!
			//(You have to be careful, there can be erased more activities here)
		}
		else
			act=this->activitiesList.next();
	}

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
		bool erased=false;
		if(ctr->type==CONSTRAINT_STUDENTS_SET_NOT_AVAILABLE){
			ConstraintStudentsSetNotAvailable* crt_constraint=(ConstraintStudentsSetNotAvailable*)ctr;
			if(subgroupName == crt_constraint->students){
				this->removeTimeConstraint(ctr);
				erased=true;
			}
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_N_HOURS_DAILY){
			ConstraintStudentsSetNHoursDaily* crt_constraint=(ConstraintStudentsSetNHoursDaily*)ctr;
			if(subgroupName == crt_constraint->students){
				this->removeTimeConstraint(ctr);
				erased=true;
			}
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_NO_GAPS){
			ConstraintStudentsSetNoGaps* crt_constraint=(ConstraintStudentsSetNoGaps*)ctr;
			if(subgroupName == crt_constraint->students){
				this->removeTimeConstraint(ctr);
				erased=true;
			}
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_INTERVAL_MAX_DAYS_PER_WEEK){
			ConstraintStudentsSetIntervalMaxDaysPerWeek* crt_constraint=(ConstraintStudentsSetIntervalMaxDaysPerWeek*)ctr;
			if(subgroupName == crt_constraint->students){
				this->removeTimeConstraint(ctr);
				erased=true;
			}
		}
		else if(ctr->type==CONSTRAINT_ACTIVITIES_PREFERRED_TIMES){
			ConstraintActivitiesPreferredTimes* crt_constraint=(ConstraintActivitiesPreferredTimes*)ctr;
			if(subgroupName == crt_constraint->studentsName){
				this->removeTimeConstraint(ctr);
				erased=true;
			}
		}

		if(!erased)
			ctr=this->timeConstraintsList.next();
		else
			ctr=this->timeConstraintsList.current();
	}

	return true;
}

int Rules::searchSubgroup(const QString& yearName, const QString& groupName, const QString& subgroupName)
{
	StudentsYear* sty=this->yearsList.at(this->searchYear(yearName));
	assert(sty);
	StudentsGroup* stg=sty->groupsList.at(this->searchGroup(yearName, groupName));
	assert(stg);
	
	StudentsSubgroup* sts;
	int i;
	for(sts=stg->subgroupsList.first(), i=0; sts; sts=stg->subgroupsList.next(), i++)
		if(sts->name==subgroupName)
			return i;
	return -1;
}

bool Rules::modifySubgroup(const QString& yearName, const QString& groupName, const QString& initialSubgroupName, const QString& finalSubgroupName, int finalNumberOfStudents)
{
	assert(searchSubgroup(yearName, groupName, initialSubgroupName)>=0);
	assert(searchStudentsSet(finalSubgroupName)==NULL || initialSubgroupName==finalSubgroupName);

	StudentsYear* sty;
	for(sty=this->yearsList.first(); sty; sty=this->yearsList.next())
		if(sty->name==yearName)
			break;
	assert(sty);

	StudentsGroup* stg;
	for(stg=sty->groupsList.first(); stg; stg=sty->groupsList.next())
		if(stg->name==groupName)
			break;
	assert(stg);

	StudentsSubgroup* sts;
	for(sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next())
		if(sts->name==initialSubgroupName){
			sts->name=finalSubgroupName;
			sts->numberOfStudents=finalNumberOfStudents;
			break;
		}
	assert(sts);

	//TODO: improve this part
	for(Activity* act=this->activitiesList.first(); act; act=this->activitiesList.next())
		act->renameStudents(initialSubgroupName, finalSubgroupName);

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ctr=this->timeConstraintsList.next()){
		if(ctr->type==CONSTRAINT_STUDENTS_SET_NOT_AVAILABLE){
			ConstraintStudentsSetNotAvailable* crt_constraint=(ConstraintStudentsSetNotAvailable*)ctr;
			if(initialSubgroupName == crt_constraint->students)
				crt_constraint->students=finalSubgroupName;
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_N_HOURS_DAILY){
			ConstraintStudentsSetNHoursDaily* crt_constraint=(ConstraintStudentsSetNHoursDaily*)ctr;
			if(initialSubgroupName == crt_constraint->students)
				crt_constraint->students=finalSubgroupName;
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_NO_GAPS){
			ConstraintStudentsSetNoGaps* crt_constraint=(ConstraintStudentsSetNoGaps*)ctr;
			if(initialSubgroupName == crt_constraint->students)
				crt_constraint->students=finalSubgroupName;
		}
		else if(ctr->type==CONSTRAINT_STUDENTS_SET_INTERVAL_MAX_DAYS_PER_WEEK){
			ConstraintStudentsSetIntervalMaxDaysPerWeek* crt_constraint=(ConstraintStudentsSetIntervalMaxDaysPerWeek*)ctr;
			if(initialSubgroupName == crt_constraint->students)
				crt_constraint->students=finalSubgroupName;
		}
		else if(ctr->type==CONSTRAINT_ACTIVITIES_PREFERRED_TIMES){
			ConstraintActivitiesPreferredTimes* crt_constraint=(ConstraintActivitiesPreferredTimes*)ctr;
			if(initialSubgroupName == crt_constraint->studentsName)
				crt_constraint->studentsName=finalSubgroupName;
		}
	}

	this->internalStructureComputed=false;

	return true;
}

void Rules::sortSubgroupsAlphabetically(const QString& yearName, const QString& groupName)
{
	StudentsYear* sty;
	for(sty=this->yearsList.first(); sty; sty=this->yearsList.next())
		if(sty->name==yearName)
			break;
	assert(sty);

	StudentsGroup* stg;
	for(stg=sty->groupsList.first(); stg; stg=sty->groupsList.next())
		if(stg->name==groupName)
			break;
	assert(stg);
	
	stg->subgroupsList.sort();
	
	this->internalStructureComputed=false;
}	

bool Rules::addSimpleActivity(
	int _id,
	int _activityGroupId,
	const QStringList& _teachersNames,
	const QString& _subjectName,
	const QString& _subjectTagName,
	const QStringList& _studentsNames,
	int _duration, /*duration, in hours*/
	int _totalDuration,
	int _parity, /*parity: PARITY_WEEKLY or PARITY_BIWEEKLY*/
	int _preferredDay,
	int _preferredHour)
{
	assert(_parity==PARITY_WEEKLY || _parity==PARITY_BIWEEKLY); //weekly or bi-weekly

	Activity *act=new Activity(*this, _id, _activityGroupId, _teachersNames, _subjectName, _subjectTagName,
		_studentsNames, _duration, _totalDuration, _parity);

	this->activitiesList.append(act);

	if(_preferredDay>=0 || _preferredHour>=0){
		TimeConstraint *ctr=new ConstraintActivityPreferredTime(1.0, false, _id, _preferredDay, _preferredHour);
		bool tmp=this->addTimeConstraint(ctr);
		assert(tmp);
	}

	this->internalStructureComputed=false;

	return true;
}

bool Rules::addSplitActivity(
	int _firstActivityId,
	int _activityGroupId,
	const QStringList& _teachersNames,
	const QString& _subjectName,
	const QString& _subjectTagName,
	const QStringList& _studentsNames,
	int _nSplits,
	int _totalDuration,
	int _durations[],
	int _parities[],
	int _minDayDistance,
	int _preferredDays[],
	int _preferredHours[])
{
	assert(_firstActivityId==_activityGroupId);

	int acts[MAX_CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES];
	assert(_nSplits<=MAX_CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES);

	for(int i=0; i<_nSplits; i++)
		assert(_parities[i]==PARITY_WEEKLY || _parities[i]==PARITY_BIWEEKLY); //weekly or bi-weekly

	for(int i=0; i<_nSplits; i++){
		Activity *act;
		if(i==0)
			act=new Activity(*this, _firstActivityId+i, _activityGroupId,
				_teachersNames, _subjectName, _subjectTagName, _studentsNames,
				_durations[i], _totalDuration, _parities[i]);
		else
			act=new Activity(*this, _firstActivityId+i, _activityGroupId,
				_teachersNames, _subjectName, _subjectTagName, _studentsNames,
				_durations[i], _totalDuration, _parities[i]);

		this->activitiesList.append(act);

		acts[i]=_firstActivityId+i;

		if(_preferredDays[i]>=0 || _preferredHours[i]>=0){
			TimeConstraint *constr=new ConstraintActivityPreferredTime(1.0, false, act->id, _preferredDays[i], _preferredHours[i]); //non-compulsory constraint
			bool tmp = this->addTimeConstraint(constr);
			assert(tmp);
		}
	}

	if(_minDayDistance>0){
		TimeConstraint *constr=new ConstraintMinNDaysBetweenActivities(1.0, true, _nSplits, acts, _minDayDistance); //compulsory constraint
		bool tmp=this->addTimeConstraint(constr);
		assert(tmp);
	}

	this->internalStructureComputed=false;

	return true;
}

void Rules::removeActivity(int _id)
{
	for(Activity *act=this->activitiesList.first(); act; act=this->activitiesList.next())
		if(_id==act->id){
			//removing ConstraintActivityPreferredTime-s referring to this activity
			for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
				if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_TIME){
					ConstraintActivityPreferredTime *apt=(ConstraintActivityPreferredTime*)ctr;
					if(apt->activityId==act->id){
						cout<<"Removing constraint "<<(apt->getDescription(*this))<<endl;
						this->removeTimeConstraint(ctr);
						ctr=this->timeConstraintsList.current();
					}
					else
						ctr=this->timeConstraintsList.next();
				}
				else
					ctr=this->timeConstraintsList.next();
			}
			//removing ConstraintActivityPreferredTimes-s referring to this activity
			for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
				if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_TIMES){
					ConstraintActivityPreferredTimes *apt=(ConstraintActivityPreferredTimes*)ctr;
					if(apt->activityId==act->id){
						cout<<"Removing constraint "<<(apt->getDescription(*this))<<endl;
						this->removeTimeConstraint(ctr);
						ctr=this->timeConstraintsList.current();
					}
					else
						ctr=this->timeConstraintsList.next();
				}
				else
					ctr=this->timeConstraintsList.next();
			}
			//removing ConstraintActivityEndsDay-s referring to this activity
			for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
				if(ctr->type==CONSTRAINT_ACTIVITY_ENDS_DAY){
					ConstraintActivityEndsDay *apt=(ConstraintActivityEndsDay*)ctr;
					if(apt->activityId==act->id){
						cout<<"Removing constraint "<<(apt->getDescription(*this))<<endl;
						this->removeTimeConstraint(ctr);
						ctr=this->timeConstraintsList.current();
					}
					else
						ctr=this->timeConstraintsList.next();
				}
				else
					ctr=this->timeConstraintsList.next();
			}
			//removing Constraint2ActivitiesConsecutive-s referring to this activity
			for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
				if(ctr->type==CONSTRAINT_2_ACTIVITIES_CONSECUTIVE){
					Constraint2ActivitiesConsecutive *apt=(Constraint2ActivitiesConsecutive*)ctr;
					if(apt->firstActivityId==act->id || apt->secondActivityId==act->id){
						cout<<"Removing constraint "<<(apt->getDescription(*this))<<endl;
						this->removeTimeConstraint(ctr);
						ctr=this->timeConstraintsList.current();
					}
					else
						ctr=this->timeConstraintsList.next();
				}
				else
					ctr=this->timeConstraintsList.next();
			}
			//removing Constraint2ActivitiesGrouped-s referring to this activity
			for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
				if(ctr->type==CONSTRAINT_2_ACTIVITIES_GROUPED){
					Constraint2ActivitiesGrouped *apt=(Constraint2ActivitiesGrouped*)ctr;
					if(apt->firstActivityId==act->id || apt->secondActivityId==act->id){
						cout<<"Removing constraint "<<(apt->getDescription(*this))<<endl;
						this->removeTimeConstraint(ctr);
						ctr=this->timeConstraintsList.current();
					}
					else
						ctr=this->timeConstraintsList.next();
				}
				else
					ctr=this->timeConstraintsList.next();
			}

			//removing ConstraintActivityPreferredRoom-s referring to this activity
			for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; )
				if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_ROOM){
					if(((ConstraintActivityPreferredRoom*)ctr)->activityId==act->id){
						this->removeSpaceConstraint(ctr);
						ctr=this->spaceConstraintsList.current();
					}
					else
						ctr=this->spaceConstraintsList.next();
				}
				else
					ctr=this->spaceConstraintsList.next();
			//removing ConstraintActivityPreferredRooms-s referring to this activity
			for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; )
				if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_ROOMS){
					if(((ConstraintActivityPreferredRooms*)ctr)->activityId==act->id){
						this->removeSpaceConstraint(ctr);
						ctr=this->spaceConstraintsList.current();
					}
					else
						ctr=this->spaceConstraintsList.next();
				}
				else
					ctr=this->spaceConstraintsList.next();

			//remove the activity
			this->activitiesList.remove(); 
			break;
		}

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; )
		if(ctr->type==CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES){
			((ConstraintMinNDaysBetweenActivities*)ctr)->removeUseless(*this);
			if(((ConstraintMinNDaysBetweenActivities*)ctr)->n_activities<2){
				this->removeTimeConstraint(ctr);
				ctr=this->timeConstraintsList.current();
			}
			else
				ctr=this->timeConstraintsList.next();
		}
		else
			ctr=this->timeConstraintsList.next();

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; )
		if(ctr->type==CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME){
			((ConstraintActivitiesSameStartingTime*)ctr)->removeUseless(*this);
			if(((ConstraintActivitiesSameStartingTime*)ctr)->n_activities<2){
				this->removeTimeConstraint(ctr);
				ctr=this->timeConstraintsList.current();
			}
			else
				ctr=this->timeConstraintsList.next();
		}
		else
			ctr=this->timeConstraintsList.next();

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; )
		if(ctr->type==CONSTRAINT_ACTIVITIES_SAME_STARTING_HOUR){
			((ConstraintActivitiesSameStartingHour*)ctr)->removeUseless(*this);
			if(((ConstraintActivitiesSameStartingHour*)ctr)->n_activities<2){
				this->removeTimeConstraint(ctr);
				ctr=this->timeConstraintsList.current();
			}
			else
				ctr=this->timeConstraintsList.next();
		}
		else
			ctr=this->timeConstraintsList.next();

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; )
		if(ctr->type==CONSTRAINT_ACTIVITIES_NOT_OVERLAPPING){
			((ConstraintActivitiesNotOverlapping*)ctr)->removeUseless(*this);
			if(((ConstraintActivitiesNotOverlapping*)ctr)->n_activities<2){
				this->removeTimeConstraint(ctr);
				ctr=this->timeConstraintsList.current();
			}
			else
				ctr=this->timeConstraintsList.next();
		}
		else
			ctr=this->timeConstraintsList.next();

	for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; )
		if(ctr->type==CONSTRAINT_ACTIVITIES_SAME_ROOM){
			((ConstraintActivitiesSameRoom*)ctr)->removeUseless(*this);
			if(((ConstraintActivitiesSameRoom*)ctr)->n_activities<2){
				this->removeSpaceConstraint(ctr);
				ctr=this->spaceConstraintsList.current();
			}
			else
				ctr=this->spaceConstraintsList.next();
		}
		else
			ctr=this->spaceConstraintsList.next();

	this->internalStructureComputed=false;
}

void Rules::removeActivity(int _id, int _activityGroupId)
{
	for(Activity *act=this->activitiesList.first(); act; ){
		if(_id==act->id || _activityGroupId>0 && _activityGroupId==act->activityGroupId){
			//removing ConstraintActivityPreferredTime-s referring to this activity
			for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
				if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_TIME){
					ConstraintActivityPreferredTime *apt=(ConstraintActivityPreferredTime*)ctr;
					if(apt->activityId==act->id){
						cout<<"Removing constraint "<<(apt->getDescription(*this))<<endl;
						this->removeTimeConstraint(ctr);
						ctr=this->timeConstraintsList.current();
					}
					else
						ctr=this->timeConstraintsList.next();
				}
				else
					ctr=this->timeConstraintsList.next();
			}
			//removing ConstraintActivityPreferredTimes-s referring to this activity
			for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
				if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_TIMES){
					ConstraintActivityPreferredTimes *apt=(ConstraintActivityPreferredTimes*)ctr;
					if(apt->activityId==act->id){
						cout<<"Removing constraint "<<(apt->getDescription(*this))<<endl;
						this->removeTimeConstraint(ctr);
						ctr=this->timeConstraintsList.current();
					}
					else
						ctr=this->timeConstraintsList.next();
				}
				else
					ctr=this->timeConstraintsList.next();
			}
			//removing ConstraintActivityEndsDay-s referring to this activity
			for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
				if(ctr->type==CONSTRAINT_ACTIVITY_ENDS_DAY){
					ConstraintActivityEndsDay *apt=(ConstraintActivityEndsDay*)ctr;
					if(apt->activityId==act->id){
						cout<<"Removing constraint "<<(apt->getDescription(*this))<<endl;
						this->removeTimeConstraint(ctr);
						ctr=this->timeConstraintsList.current();
					}
					else
						ctr=this->timeConstraintsList.next();
				}
				else
					ctr=this->timeConstraintsList.next();
			}
			//removing Constraint2ActivitiesConsecutive-s referring to this activity
			for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
				if(ctr->type==CONSTRAINT_2_ACTIVITIES_CONSECUTIVE){
					Constraint2ActivitiesConsecutive *apt=(Constraint2ActivitiesConsecutive*)ctr;
					if(apt->firstActivityId==act->id || apt->secondActivityId==act->id){
						cout<<"Removing constraint "<<(apt->getDescription(*this))<<endl;
						this->removeTimeConstraint(ctr);
						ctr=this->timeConstraintsList.current();
					}
					else
						ctr=this->timeConstraintsList.next();
				}
				else
					ctr=this->timeConstraintsList.next();
			}
			//removing Constraint2ActivitiesGrouped-s referring to this activity
			for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ){
				if(ctr->type==CONSTRAINT_2_ACTIVITIES_GROUPED){
					Constraint2ActivitiesGrouped *apt=(Constraint2ActivitiesGrouped*)ctr;
					if(apt->firstActivityId==act->id || apt->secondActivityId==act->id){
						cout<<"Removing constraint "<<(apt->getDescription(*this))<<endl;
						this->removeTimeConstraint(ctr);
						ctr=this->timeConstraintsList.current();
					}
					else
						ctr=this->timeConstraintsList.next();
				}
				else
					ctr=this->timeConstraintsList.next();
			}
			
			//removing ConstraintActivityPreferredRoom-s referring to this activity
			for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; )
				if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_ROOM){
					if(((ConstraintActivityPreferredRoom*)ctr)->activityId==act->id){
						this->removeSpaceConstraint(ctr);
						ctr=this->spaceConstraintsList.current();
					}
					else
						ctr=this->spaceConstraintsList.next();
				}
				else
					ctr=this->spaceConstraintsList.next();
			//removing ConstraintActivityPreferredRooms-s referring to this activity
			for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; )
				if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_ROOMS){
					if(((ConstraintActivityPreferredRooms*)ctr)->activityId==act->id){
						this->removeSpaceConstraint(ctr);
						ctr=this->spaceConstraintsList.current();
					}
					else
						ctr=this->spaceConstraintsList.next();
				}
				else
					ctr=this->spaceConstraintsList.next();

			this->activitiesList.remove(); //if this is the last activity, then we will make one more comparison above
			act=this->activitiesList.current();
		}
		else
			act=this->activitiesList.next();
	}

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; )
		if(ctr->type==CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES){
			((ConstraintMinNDaysBetweenActivities*)ctr)->removeUseless(*this);
			if(((ConstraintMinNDaysBetweenActivities*)ctr)->n_activities<2){
				this->removeTimeConstraint(ctr);
				ctr=this->timeConstraintsList.current();
			}
			else
				ctr=this->timeConstraintsList.next();
		}
		else
			ctr=this->timeConstraintsList.next();

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; )
		if(ctr->type==CONSTRAINT_ACTIVITIES_SAME_STARTING_TIME){
			((ConstraintActivitiesSameStartingTime*)ctr)->removeUseless(*this);
			if(((ConstraintActivitiesSameStartingTime*)ctr)->n_activities<2){
				this->removeTimeConstraint(ctr);
				ctr=this->timeConstraintsList.current();
			}
			else
				ctr=this->timeConstraintsList.next();
		}
		else
			ctr=this->timeConstraintsList.next();

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; )
		if(ctr->type==CONSTRAINT_ACTIVITIES_SAME_STARTING_HOUR){
			((ConstraintActivitiesSameStartingHour*)ctr)->removeUseless(*this);
			if(((ConstraintActivitiesSameStartingHour*)ctr)->n_activities<2){
				this->removeTimeConstraint(ctr);
				ctr=this->timeConstraintsList.current();
			}
			else
				ctr=this->timeConstraintsList.next();
		}
		else
			ctr=this->timeConstraintsList.next();

	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; )
		if(ctr->type==CONSTRAINT_ACTIVITIES_NOT_OVERLAPPING){
			((ConstraintActivitiesNotOverlapping*)ctr)->removeUseless(*this);
			if(((ConstraintActivitiesNotOverlapping*)ctr)->n_activities<2){
				this->removeTimeConstraint(ctr);
				ctr=this->timeConstraintsList.current();
			}
			else
				ctr=this->timeConstraintsList.next();
		}
		else
			ctr=this->timeConstraintsList.next();

	for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; )
		if(ctr->type==CONSTRAINT_ACTIVITIES_SAME_ROOM){
			((ConstraintActivitiesSameRoom*)ctr)->removeUseless(*this);
			if(((ConstraintActivitiesSameRoom*)ctr)->n_activities<2){
				this->removeSpaceConstraint(ctr);
				ctr=this->spaceConstraintsList.current();
			}
			else
				ctr=this->spaceConstraintsList.next();
		}
		else
			ctr=this->spaceConstraintsList.next();

	this->internalStructureComputed=false;
}

void Rules::modifyActivity(
	int _id,
	int _activityGroupId,
	const QStringList& _teachersNames,
	const QString& _subjectName,
	const QString& _subjectTagName,
	const QStringList& _studentsNames,
	int _nTotalStudents,
	int _nSplits,
	int _totalDuration,
	int _durations[],
	int _parities[])
{
	int i=0;
	for(Activity* act=this->activitiesList.first(); act; act=this->activitiesList.next())
		if(_activityGroupId==0 && act->id==_id || _activityGroupId!=0 && act->activityGroupId==_activityGroupId){
			act->teachersNames=_teachersNames;
			act->subjectName=_subjectName;
			act->subjectTagName=_subjectTagName;
			act->studentsNames=_studentsNames;
			act->duration=_durations[i];
			act->parity=_parities[i];
			act->totalDuration=_totalDuration;
			act->nTotalStudents=_nTotalStudents;
			i++;
		}
		
	assert(i==_nSplits);
	
	this->internalStructureComputed=false;
}

bool Rules::addEquipment(Equipment* eq)
{
	if(this->searchEquipment(eq->name) >= 0)
		return false;
	this->equipmentsList.append(eq);
	this->internalStructureComputed=false;
	return true;
}

int Rules::searchEquipment(const QString& equipmentName)
{
	int i;
	Equipment* eq;
	for(eq=this->equipmentsList.first(), i=0; eq; eq=this->equipmentsList.next(), i++)
		if(eq->name==equipmentName)
			return i;
	return -1;
}

bool Rules::removeEquipment(const QString& equipmentName)
{
	int i=this->searchEquipment(equipmentName);
	if(i<0)
		return false;

	Equipment* searchedEquipment=this->equipmentsList.at(i);
	assert(searchedEquipment->name==equipmentName);
	
	//check the constraints
	for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; )
		if(ctr->type==CONSTRAINT_SUBJECT_REQUIRES_EQUIPMENTS){
			ConstraintSubjectRequiresEquipments* csre=(ConstraintSubjectRequiresEquipments*)ctr;
			QStringList::Iterator it;
			for(it=csre->equipmentsNames.begin(); it!=csre->equipmentsNames.end(); it++)
				if(*it==equipmentName)
					break;
			if(it!=csre->equipmentsNames.end()){
				int tmp=csre->removeRequiredEquipment(equipmentName);
				assert(tmp==1);
			}

			//remove the constraint if equipments list empty
			if(csre->equipmentsNames.isEmpty()){
				this->removeSpaceConstraint(ctr);
				ctr=this->spaceConstraintsList.current();
			}
			else
				ctr=this->spaceConstraintsList.next();
		}
		else
			ctr=this->spaceConstraintsList.next();
	
	//Remove this equipment from all the rooms that posess it.
	for(Room* rm=this->roomsList.first(); rm; rm=this->roomsList.next())
		rm->removeEquipment(equipmentName);

	this->equipmentsList.remove(i);
	this->internalStructureComputed=false;
	return true;
}

bool Rules::modifyEquipment(const QString& initialEquipmentName, const QString& finalEquipmentName)
{
	int i=this->searchEquipment(initialEquipmentName);
	assert(i>=0);
	assert(this->searchEquipment(finalEquipmentName)==-1 || initialEquipmentName==finalEquipmentName);

	Equipment* searchedEquipment=this->equipmentsList.at(i);
	assert(searchedEquipment->name==initialEquipmentName);
	
	//check the constraints
	for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; ctr=this->spaceConstraintsList.next())
		if(ctr->type==CONSTRAINT_SUBJECT_REQUIRES_EQUIPMENTS){
			ConstraintSubjectRequiresEquipments* csre=(ConstraintSubjectRequiresEquipments*)ctr;
			for(QStringList::Iterator it=csre->equipmentsNames.begin(); it!=csre->equipmentsNames.end(); it++)
				if(*it==initialEquipmentName)
					*it=finalEquipmentName;
		}
	
	//Modify this equipment in all the rooms that posess it.
	for(Room* rm=this->roomsList.first(); rm; rm=this->roomsList.next())
		for(QStringList::Iterator it=rm->equipments.begin(); it!=rm->equipments.end(); it++)
			if(*it==initialEquipmentName)
				*it=finalEquipmentName;
				
	searchedEquipment->name=finalEquipmentName;

	this->internalStructureComputed=false;
	return true;
}

void Rules::sortEquipmentsAlphabetically()
{
	this->equipmentsList.sort();

	this->internalStructureComputed=false;
}

bool Rules::addRoom(Room* rm)
{
	if(this->searchRoom(rm->name) >= 0)
		return false;
	this->roomsList.append(rm);
	this->internalStructureComputed=false;
	return true;
}

int Rules::searchRoom(const QString& roomName)
{
	int i;
	Room* rm;
	for(rm=this->roomsList.first(), i=0; rm; rm=this->roomsList.next(), i++)
		if(rm->name==roomName)
			return i;
	return -1;
}

bool Rules::removeRoom(const QString& roomName)
{
	int i=this->searchRoom(roomName);
	if(i<0)
		return false;

	Room* searchedRoom=this->roomsList.at(i);
	assert(searchedRoom->name==roomName);

	//Removing useless ConstraintRoomTypeNotAllowedSubjects
	//Firstly, check whether the type of this room exists at another room
	bool typeExisting=false;
	for(Room *rm=this->roomsList.first(); rm; rm=this->roomsList.next())
		if(rm->name!=searchedRoom->name && rm->type==searchedRoom->type){
			typeExisting=true;
			break;
		}
	for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; )
		if(!typeExisting && ctr->type==CONSTRAINT_ROOM_TYPE_NOT_ALLOWED_SUBJECTS){
			ConstraintRoomTypeNotAllowedSubjects* crtas=(ConstraintRoomTypeNotAllowedSubjects*)ctr;
			if(crtas->roomType==searchedRoom->type){
				this->removeSpaceConstraint(ctr);
				ctr=this->spaceConstraintsList.current();
			}
			else
				ctr=this->spaceConstraintsList.next();
		}
		else if(ctr->type==CONSTRAINT_ROOM_NOT_AVAILABLE){
			ConstraintRoomNotAvailable* crna=(ConstraintRoomNotAvailable*)ctr;
			if(crna->roomName==roomName){
				this->removeSpaceConstraint(ctr);
				ctr=this->spaceConstraintsList.current();
			}
			else
				ctr=this->spaceConstraintsList.next();
		}
		else if(ctr->type==CONSTRAINT_TEACHER_REQUIRES_ROOM){
			ConstraintTeacherRequiresRoom* c=(ConstraintTeacherRequiresRoom*)ctr;
			if(c->roomName==roomName){
				this->removeSpaceConstraint(ctr);
				ctr=this->spaceConstraintsList.current();
			}
			else
				ctr=this->spaceConstraintsList.next();
		}
		else if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_ROOM){
			ConstraintActivityPreferredRoom* c=(ConstraintActivityPreferredRoom*)ctr;
			if(c->roomName==roomName){
				this->removeSpaceConstraint(ctr);
				ctr=this->spaceConstraintsList.current();
			}
			else
				ctr=this->spaceConstraintsList.next();
		}
		else if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_ROOMS){
			ConstraintActivityPreferredRooms* c=(ConstraintActivityPreferredRooms*)ctr;
			int t=c->roomsNames.remove(roomName);
			assert(t<=1);
			if(t==1 && c->roomsNames.count()==0){
				this->removeSpaceConstraint(ctr);
				ctr=this->spaceConstraintsList.current();
			}
			else if(t==1 && c->roomsNames.count()==1){
				ConstraintActivityPreferredRoom* c2=new ConstraintActivityPreferredRoom
				 (c->weight, c->compulsory, c->activityId, *(c->roomsNames.at(0)));

				QMessageBox::information(NULL, QObject::tr("FET information"), 
				 QObject::tr("The constraint\n%1 will be modified into constraint\n%2 because"
				 " there is only one room left in the constraint")
				 .arg(c->getDetailedDescription(*this))
				 .arg(c2->getDetailedDescription(*this)));

				this->removeSpaceConstraint(ctr);
				ctr=this->spaceConstraintsList.current();
				this->addSpaceConstraint(c2);
			}
			else
				ctr=this->spaceConstraintsList.next();
		}
		else
			ctr=this->spaceConstraintsList.next();

	this->roomsList.remove(i);
	this->internalStructureComputed=false;
	return true;
}

bool Rules::modifyRoom(const QString& initialRoomName, const QString& finalRoomName, const QString& type, int capacity)
{
	int i=this->searchRoom(initialRoomName);
	if(i<0)
		return false;

	Room* searchedRoom=this->roomsList.at(i);
	assert(searchedRoom->name==initialRoomName);

	//Removing useless ConstraintRoomTypeNotAllowedSubjects
	//Firstly, check whether the type of this room exists at another room
	bool typeExisting=false;
	for(Room *rm=this->roomsList.first(); rm; rm=this->roomsList.next())
		if(rm->name!=searchedRoom->name && rm->type==searchedRoom->type){
			typeExisting=true;
			break;
		}
	if(searchedRoom->type==type) //type unchanged, keep the constraints
		typeExisting=true;
	for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; )
		if(!typeExisting && ctr->type==CONSTRAINT_ROOM_TYPE_NOT_ALLOWED_SUBJECTS){
			ConstraintRoomTypeNotAllowedSubjects* crtas=(ConstraintRoomTypeNotAllowedSubjects*)ctr;
			if(crtas->roomType==searchedRoom->type){
				this->removeSpaceConstraint(ctr);
				ctr=this->spaceConstraintsList.current();
			}
			else
				ctr=this->spaceConstraintsList.next();
		}
		else if(ctr->type==CONSTRAINT_ROOM_NOT_AVAILABLE){
			ConstraintRoomNotAvailable* crna=(ConstraintRoomNotAvailable*)ctr;
			if(crna->roomName==initialRoomName)
				crna->roomName=finalRoomName;
			ctr=this->spaceConstraintsList.next();
		}
		else if(ctr->type==CONSTRAINT_TEACHER_REQUIRES_ROOM){
			ConstraintTeacherRequiresRoom* c=(ConstraintTeacherRequiresRoom*)ctr;
			if(c->roomName==initialRoomName)
				c->roomName=finalRoomName;
			ctr=this->spaceConstraintsList.next();
		}
		else if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_ROOM){
			ConstraintActivityPreferredRoom* c=(ConstraintActivityPreferredRoom*)ctr;
			if(c->roomName==initialRoomName)
				c->roomName=finalRoomName;
			ctr=this->spaceConstraintsList.next();
		}
		else if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_ROOMS){
			ConstraintActivityPreferredRooms* c=(ConstraintActivityPreferredRooms*)ctr;
			int t=0;
			for(QStringList::Iterator it=c->roomsNames.begin(); it!=c->roomsNames.end(); it++){
				if((*it)==initialRoomName){
					*it=finalRoomName;
					t++;
				}
			}
			assert(t<=1);
			ctr=this->spaceConstraintsList.next();
		}
		else
			ctr=this->spaceConstraintsList.next();

	searchedRoom->name=finalRoomName;
	searchedRoom->type=type;
	searchedRoom->capacity=capacity;

	this->internalStructureComputed=false;
	return true;
}

void Rules::sortRoomsAlphabetically()
{
	this->roomsList.sort();

	this->internalStructureComputed=false;
}

bool Rules::addTimeConstraint(TimeConstraint *ctr)
{
	bool ok=true;

	//TODO: improve this

	//check if this constraint is already added, for ConstraintActivityPreferredTime
	if(ctr->type==CONSTRAINT_ACTIVITY_PREFERRED_TIME){
		TimeConstraint *ctr2;
		for(ctr2=this->timeConstraintsList.first(); ctr2; ctr2=this->timeConstraintsList.next())
			if(
			 (ctr2->type==CONSTRAINT_ACTIVITY_PREFERRED_TIME) &&
			 (((ConstraintActivityPreferredTime*)ctr2)->activityId
			 ==
			 ((ConstraintActivityPreferredTime*)ctr)->activityId)
			)
				break;

		if(ctr2)
			ok=false;
	}

	//check if this constraint is already added, for ConstraintMinNDaysBetweenActivities
	if(ctr->type==CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES){
		TimeConstraint *ctr2;
		for(ctr2=this->timeConstraintsList.first(); ctr2; ctr2=this->timeConstraintsList.next())
			if(ctr2->type==CONSTRAINT_MIN_N_DAYS_BETWEEN_ACTIVITIES)
				if(
				 *((ConstraintMinNDaysBetweenActivities*)ctr2)
				 ==
				 *((ConstraintMinNDaysBetweenActivities*)ctr)
				 )
					break;

		if(ctr2)
			ok=false;
	}

	if(ok){
		this->timeConstraintsList.append(ctr);
		this->internalStructureComputed=false;
		return true;
	}
	else
		return false;
}

bool Rules::removeTimeConstraint(TimeConstraint *ctr)
{
	this->internalStructureComputed=false;
	return (this->timeConstraintsList.removeRef(ctr));
}

bool Rules::addSpaceConstraint(SpaceConstraint *ctr)
{
	bool ok=true;

	//TODO: check if this constraint is already added...(if any possibility of duplicates)

	if(ok){
		this->spaceConstraintsList.append(ctr);
		this->internalStructureComputed=false;
		return true;
	}
	else
		return false;
}

bool Rules::removeSpaceConstraint(SpaceConstraint *ctr)
{
	this->internalStructureComputed=false;
	return (this->spaceConstraintsList.removeRef(ctr));
}

bool Rules::read(const QString& filename)
{
	QFile file(filename);
	if(!file.open(IO_ReadOnly)){
		return false;
	}
	QDomDocument doc("xml_rules");
	if(!doc.setContent(&file)){
		file.close();
		cout<<"Could not open file - not existing or in use\n";
		return false;
	}
	file.close();

	//Clear old rules, initialize new rules
	if(this->initialized)
		this->kill();
	this->init();

	////////////////////////////////////////

	//logging part
	QString xmlReadingLog="";
	QString tmp=OUTPUT_DIR+FILE_SEP+XML_PARSING_LOG_FILENAME;
	QFile file2(tmp);
	if(!file2.open(IO_WriteOnly)){
		assert(0);
		exit(1);
	}
	QTextStream logStream(&file2);

	QDomElement elem1=doc.documentElement();
	xmlReadingLog+=" Found "+elem1.tagName()+" tag\n";
	bool okAbove3_12_17=true;
	bool warning=false;
	if(elem1.tagName()!="FET")
		okAbove3_12_17=false;
	else{
		QDomDocumentType dt=doc.doctype();
		if(dt.isNull() || dt.name()!="FET")
			okAbove3_12_17=false;
		else{
			QDomAttr a=elem1.attributeNode("version");
			QString version=a.value();
			int v[3];
			//cout<<"version=|"<<version<<"|"<<endl;
			int t=sscanf((const char*)(version), "%d.%d.%d", &v[0], &v[1], &v[2]);
			assert(t==3);

			int w[3];
			t=sscanf((const char*)(FET_VERSION), "%d.%d.%d", &w[0], &w[1], &w[2]);
			assert(t==3);
			
			if(v[0]>w[0] || (v[0]==w[0] && v[1]>w[1]) || (v[0]==w[0]&&v[1]==w[1]&&v[2]>w[2]))
				warning=true;
		}
	}
	if(!okAbove3_12_17){ //trying version 3.6.1 to version 3.12.16
		cout<<"Invalid fet 3.12.17 or above - trying older version"<<endl;
		if(elem1.tagName()!=INPUT_FILE_TAG_3_6_1){
			file2.close();
			return false;
		}
	}
	else if(warning){
		QMessageBox::warning(NULL, QObject::tr("FET information"), 
		 QObject::tr("Trying to open a newer file - please update your FET software to the latest version"));
	}
	
	this->nHoursPerDay=12;
	this->hoursOfTheDay[0]="0";
	this->hoursOfTheDay[1]="1";
	this->hoursOfTheDay[2]="2";
	this->hoursOfTheDay[3]="3";
	this->hoursOfTheDay[4]="4";
	this->hoursOfTheDay[5]="5";
	this->hoursOfTheDay[6]="6";
	this->hoursOfTheDay[7]="7";
	this->hoursOfTheDay[8]="8";
	this->hoursOfTheDay[9]="9";
	this->hoursOfTheDay[10]="10";
	this->hoursOfTheDay[11]="11";
	this->hoursOfTheDay[12]="12";

	this->nDaysPerWeek=5;
	this->daysOfTheWeek[0] = QObject::tr("Monday");
	this->daysOfTheWeek[1] = QObject::tr("Tuesday");
	this->daysOfTheWeek[2] = QObject::tr("Wednesday");
	this->daysOfTheWeek[3] = QObject::tr("Thursday");
	this->daysOfTheWeek[4] = QObject::tr("Friday");

	this->institutionName=QObject::tr("Default institution");
	this->comments=QObject::tr("Default comments");

	for(QDomNode node2=elem1.firstChild(); !node2.isNull(); node2=node2.nextSibling()){
		QDomElement elem2=node2.toElement();
		if(elem2.isNull())
			xmlReadingLog+="  Null node here\n";
		xmlReadingLog+="  Found "+elem2.tagName()+" tag\n";
		if(elem2.tagName()=="Institution_Name"){
			this->institutionName=elem2.text();
			xmlReadingLog+="  Found institution name="+this->institutionName+"\n";
		}
		else if(elem2.tagName()=="Comments"){
			this->comments=elem2.text();
			xmlReadingLog+="  Found comments="+this->comments+"\n";
		}
		else if(elem2.tagName()=="Hours_List"){
			int tmp=0;
			for(QDomNode node3=elem2.firstChild(); !node3.isNull(); node3=node3.nextSibling()){
				QDomElement elem3=node3.toElement();
				if(elem3.isNull()){
					xmlReadingLog+="   Null node here\n";
					continue;
				}
				xmlReadingLog+="   Found "+elem3.tagName()+" tag\n";
				if(elem3.tagName()=="Number"){
					this->nHoursPerDay=elem3.text().toInt();
					xmlReadingLog+="   Found the number of hours per day = "+
					 QString::number(this->nHoursPerDay)+"\n";
					assert(this->nHoursPerDay>0);
				}
				else if(elem3.tagName()=="Name"){
					this->hoursOfTheDay[tmp]=elem3.text();
					xmlReadingLog+="   Found hour "+this->hoursOfTheDay[tmp]+"\n";
					tmp++;
				}
			}
		}
		else if(elem2.tagName()=="Days_List"){
			int tmp=0;
			for(QDomNode node3=elem2.firstChild(); !node3.isNull(); node3=node3.nextSibling()){
				QDomElement elem3=node3.toElement();
				if(elem3.isNull()){
					xmlReadingLog+="   Null node here\n";
					continue;
				}
				xmlReadingLog+="   Found "+elem3.tagName()+" tag\n";
				if(elem3.tagName()=="Number"){
					this->nDaysPerWeek=elem3.text().toInt();
					xmlReadingLog+="   Found the number of days per week = "+
					 QString::number(this->nDaysPerWeek)+"\n";
					assert(this->nDaysPerWeek>0);
				}
				else if(elem3.tagName()=="Name"){
					this->daysOfTheWeek[tmp]=elem3.text();
					xmlReadingLog+="   Found day "+this->daysOfTheWeek[tmp]+"\n";
					tmp++;
				}
			}
			assert(tmp==this->nDaysPerWeek);
		}
		else if(elem2.tagName()=="Teachers_List"){
			int tmp=0;
			for(QDomNode node3=elem2.firstChild(); !node3.isNull(); node3=node3.nextSibling()){
				QDomElement elem3=node3.toElement();
				if(elem3.isNull()){
					xmlReadingLog+="   Null node here\n";
					continue;
				}
				xmlReadingLog+="   Found "+elem3.tagName()+" tag\n";
				if(elem3.tagName()=="Teacher"){
					Teacher* teacher=new Teacher();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Name"){
							teacher->name=elem4.text();
							xmlReadingLog+="    Read teacher name: "+teacher->name+"\n";
						}
					}
					bool tmp2=this->addTeacher(teacher);
					assert(tmp2==true);
					tmp++;
					xmlReadingLog+="   Teacher added\n";
				}
			}
			assert((uint)(tmp)==this->teachersList.count());
			xmlReadingLog+="  Added "+QString::number(tmp)+" teachers\n";
		}
		else if(elem2.tagName()=="Subjects_List"){
			int tmp=0;
			for(QDomNode node3=elem2.firstChild(); !node3.isNull(); node3=node3.nextSibling()){
				QDomElement elem3=node3.toElement();
				if(elem3.isNull()){
					xmlReadingLog+="   Null node here\n";
					continue;
				}
				xmlReadingLog+="   Found "+elem3.tagName()+" tag\n";
				if(elem3.tagName()=="Subject"){
					Subject* subject=new Subject();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Name"){
							subject->name=elem4.text();
							xmlReadingLog+="    Read subject name: "+subject->name+"\n";
						}
					}
					bool tmp2=this->addSubject(subject);
					assert(tmp2==true);
					tmp++;
					xmlReadingLog+="   Subject inserted\n";
				}
			}
			assert((uint)(tmp)==this->subjectsList.count());
			xmlReadingLog+="  Added "+QString::number(tmp)+" subjects\n";
		}
		else if(elem2.tagName()=="Subject_Tags_List"){
			int tmp=0;
			for(QDomNode node3=elem2.firstChild(); !node3.isNull(); node3=node3.nextSibling()){
				QDomElement elem3=node3.toElement();
				if(elem3.isNull()){
					xmlReadingLog+="   Null node here\n";
					continue;
				}
				xmlReadingLog+="   Found "+elem3.tagName()+" tag\n";
				if(elem3.tagName()=="Subject_Tag"){
					SubjectTag* subjectTag=new SubjectTag();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Name"){
							subjectTag->name=elem4.text();
							xmlReadingLog+="    Read subject tag name: "+subjectTag->name+"\n";
						}
					}
					bool tmp2=this->addSubjectTag(subjectTag);
					assert(tmp2==true);
					tmp++;
					xmlReadingLog+="   Subject tag inserted\n";
				}
			}
			assert((uint)(tmp)==this->subjectTagsList.count());
			xmlReadingLog+="  Added "+QString::number(tmp)+" subject tags\n";
		}
		else if(elem2.tagName()=="Students_List"){
			int ny=0;
			for(QDomNode node3=elem2.firstChild(); !node3.isNull(); node3=node3.nextSibling()){
				QDomElement elem3=node3.toElement();
				if(elem3.isNull()){
					xmlReadingLog+="   Null node here\n";
					continue;
				}
				xmlReadingLog+="   Found "+elem3.tagName()+" tag\n";
				if(elem3.tagName()=="Year"){
					StudentsYear* sty=new StudentsYear();
					int ng=0;

					bool tmp2=this->addYear(sty);
					assert(tmp2==true);
					ny++;

					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Name"){
							sty->name=elem4.text();
							xmlReadingLog+="    Read year name: "+sty->name+"\n";
						}
						else if(elem4.tagName()=="Number_of_Students"){
							sty->numberOfStudents=elem4.text().toInt();
							xmlReadingLog+="    Read year number of students: "+QString::number(sty->numberOfStudents)+"\n";
						}
						else if(elem4.tagName()=="Group"){
							StudentsGroup* stg=new StudentsGroup();
							int ns=0;

							bool tmp4=this->addGroup(sty->name, stg);
							assert(tmp4==true);
							ng++;

							for(QDomNode node5=elem4.firstChild(); !node5.isNull(); node5=node5.nextSibling()){
								QDomElement elem5=node5.toElement();
								if(elem5.isNull()){
									xmlReadingLog+="     Null node here\n";
									continue;
								}
								xmlReadingLog+="     Found "+elem5.tagName()+" tag\n";
								if(elem5.tagName()=="Name"){
									stg->name=elem5.text();
									xmlReadingLog+="     Read group name: "+stg->name+"\n";
								}
								else if(elem5.tagName()=="Number_of_Students"){
									stg->numberOfStudents=elem5.text().toInt();
									xmlReadingLog+="     Read group number of students: "+QString::number(stg->numberOfStudents)+"\n";
								}
								else if(elem5.tagName()=="Subgroup"){
									StudentsSubgroup* sts=new StudentsSubgroup();

									bool tmp6=this->addSubgroup(sty->name, stg->name, sts);
									assert(tmp6==true);
									ns++;

									for(QDomNode node6=elem5.firstChild(); !node6.isNull(); node6=node6.nextSibling()){
										QDomElement elem6=node6.toElement();
										if(elem6.isNull()){
											xmlReadingLog+="     Null node here\n";
											continue;
										}
										xmlReadingLog+="     Found "+elem6.tagName()+" tag\n";
										if(elem6.tagName()=="Name"){
											sts->name=elem6.text();
											xmlReadingLog+="     Read subgroup name: "+sts->name+"\n";
										}
										else if(elem6.tagName()=="Number_of_Students"){
											sts->numberOfStudents=elem6.text().toInt();
											xmlReadingLog+="     Read subgroup number of students: "+QString::number(sts->numberOfStudents)+"\n";
										}
									}
								}
							}
							assert((uint)(ns) == stg->subgroupsList.count());
							xmlReadingLog+="    Added "+QString::number(ns)+" subgroups\n";
						}
					}
					assert((uint)(ng) == sty->groupsList.count());
					xmlReadingLog+="   Added "+QString::number(ng)+" groups\n";
				}
			}
			xmlReadingLog+="  Added "+QString::number(ny)+" years\n";
			assert(this->yearsList.count()==(uint)(ny));
		}
		else if(elem2.tagName()=="Activities_List"){
			int na=0;
			for(QDomNode node3=elem2.firstChild(); !node3.isNull(); node3=node3.nextSibling()){
				QDomElement elem3=node3.toElement();
				if(elem3.isNull()){
					xmlReadingLog+="   Null node here\n";
					continue;
				}
				xmlReadingLog+="   Found "+elem3.tagName()+" tag\n";
				if(elem3.tagName()=="Activity"){
					bool correct=true;
				
					QString tn="";
					QStringList tl;
					QString sjn="";
					QString stgn="";
					QString stn="";
					QStringList stl;
					int p=PARITY_NOT_INITIALIZED;
					int td=-1;
					int d=-1;
					int id=-1;
					int gid=-1;
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="   Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weekly"){
							xmlReadingLog+="    Current activity is weekly\n";
							assert(p==PARITY_NOT_INITIALIZED);
							p=PARITY_WEEKLY;
						}
						else if(elem4.tagName()=="Biweekly"){
							xmlReadingLog+="    Current activity is biweekly\n";
							assert(p==PARITY_NOT_INITIALIZED);
							p=PARITY_BIWEEKLY;
						}
						else if(elem4.tagName()=="Teacher"){
							tn=elem4.text();
							xmlReadingLog+="    Crt. activity teacher="+tn+"\n";
							tl.append(tn);
							if(this->searchTeacher(tn)<0)
								correct=false;
						}
						else if(elem4.tagName()=="Subject"){
							sjn=elem4.text();
							xmlReadingLog+="    Crt. activity subject="+sjn+"\n";
							if(this->searchSubject(sjn)<0)
								correct=false;
						}
						else if(elem4.tagName()=="Subject_Tag"){
							stgn=elem4.text();
							xmlReadingLog+="    Crt. activity subject tag="+stgn+"\n";
							if(this->searchSubjectTag(stgn)<0 && stgn!="")
								correct=false;
						}
						else if(elem4.tagName()=="Students"){
							stn=elem4.text();
							xmlReadingLog+="    Crt. activity students+="+stn+"\n";
							stl.append(stn);
							if(this->searchStudentsSet(stn)==NULL)
								correct=false;
						}
						else if(elem4.tagName()=="Duration"){
							d=elem4.text().toInt();
							xmlReadingLog+="    Crt. activity duration="+QString::number(d)+"\n";
						}
						else if(elem4.tagName()=="Total_Duration"){
							td=elem4.text().toInt();
							xmlReadingLog+="    Crt. activity total duration="+QString::number(td)+"\n";
						}
						else if(elem4.tagName()=="Id"){
							id=elem4.text().toInt();
							xmlReadingLog+="    Crt. activity id="+QString::number(id)+"\n";
						}
						else if(elem4.tagName()=="Activity_Group_Id"){
							gid=elem4.text().toInt();
							xmlReadingLog+="    Crt. activity group id="+QString::number(gid)+"\n";
						}
					}
					if(correct){
						assert(id>=0 && gid>=0);
						assert(d>0);
						if(td<0)
							td=d;
						this->addSimpleActivity(id, gid, tl, sjn, stgn, stl,
							d, td, p, -1, -1);
						na++;
						xmlReadingLog+="   Added the activity\n";
					}
					else{
						xmlReadingLog+="   Activity with id ="+QString::number(id)+" contains invalid data - skipping\n";
						QMessageBox::warning(NULL, QObject::tr("FET information"), 
						 QObject::tr("Activity with id=%1 contains invalid data - skipping").arg(id));
					}
				}
			}
			xmlReadingLog+="  Added "+QString::number(na)+" activities\n";
		}
		else if(elem2.tagName()=="Equipments_List"){
			int tmp=0;
			for(QDomNode node3=elem2.firstChild(); !node3.isNull(); node3=node3.nextSibling()){
				QDomElement elem3=node3.toElement();
				if(elem3.isNull()){
					xmlReadingLog+="   Null node here\n";
					continue;
				}
				xmlReadingLog+="   Found "+elem3.tagName()+" tag\n";
				if(elem3.tagName()=="Equipment"){
					Equipment* eq=new Equipment();
					eq->name="";
					
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Name"){
							eq->name=elem4.text();
							xmlReadingLog+="    Read equipment name: "+eq->name+"\n";
						}
					}
					bool tmp2=this->addEquipment(eq);
					assert(tmp2==true);
					tmp++;
					xmlReadingLog+="   Equipment added\n";
				}
			}
			assert((uint)(tmp)==this->equipmentsList.count());
			xmlReadingLog+="  Added "+QString::number(tmp)+" equipments\n";
		}
		else if(elem2.tagName()=="Rooms_List"){
			int tmp=0;
			for(QDomNode node3=elem2.firstChild(); !node3.isNull(); node3=node3.nextSibling()){
				QDomElement elem3=node3.toElement();
				if(elem3.isNull()){
					xmlReadingLog+="   Null node here\n";
					continue;
				}
				xmlReadingLog+="   Found "+elem3.tagName()+" tag\n";
				if(elem3.tagName()=="Room"){
					Room* rm=new Room();
					rm->name="";
					rm->type="";
					rm->capacity=1000; //infinite, if not specified
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Name"){
							rm->name=elem4.text();
							xmlReadingLog+="    Read room name: "+rm->name+"\n";
						}
						else if(elem4.tagName()=="Type"){
							rm->type=elem4.text();
							xmlReadingLog+="    Read room type: "+rm->type+"\n";
						}
						else if(elem4.tagName()=="Capacity"){
							rm->capacity=elem4.text().toInt();
							xmlReadingLog+="    Read room capacity: "+QString::number(rm->capacity)+"\n";
						}
						else if(elem4.tagName()=="Equipment"){
							rm->addEquipment(elem4.text());
							xmlReadingLog+="    Read room equipment: "+elem4.text()+"\n";
						}
					}
					bool tmp2=this->addRoom(rm);
					if(!tmp2){
						cout<<"Duplicate room - "<<rm->name<<endl;
						assert(0);
						exit(1);
					}
					assert(tmp2==true);
					tmp++;
					xmlReadingLog+="   Room added\n";
				}
			}
			assert((uint)(tmp)==this->roomsList.count());
			xmlReadingLog+="  Added "+QString::number(tmp)+" rooms\n";
		}
		else if(elem2.tagName()=="Time_Constraints_List"){
			int nc=0;
			TimeConstraint *crt_constraint;
			for(QDomNode node3=elem2.firstChild(); !node3.isNull(); node3=node3.nextSibling()){
				crt_constraint=NULL;
				QDomElement elem3=node3.toElement();
				if(elem3.isNull()){
					xmlReadingLog+="   Null node here\n";
					continue;
				}
				xmlReadingLog+="   Found "+elem3.tagName()+" tag\n";
				if(elem3.tagName()=="ConstraintBasicCompulsoryTime"){
					ConstraintBasicCompulsoryTime* cn=new ConstraintBasicCompulsoryTime();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintTeacherNotAvailable"){
					ConstraintTeacherNotAvailable* cn=new ConstraintTeacherNotAvailable();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Teacher_Name"){
							cn->teacherName=elem4.text();
							xmlReadingLog+="    Read teacher name="+cn->teacherName+"\n";
						}
						else if(elem4.tagName()=="Day"){
							for(cn->d=0; cn->d<this->nDaysPerWeek; cn->d++)
								if(this->daysOfTheWeek[cn->d]==elem4.text())
									break;
							assert(cn->d<this->nDaysPerWeek);
							xmlReadingLog+="    Crt. day="+this->daysOfTheWeek[cn->d]+"\n";
						}
						else if(elem4.tagName()=="Start_Hour"){
							for(cn->h1=0; cn->h1 < this->nHoursPerDay; cn->h1++)
								if(this->hoursOfTheDay[cn->h1]==elem4.text())
									break;
							assert(cn->h1>=0 && cn->h1 < this->nHoursPerDay);
							xmlReadingLog+="    Start hour="+this->hoursOfTheDay[cn->h1]+"\n";
						}
						else if(elem4.tagName()=="End_Hour"){
							for(cn->h2=0; cn->h2 < this->nHoursPerDay; cn->h2++)
								if(this->hoursOfTheDay[cn->h2]==elem4.text())
									break;
							assert(cn->h2>0 && cn->h2 <= this->nHoursPerDay);
							xmlReadingLog+="    End hour="+this->hoursOfTheDay[cn->h2]+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintTeacherMaxDaysPerWeek"){
					ConstraintTeacherMaxDaysPerWeek* cn=new ConstraintTeacherMaxDaysPerWeek();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Teacher_Name"){
							cn->teacher=elem4.text();
							xmlReadingLog+="    Read teacher name="+cn->teacher+"\n";
						}
						else if(elem4.tagName()=="Max_Days_Per_Week"){
							cn->maxDaysPerWeek=elem4.text().toInt();
							assert(cn->maxDaysPerWeek>0 && cn->maxDaysPerWeek <= this->nDaysPerWeek);
							xmlReadingLog+="    Max. days per week="+QString::number(cn->maxDaysPerWeek)+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintStudentsSetNotAvailable"){
					ConstraintStudentsSetNotAvailable* cn=new ConstraintStudentsSetNotAvailable();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Day"){
							for(cn->d=0; cn->d<this->nDaysPerWeek; cn->d++)
								if(this->daysOfTheWeek[cn->d]==elem4.text())
									break;
							assert(cn->d<this->nDaysPerWeek);
							xmlReadingLog+="    Crt. day="+this->daysOfTheWeek[cn->d]+"\n";
						}
						else if(elem4.tagName()=="Start_Hour"){
							for(cn->h1=0; cn->h1 < this->nHoursPerDay; cn->h1++)
								if(this->hoursOfTheDay[cn->h1]==elem4.text())
									break;
							assert(cn->h1>=0 && cn->h1 < this->nHoursPerDay);
							xmlReadingLog+="    Start hour="+this->hoursOfTheDay[cn->h1]+"\n";
						}
						else if(elem4.tagName()=="End_Hour"){
							for(cn->h2=0; cn->h2 < this->nHoursPerDay; cn->h2++)
								if(this->hoursOfTheDay[cn->h2]==elem4.text())
									break;
							assert(cn->h2>0 && cn->h2 <= this->nHoursPerDay);
							xmlReadingLog+="    End hour="+this->hoursOfTheDay[cn->h2]+"\n";
						}
						else if(elem4.tagName()=="Students"){
							cn->students=elem4.text();
							xmlReadingLog+="    Read students name="+cn->students+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintMinNDaysBetweenActivities"){
					ConstraintMinNDaysBetweenActivities* cn=new ConstraintMinNDaysBetweenActivities();
					int n_act=0;
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Number_of_Activities"){
							cn->n_activities=elem4.text().toInt();
							xmlReadingLog+="    Read n_activities="+QString::number(cn->n_activities)+"\n";
						}
						else if(elem4.tagName()=="Activity_Id"){
							cn->activitiesId[n_act]=elem4.text().toInt();
							xmlReadingLog+="    Read activity id="+QString::number(cn->activitiesId[n_act])+"\n";
							n_act++;
						}
						else if(elem4.tagName()=="MinDays"){
							cn->minDays=elem4.text().toInt();
							xmlReadingLog+="    Read MinDays="+QString::number(cn->minDays)+"\n";
						}
					}
					assert(n_act==cn->n_activities);
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintActivitiesNotOverlapping"){
					ConstraintActivitiesNotOverlapping* cn=new ConstraintActivitiesNotOverlapping();
					int n_act=0;
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Number_of_Activities"){
							cn->n_activities=elem4.text().toInt();
							xmlReadingLog+="    Read n_activities="+QString::number(cn->n_activities)+"\n";
						}
						else if(elem4.tagName()=="Activity_Id"){
							cn->activitiesId[n_act]=elem4.text().toInt();
							xmlReadingLog+="    Read activity id="+QString::number(cn->activitiesId[n_act])+"\n";
							n_act++;
						}
					}
					assert(n_act==cn->n_activities);
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintActivitiesSameStartingTime"){
					ConstraintActivitiesSameStartingTime* cn=new ConstraintActivitiesSameStartingTime();
					int n_act=0;
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Number_of_Activities"){
							cn->n_activities=elem4.text().toInt();
							xmlReadingLog+="    Read n_activities="+QString::number(cn->n_activities)+"\n";
						}
						else if(elem4.tagName()=="Activity_Id"){
							cn->activitiesId[n_act]=elem4.text().toInt();
							xmlReadingLog+="    Read activity id="+QString::number(cn->activitiesId[n_act])+"\n";
							n_act++;
						}
					}
					assert(cn->n_activities==n_act);
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintActivitiesSameStartingHour"){
					ConstraintActivitiesSameStartingHour* cn=new ConstraintActivitiesSameStartingHour();
					int n_act=0;
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Number_of_Activities"){
							cn->n_activities=elem4.text().toInt();
							xmlReadingLog+="    Read n_activities="+QString::number(cn->n_activities)+"\n";
						}
						else if(elem4.tagName()=="Activity_Id"){
							cn->activitiesId[n_act]=elem4.text().toInt();
							xmlReadingLog+="    Read activity id="+QString::number(cn->activitiesId[n_act])+"\n";
							n_act++;
						}
					}
					assert(cn->n_activities==n_act);
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintTeachersNoMoreThanXHoursContinuously"){
					ConstraintTeachersNoMoreThanXHoursContinuously* cn=new ConstraintTeachersNoMoreThanXHoursContinuously();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Maximum_Hours_Continuously"){
							cn->maxHoursContinuously=elem4.text().toInt();
							xmlReadingLog+="    Read maxHoursContinuously="+QString::number(cn->maxHoursContinuously)+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintTeachersNoMoreThanXHoursDaily"){
					ConstraintTeachersNoMoreThanXHoursDaily* cn=new ConstraintTeachersNoMoreThanXHoursDaily();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Maximum_Hours_Daily"){
							cn->maxHoursDaily=elem4.text().toInt();
							xmlReadingLog+="    Read maxHoursDaily="+QString::number(cn->maxHoursDaily)+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintTeachersSubgroupsNoMoreThanXHoursDaily"){
					ConstraintTeachersSubgroupsNoMoreThanXHoursDaily* cn=new ConstraintTeachersSubgroupsNoMoreThanXHoursDaily();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Maximum_Hours_Daily"){
							cn->maxHoursDaily=elem4.text().toInt();
							xmlReadingLog+="    Read maxHoursDaily="+QString::number(cn->maxHoursDaily)+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintStudentsNHoursDaily"){
					ConstraintStudentsNHoursDaily* cn=new ConstraintStudentsNHoursDaily();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="MaxHoursDaily"){
							cn->maxHoursDaily=elem4.text().toInt();
							xmlReadingLog+="    Read maxHoursDaily="+QString::number(cn->maxHoursDaily)+"\n";
						}
						else if(elem4.tagName()=="MinHoursDaily"){
							cn->minHoursDaily=elem4.text().toInt();
							xmlReadingLog+="    Read minHoursDaily="+QString::number(cn->minHoursDaily)+"\n";
						}
					}
					assert(cn->maxHoursDaily>=0 || cn->minHoursDaily>=0);
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintStudentsSetNHoursDaily"){
					ConstraintStudentsSetNHoursDaily* cn=new ConstraintStudentsSetNHoursDaily();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="MaxHoursDaily"){
							cn->maxHoursDaily=elem4.text().toInt();
							xmlReadingLog+="    Read maxHoursDaily="+QString::number(cn->maxHoursDaily)+"\n";
						}
						else if(elem4.tagName()=="MinHoursDaily"){
							cn->minHoursDaily=elem4.text().toInt();
							xmlReadingLog+="    Read minHoursDaily="+QString::number(cn->minHoursDaily)+"\n";
						}
						else if(elem4.tagName()=="Students"){
							cn->students=elem4.text();
							xmlReadingLog+="    Read students name="+cn->students+"\n";
						}
					}
					assert(cn->maxHoursDaily>=0 || cn->minHoursDaily>=0);
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintActivityPreferredTime"){
					ConstraintActivityPreferredTime* cn=new ConstraintActivityPreferredTime();
					cn->day = cn->hour = -1;
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Activity_Id"){
							cn->activityId=elem4.text().toInt();
							xmlReadingLog+="    Read activity id="+QString::number(cn->activityId)+"\n";
						}
						else if(elem4.tagName()=="Preferred_Day"){
							for(cn->day=0; cn->day<this->nDaysPerWeek; cn->day++)
								if(this->daysOfTheWeek[cn->day]==elem4.text())
									break;
							assert(cn->day<this->nDaysPerWeek);
							xmlReadingLog+="    Preferred day="+this->daysOfTheWeek[cn->day]+"\n";
						}
						else if(elem4.tagName()=="Preferred_Hour"){
							for(cn->hour=0; cn->hour < this->nHoursPerDay; cn->hour++)
								if(this->hoursOfTheDay[cn->hour]==elem4.text())
									break;
							assert(cn->hour>=0 && cn->hour < this->nHoursPerDay);
							xmlReadingLog+="    Preferred hour="+this->hoursOfTheDay[cn->hour]+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintActivityEndsDay"){
					ConstraintActivityEndsDay* cn=new ConstraintActivityEndsDay();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Activity_Id"){
							cn->activityId=elem4.text().toInt();
							xmlReadingLog+="    Read activity id="+QString::number(cn->activityId)+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintActivityPreferredTimes"){
					ConstraintActivityPreferredTimes* cn=new ConstraintActivityPreferredTimes();
					cn->nPreferredTimes=0;
					int i;
					for(i=0; i<MAX_N_CONSTRAINT_ACTIVITY_PREFERRED_TIMES; i++){
						cn->days[i] = cn->hours[i] = -1;
					}
					i=0;
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Activity_Id"){
							cn->activityId=elem4.text().toInt();
							xmlReadingLog+="    Read activity id="+QString::number(cn->activityId)+"\n";
						}
						else if(elem4.tagName()=="Number_of_Preferred_Times"){
							cn->nPreferredTimes=elem4.text().toInt();
							xmlReadingLog+="    Read number of preferred times="+QString::number(cn->nPreferredTimes)+"\n";
						}
						else if(elem4.tagName()=="Preferred_Time"){
							xmlReadingLog+="    Read: preferred time\n";

							for(QDomNode node5=elem4.firstChild(); !node5.isNull(); node5=node5.nextSibling()){
								QDomElement elem5=node5.toElement();
								if(elem5.isNull()){
									xmlReadingLog+="    Null node here\n";
									continue;
								}
								xmlReadingLog+="    Found "+elem5.tagName()+" tag\n";
								if(elem5.tagName()=="Preferred_Day"){
									for(cn->days[i]=0; cn->days[i]<this->nDaysPerWeek; cn->days[i]++)
										if(this->daysOfTheWeek[cn->days[i]]==elem5.text())
											break;
									assert(cn->days[i]<this->nDaysPerWeek);
									xmlReadingLog+="    Preferred day="+this->daysOfTheWeek[cn->days[i]]+"("+QString::number(i)+")"+"\n";
								}
								else if(elem5.tagName()=="Preferred_Hour"){
									for(cn->hours[i]=0; cn->hours[i] < this->nHoursPerDay; cn->hours[i]++)
										if(this->hoursOfTheDay[cn->hours[i]]==elem5.text())
											break;
									assert(cn->hours[i]>=0 && cn->hours[i] < this->nHoursPerDay);
									xmlReadingLog+="    Preferred hour="+this->hoursOfTheDay[cn->hours[i]]+"\n";
								}
							}

							i++;
						}
					}
					assert(i==cn->nPreferredTimes);
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintBreak"){
					ConstraintBreak* cn=new ConstraintBreak();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Day"){
							for(cn->d=0; cn->d<this->nDaysPerWeek; cn->d++)
								if(this->daysOfTheWeek[cn->d]==elem4.text())
									break;
							assert(cn->d<this->nDaysPerWeek);
							xmlReadingLog+="    Crt. day="+this->daysOfTheWeek[cn->d]+"\n";
						}
						else if(elem4.tagName()=="Start_Hour"){
							for(cn->h1=0; cn->h1 < this->nHoursPerDay; cn->h1++)
								if(this->hoursOfTheDay[cn->h1]==elem4.text())
									break;
							assert(cn->h1>=0 && cn->h1 < this->nHoursPerDay);
							xmlReadingLog+="    Start hour="+this->hoursOfTheDay[cn->h1]+"\n";
						}
						else if(elem4.tagName()=="End_Hour"){
							for(cn->h2=0; cn->h2 < this->nHoursPerDay; cn->h2++)
								if(this->hoursOfTheDay[cn->h2]==elem4.text())
									break;
							assert(cn->h2>0 && cn->h2 <= this->nHoursPerDay);
							xmlReadingLog+="    End hour="+this->hoursOfTheDay[cn->h2]+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintTeachersNoGaps"){
					ConstraintTeachersNoGaps* cn=new ConstraintTeachersNoGaps();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintStudentsNoGaps"){
					ConstraintStudentsNoGaps* cn=new ConstraintStudentsNoGaps();
					bool compulsory_read=false;
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
							compulsory_read=true;
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintStudentsSetNoGaps"){
					ConstraintStudentsSetNoGaps* cn=new ConstraintStudentsSetNoGaps();
					bool compulsory_read=false;
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
							compulsory_read=true;
						}
						else if(elem4.tagName()=="Students"){
							cn->students=elem4.text();
							xmlReadingLog+="    Read students name="+cn->students+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintStudentsEarly"){
					ConstraintStudentsEarly* cn=new ConstraintStudentsEarly();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintStudentsSetIntervalMaxDaysPerWeek"){
					ConstraintStudentsSetIntervalMaxDaysPerWeek* cn=new ConstraintStudentsSetIntervalMaxDaysPerWeek();
					assert(cn!=NULL);
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Start_Hour"){
							for(cn->h1=0; cn->h1 < this->nHoursPerDay; cn->h1++)
								if(this->hoursOfTheDay[cn->h1]==elem4.text())
									break;
							assert(cn->h1>=0 && cn->h1 < this->nHoursPerDay);
							xmlReadingLog+="    Start hour="+this->hoursOfTheDay[cn->h1]+"\n";
						}
						else if(elem4.tagName()=="End_Hour"){
							for(cn->h2=0; cn->h2 < this->nHoursPerDay; cn->h2++)
								if(this->hoursOfTheDay[cn->h2]==elem4.text())
									break;
							assert(cn->h2>0 && cn->h2 <= this->nHoursPerDay);
							xmlReadingLog+="    End hour="+this->hoursOfTheDay[cn->h2]+"\n";
						}
						else if(elem4.tagName()=="Students"){
							cn->students=elem4.text();
							xmlReadingLog+="    Read students name="+cn->students+"\n";
						}
						if(elem4.tagName()=="Max_Intervals"){
							cn->n=elem4.text().toInt();
							xmlReadingLog+="    Adding max intervals="+QString::number(cn->n)+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="Constraint2ActivitiesConsecutive"){
					Constraint2ActivitiesConsecutive* cn=new Constraint2ActivitiesConsecutive();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="First_Activity_Id"){
							cn->firstActivityId=elem4.text().toInt();
							xmlReadingLog+="    Read first activity id="+QString::number(cn->firstActivityId)+"\n";
						}
						else if(elem4.tagName()=="Second_Activity_Id"){
							cn->secondActivityId=elem4.text().toInt();
							xmlReadingLog+="    Read second activity id="+QString::number(cn->secondActivityId)+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="Constraint2ActivitiesGrouped"){
					Constraint2ActivitiesGrouped* cn=new Constraint2ActivitiesGrouped();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="First_Activity_Id"){
							cn->firstActivityId=elem4.text().toInt();
							xmlReadingLog+="    Read first activity id="+QString::number(cn->firstActivityId)+"\n";
						}
						else if(elem4.tagName()=="Second_Activity_Id"){
							cn->secondActivityId=elem4.text().toInt();
							xmlReadingLog+="    Read second activity id="+QString::number(cn->secondActivityId)+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintActivitiesPreferredTimes"){
					ConstraintActivitiesPreferredTimes* cn=new ConstraintActivitiesPreferredTimes();
					cn->nPreferredTimes=0;
					int i;
					for(i=0; i<MAX_N_CONSTRAINT_ACTIVITIES_PREFERRED_TIMES; i++){
						cn->days[i] = cn->hours[i] = -1;
					}
					cn->teacherName="";
					cn->studentsName="";
					cn->subjectName="";
					
					i=0;
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Teacher_Name"){
							cn->teacherName=elem4.text();
							xmlReadingLog+="    Read teacher name="+cn->teacherName+"\n";
						}
						else if(elem4.tagName()=="Students_Name"){
							cn->studentsName=elem4.text();
							xmlReadingLog+="    Read students name="+cn->studentsName+"\n";
						}
						else if(elem4.tagName()=="Subject_Name"){
							cn->subjectName=elem4.text();
							xmlReadingLog+="    Read subject name="+cn->subjectName+"\n";
						}
						else if(elem4.tagName()=="Number_of_Preferred_Times"){
							cn->nPreferredTimes=elem4.text().toInt();
							xmlReadingLog+="    Read number of preferred times="+QString::number(cn->nPreferredTimes)+"\n";
						}
						else if(elem4.tagName()=="Preferred_Time"){
							xmlReadingLog+="    Read: preferred time\n";

							for(QDomNode node5=elem4.firstChild(); !node5.isNull(); node5=node5.nextSibling()){
								QDomElement elem5=node5.toElement();
								if(elem5.isNull()){
									xmlReadingLog+="    Null node here\n";
									continue;
								}
								xmlReadingLog+="    Found "+elem5.tagName()+" tag\n";
								if(elem5.tagName()=="Preferred_Day"){
									for(cn->days[i]=0; cn->days[i]<this->nDaysPerWeek; cn->days[i]++)
										if(this->daysOfTheWeek[cn->days[i]]==elem5.text())
											break;
									assert(cn->days[i]<this->nDaysPerWeek);
									xmlReadingLog+="    Preferred day="+this->daysOfTheWeek[cn->days[i]]+"("+QString::number(i)+")"+"\n";
								}
								else if(elem5.tagName()=="Preferred_Hour"){
									for(cn->hours[i]=0; cn->hours[i] < this->nHoursPerDay; cn->hours[i]++)
										if(this->hoursOfTheDay[cn->hours[i]]==elem5.text())
											break;
									assert(cn->hours[i]>=0 && cn->hours[i] < this->nHoursPerDay);
									xmlReadingLog+="    Preferred hour="+this->hoursOfTheDay[cn->hours[i]]+"\n";
								}
							}

							i++;
						}
					}
					assert(i==cn->nPreferredTimes);
					crt_constraint=cn;
				}


				assert(crt_constraint!=NULL);
				bool tmp=this->addTimeConstraint(crt_constraint);
				assert(tmp);
				nc++;
			}
			xmlReadingLog+="  Added "+QString::number(nc)+" time constraints\n";
		}
		else if(elem2.tagName()=="Space_Constraints_List"){
			int nc=0;
			SpaceConstraint *crt_constraint;
			for(QDomNode node3=elem2.firstChild(); !node3.isNull(); node3=node3.nextSibling()){
				crt_constraint=NULL;
				QDomElement elem3=node3.toElement();
				if(elem3.isNull()){
					xmlReadingLog+="   Null node here\n";
					continue;
				}
				xmlReadingLog+="   Found "+elem3.tagName()+" tag\n";
				if(elem3.tagName()=="ConstraintBasicCompulsorySpace"){
					ConstraintBasicCompulsorySpace* cn=new ConstraintBasicCompulsorySpace();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintRoomNotAvailable"){
					ConstraintRoomNotAvailable* cn=new ConstraintRoomNotAvailable();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Room_Name"){
							cn->roomName=elem4.text();
							xmlReadingLog+="    Read room name="+cn->roomName+"\n";
						}
						else if(elem4.tagName()=="Day"){
							for(cn->d=0; cn->d<this->nDaysPerWeek; cn->d++)
								if(this->daysOfTheWeek[cn->d]==elem4.text())
									break;
							assert(cn->d<this->nDaysPerWeek);
							xmlReadingLog+="    Crt. day="+this->daysOfTheWeek[cn->d]+"\n";
						}
						else if(elem4.tagName()=="Start_Hour"){
							for(cn->h1=0; cn->h1 < this->nHoursPerDay; cn->h1++)
								if(this->hoursOfTheDay[cn->h1]==elem4.text())
									break;
							assert(cn->h1>=0 && cn->h1 < this->nHoursPerDay);
							xmlReadingLog+="    Start hour="+this->hoursOfTheDay[cn->h1]+"\n";
						}
						else if(elem4.tagName()=="End_Hour"){
							for(cn->h2=0; cn->h2 < this->nHoursPerDay; cn->h2++)
								if(this->hoursOfTheDay[cn->h2]==elem4.text())
									break;
							assert(cn->h2>0 && cn->h2 <= this->nHoursPerDay);
							xmlReadingLog+="    End hour="+this->hoursOfTheDay[cn->h2]+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintRoomTypeNotAllowedSubjects"){
					ConstraintRoomTypeNotAllowedSubjects* cn=new ConstraintRoomTypeNotAllowedSubjects();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Room_Type"){
							cn->roomType=elem4.text();
							xmlReadingLog+="    Read room type="+cn->roomType+"\n";
						}
						else if(elem4.tagName()=="Subject"){
							cn->addNotAllowedSubject(elem4.text());
							xmlReadingLog+="    Read not allowed subject="+elem4.text()+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintSubjectRequiresEquipments"){
					ConstraintSubjectRequiresEquipments* cn=new ConstraintSubjectRequiresEquipments();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Subject"){
							cn->subjectName=elem4.text();
							xmlReadingLog+="    Read subject="+cn->subjectName+"\n";
						}
						else if(elem4.tagName()=="Equipment"){
							cn->addRequiredEquipment(elem4.text());
							xmlReadingLog+="    Read required equipment="+elem4.text()+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintSubjectSubjectTagRequireEquipments"){
					ConstraintSubjectSubjectTagRequireEquipments* cn=new ConstraintSubjectSubjectTagRequireEquipments();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Subject"){
							cn->subjectName=elem4.text();
							xmlReadingLog+="    Read subject="+cn->subjectName+"\n";
						}
						else if(elem4.tagName()=="Subject_Tag"){
							cn->subjectTagName=elem4.text();
							xmlReadingLog+="    Read subject tag="+cn->subjectTagName+"\n";
						}
						else if(elem4.tagName()=="Equipment"){
							cn->addRequiredEquipment(elem4.text());
							xmlReadingLog+="    Read required equipment="+elem4.text()+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintTeacherRequiresRoom"){
					ConstraintTeacherRequiresRoom* cn=new ConstraintTeacherRequiresRoom();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Teacher"){
							cn->teacherName=elem4.text();
							xmlReadingLog+="    Read teacher="+cn->teacherName+"\n";
						}
						else if(elem4.tagName()=="Room"){
							cn->roomName=elem4.text();
							xmlReadingLog+="    Read room="+elem4.text()+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintMinimizeNumberOfRoomsForStudents"){
					ConstraintMinimizeNumberOfRoomsForStudents* cn=new ConstraintMinimizeNumberOfRoomsForStudents();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintActivityPreferredRoom"){
					ConstraintActivityPreferredRoom* cn=new ConstraintActivityPreferredRoom();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Activity_Id"){
							cn->activityId=elem4.text().toInt();
							xmlReadingLog+="    Read activity id="+QString::number(cn->activityId)+"\n";
						}
						else if(elem4.tagName()=="Room"){
							cn->roomName=elem4.text();
							xmlReadingLog+="    Read room="+elem4.text()+"\n";
						}
					}
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintActivityPreferredRooms"){
					ConstraintActivityPreferredRooms* cn=new ConstraintActivityPreferredRooms();
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Activity_Id"){
							cn->activityId=elem4.text().toInt();
							xmlReadingLog+="    Read activity id="+QString::number(cn->activityId)+"\n";
						}
						else if(elem4.tagName()=="Number_of_Preferred_Rooms"){
							cn->_n_preferred_rooms=elem4.text().toInt();
							xmlReadingLog+="    Read year number of preferred rooms: "+QString::number(cn->_n_preferred_rooms)+"\n";
							assert(cn->_n_preferred_rooms>=2);
						}
						else if(elem4.tagName()=="Preferred_Room"){
							cn->roomsNames.append(elem4.text());
							xmlReadingLog+="    Read room="+elem4.text()+"\n";
						}
					}
					assert((uint)cn->_n_preferred_rooms==cn->roomsNames.count());
					crt_constraint=cn;
				}
				if(elem3.tagName()=="ConstraintActivitiesSameRoom"){
					ConstraintActivitiesSameRoom* cn=new ConstraintActivitiesSameRoom();
					int n_act=0;
					for(QDomNode node4=elem3.firstChild(); !node4.isNull(); node4=node4.nextSibling()){
						QDomElement elem4=node4.toElement();
						if(elem4.isNull()){
							xmlReadingLog+="    Null node here\n";
							continue;
						}
						xmlReadingLog+="    Found "+elem4.tagName()+" tag\n";
						if(elem4.tagName()=="Weight"){
							cn->weight=elem4.text().toDouble();
							xmlReadingLog+="    Adding weight="+QString::number(cn->weight)+"\n";
						}
						else if(elem4.tagName()=="Compulsory"){
							if(elem4.text()=="yes"){
								cn->compulsory=true;
								xmlReadingLog+="    Current constraint is compulsory\n";
							}
							else{
								cn->compulsory=false;
								xmlReadingLog+="    Current constraint is not compulsory\n";
							}
						}
						else if(elem4.tagName()=="Number_of_Activities"){
							cn->n_activities=elem4.text().toInt();
							xmlReadingLog+="    Read n_activities="+QString::number(cn->n_activities)+"\n";
						}
						else if(elem4.tagName()=="Activity_Id"){
							cn->activitiesId[n_act]=elem4.text().toInt();
							xmlReadingLog+="    Read activity id="+QString::number(cn->activitiesId[n_act])+"\n";
							n_act++;
						}
					}
					assert(cn->n_activities==n_act);
					crt_constraint=cn;
				}

				assert(crt_constraint!=NULL);
				bool tmp=this->addSpaceConstraint(crt_constraint);
				assert(tmp);
				nc++;
			}
			xmlReadingLog+="  Added "+QString::number(nc)+" space constraints\n";
		}
	}

	this->internalStructureComputed=false;

	logStream<<xmlReadingLog;
	file2.close();

	////////////////////////////////////////

	return true;
}

void Rules::write(const QString& filename)
{
	assert(this->initialized);

	QString s;

	QFile file(filename);
	if(!file.open(IO_WriteOnly)){
		assert(0);
		exit(1);
	}

	QTextStream tos(&file);

	s+="<!DOCTYPE FET><FET version=\""+FET_VERSION+"\">\n\n";
	
	//the institution name and comments
	s+="<Institution_Name>"+this->institutionName+"</Institution_Name>\n\n";
	s+="<Comments>"+this->comments+"</Comments>\n\n";

	//the hours and days
	s+="<Hours_List>\n	<Number>"+QString::number(this->nHoursPerDay)+"</Number>\n";
	for(int i=0; i<=this->nHoursPerDay; i++)
		s+="	<Name>"+this->hoursOfTheDay[i]+"</Name>\n";
	s+="</Hours_List>\n\n";
	s+="<Days_List>\n	<Number>"+QString::number(this->nDaysPerWeek)+"</Number>\n";
	for(int i=0; i<this->nDaysPerWeek; i++)
		s+="	<Name>"+this->daysOfTheWeek[i]+"</Name>\n";
	s+="</Days_List>\n\n";

	//students list
	s+="<Students_List>\n";
	for(StudentsYear* sty=this->yearsList.first(); sty; sty=this->yearsList.next())
		s += sty->getXMLDescription();
	s+="</Students_List>\n\n";

	//teachers list
	s += "<Teachers_List>\n";
	for(Teacher* tch=this->teachersList.first(); tch; tch=this->teachersList.next())
		s += tch->getXMLDescription();
	s += "</Teachers_List>\n\n";

	//subjects list
	s += "<Subjects_List>\n";
	for(Subject* sbj=this->subjectsList.first(); sbj; sbj=this->subjectsList.next())
		s += sbj->getXMLDescription();
	s += "</Subjects_List>\n\n";

	//subject tags list
	s += "<Subject_Tags_List>\n";
	for(SubjectTag* stg=this->subjectTagsList.first(); stg; stg=this->subjectTagsList.next())
		s += stg->getXMLDescription();
	s += "</Subject_Tags_List>\n\n";

	//activities list
	s += "<Activities_List>\n";
	for(Activity* act=this->activitiesList.first(); act; act=this->activitiesList.next()){
		s += act->getXMLDescription(*this);
		s += "\n";
	}
	s += "</Activities_List>\n\n";

	//equipments list
	s += "<Equipments_List>\n";
	for(Equipment* eq=this->equipmentsList.first(); eq; eq=this->equipmentsList.next())
		s += eq->getXMLDescription();
	s += "</Equipments_List>\n\n";

	//rooms list
	s += "<Rooms_List>\n";
	for(Room* rm=this->roomsList.first(); rm; rm=this->roomsList.next())
		s += rm->getXMLDescription();
	s += "</Rooms_List>\n\n";

	//time constraints list
	s += "<Time_Constraints_List>\n";
	for(TimeConstraint* ctr=this->timeConstraintsList.first(); ctr; ctr=this->timeConstraintsList.next())
		s += ctr->getXMLDescription(*this);
	s += "</Time_Constraints_List>\n\n";

	//constraints list
	s += "<Space_Constraints_List>\n";
	for(SpaceConstraint* ctr=this->spaceConstraintsList.first(); ctr; ctr=this->spaceConstraintsList.next())
		s += ctr->getXMLDescription(*this);
	s += "</Space_Constraints_List>\n\n";

	s+="</FET>\n";

	tos<<s;
}
