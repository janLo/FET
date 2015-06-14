/***************************************************************************
                          timetableallocatehoursform.cpp  -  description
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

#include "timetableallocatehoursform.h"
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

const bool SIMULATION_LOGGING=false; //warning: making "true" here slows down the program,
	//introducing an additional sorting if the evolution method is three-tournament.

#include <iostream>
#include <fstream>
using namespace std;

static QMutex mutex;
static TimeSolvingThread timeSolvingThread;

extern QApplication *pqapplication;

//Represents the current status of the simulation - running or stopped.
static bool simulation_running;

//Communication variables when the simulation is running.
static bool simulation_stopped=false;
static bool simulation_paused=false;
static bool simulation_write_results=false;
static bool simulation_save_position=false;

extern bool students_schedule_ready;
extern bool teachers_schedule_ready;

extern QString timeConflictsString;

TimetableAllocateHoursForm::TimetableAllocateHoursForm()
{
	simulation_running=false;
}

TimetableAllocateHoursForm::~TimetableAllocateHoursForm()
{
}

TimeSolvingThread::TimeSolvingThread()
{
	callingForm=NULL;
}

void TimeSolvingThread::run()
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

void TimetableAllocateHoursForm::generationLogging(int generation){
	assert(gt.rules.initialized && gt.rules.internalStructureComputed);
	assert(gt.timePopulation.initialized);

	TimeChromosome& c1=gt.timePopulation.bestChromosome(gt.rules);
	if(SIMULATION_LOGGING==true){
		TimeChromosome& c2=gt.timePopulation.worstChromosome(gt.rules);
		TimeChromosome& c3=gt.timePopulation.medianChromosome(gt.rules);
		double th=gt.timePopulation.totalHardFitness(gt.rules);
		double ts=gt.timePopulation.totalSoftFitness(gt.rules);
	
		//write to log file
		logg<<"Generation number "<<generation+1<<endl;

		logg<<"      Best chromosome:";
		logg<<" HardFitness="<<c1.hardFitness(gt.rules);
		logg<<", SoftFitness="<<c1.softFitness(gt.rules)<<endl;

		logg<<"    Median chromosome:";
		logg<<" HardFitness="<<c3.hardFitness(gt.rules);
		logg<<", SoftFitness="<<c3.softFitness(gt.rules)<<endl;

		logg<<"     Worst chromosome:";
		logg<<" HardFitness="<<c2.hardFitness(gt.rules);
		logg<<", SoftFitness="<<c2.softFitness(gt.rules)<<endl;

		logg<<"    Medium HardFitness="<<th/gt.timePopulation.n;
		logg<<", Medium SoftFitness="<<ts/gt.timePopulation.n;
	
		logg<<endl<<flush;

		//write to display
		cout<<"Generation number "<<generation+1<<endl;

		cout<<"      Best chromosome:";
		cout<<" HardFitness="<<c1.hardFitness(gt.rules);
		cout<<", SoftFitness="<<c1.softFitness(gt.rules)<<endl;

		cout<<"    Median chromosome:";
		cout<<" HardFitness="<<c3.hardFitness(gt.rules);
		cout<<", SoftFitness="<<c3.softFitness(gt.rules)<<endl;

		cout<<"     Worst chromosome:";
		cout<<" HardFitness="<<c2.hardFitness(gt.rules);
		cout<<", SoftFitness="<<c2.softFitness(gt.rules)<<endl;

		cout<<"    Medium HardFitness="<<th/gt.timePopulation.n;
		cout<<", Medium SoftFitness="<<ts/gt.timePopulation.n;

		cout<<endl<<flush;
	}

	//write to the Qt interface
	QString s;
	s = QObject::tr("Population number:"); s+=QString::number(gt.timePopulation.n); s+="\n";
	s += QObject::tr("Generation:"); s+=QString::number(generation+1)+"\n";
	s+=QObject::tr("Compulsory constraints conflicts:"); s+=QString::number(c1.hardFitness(gt.rules))+"\n";
	s+=QObject::tr("Non-compulsory constraints conflicts:"); s+=QString::number(c1.softFitness(gt.rules));
	pqapplication->lock();
	currentResultsTextEdit->setText(s);
	currentResultsTextEdit->repaint();
	pqapplication->unlock();
}

void TimetableAllocateHoursForm::simulationRunning(){
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
	s+=TIME_LOG_FILENAME_TXT;

	logg.open((const char*)s);

	logg<<"Population number="<<gt.timePopulation.populationNumber()<<endl;
	logg<<"Current time is: "<<ctime( &ltime )<<endl<<endl;

	for(int i=0; i<max_generations; i++){
		if(evolution_method==1){
			gt.timePopulation.evolve1(gt.rules);
		}
		else if(evolution_method==2){
			gt.timePopulation.evolve2(gt.rules);
		}
		else{
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
			gt.timePopulation.write(gt.rules, OUTPUT_DIR+FILE_SEP+"time_population_state.txt");
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

			TimeChromosome &c=gt.timePopulation.bestChromosome(gt.rules);

			getStudentsTimetable(c);
			getTeachersTimetable(c);

			//update the string representing the conflicts
			timeConflictsString = "";
			timeConflictsString += QObject::tr("COMPULSORY CONSTRAINTS CONFLICTS (more important):\n");
			c.hardFitness(gt.rules, &timeConflictsString);
			timeConflictsString += "\n--------------------------\n\n";
			timeConflictsString += QObject::tr("NON-COMPULSORY CONSTRAINTS CONFLICTS (less important):\n");
			c.softFitness(gt.rules, &timeConflictsString);

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

	TimeChromosome &c=gt.timePopulation.bestChromosome(gt.rules);

	mutex.lock(); //needed because of the updateStudentsTimetable
	pqapplication->lock();

	getStudentsTimetable(c);
	getTeachersTimetable(c);

	//update the string representing the conflicts
	timeConflictsString = "";
	timeConflictsString += QObject::tr("COMPULSORY CONSTRAINTS CONFLICTS (more important):\n");
	c.hardFitness(gt.rules, &timeConflictsString);
	timeConflictsString += "\n--------------------------\n\n";
	timeConflictsString += QObject::tr("NON-COMPULSORY CONSTRAINTS CONFLICTS (less important):\n");
	c.softFitness(gt.rules, &timeConflictsString);

	writeSimulationResults(c);

	pqapplication->unlock();
	mutex.unlock();

	simulation_running=false;
}

void TimetableAllocateHoursForm::writeSimulationResults(TimeChromosome &c){
	if(&c!=NULL)
		;	

	assert(gt.rules.initialized && gt.rules.internalStructureComputed);
	assert(gt.timePopulation.initialized);
	assert(students_schedule_ready && teachers_schedule_ready);

	QString s;

	//write the time conflicts - in txt mode
	s=OUTPUT_DIR+FILE_SEP+TIME_CONFLICTS_FILENAME;
	QFile file(s);
	if(!file.open(IO_WriteOnly))
		assert(0);
	QTextStream tos(&file);
	tos<<timeConflictsString<<endl;	
	file.close();
	//now write the solution in xml files
	//students
	s=OUTPUT_DIR+FILE_SEP+STUDENTS_TIMETABLE_FILENAME_XML;
	writeStudentsTimetable(s);
	//teachers
	s=OUTPUT_DIR+FILE_SEP+TEACHERS_TIMETABLE_FILENAME_XML;
	writeTeachersTimetable(s);

	cout<<"Writing simulation results to disk completed successfully"<<endl;
}

void TimetableAllocateHoursForm::getStudentsTimetable(TimeChromosome &c){
	assert(gt.timePopulation.initialized);
	assert(gt.rules.initialized && gt.rules.initialized);

	//assert(c.HFitness()==0); - for perfect solutions
	c.getSubgroupsTimetable(gt.rules, students_timetable_week1, students_timetable_week2);
	students_schedule_ready=true;
}

void TimetableAllocateHoursForm::getTeachersTimetable(TimeChromosome &c){
	assert(gt.timePopulation.initialized);
	assert(gt.rules.initialized && gt.rules.initialized);

	//assert(c.HFitness()==0); - for perfect solutions
	c.getTeachersTimetable(gt.rules, teachers_timetable_week1, teachers_timetable_week2);
	teachers_schedule_ready=true;
}

/**
Function writing the students' timetable in xml format to a file
*/
void TimetableAllocateHoursForm::writeStudentsTimetable(const QString& xmlfilename)
{
	assert(gt.rules.initialized && gt.rules.internalStructureComputed);
	assert(gt.timePopulation.initialized);
	assert(students_schedule_ready && teachers_schedule_ready);

	//Now we print the results to an XML file
	QFile file(xmlfilename);
	if(!file.open(IO_WriteOnly))
		assert(0);
	QTextStream tos(&file);
	tos<<"<"<<STUDENTS_TIMETABLE_TAG<<">\n";

	for(int subgroup=0; subgroup<gt.rules.nInternalSubgroups; subgroup++){
		tos<<"\n";
		tos<< "  <Subgroup name=\"";
		QString subgroup_name = gt.rules.internalSubgroupsList[subgroup]->name;
		tos<< subgroup_name << "\">\n";

		for(int k=0; k<gt.rules.nDaysPerWeek; k++){
			tos<<"   <Day name=\""<<gt.rules.daysOfTheWeek[k]<<"\">\n";
			for(int j=0; j<gt.rules.nHoursPerDay; j++){
				tos << "    <Hour name=\"" << gt.rules.hoursOfTheDay[j] << "\">\n";
				tos<<"     <Week1>";
				int ai=students_timetable_week1[subgroup][k][j]; //activity index
				if(ai!=UNALLOCATED_ACTIVITY){
					Activity* act=gt.rules.activitiesList.at(ai);
					for(QStringList::Iterator it=act->teachersNames.begin(); it!=act->teachersNames.end(); it++)
						tos<<" <Teacher name=\""<<(*it)<<"\"></Teacher>";
					tos<<"<Subject name=\""<<act->subjectName<<"\"></Subject>";
				}
				tos<<"</Week1>\n";
				tos<<"     <Week2>";
				ai=students_timetable_week2[subgroup][k][j]; //activity index
				if(ai!=UNALLOCATED_ACTIVITY){
					Activity* act=gt.rules.activitiesList.at(ai);
					for(QStringList::Iterator it=act->teachersNames.begin(); it!=act->teachersNames.end(); it++)
						tos<<" <Teacher name=\""<<(*it)<<"\"></Teacher>";
					tos<<"<Subject name=\""<<act->subjectName<<"\"></Subject>";
				}

				tos<<"</Week2>\n";
				tos << "    </Hour>\n";
			}
			tos<<"   </Day>\n";
		}
		tos<<"  </Subgroup>\n";
	}

	tos<<"\n";
	tos << "</" << STUDENTS_TIMETABLE_TAG << ">\n";

	file.close();
}

