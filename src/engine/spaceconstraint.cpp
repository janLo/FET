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
	s += "	<Compulsory>yes</Compulsory>\n";
	s += "</ConstraintBasicCompulsorySpace>\n";
	return s;
}

QString ConstraintBasicCompulsorySpace::getDescription(Rules& r)
{
	if(&r!=NULL)
		;
	
	return "Basic compulsory constraints (space), W:"
		+ QString::number(this->weight);
}

QString ConstraintBasicCompulsorySpace::getDetailedDescription(Rules& r)
{
	if(&r!=NULL)
		;

	QString s="These are the basic compulsory constraints \n";
	s+="(referring to space allocation) for any timetable\n";
	s+="Weight="; s+=QString::number(this->weight); s+="\n";

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
				(*conflictsString) += "Space constraint basic compulsory: unallocated";
				(*conflictsString) += " activity with id=";
				(*conflictsString) += QString::number(r.internalActivitiesList[i].id);
				(*conflictsString) += " - this increases the fitness factor with ";
				(*conflictsString) += QString::number(weight* r.internalActivitiesList[i].duration *
					r.internalActivitiesList[i].nSubgroups * 10000);
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
	//which correspond to the subject of the constraint,
	//and also updates the matrix roomHasActivityEquipment.
	
	this->_nActivities=0;
	for(int ac=0; ac<r.nInternalActivities; ac++)
		if(r.internalActivitiesList[ac].subjectName == this->subjectName){
			assert(this->_nActivities<MAX_ACTIVITIES_FOR_A_SUBJECT);
			this->_activities[this->_nActivities++]=ac;
			
			for(int rm=0; rm<r.nInternalRooms; rm++){
				bool ok=true;
				//Check whether this room has all the necessary equipments
				for(QStringList::Iterator it=this->equipmentsNames.begin(); it!=this->equipmentsNames.end(); it++){
					bool found=r.internalRoomsList[rm]->searchEquipment(*it);
					if(!found){
						ok=false;
						break;
					}
				}
				
				if(this->compulsory==true)
					r.roomHasActivityEquipmentsCompulsory[rm][ac]=ok;
				else
					r.roomHasActivityEquipmentsNonCompulsory[rm][ac]=ok;
			}
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

	//Calculates the number of conflicts
	//The fastest way seems to iterate over all activities
	//involved in this constraint (share the subject of this constraint),
	//find the scheduled room and check to see if this
	//room is accepted or not (in the precalculated matrix Rules::roomHasActivityEquipments).

	int nbroken;

	//without logging
if(conflictsString==NULL){
	nbroken=0;
	for(int i=0; i<this->_nActivities; i++){
		int ac=this->_activities[i];
		int rm=c.rooms[ac];
		if(rm==UNALLOCATED_SPACE)
			continue;

		if(this->compulsory){
			if(r.roomHasActivityEquipmentsCompulsory[rm][ac] == false)
				nbroken++;
		}
		else{
			if(r.roomHasActivityEquipmentsNonCompulsory[rm][ac] == false)
				nbroken++;
		}
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

		if(this->compulsory){
			if(r.roomHasActivityEquipmentsCompulsory[rm][ac] == false){
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
		else{
			if(r.roomHasActivityEquipmentsNonCompulsory[rm][ac] == false){
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
