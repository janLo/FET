/***************************************************************************
                          timetableallocateroomsform.cpp  -  description
                             -------------------
    begin                : Tue Apr 22 2003
    copyright            : (C) 2003 by Lalescu Liviu
    email                : liviu@lalescu.ro
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "timetableallocateroomsform.h"
#include "fetmainform.h"
#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fet.h"

#include <qcombobox.h>
#include <qmessagebox.h>
#include <qgroupbox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qtable.h>
#include <qapplication.h>
#include <qtextedit.h>
#include <qstring.h>
#include <qtextstream.h>
#include <qfile.h>

#include <iostream>
#include <fstream>
using namespace std;

static QMutex mutex;
static SpaceSolvingThread spaceSolvingThread;

extern QApplication *pqapplication;

//Represents the current state - simulation running or stopped.
static bool simulation_running;

//Communication variables when the simulation is running.
static bool simulation_stopped=false;
static bool simulation_paused=false;
static bool simulation_write_results=false;
static bool simulation_save_position=false;

extern bool students_schedule_ready;
extern bool teachers_schedule_ready;
extern bool rooms_schedule_ready;

extern QString spaceConflictsString;

TimetableAllocateRoomsForm::TimetableAllocateRoomsForm()
{
	simulation_running=false;
}

TimetableAllocateRoomsForm::~TimetableAllocateRoomsForm()
{
}

SpaceSolvingThread::SpaceSolvingThread()
{
	callingForm=NULL;
}

void SpaceSolvingThread::run()
{
	assert(callingForm!=NULL);

	pqapplication->lock();

	callingForm->startPushButton->setDisabled(TRUE);
	callingForm->stopPushButton->setEnabled(TRUE);
	callingForm->pausePushButton->setEnabled(TRUE);
	//callingForm->writeResultsPushButton->setEnabled(TRUE);
	callingForm->savePositionPushButton->setEnabled(TRUE);
	callingForm->loadPositionPushButton->setDisabled(TRUE);
	callingForm->initializeUnallocatedPushButton->setDisabled(TRUE);
	callingForm->initializeRandomlyPushButton->setDisabled(TRUE);
	callingForm->closePushButton->setDisabled(TRUE);

	pqapplication->unlock();

	callingForm->simulationRunning();

	pqapplication->lock();

	callingForm->startPushButton->setEnabled(TRUE);
	callingForm->stopPushButton->setDisabled(TRUE);
	callingForm->pausePushButton->setDisabled(TRUE);
	//callingForm->writeResultsPushButton->setDisabled(TRUE);
	callingForm->savePositionPushButton->setDisabled(TRUE);
	callingForm->loadPositionPushButton->setEnabled(TRUE);
	callingForm->initializeUnallocatedPushButton->setEnabled(TRUE);
	callingForm->initializeRandomlyPushButton->setEnabled(TRUE);
	callingForm->closePushButton->setEnabled(TRUE);

	pqapplication->unlock();
}

void TimetableAllocateRoomsForm::generationLogging(int generation){
	assert(gt.rules.initialized && gt.rules.internalStructureComputed);
	assert(gt.spacePopulation.initialized);

	SpaceChromosome& c1=gt.spacePopulation.bestChromosome(gt.rules);
	SpaceChromosome& c2=gt.spacePopulation.worstChromosome(gt.rules);
	SpaceChromosome& c3=gt.spacePopulation.medianChromosome(gt.rules);
	double th=gt.spacePopulation.totalHardFitness(gt.rules);
	double ts=gt.spacePopulation.totalSoftFitness(gt.rules);

	//write to log file
	logg<<"Generation number "<<generation+1<<endl;

	logg<<"      Best chromosome:";
	logg<<" HardFitness="<<c1.hardFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours);
	logg<<", SoftFitness="<<c1.softFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours)<<endl;

	logg<<"    Median chromosome:";
	logg<<" HardFitness="<<c3.hardFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours);
	logg<<", SoftFitness="<<c3.softFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours)<<endl;

	logg<<"     Worst chromosome:";
	logg<<" HardFitness="<<c2.hardFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours);
	logg<<", SoftFitness="<<c2.softFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours)<<endl;

	logg<<"    Medium HardFitness="<<th/gt.spacePopulation.n;
	logg<<", Medium SoftFitness="<<ts/gt.spacePopulation.n;

	logg<<endl<<flush;

	//write to display
	cout<<"Generation number "<<generation+1<<endl;

	cout<<"      Best chromosome:";
	cout<<" HardFitness="<<c1.hardFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours);
	cout<<", SoftFitness="<<c1.softFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours)<<endl;

	cout<<"    Median chromosome:";
	cout<<" HardFitness="<<c3.hardFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours);
	cout<<", SoftFitness="<<c3.softFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours)<<endl;

	cout<<"     Worst chromosome:";
	cout<<" HardFitness="<<c2.hardFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours);
	cout<<", SoftFitness="<<c2.softFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours)<<endl;

	cout<<"    Medium HardFitness="<<th/gt.spacePopulation.n;
	cout<<", Medium SoftFitness="<<ts/gt.spacePopulation.n;

	cout<<endl<<flush;

	//write to the Qt interface
	QString s;
	s = "Generation:";
	s+=QString::number(generation+1)+"\n";
	s+="Compulsory constraints conflicts:";
	s+=QString::number(c1.hardFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours))+"\n";
	s+="Non-compulsory constraints conflicts:";
	s+=QString::number(c1.softFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours));
	pqapplication->lock();
	currentResultsTextEdit->setText(s);
	currentResultsTextEdit->repaint();
	pqapplication->unlock();
}

void TimetableAllocateRoomsForm::simulationRunning(){
	simulation_running=true;

	pqapplication->lock();
#ifdef WIN32
	QMessageBox::information(NULL, QObject::tr("FET information"),
		QObject::tr("Entering simulation..."));
#else
	QMessageBox::information(this, QObject::tr("FET information"),
		QObject::tr("Entering simulation..."));
#endif
	pqapplication->unlock();

	//To print current time (starting time)
	time_t ltime;
	tzset();
	/* Get UNIX-style time and display as number and string. */
	time( &ltime );

	//open log file
	QString s=OUTPUT_DIR;
	s+=FILE_SEP;
	s+=SPACE_LOG_FILENAME_TXT;

	logg.open((const char*)s);

	logg<<"Population number="<<gt.spacePopulation.populationNumber()<<endl;
	logg<<"Current time is: "<<ctime( &ltime )<<endl<<endl;

	for(int i=0; i<max_generations; i++){
		if(evolution_method==1){
			gt.spacePopulation.evolve1(gt.rules);
		}
		else if(evolution_method==2){
			gt.spacePopulation.evolve2(gt.rules);
		}
		else{
			QMessageBox::critical(this, QObject::tr("FET information"),
				QObject::tr("error in timetableallocateroomsform.cpp (1)"));
			assert(0);
		}

		pqapplication->lock();
		generationLogging(i);
		pqapplication->unlock();

		time_t tmp;
		time( &tmp );

		if(tmp-ltime>=timelimit){
			break;
		}

		mutex.lock();
		if(simulation_stopped){
			mutex.unlock();
			break;
		}
		else
			mutex.unlock();

		mutex.lock();
		if(simulation_paused){
			simulation_paused=0;
			mutex.unlock();
			pqapplication->lock();
#ifdef WIN32
			QMessageBox::information(NULL, QObject::tr("FET information"),
				QObject::tr("Simulation paused.\nPress button to continue."));
#else
			QMessageBox::information(this, QObject::tr("FET information"),
				QObject::tr("Simulation paused.\nPress button to continue."));
#endif
			pqapplication->unlock();
		}
		else
			mutex.unlock();

		mutex.lock();
		if(simulation_save_position){
			simulation_save_position=false;
			gt.spacePopulation.write(gt.rules, OUTPUT_DIR+FILE_SEP+"space_population_state.txt");
			mutex.unlock();
			pqapplication->lock();
#ifdef WIN32
			QMessageBox::information(NULL, QObject::tr("FET information"),
				QObject::tr("Simulation position saved to hard disk.\nPress button to continue."));
#else
			QMessageBox::information(this, QObject::tr("FET information"),
				QObject::tr("Simulation position saved to hard disk.\nPress button to continue."));
#endif
			pqapplication->unlock();
		}
		else
			mutex.unlock();

		mutex.lock();
		if(simulation_write_results){
			simulation_write_results=false;
			pqapplication->lock();

			SpaceChromosome &c=gt.spacePopulation.bestChromosome(gt.rules);
			getRoomsTimetable(c);

			//update the string representing the conflicts
			spaceConflictsString = "";
			spaceConflictsString += "COMPULSORY CONSTRAINTS CONFLICTS (more important):\n";
			c.hardFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours, &spaceConflictsString);
			spaceConflictsString += "\n--------------------------\n\n";
			spaceConflictsString += "NON-COMPULSORY CONSTRAINTS CONFLICTS (less important):\n";
			c.softFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours, &spaceConflictsString);

			writeSimulationResults(c);

			pqapplication->unlock();
			mutex.unlock();
		}
		else
			mutex.unlock();
	}

	//Print ending time
	time( &ltime );
	logg<<endl<<"Current time is: "<<ctime( &ltime )<<endl;

	logg.close();

	SpaceChromosome &c=gt.spacePopulation.bestChromosome(gt.rules);

	mutex.lock(); //needed because of the updateStudentsTimetable
	pqapplication->lock();

	getRoomsTimetable(c);
	
	//update the string representing the conflicts
	spaceConflictsString = "";
	spaceConflictsString += "COMPULSORY CONSTRAINTS CONFLICTS (more important):\n";
	c.hardFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours, &spaceConflictsString);
	spaceConflictsString += "\n--------------------------\n\n";
	spaceConflictsString += "NON-COMPULSORY CONSTRAINTS CONFLICTS (less important):\n";
	c.softFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours, &spaceConflictsString);

	writeSimulationResults(c);

	pqapplication->unlock();
	mutex.unlock();

	simulation_running=false;
}

