/*
File activity.cpp 
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
#include "activity.h"
#include "rules.h"

#include <qmessagebox.h>

Activity::Activity()
{
}

Activity::Activity(
	Rules& r,
	int _id,
	int _activityGroupId,
	const QStringList& _teachersNames,
	const QString& _subjectName,
	const QString& _subjectTagName,
	const QStringList& _studentsNames,
	int _duration,
	int _totalDuration,
	int _parity,
	bool _active)
{
	this->id=_id;
	this->activityGroupId=_activityGroupId;
	this->teachersNames = _teachersNames;
	this->subjectName = _subjectName;
	this->subjectTagName = _subjectTagName;
	this->studentsNames = _studentsNames;
	this->duration=_duration;
	this->totalDuration=_totalDuration;
	this->parity=_parity;
	this->active=_active;

	this->nTotalStudents=0;
	for(QStringList::Iterator it=this->studentsNames.begin(); it!=this->studentsNames.end(); it++){
		StudentsSet* ss=r.searchStudentsSet(*it);
		this->nTotalStudents += ss->numberOfStudents;
	}
}

bool Activity::operator==(Activity& a)
{
	if(this->teachersNames != a.teachersNames)
		return false;
	if(this->subjectName != a.subjectName)
		return false;
	if(this->subjectTagName != a.subjectTagName)
		return false;
	if(this->studentsNames != a.studentsNames)
		return false;
	if(this->duration != a.duration)
	    return false;
	if(this->parity != a.parity)
	    return false;
	return true;
}

bool Activity::searchTeacher(const QString& teacherName)
{
	return this->teachersNames.find(teacherName)!=this->teachersNames.end();
}

void Activity::removeTeacher(const QString& teacherName)
{
	this->teachersNames.remove(teacherName);
}

void Activity::renameTeacher(const QString& initialTeacherName, const QString& finalTeacherName)
{
	int t=0;
	for(QStringList::iterator it=this->teachersNames.begin(); it!=this->teachersNames.end(); it++)
		if((*it)==initialTeacherName){
			*it=finalTeacherName;
			t++;
		}
	assert(t<=1);
}

bool Activity::searchStudents(const QString& studentsName)
{
	return this->studentsNames.find(studentsName)!=this->studentsNames.end();
}

void Activity::removeStudents(const QString& studentsName)
{
	this->studentsNames.remove(studentsName);
}

void Activity::renameStudents(const QString& initialStudentsName, const QString& finalStudentsName)
{
	int t=0;
	for(QStringList::iterator it=this->studentsNames.begin(); it!=this->studentsNames.end(); it++)
		if((*it)==initialStudentsName){
			*it=finalStudentsName;
			t++;
		}
	assert(t<=1);
}

void Activity::computeInternalStructure(Rules& r)
{
	//the internal subgroups list must be computed before entering here.

	//teachers
	this->nTeachers=0;
	for(QStringList::Iterator it=this->teachersNames.begin(); it!=this->teachersNames.end(); it++){
		int tmp;
		for(tmp=0; tmp<r.nInternalTeachers; tmp++){
			if(r.internalTeachersList[tmp]->name == (*it))
				break;
		}
		assert(tmp < r.nInternalTeachers);
		assert(this->nTeachers<MAX_TEACHERS_PER_ACTIVITY);
		this->teachers[this->nTeachers++]=tmp;
	}

	//subjects
	this->subjectIndex = r.searchSubject(this->subjectName);
	assert(this->subjectIndex>=0);

	//subject tags
	this->subjectTagIndex = r.searchSubjectTag(this->subjectTagName);

	//students	
	this->nSubgroups=0;
	for(QStringList::Iterator it=this->studentsNames.begin(); it!=this->studentsNames.end(); it++){
		StudentsSet* ss=r.searchStudentsSet(*it);
		assert(ss);
		if(ss->type==STUDENTS_SUBGROUP){
			int tmp;
			for(tmp=0; tmp<=r.nInternalSubgroups; tmp++)
				if(r.internalSubgroupsList[tmp]->name == ss->name)
					break;
			assert(tmp<r.nInternalSubgroups);
			assert(this->nSubgroups<MAX_SUBGROUPS_PER_ACTIVITY);
			
			bool duplicate=false;
			for(int j=0; j<this->nSubgroups; j++)
				if(this->subgroups[j]==tmp)
					duplicate=true;
			if(duplicate){
				QString s;
				s=QObject::tr(QString("Warning: activity with id=%1\ncontains duplicated subgroups. Automatically correcting..."))
					.arg(this->id);
				QMessageBox::warning(NULL, QObject::tr("FET information"), s, QObject::tr("&Ok"));
			}
			else
				this->subgroups[this->nSubgroups++]=tmp;
		}
		else if(ss->type==STUDENTS_GROUP){
			StudentsGroup* stg=(StudentsGroup*)ss;
			for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next()){
				int tmp;
				for(tmp=0; tmp<=r.nInternalSubgroups; tmp++)
					if(r.internalSubgroupsList[tmp]->name == sts->name)
						break;
				assert(tmp<r.nInternalSubgroups);
				assert(this->nSubgroups<MAX_SUBGROUPS_PER_ACTIVITY);

				bool duplicate=false;
				for(int j=0; j<this->nSubgroups; j++)
					if(this->subgroups[j]==tmp)
						duplicate=true;
				if(duplicate){
					QString s;
					s=QObject::tr(QString("Warning: activity with id=%1\ncontains duplicated subgroups. Automatically correcting..."))
						.arg(this->id);
					QMessageBox::warning(NULL, QObject::tr("FET information"), s, QObject::tr("&Ok"));
				}
				else
					this->subgroups[this->nSubgroups++]=tmp;
			}
		}
		else if(ss->type==STUDENTS_YEAR){
			StudentsYear* sty=(StudentsYear*)ss;
			for(StudentsGroup* stg=sty->groupsList.first(); stg; stg=sty->groupsList.next()){
				for(StudentsSubgroup* sts=stg->subgroupsList.first(); sts; sts=stg->subgroupsList.next()){
					int tmp;
					for(tmp=0; tmp<=r.nInternalSubgroups; tmp++)
						if(r.internalSubgroupsList[tmp]->name == sts->name)
							break;
					assert(tmp<r.nInternalSubgroups);
					assert(this->nSubgroups<MAX_SUBGROUPS_PER_ACTIVITY);

					bool duplicate=false;
					for(int j=0; j<this->nSubgroups; j++)
						if(this->subgroups[j]==tmp)
							duplicate=true;
					if(duplicate){
						QString s;
						s=QObject::tr(QString("Warning: activity with id=%1\ncontains duplicated subgroups. Automatically correcting..."))
							.arg(this->id);
						QMessageBox::warning(NULL, QObject::tr("FET information"), s, QObject::tr("&Ok"));
					}
					else{
						this->subgroups[this->nSubgroups++]=tmp;
					}
				}
			}
		}
		else
			assert(0);
	}
}

QString Activity::getXmlDescription(Rules& r)
{
	if(&r==NULL){
	}

	QString s="<Activity>\n";
	for(QStringList::Iterator it=this->teachersNames.begin(); it!=this->teachersNames.end(); it++)
		s+="	<Teacher>" + protect(*it) + "</Teacher>\n";
	s+="	<Subject>"+protect(this->subjectName)+"</Subject>\n";
	s+="	<Subject_Tag>"+protect(this->subjectTagName)+"</Subject_Tag>\n";

	s+="	<Duration>"+QString::number(this->duration)+"</Duration>\n";
	s+="	<Total_Duration>"+QString::number(this->totalDuration)+"</Total_Duration>\n";
	s+="	<Id>"+QString::number(this->id)+"</Id>\n";
	s+="	<Activity_Group_Id>"+QString::number(this->activityGroupId)+"</Activity_Group_Id>\n";
	if(this->parity==PARITY_WEEKLY)
		s+="	<Weekly></Weekly>\n";
	else{
		assert(this->parity==PARITY_BIWEEKLY);
		s+="	<Biweekly></Biweekly>\n";
	}
	if(this->active==true)
		s+="	<Active>yes</Active>\n";
	else
		s+="	<Active>no</Active>\n";
	for(QStringList::Iterator it=this->studentsNames.begin(); it!=this->studentsNames.end(); it++)
		s+="	<Students>" + protect(*it) + "</Students>\n";

	s+="</Activity>";

	return s;
}

QString Activity::getDescription(Rules& r)
{
	if(&r==NULL){	
	}

	QString s;
	if(!this->isSplit())
		s=QObject::tr("Activity: ");
	else
		s=QObject::tr("Sub-activity: ");
	s+=QObject::tr("T:");
	for(QStringList::Iterator it=this->teachersNames.begin(); it!=this->teachersNames.end(); it++)
		s += *it + ", ";
	s+=QObject::tr("S:") + this->subjectName + ", ";
	if(this->subjectTagName!="")
		s+=QObject::tr("ST:") + this->subjectTagName + ", ";
	s+=QObject::tr("St:");
	for(QStringList::Iterator it=this->studentsNames.begin(); it!=this->studentsNames.end(); it++)
		s += *it + ", ";

	s += QObject::tr("Id:") + QString::number(id) + ", ";
	if(this->isSplit())
		s += QObject::tr("AGId:") + QString::number(this->activityGroupId) + ", ";

	s+=QObject::tr("D:") + QString::number(this->duration) + ", ";
	if(this->isSplit())
		s += QObject::tr("TD:") + QString::number(this->totalDuration) + ", ";

	if(this->parity==PARITY_BIWEEKLY)
		s+=QObject::tr("Bi-weekly, ");
		
	if(this->active==true)
		s+=QObject::tr("A: yes");
	else
		s+=QObject::tr("A: no");

	return s;
}

QString Activity::getDetailedDescription(Rules &r)
{
	QString s;
	if(!this->isSplit())
		s=QObject::tr("Activity:\n");
	else
		s=QObject::tr("Sub-activity:\n");
	for(QStringList::Iterator it=this->teachersNames.begin(); it!=this->teachersNames.end(); it++)
		s+=QObject::tr("Teacher=") + (*it) + "\n";
	s+=QObject::tr("Subject=") + this->subjectName + "\n";
	if(this->subjectTagName!="")
		s+=QObject::tr("Subject tag=") + this->subjectTagName + "\n";
	for(QStringList::Iterator it=this->studentsNames.begin(); it!=this->studentsNames.end(); it++)
		s += QObject::tr("Students=")+ (*it) + "\n";

	s += QObject::tr("Id=") + QString::number(id) + "\n";
	if(this->isSplit())
		s += QObject::tr("Activity group id=") + QString::number(this->activityGroupId) + "\n";

	s+=QObject::tr("Duration=") + QString::number(this->duration) + "\n";
	if(this->isSplit())
		s += QObject::tr("Total duration=") + QString::number(this->totalDuration) + "\n";

	if(this->parity==PARITY_BIWEEKLY)
		s+=QObject::tr("Bi-weekly activity\n");
	else
		s+=QObject::tr("Weekly activity\n");
		
	if(this->active==true)
		s+=QObject::tr("Active: yes\n");
	else
		s+=QObject::tr("Active: no\n");
		
	int nStud=0;
	for(QStringList::Iterator it=this->studentsNames.begin(); it!=this->studentsNames.end(); it++){
		StudentsSet* ss=r.searchStudentsSet(*it);
		nStud += ss->numberOfStudents;
	}
	s+=QObject::tr("Total number of students=%1").arg(this->nTotalStudents);
	s+="\n";

	return s;
}

QString Activity::getDetailedDescriptionWithConstraints(Rules &r)
{
	QString s=this->getDetailedDescription(r);

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Time constraints directly related to this activity:");
	s+="\n";
	for(TimeConstraint* c=r.timeConstraintsList.first(); c; c=r.timeConstraintsList.next()){
		if(c->isRelatedToActivity(this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}

	s+="--------------------------------------------------\n";
	s+=QObject::tr("Space constraints directly related to this activity:");
	s+="\n";
	for(SpaceConstraint* c=r.spaceConstraintsList.first(); c; c=r.spaceConstraintsList.next()){
		if(c->isRelatedToActivity(this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}
	s+="--------------------------------------------------\n";

	return s;
}

bool Activity::isSplit()
{
	return this->totalDuration != this->duration;
}