/**
Function writing the teachers' timetable xml format to a file
*/
void TimetableAllocateHoursForm::writeTeachersTimetable(const QString& xmlfilename)
{
	assert(gt.rules.initialized && gt.rules.internalStructureComputed);
	assert(gt.timePopulation.initialized);
	assert(students_schedule_ready && teachers_schedule_ready);

	//Writing the timetable in xml format
	QFile file(xmlfilename);
	if(!file.open(IO_WriteOnly))
		assert(0);
	QTextStream tos(&file);
	tos << "<" << TEACHERS_TIMETABLE_TAG << ">\n";

	for(int i=0; i<gt.rules.nInternalTeachers; i++){
		tos<<"\n";
		tos << "  <Teacher name=\"" << gt.rules.internalTeachersList[i]->name << "\">\n";
		for(int k=0; k<gt.rules.nDaysPerWeek; k++){
			tos << "   <Day name=\"" << gt.rules.daysOfTheWeek[k] << "\">\n";
			for(int j=0; j<gt.rules.nHoursPerDay; j++){
				tos << "    <Hour name=\"" << gt.rules.hoursOfTheDay[j] << "\">\n";

				tos<<"     <Week1>";
				int ai=teachers_timetable_week1[i][k][j]; //activity index
				Activity* act=gt.rules.activitiesList.at(ai);
				if(ai!=UNALLOCATED_ACTIVITY)
					for(QStringList::Iterator it=act->studentsNames.begin(); it!=act->studentsNames.end(); it++)
						tos << "<Students name=\"" << *it << "\"></Students>";
				tos<<"</Week1>\n";

				tos<<"     <Week2>";
				ai=teachers_timetable_week2[i][k][j]; //activity index
				act=gt.rules.activitiesList.at(ai);
				if(ai!=UNALLOCATED_ACTIVITY)
					for(QStringList::Iterator it=act->studentsNames.begin(); it!=act->studentsNames.end(); it++)
						tos << "<Students name=\"" << *it << "\"></Students>";
				tos<<"</Week2>\n";

				tos << "    </Hour>\n";
			}
			tos << "   </Day>\n";
		}
		tos<<"  </Teacher>\n";
	}

	tos<<"\n";
	tos << "</" << TEACHERS_TIMETABLE_TAG << ">\n";

	file.close();
}

