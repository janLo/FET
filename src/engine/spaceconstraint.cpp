/*
File spaceconstraint.cpp
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

#define minimu(x,y)	((x)<(y)?(x):(y))
#define maximu(x,y)	((x)>(y)?(x):(y))

#include <iostream>
using namespace std;

#include "genetictimetable_defs.h"
#include "spaceconstraint.h"
#include "rules.h"
#include "spacechromosome.h"

#include <qstring.h>

static SpaceChromosome* crt_chrom=NULL;
static Rules* crt_rules=NULL;

#define yesNo(x)				((x)==0?"no":"yes")
#define yesNoTranslated(x)		((x)==0?QObject::tr("no"):QObject::tr("yes"))

static int16 roomsMatrix[MAX_ROOMS][MAX_DAYS_PER_WEEK][MAX_HOURS_PER_DAY];

static bool subgroupsRooms[MAX_TOTAL_SUBGROUPS][MAX_ROOMS]; //used only for ConstraintMinimizeNumberOfRoomsForStudents::fitness

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

SpaceConstraint::SpaceConstraint()
{
	this->type=CONSTRAINT_GENERIC_SPACE;
}

SpaceConstraint::~SpaceConstraint()
{
}

SpaceConstraint::SpaceConstraint(double w, bool c)
{
	this->weight=w;
	assert(w<=100.0);
	this->type=CONSTRAINT_GENERIC_SPACE;
	this->compulsory=c;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

ConstraintBasicCompulsorySpace::ConstraintBasicCompulsorySpace()
	: SpaceConstraint()
{
	this->type=CONSTRAINT_BASIC_COMPULSORY_SPACE;
	this->compulsory=true;
}

ConstraintBasicCompulsorySpace::ConstraintBasicCompulsorySpace(double w)
	: SpaceConstraint(w, true)
{
	this->type=CONSTRAINT_BASIC_COMPULSORY_SPACE;
}

void ConstraintBasicCompulsorySpace::computeInternalStructure(Rules& r)
{
	if(&r!=NULL)
		;
	/*do nothing*/
}

QString ConstraintBasicCompulsorySpace::getXMLDescription(Rules& r)
{
	if(&r!=NULL)
		;

	QString s = "<ConstraintBasicCompulsorySpace>\n";
	s += "	<Weight>"+QString::number(this->weight)+"</Weight>\n";
	s+="	<Compulsory>";
	s+=yesNo(this->compulsory);
	s+="</Compulsory>\n";
	s += "</ConstraintBasicCompulsorySpace>\n";
	return s;
}

QString ConstraintBasicCompulsorySpace::getDescription(Rules& r)
{
	if(&r!=NULL)
		;
	
	QString s = QObject::tr("Basic compulsory constraints (space), W:%1").arg(this->weight);
	s+=", ";
	s += QObject::tr("C:%1").arg(yesNoTranslated(this->compulsory));
	
	return s;
}

QString ConstraintBasicCompulsorySpace::getDetailedDescription(Rules& r)
{
	if(&r!=NULL)
		;

	QString s=QObject::tr("These are the basic compulsory constraints \n"
			"(referring to space allocation) for any timetable\n");
	s+=QObject::tr("Weight=%1").arg(this->weight);s+="\n";
	s+=QObject::tr("Compulsory=%1").arg(yesNoTranslated(this->compulsory));s+="\n";

	s+=QObject::tr("The basic space constraints try to avoid:\n");
	s+=QObject::tr("- unallocated activities\n");
	s+=QObject::tr("- activities with more students than the capacity of the room\n");
	s+=QObject::tr("- rooms assigned to more than one activity simultaneously\n");

	return s;
}