void TimetableAllocateRoomsForm::writeSimulationResults(SpaceChromosome &c){
	if(&c!=NULL)
		;

	assert(gt.rules.initialized && gt.rules.internalStructureComputed);
	assert(gt.spacePopulation.initialized);
	assert(students_schedule_ready && teachers_schedule_ready);
	assert(rooms_schedule_ready);

	QString s;

	//write the space conflicts - in txt mode
	s=OUTPUT_DIR+FILE_SEP+SPACE_CONFLICTS_FILENAME;
	QFile file(s);
	if(!file.open(IO_WriteOnly))
		assert(0);
	QTextStream tos(&file);
	tos<<spaceConflictsString<<endl;
	file.close();
	//now write the solution in xml files
	//rooms
	s=OUTPUT_DIR+FILE_SEP+ROOMS_TIMETABLE_FILENAME_XML;
	writeRoomsTimetable(s);

	cout<<"Writing simulation results to disk completed successfully"<<endl;
}

void TimetableAllocateRoomsForm::getRoomsTimetable(SpaceChromosome &c){
	assert(gt.spacePopulation.initialized);
	assert(gt.rules.initialized && gt.rules.internalStructureComputed);

	//assert(c.HFitness()==0); - for perfect solutions
	c.getRoomsTimetable(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours, rooms_timetable_week1, rooms_timetable_week2);
	rooms_schedule_ready=true;
}

