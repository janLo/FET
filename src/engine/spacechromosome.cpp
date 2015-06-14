/*
File spacechromosome.cpp
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

#include <iostream>
using namespace std;

#include "genetictimetable_defs.h"
#include "spacechromosome.h"
#include "rules.h"
#include "spaceconstraint.h"

int better(Rules& r, SpaceChromosome& c1, SpaceChromosome& c2, const int days[/*MAX_ACTIVITIES*/], const int hours[/*MAX_ACTIVITIES*/]){ //returns true if c1 is better than c2
	//Here the order is important, you have to compute firstly the hard fitness, then the soft fitness
	int hf1=c1.hardFitness(r, days, hours);
	int sf1=c1.softFitness(r, days, hours);
	int hf2=c2.hardFitness(r, days, hours);
	int sf2=c2.softFitness(r, days, hours);

	return better(hf1, sf1, hf2, sf2);
}

//defined in timechromosome.cpp
/*int better(int hf1, int sf1, int hf2, int sf2){
	return hf1<hf2 || hf1==hf2 && sf1<sf2;
}*/

//critical function here - must be optimized for speed
void SpaceChromosome::copy(Rules& r, SpaceChromosome& c){
	this->_hardFitness=c._hardFitness;
	this->_softFitness=c._softFitness;

	assert(r.internalStructureComputed);

	for(int i=0; i<r.nInternalActivities; i++)
		this->rooms[i] = c.rooms[i];
	//memcpy(rooms, c.rooms, r.nActivities * sizeof(rooms[0]));
}

void SpaceChromosome::init(Rules& r){
	assert(r.internalStructureComputed);

	for(int i=0; i<r.nInternalActivities; i++)
		this->rooms[i]=UNALLOCATED_SPACE;

	this->_hardFitness=this->_softFitness=-1;
}

bool SpaceChromosome::read(Rules& r, const QString& filename){
	assert(r.initialized);

	QFile file(filename);
	if(!file.open(IO_ReadOnly))
		assert(0);
	QTextStream tis(&file);
	this->read(r, tis);
	file.close();

	return true;
}

bool SpaceChromosome::read(Rules &r, QTextStream &tis){
	assert(r.initialized);
	assert(r.internalStructureComputed);

	for(int i=0; i<r.nInternalActivities; i++){
		tis >> this->rooms[i];
		if(tis.eof()){
			//The rules and the solution do not match (1)
			return false;
		}

		if(this->rooms[i]>=r.nInternalRooms && this->rooms[i]!=UNALLOCATED_SPACE){
			//The rules and the solution do not match (2)
			return false;
		}
	}

	this->_hardFitness=this->_softFitness=-1;

	return true;
}

void SpaceChromosome::write(Rules& r, const QString &filename){
	assert(r.initialized);

	QFile file(filename);
	if(!file.open(IO_WriteOnly))
		assert(0);
	QTextStream tos(&file);
	this->write(r, tos);
	file.close();
}

void SpaceChromosome::write(Rules& r, QTextStream &tos){
	assert(r.initialized);
	assert(r.internalStructureComputed);

	for(int i=0; i<r.nInternalActivities; i++){
		tos << this->rooms[i] << endl;
	}
}

void SpaceChromosome::makeRoomsUnallocated(Rules& r){
	assert(r.initialized);
	assert(r.internalStructureComputed);

	for(int i=0; i<r.nInternalActivities; i++)
		this->rooms[i]=UNALLOCATED_SPACE;

	this->_hardFitness=this->_softFitness=-1;
}

void SpaceChromosome::makeRoomsRandom(Rules& r){
	assert(r.initialized);
	assert(r.internalStructureComputed);

	for(int i=0; i<r.nInternalActivities; i++)
		this->rooms[i] = rand()%r.nInternalRooms;

	this->_hardFitness = this->_softFitness = -1;
}


int SpaceChromosome::hardFitness(Rules& r, const int days[/*MAX_ACTIVITIES*/], const int hours[/*MAX_ACTIVITIES*/], QString* conflictsString){
	assert(r.initialized);
	assert(r.internalStructureComputed);

	if(this->_hardFitness>=0 && conflictsString==NULL)
	//If you want to see the log, you have to recompute the fitness, even if it is
	//already computed
		return this->_hardFitness;

	this->_hardFitness=0;
	for(int i=0; i<r.nInternalSpaceConstraints; i++)
		if(r.internalSpaceConstraintsList[i]->compulsory==true)
			this->_hardFitness += r.internalSpaceConstraintsList[i]->fitness(*this, r, days, hours, conflictsString);

	return this->_hardFitness;
}

int SpaceChromosome::softFitness(Rules& r, const int days[/*MAX_ACTIVITIES*/], const int hours[/*MAX_ACTIVITIES*/], QString* conflictsString){
	assert(r.initialized);
	assert(r.internalStructureComputed);

	if(this->_softFitness>=0 && conflictsString==NULL) //If you want to see the log, you have to recompute the fitness, even if it is already computed
		return this->_softFitness;

	this->_softFitness=0;
	for(int i=0; i<r.nInternalSpaceConstraints; i++)
		if(r.internalSpaceConstraintsList[i]->compulsory==false)
			this->_softFitness += r.internalSpaceConstraintsList[i]->fitness(*this, r, days, hours, conflictsString);

	return this->_softFitness;
}