//critical function here - must be optimized for speed
int ConstraintBasicCompulsorySpace::fitness(
	SpaceChromosome& c,
	Rules& r,
	const int days[/*MAX_ACTIVITIES*/],
	const int hours[/*MAX_ACTIVITIES*/],
	QString* conflictsString)
	{

	assert(r.internalStructureComputed);

	int roomsConflicts;

	//This constraint fitness calculation routine is called firstly,
	//so we can compute the rooms conflicts faster this way.
	if(crt_chrom!=&c || crt_rules!=&r){
		roomsConflicts = c.getRoomsMatrix(r, days, hours, roomsMatrix);

		crt_chrom = &c;
		crt_rules = &r;
	}
	else{
		cout<<"Warning here ****"<<endl;
		roomsConflicts=-1;
	}

	int i;

	int unallocated; //unallocated activities
	int nre; //number of room exhaustions
	int nor; //number of overwhelmed rooms

	//part without logging....................................................................
if(conflictsString==NULL){
	//Unallocated activities
	unallocated=0;
	nor=0;
	for(i=0; i<r.nInternalActivities; i++)
		if(c.rooms[i]==UNALLOCATED_SPACE){
			//Firstly, we consider a big clash each unallocated activity.
			//Needs to be very a large constant, bigger than any other broken constraint.
			unallocated += /*r.internalActivitiesList[i].duration * r.internalActivitiesList[i].nSubgroups * */ 10000;
			//(an unallocated activity for a year is more important than an unallocated activity for a subgroup)
		}
		else{
			//The capacity of each room must be respected
			//(the number of students must be less than the capacity)
			int rm=c.rooms[i];
			if(r.internalActivitiesList[i].nTotalStudents>r.internalRoomsList[rm]->capacity){
				if(r.internalActivitiesList[i].parity==PARITY_WEEKLY)
					nor+=2;
				else
					nor++;
			}
		}

	//Below, for rooms, please remember that 2 means a weekly activity
	//and 1 bi-weekly one. So, is the matrix roomsMatrix[rooms][day][hour]==2,
	//it is ok.

	//Calculates the number of rooms exhaustion (when a room is occupied
	//for more than one activity at the same time)
	/*nre=0;
	for(i=0; i<r.nInternalRooms; i++)
		for(int j=0; j<r.nDaysPerWeek; j++)
			for(int k=0; k<r.nHoursPerDay; k++){
				int tmp=roomsMatrix[i][j][k]-2;
				if(tmp>0){
					if(conflictsString!=NULL){
						(*conflictsString)+="Space constraint basic ";
						(*conflictsString)+="compulsory: rooms with name ";
						(*conflictsString)+=r.internalRoomsList[i]->name;
						(*conflictsString)+=" has more than one allocated activity on";
						(*conflictsString)+=" day ";
						(*conflictsString)+=r.daysOfTheWeek[j];
						(*conflictsString)+=", hour ";
						(*conflictsString)+=QString::number(k);
						(*conflictsString)+=". This increases the fitness factor with ";
						(*conflictsString)+=QString::number(tmp*weight);
						(*conflictsString)+="\n";
					}
					nre+=tmp;
				}
			}*/
	nre = roomsConflicts;
}
	//part with logging....................................................................
else{
	//Unallocated activities
	unallocated=0;
	nor=0;
	for(i=0; i<r.nInternalActivities; i++)
		if(c.rooms[i]==UNALLOCATED_SPACE){
			//Firstly, we consider a big clash each unallocated activity.
			//Needs to be very a large constant, bigger than any other broken constraint.
			unallocated += /*r.internalActivitiesList[i].duration * r.internalActivitiesList[i].nSubgroups * */ 10000;
			//(an unallocated activity for a year is more important than an unallocated activity for a subgroup)
			if(conflictsString!=NULL){
				(*conflictsString) += QObject::tr("Space constraint basic compulsory: unallocated activity with id=%1").arg(r.internalActivitiesList[i].id);
				(*conflictsString) += QObject::tr(" - this increases the conflicts factor with %1")
					.arg(weight* /*r.internalActivitiesList[i].duration * r.internalActivitiesList[i].nSubgroups * */ 10000);
				(*conflictsString) += "\n";
			}
		}
		else{
			//The capacity of each room must be respected
			//(the number of students must be less than the capacity)
			int rm=c.rooms[i];
			if(r.internalActivitiesList[i].nTotalStudents>r.internalRoomsList[rm]->capacity){
				int tmp;
				if(r.internalActivitiesList[i].parity==PARITY_WEEKLY)
					tmp=2;
				else
					tmp=1;

				nor+=tmp;

				if(conflictsString!=NULL){
					QString s;
					s=QObject::tr("Space constraint basic compulsory: room %1 has allocated activity with id %2 and the capacity of the room is overloaded")
					.arg(r.internalRoomsList[rm]->name).arg(r.internalActivitiesList[i].id);
					(*conflictsString) += s;
					(*conflictsString) += "\n";
				}
			}
		}

	//Below, for rooms, please remember that 2 means a weekly activity
	//and 1 bi-weekly one. So, is the matrix roomsMatrix[rooms][day][hour]==2,
	//it is ok.

	//Calculates the number of rooms exhaustion (when a room is occupied
	//for more than one activity at the same time)
	nre=0;
	for(i=0; i<r.nInternalRooms; i++)
		for(int j=0; j<r.nDaysPerWeek; j++)
			for(int k=0; k<r.nHoursPerDay; k++){
				int tmp=roomsMatrix[i][j][k]-2;
				if(tmp>0){
					if(conflictsString!=NULL){
						(*conflictsString)+=QObject::tr("Space constraint basic compulsory: room with name %1 has more than one allocated activity on day %2, hour %3.")
							.arg(r.internalRoomsList[i]->name)
							.arg(r.daysOfTheWeek[j])
							.arg(r.hoursOfTheDay[k]);
						(*conflictsString)+=" ";
						(*conflictsString)+=QObject::tr("This increases the conflicts factor with %1").arg(tmp*weight);
						(*conflictsString)+="\n";
					}
					nre+=tmp;
				}
			}
}
	if(roomsConflicts!=-1)
		assert(nre==roomsConflicts); //just a check, works only on logged fitness calculation

	return int (ceil ( weight * (unallocated + nre + nor) ) ); //fitness factor
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

ConstraintRoomNotAvailable::ConstraintRoomNotAvailable()
	: SpaceConstraint()
{
	this->type=CONSTRAINT_ROOM_NOT_AVAILABLE;
}

ConstraintRoomNotAvailable::ConstraintRoomNotAvailable(double w, bool c, const QString& rn, int day, int start_hour, int end_hour)
	: SpaceConstraint(w, c)
{
	this->roomName=rn;
	this->d=day;
	this->h1=start_hour;
	this->h2=end_hour;
	this->type=CONSTRAINT_ROOM_NOT_AVAILABLE;
}

QString ConstraintRoomNotAvailable::getXMLDescription(Rules& r){
	QString s="<ConstraintRoomNotAvailable>\n";
	s+="	<Weight>"+QString::number(weight)+"</Weight>\n";
	s+="	<Compulsory>";
	s+=yesNo(this->compulsory);
	s+="</Compulsory>\n";
	s+="	<Room_Name>"+this->roomName+"</Room_Name>\n";
	s+="	<Day>"+r.daysOfTheWeek[this->d]+"</Day>\n";
	s+="	<Start_Hour>"+r.hoursOfTheDay[this->h1]+"</Start_Hour>\n";
	s+="	<End_Hour>"+r.hoursOfTheDay[this->h2]+"</End_Hour>\n";
	s+="</ConstraintRoomNotAvailable>\n";
	return s;
}

QString ConstraintRoomNotAvailable::getDescription(Rules& r){
	QString s=QObject::tr("Room not available");s+=",";
	s+=(QObject::tr("W:%1").arg(this->weight));s+=", ";
	s+=(QObject::tr("C:%1").arg(yesNoTranslated(this->compulsory)));s+=", ";
	s+=(QObject::tr("R:%1").arg(this->roomName));s+=", ";
	s+=(QObject::tr("D:%1").arg(r.daysOfTheWeek[this->d]));s+=", ";
	s+=(QObject::tr("SH:%1").arg(r.hoursOfTheDay[this->h1]));s+=", ";
	s+=(QObject::tr("EH:%1").arg(r.hoursOfTheDay[this->h2]));s+=", ";

	return s;
}

QString ConstraintRoomNotAvailable::getDetailedDescription(Rules& r){
	QString s=QObject::tr("Space constraint");s+="\n";
	s+=QObject::tr("Room not available");s+="\n";
	s+=(QObject::tr("Weight=%1").arg(this->weight));s+="\n";
	s+=(QObject::tr("Compulsory=%1").arg(yesNoTranslated(this->compulsory)));s+="\n";
	s+=(QObject::tr("Room=%1").arg(this->roomName));s+="\n";
	s+=(QObject::tr("Day=%1").arg(r.daysOfTheWeek[this->d]));s+="\n";
	s+=(QObject::tr("Start hour=%1").arg(r.hoursOfTheDay[this->h1]));s+="\n";
	s+=(QObject::tr("End hour=%1").arg(r.hoursOfTheDay[this->h2]));s+="\n";

	return s;
}

void ConstraintRoomNotAvailable::computeInternalStructure(Rules& r){
	this->room_ID=r.searchRoom(this->roomName);
	assert(this->room_ID>=0);
}

//critical function here - must be optimized for speed
int ConstraintRoomNotAvailable::fitness(
	SpaceChromosome& c,
	Rules& r,
	const int days[/*MAX_ACTIVITIES*/],
	const int hours[/*MAX_ACTIVITIES*/],
	QString* conflictsString)
{
	//if the matrices roomsMatrix is already calculated, do not calculate it again!
	if(crt_chrom!=&c || crt_rules!=&r){
		c.getRoomsMatrix(r, days, hours, roomsMatrix);

		crt_chrom = &c;
		crt_rules = &r;
	}

	//Calculates the number of hours when the roomr is supposed to be occupied,
	//but it is not available
	//This function consideres all the hours, I mean if there are for example 5 weekly courses
	//scheduled on that hour (which is already a broken compulsory constraint - we only
	//are allowed 1 weekly activity for a certain room at a certain hour) we calculate
	//5 broken constraints for that function.
	//TODO: decide if it is better to consider only 2 or 10 as a return value in this particular case
	//(currently it is 10)
	int i=this->room_ID;
	int j=this->d;
	int nbroken;

	//without logging
if(conflictsString==NULL){
	nbroken=0;
	for(int k=h1; k<h2; k++)
		if(roomsMatrix[i][j][k]>0)
			nbroken+=roomsMatrix[i][j][k];
}
	//with logging
else{
	nbroken=0;
	for(int k=h1; k<h2; k++)
		if(roomsMatrix[i][j][k]>0){
			if(conflictsString!=NULL){
				*conflictsString+=
					(QObject::tr("Space constraint room not available broken for room %1 on day %2, hour %3")
					.arg(r.internalRoomsList[i]->name)
					.arg(r.daysOfTheWeek[j])
					.arg(k));
				*conflictsString += ". ";
				*conflictsString += (QObject::tr("This increases the conflicts factor with %1").arg(roomsMatrix[i][j][k]*weight));
				*conflictsString += "\n";
			}
			nbroken+=roomsMatrix[i][j][k];
		}
}

	return int (ceil ( weight * nbroken ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

ConstraintRoomTypeNotAllowedSubjects::ConstraintRoomTypeNotAllowedSubjects()
	: SpaceConstraint()
{
	this->type=CONSTRAINT_ROOM_TYPE_NOT_ALLOWED_SUBJECTS;
}

ConstraintRoomTypeNotAllowedSubjects::ConstraintRoomTypeNotAllowedSubjects(double w, bool c, const QString& room_type)
	: SpaceConstraint(w, c)
{
	this->type=CONSTRAINT_ROOM_TYPE_NOT_ALLOWED_SUBJECTS;
	this->roomType=room_type;
}

void ConstraintRoomTypeNotAllowedSubjects::computeInternalStructure(Rules& r)
{
	this->_nRooms=0;
	for(int i=0; i<r.nInternalRooms; i++)
		if(r.internalRoomsList[i]->type == this->roomType)
			this->_rooms[this->_nRooms++]=i;
			
	this->_nActivities=0;
	for(int i=0; i<r.nInternalActivities; i++)
		if(this->searchNotAllowedSubject(r.internalActivitiesList[i].subjectName) == true)
			this->_activities[this->_nActivities++]=i;
}

void ConstraintRoomTypeNotAllowedSubjects::addNotAllowedSubject(const QString& subjectName)
{
	this->subjects.append(subjectName);
}

int ConstraintRoomTypeNotAllowedSubjects::removeNotAllowedSubject(const QString& subjectName)
{
	int tmp=this->subjects.remove(subjectName);
	return tmp;
}

bool ConstraintRoomTypeNotAllowedSubjects::searchNotAllowedSubject(const QString& subjectName)
{
	int tmp=this->subjects.findIndex(subjectName);
	if(tmp>=0)
		return true;
	else
		return false;
}

QString ConstraintRoomTypeNotAllowedSubjects::getXMLDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s="<ConstraintRoomTypeNotAllowedSubjects>\n";
	s+="	<Weight>"+QString::number(weight)+"</Weight>\n";
	s+="	<Compulsory>";
	s+=yesNo(this->compulsory);
	s+="</Compulsory>\n";
	s+="	<Room_Type>"+this->roomType+"</Room_Type>\n";

	for(QStringList::Iterator it=this->subjects.begin(); it!=this->subjects.end(); it++)
		s+="	<Subject>"+(*it)+"</Subject>\n";
	s+="</ConstraintRoomTypeNotAllowedSubjects>\n";

	return s;
}

QString ConstraintRoomTypeNotAllowedSubjects::getDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s="Room type not allowed subjects"; s+=", ";
	s+=QObject::tr("W:%1").arg(this->weight);s+=", ";
	s+=QObject::tr("C:%1").arg(yesNoTranslated(this->compulsory));s+=", ";
	s+=QObject::tr("RT:%1").arg(this->roomType);s+=",";

	for(QStringList::Iterator it=this->subjects.begin(); it!=this->subjects.end(); it++)
		s+=QObject::tr("S:%1").arg(*it); s+=",";

	return s;
}

QString ConstraintRoomTypeNotAllowedSubjects::getDetailedDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s=QObject::tr("Space constraint"); s+="\n";
	s+=QObject::tr("Room type not allowed subjects"); s+="\n";
	s+=QObject::tr("Weight=%1").arg(this->weight);s+="\n";
	s+=QObject::tr("Compulsory=%1").arg(yesNoTranslated(this->compulsory));s+="\n";
	s+=QObject::tr("Room Type=%1").arg(this->roomType);s+="\n";

	for(QStringList::Iterator it=this->subjects.begin(); it!=this->subjects.end(); it++){
		s+=QObject::tr("Subject=%1").arg(*it); 
		s+="\n";
	}

	return s;
}

//critical function here - must be optimized for speed
int ConstraintRoomTypeNotAllowedSubjects::fitness(
	SpaceChromosome& c,
	Rules& r,
	const int days[/*MAX_ACTIVITIES*/],
	const int hours[/*MAX_ACTIVITIES*/],
	QString* conflictsString)
{
	//if the matrix roomsMatrix is already calculated, do not calculate it again!
	if(crt_chrom!=&c || crt_rules!=&r){
		c.getRoomsMatrix(r, days, hours, roomsMatrix);

		crt_chrom = &c;
		crt_rules = &r;
	}

	//Calculates the number of conflicts - the type of the room
	//does not allow a certain kind of subject
	//The fastest way seems to iterate over all activities
	//involved in this constraint (every not allowed subject),
	//find the scheduled room and check to see if the type
	//of this room is accepted or not.

	int nbroken;

	//without logging
if(conflictsString==NULL){
	nbroken=0;
	for(int i=0; i<this->_nActivities; i++){
		int ac=this->_activities[i];
		int rm=c.rooms[ac];
		if(rm==UNALLOCATED_SPACE)
			continue;
		QString typ=r.internalRoomsList[rm]->type;
		//maybe this can be a little speed up by keeping a list of the types
		//and comparing integers instead of strings.
		if(typ==this->roomType)
			nbroken++;
	}
}
	//with logging
else{
	nbroken=0;
	for(int i=0; i<this->_nActivities; i++){
		int ac=this->_activities[i];
		int rm=c.rooms[ac];
		if(rm==UNALLOCATED_SPACE)
			continue;
		QString typ=r.internalRoomsList[rm]->type;
		//maybe this can be a little speed up by keeping a list of the types
		//and comparing integers instead of strings.
		if(typ==this->roomType){
			if(conflictsString!=NULL){
				*conflictsString+=
					(QObject::tr("Space constraint room type not allowed subjects broken for room %1, activity with id %2")
					.arg(r.internalRoomsList[rm]->name)
					.arg(r.internalActivitiesList[ac].id));
				*conflictsString += ". ";
				*conflictsString += (QObject::tr("This increases the conflicts factor with %1").arg(weight));
				*conflictsString += "\n";
			}

			nbroken++;
		}
	}
}

	return int (ceil ( weight * nbroken ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

ConstraintSubjectRequiresEquipments::ConstraintSubjectRequiresEquipments()
	: SpaceConstraint()
{
	this->type=CONSTRAINT_SUBJECT_REQUIRES_EQUIPMENTS;
}

ConstraintSubjectRequiresEquipments::ConstraintSubjectRequiresEquipments(double w, bool c, const QString& subj)
	: SpaceConstraint(w, c)
{
	this->type=CONSTRAINT_SUBJECT_REQUIRES_EQUIPMENTS;
	this->subjectName=subj;
}

void ConstraintSubjectRequiresEquipments::computeInternalStructure(Rules& r)
{
	//This procedure computes the internal list of all the activities
	//which correspond to the subject of the constraint.
	//Computes also the internal list of indices of equipments
	
	this->_nActivities=0;
	for(int ac=0; ac<r.nInternalActivities; ac++)
		if(r.internalActivitiesList[ac].subjectName == this->subjectName){
			assert(this->_nActivities<MAX_ACTIVITIES_FOR_A_SUBJECT);
			this->_activities[this->_nActivities++]=ac;
		}
		
	this->_nEquipments=0;
	for(int eq=0; eq<r.nInternalEquipments; eq++)
		if(this->searchRequiredEquipment(r.internalEquipmentsList[eq]->name)){
			assert(this->_nEquipments<MAX_EQUIPMENTS_FOR_A_CONSTRAINT);
			this->_equipments[this->_nEquipments++]=eq;
		}
}

void ConstraintSubjectRequiresEquipments::addRequiredEquipment(const QString& equip)
{
	this->equipmentsNames.append(equip);
}

int ConstraintSubjectRequiresEquipments::removeRequiredEquipment(const QString& equip)
{
	int tmp=this->equipmentsNames.remove(equip);
	return tmp;
}

bool ConstraintSubjectRequiresEquipments::searchRequiredEquipment(const QString& equip)
{
	int tmp=this->equipmentsNames.findIndex(equip);
	if(tmp>=0)
		return true;
	else
		return false;
}

QString ConstraintSubjectRequiresEquipments::getXMLDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s="<ConstraintSubjectRequiresEquipments>\n";
	s+="	<Weight>"+QString::number(weight)+"</Weight>\n";
	s+="	<Compulsory>";
	s+=yesNo(this->compulsory);
	s+="</Compulsory>\n";
	s+="	<Subject>"+this->subjectName+"</Subject>\n";

	for(QStringList::Iterator it=this->equipmentsNames.begin(); it!=this->equipmentsNames.end(); it++)
		s+="	<Equipment>"+(*it)+"</Equipment>\n";
		
	s+="</ConstraintSubjectRequiresEquipments>\n";

	return s;
}

QString ConstraintSubjectRequiresEquipments::getDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s="Subject requires equipments"; s+=", ";
	s+=QObject::tr("W:%1").arg(this->weight);s+=", ";
	s+=QObject::tr("C:%1").arg(yesNoTranslated(this->compulsory));s+=", ";
	s+=QObject::tr("S:%1").arg(this->subjectName);s+=",";

	for(QStringList::Iterator it=this->equipmentsNames.begin(); it!=this->equipmentsNames.end(); it++)
		s+=QObject::tr("E:%1").arg(*it); s+=",";

	return s;
}

QString ConstraintSubjectRequiresEquipments::getDetailedDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s=QObject::tr("Space constraint"); s+="\n";
	s+=QObject::tr("Subject requires equipments"); s+="\n";
	s+=QObject::tr("Weight=%1").arg(this->weight);s+="\n";
	s+=QObject::tr("Compulsory=%1").arg(yesNoTranslated(this->compulsory));s+="\n";
	s+=QObject::tr("Subject=%1").arg(this->subjectName);s+="\n";

	for(QStringList::Iterator it=this->equipmentsNames.begin(); it!=this->equipmentsNames.end(); it++){
		s+=QObject::tr("Equipment=%1").arg(*it); 
		s+="\n";
	}

	return s;
}

//critical function here - must be optimized for speed
int ConstraintSubjectRequiresEquipments::fitness(
	SpaceChromosome& c,
	Rules& r,
	const int days[/*MAX_ACTIVITIES*/],
	const int hours[/*MAX_ACTIVITIES*/],
	QString* conflictsString)
{
	//if the matrix roomsMatrix is already calculated, do not calculate it again!
	if(crt_chrom!=&c || crt_rules!=&r){
		c.getRoomsMatrix(r, days, hours, roomsMatrix);

		crt_chrom = &c;
		crt_rules = &r;
	}

	//Calculates the number of conflicts.
	//The fastest way seems to iterate over all activities
	//involved in this constraint (share the subject of this constraint),
	//find the scheduled room and check to see if this
	//room is accepted or not.

	int nbroken;

	//without logging
if(conflictsString==NULL){
	nbroken=0;
	for(int i=0; i<this->_nActivities; i++){	
		int ac=this->_activities[i];
		int rm=c.rooms[ac];
		if(rm==UNALLOCATED_SPACE)
			continue;
		
		bool ok=true;
		for(int j=0; j<this->_nEquipments; j++){
			int eq=this->_equipments[j];
			if(!r.roomHasEquipment[rm][eq]){
				ok=false;
				break;
			}
		}

		if(!ok)
			nbroken++;
	}
}
	//with logging
else{
	nbroken=0;
	for(int i=0; i<this->_nActivities; i++){	
		int ac=this->_activities[i];
		int rm=c.rooms[ac];
		if(rm==UNALLOCATED_SPACE)
			continue;
		
		bool ok=true;
		for(int j=0; j<this->_nEquipments; j++){
			int eq=this->_equipments[j];
			if(!r.roomHasEquipment[rm][eq]){
				ok=false;
				break;
			}
		}

		if(!ok){
			if(conflictsString!=NULL){
				*conflictsString+=
					(QObject::tr("Space constraint subject requires equipments broken for room %1, activity with id %2")
					.arg(r.internalRoomsList[rm]->name)
					.arg(r.internalActivitiesList[ac].id));
				*conflictsString += ". ";
				*conflictsString += (QObject::tr("This increases the conflicts factor with %1").arg(weight));
				*conflictsString += "\n";
			}

			nbroken++;
		}
	}
}

	return int (ceil ( weight * nbroken ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

ConstraintSubjectSubjectTagRequireEquipments::ConstraintSubjectSubjectTagRequireEquipments()
	: SpaceConstraint()
{
	this->type=CONSTRAINT_SUBJECT_SUBJECT_TAG_REQUIRE_EQUIPMENTS;
}

ConstraintSubjectSubjectTagRequireEquipments::ConstraintSubjectSubjectTagRequireEquipments(double w, bool c, const QString& subj, const QString& subt)
	: SpaceConstraint(w, c)
{
	this->type=CONSTRAINT_SUBJECT_SUBJECT_TAG_REQUIRE_EQUIPMENTS;
	this->subjectName=subj;
	this->subjectTagName=subt;
}

void ConstraintSubjectSubjectTagRequireEquipments::computeInternalStructure(Rules& r)
{
	//This procedure computes the internal list of all the activities
	//which correspond to the subject of the constraint.
	//Computes also the internal list of indices of equipments
	
	this->_nActivities=0;
	for(int ac=0; ac<r.nInternalActivities; ac++)
		if(r.internalActivitiesList[ac].subjectName == this->subjectName
		 &&r.internalActivitiesList[ac].subjectTagName==this->subjectTagName){
			assert(this->_nActivities<MAX_ACTIVITIES_FOR_A_SUBJECT);
			this->_activities[this->_nActivities++]=ac;
		}
		
	this->_nEquipments=0;
	for(int eq=0; eq<r.nInternalEquipments; eq++)
		if(this->searchRequiredEquipment(r.internalEquipmentsList[eq]->name)){
			assert(this->_nEquipments<MAX_EQUIPMENTS_FOR_A_CONSTRAINT);
			this->_equipments[this->_nEquipments++]=eq;
		}
}

void ConstraintSubjectSubjectTagRequireEquipments::addRequiredEquipment(const QString& equip)
{
	this->equipmentsNames.append(equip);
}

int ConstraintSubjectSubjectTagRequireEquipments::removeRequiredEquipment(const QString& equip)
{
	int tmp=this->equipmentsNames.remove(equip);
	return tmp;
}

bool ConstraintSubjectSubjectTagRequireEquipments::searchRequiredEquipment(const QString& equip)
{
	int tmp=this->equipmentsNames.findIndex(equip);
	if(tmp>=0)
		return true;
	else
		return false;
}

QString ConstraintSubjectSubjectTagRequireEquipments::getXMLDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s="<ConstraintSubjectSubjectTagRequireEquipments>\n";
	s+="	<Weight>"+QString::number(weight)+"</Weight>\n";
	s+="	<Compulsory>";
	s+=yesNo(this->compulsory);
	s+="</Compulsory>\n";
	s+="	<Subject>"+this->subjectName+"</Subject>\n";
	s+="	<Subject_Tag>"+this->subjectTagName+"</Subject_Tag>\n";

	for(QStringList::Iterator it=this->equipmentsNames.begin(); it!=this->equipmentsNames.end(); it++)
		s+="	<Equipment>"+(*it)+"</Equipment>\n";
		
	s+="</ConstraintSubjectSubjectTagRequireEquipments>\n";

	return s;
}

QString ConstraintSubjectSubjectTagRequireEquipments::getDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s="Subject subject tag require equipments"; s+=", ";
	s+=QObject::tr("W:%1").arg(this->weight);s+=", ";
	s+=QObject::tr("C:%1").arg(yesNoTranslated(this->compulsory));s+=", ";
	s+=QObject::tr("S:%1").arg(this->subjectName);s+=",";
	s+=QObject::tr("ST:%1").arg(this->subjectTagName);s+=",";

	for(QStringList::Iterator it=this->equipmentsNames.begin(); it!=this->equipmentsNames.end(); it++)
		s+=QObject::tr("E:%1").arg(*it); s+=",";

	return s;
}

QString ConstraintSubjectSubjectTagRequireEquipments::getDetailedDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s=QObject::tr("Space constraint"); s+="\n";
	s+=QObject::tr("Subject subject tag require equipments"); s+="\n";
	s+=QObject::tr("Weight=%1").arg(this->weight);s+="\n";
	s+=QObject::tr("Compulsory=%1").arg(yesNoTranslated(this->compulsory));s+="\n";
	s+=QObject::tr("Subject=%1").arg(this->subjectName);s+="\n";
	s+=QObject::tr("Subject tag=%1").arg(this->subjectTagName);s+="\n";

	for(QStringList::Iterator it=this->equipmentsNames.begin(); it!=this->equipmentsNames.end(); it++){
		s+=QObject::tr("Equipment=%1").arg(*it); 
		s+="\n";
	}

	return s;
}

//critical function here - must be optimized for speed
int ConstraintSubjectSubjectTagRequireEquipments::fitness(
	SpaceChromosome& c,
	Rules& r,
	const int days[/*MAX_ACTIVITIES*/],
	const int hours[/*MAX_ACTIVITIES*/],
	QString* conflictsString)
{
	//if the matrix roomsMatrix is already calculated, do not calculate it again!
	if(crt_chrom!=&c || crt_rules!=&r){
		c.getRoomsMatrix(r, days, hours, roomsMatrix);

		crt_chrom = &c;
		crt_rules = &r;
	}

	//Calculates the number of conflicts.
	//The fastest way seems to iterate over all activities
	//involved in this constraint (share the subject of this constraint),
	//find the scheduled room and check to see if this
	//room is accepted or not.

	int nbroken;

	//without logging
if(conflictsString==NULL){
	nbroken=0;
	for(int i=0; i<this->_nActivities; i++){	
		int ac=this->_activities[i];
		int rm=c.rooms[ac];
		if(rm==UNALLOCATED_SPACE)
			continue;
		
		bool ok=true;
		for(int j=0; j<this->_nEquipments; j++){
			int eq=this->_equipments[j];
			if(!r.roomHasEquipment[rm][eq]){
				ok=false;
				break;
			}
		}

		if(!ok)
			nbroken++;
	}
}
	//with logging
else{
	nbroken=0;
	for(int i=0; i<this->_nActivities; i++){	
		int ac=this->_activities[i];
		int rm=c.rooms[ac];
		if(rm==UNALLOCATED_SPACE)
			continue;
		
		bool ok=true;
		for(int j=0; j<this->_nEquipments; j++){
			int eq=this->_equipments[j];
			if(!r.roomHasEquipment[rm][eq]){
				ok=false;
				break;
			}
		}

		if(!ok){
			if(conflictsString!=NULL){
				*conflictsString+=
					(QObject::tr("Space constraint subject subject tag require equipments broken for room %1, activity with id %2")
					.arg(r.internalRoomsList[rm]->name)
					.arg(r.internalActivitiesList[ac].id));
				*conflictsString += ". ";
				*conflictsString += (QObject::tr("This increases the conflicts factor with %1").arg(weight));
				*conflictsString += "\n";
			}

			nbroken++;
		}
	}
}

	return int (ceil ( weight * nbroken ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

ConstraintTeacherRequiresRoom::ConstraintTeacherRequiresRoom()
	: SpaceConstraint()
{
	this->type=CONSTRAINT_TEACHER_REQUIRES_ROOM;
}

ConstraintTeacherRequiresRoom::ConstraintTeacherRequiresRoom(double w, bool c, const QString& teach, const QString& room)
	: SpaceConstraint(w, c)
{
	this->type=CONSTRAINT_TEACHER_REQUIRES_ROOM;
	this->teacherName=teach;
	this->roomName=room;
}

void ConstraintTeacherRequiresRoom::computeInternalStructure(Rules& r)
{
	//This procedure computes the internal list of all the activities
	//which correspond to the teacher of the constraint,	
	this->_nActivities=0;
	for(int ac=0; ac<r.nInternalActivities; ac++)
		for(QStringList::Iterator it=r.internalActivitiesList[ac].teachersNames.begin(); it!=r.internalActivitiesList[ac].teachersNames.end(); it++)
			if(*it == this->teacherName){
				assert(this->_nActivities<MAX_ACTIVITIES_FOR_A_TEACHER);
				this->_activities[this->_nActivities++]=ac;
			}
		
	this->_room = r.searchRoom(this->roomName);
	assert(this->_room>=0);
}

QString ConstraintTeacherRequiresRoom::getXMLDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s="<ConstraintTeacherRequiresRoom>\n";
	s+="	<Weight>"+QString::number(weight)+"</Weight>\n";
	s+="	<Compulsory>";
	s+=yesNo(this->compulsory);
	s+="</Compulsory>\n";
	s+="	<Teacher>"+this->teacherName+"</Teacher>\n";
	s+="	<Room>"+this->roomName+"</Room>\n";
		
	s+="</ConstraintTeacherRequiresRoom>\n";

	return s;
}

QString ConstraintTeacherRequiresRoom::getDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s="Teacher requires room"; s+=", ";
	s+=QObject::tr("W:%1").arg(this->weight);s+=", ";
	s+=QObject::tr("C:%1").arg(yesNoTranslated(this->compulsory));s+=", ";
	s+=QObject::tr("T:%1").arg(this->teacherName);s+=",";
	s+=QObject::tr("R:%1").arg(this->roomName);s+=",";

	return s;
}

QString ConstraintTeacherRequiresRoom::getDetailedDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s=QObject::tr("Space constraint"); s+="\n";
	s+=QObject::tr("Teacher requires room"); s+="\n";
	s+=QObject::tr("Weight=%1").arg(this->weight);s+="\n";
	s+=QObject::tr("Compulsory=%1").arg(yesNoTranslated(this->compulsory));s+="\n";
	s+=QObject::tr("Teacher=%1").arg(this->teacherName);s+="\n";
	s+=QObject::tr("Room=%1").arg(this->roomName);s+="\n";

	return s;
}

//critical function here - must be optimized for speed
int ConstraintTeacherRequiresRoom::fitness(
	SpaceChromosome& c,
	Rules& r,
	const int days[/*MAX_ACTIVITIES*/],
	const int hours[/*MAX_ACTIVITIES*/],
	QString* conflictsString)
{
	//if the matrix roomsMatrix is already calculated, do not calculate it again!
	if(crt_chrom!=&c || crt_rules!=&r){
		c.getRoomsMatrix(r, days, hours, roomsMatrix);

		crt_chrom = &c;
		crt_rules = &r;
	}

	//Calculates the number of conflicts
	//The fastest way seems to iterate over all activities
	//involved in this constraint (share the teacher of this constraint)
	//and find the scheduled room and check to see if this
	//room is accepted or not

	int nbroken;

	//without logging
if(conflictsString==NULL){
	nbroken=0;
	for(int i=0; i<this->_nActivities; i++){
		int ac=this->_activities[i];
		int rm=c.rooms[ac];
		if(rm==UNALLOCATED_SPACE)
			continue;

		if(rm!=this->_room)
			nbroken++;
	}
}
	//with logging
else{
	nbroken=0;
	for(int i=0; i<this->_nActivities; i++){
		int ac=this->_activities[i];
		int rm=c.rooms[ac];
		if(rm==UNALLOCATED_SPACE)
			continue;

		if(rm!=this->_room){
			if(conflictsString!=NULL){
				*conflictsString+=
					(QObject::tr("Space constraint teacher requires room broken for teacher %1, room %2, activity with id=%3")
					.arg(this->teacherName)
					.arg(this->roomName)
					.arg(r.internalActivitiesList[ac].id));
				*conflictsString += ". ";
				*conflictsString += (QObject::tr("This increases the conflicts factor with %1").arg(weight));
				*conflictsString += "\n";
			}

			nbroken++;
		}
	}
}

	return int (ceil ( weight * nbroken ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

ConstraintMinimizeNumberOfRoomsForStudents::ConstraintMinimizeNumberOfRoomsForStudents()
	: SpaceConstraint()
{
	this->type=CONSTRAINT_MINIMIZE_NUMBER_OF_ROOMS_FOR_STUDENTS;
}

ConstraintMinimizeNumberOfRoomsForStudents::ConstraintMinimizeNumberOfRoomsForStudents(double w, bool c)
	: SpaceConstraint(w, c)
{
	this->type=CONSTRAINT_MINIMIZE_NUMBER_OF_ROOMS_FOR_STUDENTS;
}

void ConstraintMinimizeNumberOfRoomsForStudents::computeInternalStructure(Rules& r)
{
	if(&r!=NULL)
		;
	/*do nothing*/
}

QString ConstraintMinimizeNumberOfRoomsForStudents::getXMLDescription(Rules& r)
{
	if(&r!=NULL)
		;

	QString s = "<ConstraintMinimizeNumberOfRoomsForStudents>\n";
	s += "	<Weight>"+QString::number(this->weight)+"</Weight>\n";
	s += "	<Compulsory>";
	s += yesNo(this->compulsory);
	s += "</Compulsory>\n";
	s += "</ConstraintMinimizeNumberOfRoomsForStudents>\n";
	return s;
}

QString ConstraintMinimizeNumberOfRoomsForStudents::getDescription(Rules& r)
{
	if(&r!=NULL)
		;

	QString s=QObject::tr("Minimize number of rooms for students");s+=",";
	s+=(QObject::tr("W:%1").arg(this->weight));s+=", ";
	s+=(QObject::tr("C:%1").arg(yesNoTranslated(this->compulsory)));s+=", ";

	return s;
}

QString ConstraintMinimizeNumberOfRoomsForStudents::getDetailedDescription(Rules& r)
{
	if(&r!=NULL)
		;

	QString s=QObject::tr("Space constraint");s+="\n";
	s+=QObject::tr("Minimize number of rooms for each students set");s+="\n";
	s+=(QObject::tr("Weight=%1").arg(this->weight));s+="\n";
	s+=(QObject::tr("Compulsory=%1").arg(yesNoTranslated(this->compulsory)));s+="\n";

	return s;
}

//critical function here - must be optimized for speed
int ConstraintMinimizeNumberOfRoomsForStudents::fitness(
	SpaceChromosome& c,
	Rules& r,
	const int days[/*MAX_ACTIVITIES*/],
	const int hours[/*MAX_ACTIVITIES*/],
	QString* conflictsString)
	{

	//if the matrix roomsMatrix is already calculated, do not calculate it again!
	if(crt_chrom!=&c || crt_rules!=&r){
		c.getRoomsMatrix(r, days, hours, roomsMatrix);

		crt_chrom = &c;
		crt_rules = &r;
	}

	assert(r.internalStructureComputed);

	int nbroken=0;
	
	//without logging
if(conflictsString==NULL){
	nbroken=0;
	
	for(int i=0; i<r.nInternalSubgroups; i++)
		for(int j=0; j<r.nInternalRooms; j++)
			subgroupsRooms[i][j]=false;
			
	for(int i=0; i<r.nInternalActivities; i++){
		if(c.rooms[i]!=UNALLOCATED_SPACE){
			for(int j=0; j<r.internalActivitiesList[i].nSubgroups; j++){
				int k=r.internalActivitiesList[i].subgroups[j];
				subgroupsRooms[k][c.rooms[i]]=true;
			}
		}
	}
	
	int total[MAX_TOTAL_SUBGROUPS];
	for(int i=0; i<r.nInternalSubgroups; i++)
		total[i]=0;
		
	for(int i=0; i<r.nInternalSubgroups; i++)
		for(int j=0; j<r.nInternalRooms; j++)
			if(subgroupsRooms[i][j])
				total[i]++;
				
	for(int i=0; i<r.nInternalSubgroups; i++)
		if(total[i]>1)
			nbroken+=total[i]-1;
}
	//with logging
else{
	nbroken=0;
	
	for(int i=0; i<r.nInternalSubgroups; i++)
		for(int j=0; j<r.nInternalRooms; j++)
			subgroupsRooms[i][j]=false;
			
	for(int i=0; i<r.nInternalActivities; i++){
		if(c.rooms[i]!=UNALLOCATED_SPACE){
			for(int j=0; j<r.internalActivitiesList[i].nSubgroups; j++){
				int k=r.internalActivitiesList[i].subgroups[j];
				subgroupsRooms[k][c.rooms[i]]=true;
			}
		}
	}
	
	int total[MAX_TOTAL_SUBGROUPS];
	for(int i=0; i<r.nInternalSubgroups; i++)
		total[i]=0;
		
	for(int i=0; i<r.nInternalSubgroups; i++)
		for(int j=0; j<r.nInternalRooms; j++)
			if(subgroupsRooms[i][j])
				total[i]++;
				
	for(int i=0; i<r.nInternalSubgroups; i++)
		if(total[i]>1){
			nbroken+=total[i]-1;
	
			if(conflictsString!=NULL){
				*conflictsString+=
					QObject::tr("Space constraint minimize number of rooms broken for subgroup %1, number of rooms=%2")
					.arg(r.internalSubgroupsList[i]->name)
					.arg(total[i]);
				*conflictsString += ". ";
				*conflictsString += (QObject::tr("This increases the conflicts factor with %1").arg(weight));
				*conflictsString += "\n";
			}
		}
}

	return int (ceil ( weight * nbroken ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

ConstraintActivityPreferredRoom::ConstraintActivityPreferredRoom()
	: SpaceConstraint()
{
	this->type=CONSTRAINT_ACTIVITY_PREFERRED_ROOM;
}

ConstraintActivityPreferredRoom::ConstraintActivityPreferredRoom(double w, bool c, int aid, const QString& room)
	: SpaceConstraint(w, c)
{
	this->type=CONSTRAINT_ACTIVITY_PREFERRED_ROOM;
	this->activityId=aid;
	this->roomName=room;
}

void ConstraintActivityPreferredRoom::computeInternalStructure(Rules& r)
{
	this->_activity=-1;
	for(int ac=0; ac<r.nInternalActivities; ac++)
		if(r.internalActivitiesList[ac].id==this->activityId){
			assert(this->_activity==-1);
			this->_activity=ac;
		}
		
	this->_room = r.searchRoom(this->roomName);
	assert(this->_room>=0);
}

QString ConstraintActivityPreferredRoom::getXMLDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s="<ConstraintActivityPreferredRoom>\n";
	s+="	<Weight>"+QString::number(weight)+"</Weight>\n";
	s+="	<Compulsory>";
	s+=yesNo(this->compulsory);
	s+="</Compulsory>\n";
	s+="	<Activity_Id>"+QString::number(this->activityId)+"</Activity_Id>\n";
	s+="	<Room>"+this->roomName+"</Room>\n";
		
	s+="</ConstraintActivityPreferredRoom>\n";

	return s;
}

QString ConstraintActivityPreferredRoom::getDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s="Activity preferred room"; s+=", ";
	s+=QObject::tr("W:%1").arg(this->weight);s+=", ";
	s+=QObject::tr("C:%1").arg(yesNoTranslated(this->compulsory));s+=", ";
	s+=QObject::tr("A:%1").arg(this->activityId);s+=", ";
	s+=QObject::tr("R:%1").arg(this->roomName);

	return s;
}

QString ConstraintActivityPreferredRoom::getDetailedDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s=QObject::tr("Space constraint"); s+="\n";
	s+=QObject::tr("Activity preferred room"); s+="\n";
	s+=QObject::tr("Weight=%1").arg(this->weight);s+="\n";
	s+=QObject::tr("Compulsory=%1").arg(yesNoTranslated(this->compulsory));s+="\n";
	s+=QObject::tr("Activity id=%1").arg(this->activityId);s+="\n";
	s+=QObject::tr("Room=%1").arg(this->roomName);s+="\n";

	return s;
}

//critical function here - must be optimized for speed
int ConstraintActivityPreferredRoom::fitness(
	SpaceChromosome& c,
	Rules& r,
	const int days[/*MAX_ACTIVITIES*/],
	const int hours[/*MAX_ACTIVITIES*/],
	QString* conflictsString)
{
	//if the matrix roomsMatrix is already calculated, do not calculate it again!
	if(crt_chrom!=&c || crt_rules!=&r){
		c.getRoomsMatrix(r, days, hours, roomsMatrix);

		crt_chrom = &c;
		crt_rules = &r;
	}

	//Calculates the number of conflicts

	int nbroken;

	//without logging
if(conflictsString==NULL){
	nbroken=0;
	int parity=r.internalActivitiesList[this->_activity].parity==PARITY_WEEKLY?2:1;

	int rm=c.rooms[this->_activity];
	if(rm!=UNALLOCATED_SPACE && rm!=this->_room)
		nbroken+=parity;
}
	//with logging
else{
	nbroken=0;
	int parity=r.internalActivitiesList[this->_activity].parity==PARITY_WEEKLY?2:1;

	int rm=c.rooms[this->_activity];
	if(rm!=UNALLOCATED_SPACE && rm!=this->_room){
		if(conflictsString!=NULL){
			*conflictsString+=
				(QObject::tr("Space constraint activity preferred room broken for activity with id=%1, room=%2")
				.arg(this->activityId)
				.arg(this->roomName));
			*conflictsString += ". ";
			*conflictsString += (QObject::tr("This increases the conflicts factor with %1").arg(weight*parity));
			*conflictsString += "\n";
		}

		nbroken+=parity;
	}
}

	return int (ceil ( weight * nbroken ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

ConstraintActivityPreferredRooms::ConstraintActivityPreferredRooms()
	: SpaceConstraint()
{
	this->type=CONSTRAINT_ACTIVITY_PREFERRED_ROOMS;
}

ConstraintActivityPreferredRooms::ConstraintActivityPreferredRooms(double w, bool c, int aid, const QStringList& roomsList)
	: SpaceConstraint(w, c)
{
	this->type=CONSTRAINT_ACTIVITY_PREFERRED_ROOMS;
	this->activityId=aid;
	this->roomsNames=roomsList;
	assert(roomsList.count()<=(uint)MAX_CONSTRAINT_ACTIVITY_PREFERRED_ROOMS);
}

void ConstraintActivityPreferredRooms::computeInternalStructure(Rules& r)
{
	this->_activity=-1;
	for(int ac=0; ac<r.nInternalActivities; ac++)
		if(r.internalActivitiesList[ac].id==this->activityId){
			assert(this->_activity==-1);
			this->_activity=ac;
		}
		
	this->_n_preferred_rooms=this->roomsNames.count();
	int i=0;
	for(QStringList::Iterator it=this->roomsNames.begin(); it!=this->roomsNames.end(); it++){
		this->_rooms[i] = r.searchRoom(*it);
		assert(this->_rooms[i]>=0);
		i++;
	}
}

QString ConstraintActivityPreferredRooms::getXMLDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s="<ConstraintActivityPreferredRooms>\n";
	s+="	<Weight>"+QString::number(weight)+"</Weight>\n";
	s+="	<Compulsory>";
	s+=yesNo(this->compulsory);
	s+="</Compulsory>\n";
	s+="	<Activity_Id>"+QString::number(this->activityId)+"</Activity_Id>\n";
	s+="	<Number_of_Preferred_Rooms>"+QString::number(this->roomsNames.count())+"</Number_of_Preferred_Rooms>\n";
	for(QStringList::Iterator it=this->roomsNames.begin(); it!=this->roomsNames.end(); it++)
		s+="	<Preferred_Room>"+(*it)+"</Preferred_Room>\n";
		
	s+="</ConstraintActivityPreferredRooms>\n";

	return s;
}

QString ConstraintActivityPreferredRooms::getDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s="Activity preferred rooms"; s+=", ";
	s+=QObject::tr("W:%1").arg(this->weight);s+=", ";
	s+=QObject::tr("C:%1").arg(yesNoTranslated(this->compulsory));s+=", ";
	s+=QObject::tr("A:%1").arg(this->activityId);
	for(QStringList::Iterator it=this->roomsNames.begin(); it!=this->roomsNames.end(); it++){
		s+=", ";
		s+=QObject::tr("R:%1").arg(*it);
	}

	return s;
}

QString ConstraintActivityPreferredRooms::getDetailedDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s=QObject::tr("Space constraint"); s+="\n";
	s+=QObject::tr("Activity preferred rooms"); s+="\n";
	s+=QObject::tr("Weight=%1").arg(this->weight);s+="\n";
	s+=QObject::tr("Compulsory=%1").arg(yesNoTranslated(this->compulsory));s+="\n";
	s+=QObject::tr("Activity id=%1").arg(this->activityId);s+="\n";
	for(QStringList::Iterator it=this->roomsNames.begin(); it!=this->roomsNames.end(); it++){
		s+=QObject::tr("Room=%1").arg(*it);
		s+="\n";
	}

	return s;
}

//critical function here - must be optimized for speed
int ConstraintActivityPreferredRooms::fitness(
	SpaceChromosome& c,
	Rules& r,
	const int days[/*MAX_ACTIVITIES*/],
	const int hours[/*MAX_ACTIVITIES*/],
	QString* conflictsString)
{
	//if the matrix roomsMatrix is already calculated, do not calculate it again!
	if(crt_chrom!=&c || crt_rules!=&r){
		c.getRoomsMatrix(r, days, hours, roomsMatrix);

		crt_chrom = &c;
		crt_rules = &r;
	}

	//Calculates the number of conflicts

	int nbroken;

	//without logging
if(conflictsString==NULL){
	nbroken=0;
	int parity=r.internalActivitiesList[this->_activity].parity==PARITY_WEEKLY?2:1;

	int rm=c.rooms[this->_activity];
	if(rm!=UNALLOCATED_SPACE){
		int i;
		for(i=0; i<this->_n_preferred_rooms; i++)
			if(this->_rooms[i]==rm)
				break;
		if(i==this->_n_preferred_rooms)
			nbroken+=parity;
	}
}
	//with logging
else{
	nbroken=0;
	int parity=r.internalActivitiesList[this->_activity].parity==PARITY_WEEKLY?2:1;

	int rm=c.rooms[this->_activity];
	if(rm!=UNALLOCATED_SPACE){
		int i;
		for(i=0; i<this->_n_preferred_rooms; i++)
			if(this->_rooms[i]==rm)
				break;
		if(i==this->_n_preferred_rooms){
			if(conflictsString!=NULL){
				*conflictsString+=
					(QObject::tr("Space constraint activity preferred rooms broken for activity with id=%1")
					.arg(this->activityId));
				*conflictsString += ". ";
				*conflictsString += (QObject::tr("This increases the conflicts factor with %1").arg(weight*parity));
				*conflictsString += "\n";
			}

			nbroken+=parity;
		}
	}
}

	return int (ceil ( weight * nbroken ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

ConstraintActivitiesSameRoom::ConstraintActivitiesSameRoom()
	: SpaceConstraint()
{
	type=CONSTRAINT_ACTIVITIES_SAME_ROOM;
}

ConstraintActivitiesSameRoom::ConstraintActivitiesSameRoom(double w, bool c, int nact, const int act[])
 : SpaceConstraint(w, c)
 {
	assert(nact>=2 && nact<=MAX_CONSTRAINT_ACTIVITIES_SAME_ROOM);
	this->n_activities=nact;
	for(int i=0; i<nact; i++)
		this->activitiesId[i]=act[i];

	this->type=CONSTRAINT_ACTIVITIES_SAME_ROOM;
}

void ConstraintActivitiesSameRoom::computeInternalStructure(Rules &r)
{
	//compute the indices of the activities,
	//based on their unique ID

	for(int j=0; j<n_activities; j++)
		this->_activities[j]=-1;

	for(int i=0; i<this->n_activities; i++){
		int j;
		Activity* act;
		for(j=0, act=r.activitiesList.first(); act; act=r.activitiesList.next(), j++)
			if(act->id==this->activitiesId[i])
				this->_activities[i]=j;
	}

	for(int j=0; j<this->n_activities; j++)
		assert(this->_activities[j]>=0);
}

void ConstraintActivitiesSameRoom::removeUseless(Rules& r)
{
	//remove the activitiesId which no longer exist (used after the deletion of an activity)

	for(int j=0; j<this->n_activities; j++)
		this->_activities[j]=-1;

	for(int i=0; i<this->n_activities; i++){
		int j;
		Activity* act;
		for(j=0, act=r.activitiesList.first(); act; act=r.activitiesList.next(), j++)
			if(act->id==this->activitiesId[i])
				this->_activities[i]=j;
	}

	int i, j;
	for(i=0, j=0; j<this->n_activities; j++)
		if(this->_activities[j]>=0) //valid activity
			this->activitiesId[i++]=this->activitiesId[j];
	this->n_activities=i;
}

QString ConstraintActivitiesSameRoom::getXMLDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s="<ConstraintActivitiesSameRoom>\n";
	s+="	<Weight>"+QString::number(this->weight)+"</Weight>\n";
	s+="	<Compulsory>";s+=yesNo(this->compulsory);s+="</Compulsory>\n";
	s+="	<Number_of_Activities>"+QString::number(this->n_activities)+"</Number_of_Activities>\n";
	for(int i=0; i<this->n_activities; i++)
		s+="	<Activity_Id>"+QString::number(this->activitiesId[i])+"</Activity_Id>\n";
	s+="</ConstraintActivitiesSameRoom>\n";
	return s;
}

QString ConstraintActivitiesSameRoom::getDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s;
	s+=QObject::tr("Activities same room");s+=", ";
	s+=(QObject::tr("W:%1").arg(this->weight));s+=", ";
	s+=(QObject::tr("C:%1").arg(yesNoTranslated(this->compulsory)));s+=", ";
	s+=(QObject::tr("NA:%1").arg(this->n_activities));s+=", ";
	for(int i=0; i<this->n_activities; i++){
		s+=(QObject::tr("ID:%1").arg(this->activitiesId[i]));s+=", ";
	}

	return s;
}

QString ConstraintActivitiesSameRoom::getDetailedDescription(Rules& r){
	if(&r!=NULL)
		;

	QString s;
	
	s=QObject::tr("Space constraint");s+="\n";
	s+=QObject::tr("Activities must have the same room");s+="\n";
	s+=(QObject::tr("Weight=%1").arg(this->weight));s+="\n";
	s+=(QObject::tr("Compulsory=%1").arg(yesNoTranslated(this->compulsory)));s+="\n";
	s+=(QObject::tr("Number of activities=%1").arg(this->n_activities));s+="\n";
	for(int i=0; i<this->n_activities; i++){
		s+=(QObject::tr("Activity with id=%1").arg(this->activitiesId[i]));s+="\n";
	}

	return s;
}

//critical function here - must be optimized for speed
int ConstraintActivitiesSameRoom::fitness(
	SpaceChromosome& c,
	Rules& r,
	const int days[/*MAX_ACTIVITIES*/],
	const int hours[/*MAX_ACTIVITIES*/],
	QString* conflictsString)
{
	assert(r.internalStructureComputed);

	//if the matrices roomsMatrix is already calculated, do not calculate it again!
	if(crt_chrom!=&c || crt_rules!=&r){
		c.getRoomsMatrix(r, days, hours, roomsMatrix);

		crt_chrom = &c;
		crt_rules = &r;
	}

	int nbroken;

	//without logging
	if(conflictsString==NULL){
		nbroken=0;
		for(int i=1; i<this->n_activities; i++){
			int t1=c.rooms[this->_activities[i]];
			if(t1!=UNALLOCATED_SPACE){
				for(int j=0; j<i; j++){
					int t2=c.rooms[this->_activities[j]];
					if(t2!=UNALLOCATED_SPACE){
						int tmp=0;

						//activity weekly - counts as double
						if(r.internalActivitiesList[this->_activities[i]].parity==PARITY_WEEKLY &&
						 r.internalActivitiesList[this->_activities[j]].parity==PARITY_WEEKLY)
							tmp = 4;
						else if(r.internalActivitiesList[this->_activities[i]].parity==PARITY_WEEKLY ||
						 r.internalActivitiesList[this->_activities[j]].parity==PARITY_WEEKLY)
							tmp = 2;
						else
							tmp = 1;

						nbroken+=tmp;
					}
				}
			}
		}
	}
	//with logging
	else{
		nbroken=0;
		for(int i=1; i<this->n_activities; i++){
			int t1=c.rooms[this->_activities[i]];
			if(t1!=UNALLOCATED_SPACE){
				for(int j=0; j<i; j++){
					int t2=c.rooms[this->_activities[j]];
					if(t2!=UNALLOCATED_SPACE){
						int tmp=0;

						//activity weekly - counts as double
						if(r.internalActivitiesList[this->_activities[i]].parity==PARITY_WEEKLY &&
						 r.internalActivitiesList[this->_activities[j]].parity==PARITY_WEEKLY)
							tmp = 4;
						else if(r.internalActivitiesList[this->_activities[i]].parity==PARITY_WEEKLY ||
						 r.internalActivitiesList[this->_activities[j]].parity==PARITY_WEEKLY)
							tmp = 2;
						else
							tmp = 1;

						nbroken+=tmp;

						if(tmp>0 && conflictsString!=NULL){
							*conflictsString+=(QObject::tr("Space constraint activities same room broken, because activity with id=%1 is not in the same room with activity with id=%2")
								.arg(this->activitiesId[i])
								.arg(this->activitiesId[j]));
							*conflictsString+=", ";
							*conflictsString+=(QObject::tr("conflicts factor increase=%1").arg(tmp*weight));
							*conflictsString+="\n";
						}
					}
				}
			}
		}
	}

	return int (ceil ( weight * nbroken ) );
}
