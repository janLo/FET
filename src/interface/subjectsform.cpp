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
#include "subjectsform.h"
#include "fetmainform.h"
#include "studentsset.h"
#include "teacher.h"
#include "subject.h"

#include <qlistbox.h>
#include <qinputdialog.h>

SubjectsForm::SubjectsForm()
 : SubjectsForm_template()
{
	subjectsListBox->clear();
	for(Subject* sbj=gt.rules.subjectsList.first(); sbj; sbj=gt.rules.subjectsList.next())
		subjectsListBox->insertItem(sbj->name);
}


SubjectsForm::~SubjectsForm()
{
}

void SubjectsForm::addSubject()
{
	bool ok = FALSE;
	Subject* sbj=new Subject();
	sbj->name = QInputDialog::getText( QObject::tr("User input"), QObject::tr("Please enter subject's name") ,
                    QLineEdit::Normal, QString::null, &ok, this );

	if ( ok && !((sbj->name).isEmpty()) ){
		// user entered something and pressed OK
		if(!gt.rules.addSubject(sbj)){
			QMessageBox::information( this, QObject::tr("Subject insertion dialog"),
				QObject::tr("Could not insert item. Must be a duplicate"));
			delete sbj;
		}
		else{
			subjectsListBox->insertItem(sbj->name);
		}
	}
	else
		delete sbj;// user entered nothing or pressed Cancel
}

void SubjectsForm::removeSubject()
{
	if(subjectsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected subject"));
		return;
	}

	QString text=subjectsListBox->currentText();
	int subject_ID=gt.rules.searchSubject(text);
	if(subject_ID<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected subject"));
		return;
	}

	if(QMessageBox::warning( this, QObject::tr("FET"),
		QObject::tr("Are you sure you want to delete this subject and all related activities?\n"),
		QObject::tr("Yes"), QObject::tr("No"), 0, 0, 1 ) == 1)
		return;

	int tmp=gt.rules.removeSubject(text);
	if(tmp)
		subjectsListBox->removeItem(subjectsListBox->currentItem());
	this->show();
}