/**
Function writing the rooms' timetable xml format to a file
*/
void TimetableAllocateRoomsForm::writeRoomsTimetable(const QString& xmlfilename)
{
	assert(gt.rules.initialized && gt.rules.internalStructureComputed);
	assert(gt.spacePopulation.initialized);
	assert(students_schedule_ready && teachers_schedule_ready);
	assert(rooms_schedule_ready);

	//Writing the timetable in xml format
	QFile file(xmlfilename);
	if(!file.open(IO_WriteOnly))
		assert(0);
	QTextStream tos(&file);
	tos << "<" << ROOMS_TIMETABLE_TAG << ">\n";

	for(int i=0; i<gt.rules.nInternalRooms; i++){
		tos<<"\n";
		tos << "  <Room name=\"" << gt.rules.internalRoomsList[i]->name << "\">\n";
		for(int k=0; k<gt.rules.nDaysPerWeek; k++){
			tos << "   <Day name=\"" << gt.rules.daysOfTheWeek[k] << "\">\n";
			for(int j=0; j<gt.rules.nHoursPerDay; j++){
				tos << "    <Hour name=\"" << gt.rules.hoursOfTheDay[j] << "\">\n";

				tos<<"     <Week1>";
				int ai=rooms_timetable_week1[i][k][j]; //activity index
				Activity* act=gt.rules.activitiesList.at(ai);
				if(ai!=UNALLOCATED_ACTIVITY)
					for(QStringList::Iterator it=act->studentsNames.begin(); it!=act->studentsNames.end(); it++)
						tos << "<Students name=\"" << *it << "\"></Students>";
				tos<<"</Week1>\n";

				tos<<"     <Week2>";
				ai=rooms_timetable_week2[i][k][j]; //activity index
				act=gt.rules.activitiesList.at(ai);
				if(ai!=UNALLOCATED_ACTIVITY)
					for(QStringList::Iterator it=act->studentsNames.begin(); it!=act->studentsNames.end(); it++)
						tos << "<Students name=\"" << *it << "\"></Students>";
				tos<<"</Week2>\n";

				tos << "    </Hour>\n";
			}
			tos << "   </Day>\n";
		}
		tos<<"  </Room>\n";
	}

	tos<<"\n";
	tos << "</" << ROOMS_TIMETABLE_TAG << ">\n";

	file.close();
}