void TimetableAllocateHoursForm::start(){
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

	if(!gt.timePopulation.initialized){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("You didn't initialize or load the initial state"));
		return;
	}

	timeSolvingThread.callingForm=this;
	timeSolvingThread.start();
}

void TimetableAllocateHoursForm::stop(){
	mutex.lock();
	simulation_stopped=true;
	mutex.unlock();

	QMessageBox::information(this, QObject::tr("FET information"),
		QObject::tr("Simulation completed successfully"));
}

void TimetableAllocateHoursForm::pause(){
	mutex.lock();
	simulation_paused=true;
	mutex.unlock();
}

void TimetableAllocateHoursForm::write(){
	if(simulation_running){
		mutex.lock();
		simulation_write_results=true;
		mutex.unlock();
	}
	else{
		TimeChromosome &c=gt.timePopulation.bestChromosome(gt.rules);
		getStudentsTimetable(c);
		getTeachersTimetable(c);
		writeSimulationResults(c);

		//update the string representing the conflicts
		timeConflictsString = "";
		timeConflictsString += QObject::tr("COMPULSORY CONSTRAINTS CONFLICTS (more important):\n");
		c.hardFitness(gt.rules, &timeConflictsString);
		timeConflictsString += "\n--------------------------\n\n";
		timeConflictsString += QObject::tr("NON-COMPULSORY CONSTRAINTS CONFLICTS (less important):\n");
		c.softFitness(gt.rules, &timeConflictsString);
	}

	QMessageBox::information(this, QObject::tr("FET information"),
		QObject::tr("Simulation results should be successfully written. You may check now Timetable/View"));
}

