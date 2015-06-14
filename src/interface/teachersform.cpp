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
#include "fet.h"
#include "teachersform.h"
#include "fetmainform.h"
#include "teacher.h"

#include <qlistbox.h>
#include <qinputdialog.h>

TeachersForm::TeachersForm()
 : TeachersForm_template()
{
	teachersListBox->clear();
	for(Teacher* tch=gt.rules.teachersList.first(); tch; tch=gt.rules.teachersList.next())
		teachersListBox->insertItem(tch->name);
}


TeachersForm::~TeachersForm()
{
}

void TeachersForm::addTeacher()
{
	bool ok = FALSE;
	Teacher* tch=new Teacher();
	tch->name = QInputDialog::getText( QObject::tr("User input"), QObject::tr("Please enter teacher's name") ,
                    QLineEdit::Normal, QString::null, &ok, this );

	if ( ok && !((tch->name).isEmpty()) ){
		// user entered something and pressed OK
		if(!gt.rules.addTeacher(tch)){
			QMessageBox::information( this, QObject::tr("Teacher insertion dialog"),
				QObject::tr("Could not insert item. Must be a duplicate"));
			delete tch;
		}
		else{
			teachersListBox->insertItem(tch->name);
		}
	}
	else
		delete tch;// user entered nothing or pressed Cancel
}

void TeachersForm::removeTeacher()
{
	if(teachersListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected teacher"));
		return;
	}

	QString text=teachersListBox->currentText();
	int teacher_ID=gt.rules.searchTeacher(text);
	if(teacher_ID<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected teacher"));
		return;
	}

	if(QMessageBox::warning( this, QObject::tr("FET"),
		QObject::tr("Are you sure you want to delete this teacher and all related activities and constraints?\n"),
		QObject::tr("Yes"), QObject::tr("No"), 0, 0, 1 ) == 1)
		return;

	int tmp=gt.rules.removeTeacher(text);
	if(tmp)
		teachersListBox->removeItem(teachersListBox->currentItem());
	this->show();
}

void TeachersForm::renameTeacher()
{
	if(teachersListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected teacher"));
		return;
	}

	QString initialTeacherName=teachersListBox->currentText();
	int teacher_ID=gt.rules.searchTeacher(initialTeacherName);
	if(teacher_ID<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected teacher"));
		return;
	}

	bool ok = FALSE;
	QString finalTeacherName;
	finalTeacherName = QInputDialog::getText( QObject::tr("User input"), QObject::tr("Please enter new teacher's name") ,
                    QLineEdit::Normal, QString::null, &ok, this );

	if ( ok && !(finalTeacherName.isEmpty())){
		// user entered something and pressed OK
		if(gt.rules.searchTeacher(finalTeacherName)>=0){
			QMessageBox::information( this, QObject::tr("Teacher insertion dialog"),
				QObject::tr("Could not modify item. New name must be a duplicate"));
		}
		else{
			gt.rules.modifyTeacher(initialTeacherName, finalTeacherName);
		}
	}
	
	teachersListBox->clear();
	for(Teacher* tch=gt.rules.teachersList.first(); tch; tch=gt.rules.teachersList.next())
		teachersListBox->insertItem(tch->name);
}

void TeachersForm::sortTeachers()
{
	gt.rules.sortTeachersAlphabetically();

	teachersListBox->clear();
	for(Teacher* tch=gt.rules.teachersList.first(); tch; tch=gt.rules.teachersList.next())
		teachersListBox->insertItem(tch->name);
}