void TimetableAllocateRoomsForm::start(){
	simulation_paused=false;
	simulation_stopped=false;
	simulation_save_position=false;

	if(!gt.rules.initialized || gt.rules.activitiesList.isEmpty()){
		QMessageBox::critical(this, QObject::tr("FET information"),
			QObject::tr("You have entered simulation with uninitialized rules or 0 activities...aborting"));
		assert(0);
		exit(1);
		return;
	}
	//if(!gt.rules.internalStructureComputed){
	if(!gt.spacePopulation.initialized){
		QMessageBox::information(this, QObject::tr("FET information"), 
			QObject::tr("You didn't initialize or load the initial state"));
		return;
	}

	if(!(students_schedule_ready && teachers_schedule_ready)){
		QMessageBox::information(this, QObject::tr("FET information"), 
			QObject::tr("You didn't allocate the hours prior to allocating the rooms"));
		return;
	}

	spaceSolvingThread.callingForm=this;
	spaceSolvingThread.start();
}

void TimetableAllocateRoomsForm::stop(){
	mutex.lock();
	simulation_stopped=true;
	mutex.unlock();

	QMessageBox::information(this, QObject::tr("FET information"),
		QObject::tr("Simulation completed successfully"));
}

void TimetableAllocateRoomsForm::pause(){
	mutex.lock();
	simulation_paused=true;
	mutex.unlock();
}