//critical function here - must be optimized for speed
void SpaceChromosome::crossover(Rules& r, SpaceChromosome& c1, SpaceChromosome& c2){
	assert(r.internalStructureComputed);

	int q=rand()%(r.nInternalActivities+1);
	int i;
	for(i=0; i<q; i++)
		this->rooms[i]=c1.rooms[i];
	//memcpy(rooms, c1.rooms, q*sizeof(rooms[0]));
	for(; i<r.nInternalActivities; i++)
		this->rooms[i]=c2.rooms[i];
	//memcpy(rooms+q, c2.rooms+q, (r.nActivities-q)*sizeof(rooms[0]));

	this->_hardFitness = this->_softFitness = -1;
}

//critical function here - must be optimized for speed
void SpaceChromosome::mutate1(Rules& r){
	int p,q,k;
	
	assert(r.internalStructureComputed);

	p=rand()%r.nInternalActivities;

	do{
		q=rand()%r.nInternalActivities;
	}while(p==q); //I think this is the fastest solution
		//because, suppose we have 10% of allowed values for q. Then, the probability 
		//that this step is performed 10 times is (0.9)^10=0.34...
		//obviusly, 10% is very little, generally we deal with more than 90% allowed values
	k=this->rooms[p]; 
	this->rooms[p]=this->rooms[q]; 
	this->rooms[q]=k; //exchange the values

	this->_hardFitness = this->_softFitness = -1;
}

//critical function here - must be optimized for speed
void SpaceChromosome::mutate2(Rules& r)
{
	assert(r.internalStructureComputed);

	int p,k;
	p=rand()%r.nInternalActivities;
	k=rand()%r.nInternalRooms;
	this->rooms[p]=k;

	this->_hardFitness = this->_softFitness = -1;
}

int SpaceChromosome::getRoomsMatrix(
	Rules& r, 
	const int days[/*MAX_ACTIVITIES*/], 
	const int hours[/*MAX_ACTIVITIES*/], 
	int16 a[MAX_ROOMS][MAX_DAYS_PER_WEEK][MAX_HOURS_PER_DAY])
{
	assert(r.initialized);
	assert(r.internalStructureComputed);

	int conflicts=0;

	int i;
	for(i=0; i<r.nInternalRooms; i++)
		for(int j=0; j<r.nDaysPerWeek; j++)
			for(int k=0; k<r.nHoursPerDay; k++)
				a[i][j][k]=0;

	for(i=0; i<r.nInternalActivities; i++){
		int room=this->rooms[i];
		int hour = hours[i];
		int day = days[i];
		if(room!=UNALLOCATED_SPACE && hour!=UNALLOCATED_TIME && day!=UNALLOCATED_TIME) {
			Activity* act=&r.internalActivitiesList[i];
			for(int dd=0; dd<act->duration && hour+dd<r.nHoursPerDay; dd++){
				int tmp=a[room][day][hour+dd];
				if(act->parity==PARITY_WEEKLY){
					conflicts += tmp<2 ? tmp : 2;
					a[room][day][hour+dd]+=2;
				}
				else{
					assert(act->parity==PARITY_BIWEEKLY);
					conflicts += tmp<2 ? 0 : 1;
					a[room][day][hour+dd]++;
				}
			}
		}
	}

	return conflicts;
}

void SpaceChromosome::getRoomsTimetable(
	Rules& r,
	const int days[/*MAX_ACTIVITIES*/],
	const int hours[/*MAX_ACTIVITIES*/],
	int16 a1[MAX_ROOMS][MAX_DAYS_PER_WEEK][MAX_HOURS_PER_DAY],
	int16 a2[MAX_ROOMS][MAX_DAYS_PER_WEEK][MAX_HOURS_PER_DAY])
{
	assert(r.initialized);
	assert(r.internalStructureComputed);

	int i, j, k;
	for(i=0; i<r.nInternalRooms; i++)
		for(j=0; j<r.nDaysPerWeek; j++)
			for(k=0; k<r.nHoursPerDay; k++)
				a1[i][j][k]=a2[i][j][k]=UNALLOCATED_ACTIVITY;

	Activity *act;
	for(act=r.activitiesList.first(), i=0; act; act=r.activitiesList.next(), i++){
		int room=this->rooms[i];
		int day=days[i];
		int hour=hours[i];
		if(room!=UNALLOCATED_SPACE && day!=UNALLOCATED_TIME && hour!=UNALLOCATED_TIME){
			for(int dd=0; dd < act->duration && hour+dd < r.nHoursPerDay; dd++){
				if(a1[room][day][hour+dd]==UNALLOCATED_ACTIVITY)
					a1[room][day][hour+dd]=i;
				else
					a2[room][day][hour+dd]=i;
			}
		}
	}
}