void TimetableAllocateHoursForm::savePosition()
{
	simulation_save_position=true;
}

void TimetableAllocateHoursForm::loadPosition()
{
	if(!gt.rules.initialized || gt.rules.activitiesList.isEmpty()){
		QMessageBox::critical(this, QObject::tr("FET information"),
			QObject::tr("You have entered simulation with uninitialized rules or 0 activities...aborting"));
		assert(0);
		exit(1);
		return;
	}

	gt.rules.computeInternalStructure();
	bool prev_state=gt.timePopulation.initialized;
	gt.timePopulation.init(gt.rules, population_number);
	bool existing_file=gt.timePopulation.read(gt.rules, OUTPUT_DIR+FILE_SEP+"time_population_state.txt");
	if(existing_file==false){
		QMessageBox::warning(this, QObject::tr("FET information"),
			QObject::tr("You did not save any internal state yet - aborting operation"));
		gt.timePopulation.initialized=prev_state;
		return;
	}
	QMessageBox::information(this, QObject::tr("FET information"),
		QObject::tr("Simulation position restored from hard disk. You may now continue the simulation"));
	this->writeResultsPushButton->setEnabled(TRUE);
}

void TimetableAllocateHoursForm::initializeUnallocated()
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

	if(!gt.rules.internalStructureComputed)
		gt.rules.computeInternalStructure();
	gt.timePopulation.init(gt.rules, population_number);

	gt.timePopulation.makeTimesUnallocated(gt.rules);
	QMessageBox::information(this, QObject::tr("FET information"),
		QObject::tr("Initialized with unallocated data - now you can start simulation"));
	this->writeResultsPushButton->setEnabled(TRUE);
}

void TimetableAllocateHoursForm::initializeRandomly()
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

	if(!gt.rules.internalStructureComputed)
		gt.rules.computeInternalStructure();
	gt.timePopulation.init(gt.rules, population_number);

	gt.timePopulation.makeTimesRandom(gt.rules);
	QMessageBox::information(this, QObject::tr("FET information"), 
		QObject::tr("Initialized with random data - now you can start simulation"));
	this->writeResultsPushButton->setEnabled(TRUE);
}

void TimetableAllocateHoursForm::closePressed()
{
	this->close();
}