void TimetableAllocateRoomsForm::write(){
	if(simulation_running){
		mutex.lock();
		simulation_write_results=true;
		mutex.unlock();
	}
	else{
		SpaceChromosome &c=gt.spacePopulation.bestChromosome(gt.rules);
		getRoomsTimetable(c);
		writeSimulationResults(c);

		//update the string representing the conflicts
		spaceConflictsString = "";
		spaceConflictsString += "COMPULSORY CONSTRAINTS CONFLICTS (more important):\n";
		c.hardFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours, &spaceConflictsString);
		spaceConflictsString += "\n--------------------------\n\n";
		spaceConflictsString += "NON-COMPULSORY CONSTRAINTS CONFLICTS (less important):\n";
		c.softFitness(gt.rules, gt.spacePopulation.days, gt.spacePopulation.hours, &spaceConflictsString);
	}

	QMessageBox::information(this, QObject::tr("FET information"),
		QObject::tr("Simulation results should be successfully written. You may check now Timetable/View"));
}

void TimetableAllocateRoomsForm::savePosition()
{
	simulation_save_position=true;
}

void TimetableAllocateRoomsForm::loadPosition()
{
	if(!gt.rules.initialized || gt.rules.activitiesList.isEmpty()){
		QMessageBox::critical(this, QObject::tr("FET information"),
			QObject::tr("You have entered simulation with uninitialized rules or 0 activities...aborting"));
		assert(0);
		exit(1);
		return;
	}

	bool prev_state=gt.spacePopulation.initialized;
	gt.spacePopulation.init(gt.rules, population_number, gt.timePopulation.bestChromosome(gt.rules));
	bool existing_file=gt.spacePopulation.read(gt.rules, OUTPUT_DIR+FILE_SEP+"space_population_state.txt");
	if(existing_file==false){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("You did not save any internal state yet - aborting operation"));
		gt.spacePopulation.initialized=prev_state;
		return;
	}
	QMessageBox::information(this, QObject::tr("FET information"),
		QObject::tr("Simulation position restored from hard disk. You may now continue the simulation"));
	this->writeResultsPushButton->setEnabled(TRUE);
}

void TimetableAllocateRoomsForm::initializeUnallocated()
{
	if(!gt.rules.initialized){
		QMessageBox::critical(this, QObject::tr("FET information"),
			QObject::tr("You have entered simulation with uninitialized rules...aborting"));
		assert(0);
		exit(1);
		return;
	}
	if(gt.rules.activitiesList.isEmpty()){
		QMessageBox::critical(this, QObject::tr("FET information"),
			QObject::tr("You have entered simulation with 0 activities...aborting"));
		assert(0);
		exit(1);
		return;
	}

	assert(gt.rules.internalStructureComputed);
	assert(students_schedule_ready && teachers_schedule_ready);

	gt.spacePopulation.init(gt.rules, population_number, gt.timePopulation.bestChromosome(gt.rules));
	gt.spacePopulation.makeRoomsUnallocated(gt.rules);
	QMessageBox::information(this, QObject::tr("FET information"),
		QObject::tr("Initialized with unallocated data - now you can start simulation"));
	this->writeResultsPushButton->setEnabled(TRUE);
}

void TimetableAllocateRoomsForm::initializeRandomly()
{
	if(!gt.rules.initialized){
		QMessageBox::critical(this, QObject::tr("FET information"),
			QObject::tr("You have entered simulation with uninitialized rules...aborting"));
		assert(0);
		exit(1);
		return;
	}
	if(gt.rules.activitiesList.isEmpty()){
		QMessageBox::critical(this, QObject::tr("FET information"),
			QObject::tr("You have entered simulation with 0 activities...aborting"));
		assert(0);
		exit(1);
		return;
	}

	assert(gt.rules.internalStructureComputed);
	assert(students_schedule_ready && teachers_schedule_ready);

	gt.spacePopulation.init(gt.rules, population_number, gt.timePopulation.bestChromosome(gt.rules));
	gt.spacePopulation.makeRoomsRandom(gt.rules);
	QMessageBox::information(this, QObject::tr("FET information"),
		QObject::tr("Initialized with random data - now you can start simulation"));
	this->writeResultsPushButton->setEnabled(TRUE);
}
