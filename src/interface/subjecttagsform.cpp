//
//
// C++ Implementation: $MODULE$
//
// Description:
//
//
// Author: Lalescu Liviu <liviu@lalescu.ro>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "genetictimetable_defs.h"
#include "genetictimetable.h"
#include "fet.h"
#include "subjecttagsform.h"
#include "fetmainform.h"
#include "studentsset.h"
#include "teacher.h"
#include "subject.h"
#include "subjecttag.h"

#include <qlistbox.h>
#include <qinputdialog.h>

SubjectTagsForm::SubjectTagsForm()
 : SubjectTagsForm_template()
{
	subjectTagsListBox->clear();
	for(SubjectTag* sbt=gt.rules.subjectTagsList.first(); sbt; sbt=gt.rules.subjectTagsList.next())
		subjectTagsListBox->insertItem(sbt->name);
}


SubjectTagsForm::~SubjectTagsForm()
{
}

void SubjectTagsForm::addSubjectTag()
{
	bool ok = FALSE;
	SubjectTag* sbt=new SubjectTag();
	sbt->name = QInputDialog::getText( QObject::tr("User input"), QObject::tr("Please enter subject tag's name") ,
                    QLineEdit::Normal, QString::null, &ok, this );

	if ( ok && !((sbt->name).isEmpty()) ){
		// user entered something and pressed OK
		if(!gt.rules.addSubjectTag(sbt)){
			QMessageBox::information( this, QObject::tr("Subject tag insertion dialog"),
				QObject::tr("Could not insert item. Must be a duplicate"));
			delete sbt;
		}
		else{
			subjectTagsListBox->insertItem(sbt->name);
		}
	}
	else
		delete sbt;// user entered nothing or pressed Cancel
}

void SubjectTagsForm::removeSubjectTag()
{
	if(subjectTagsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected subject tag"));
		return;
	}

	QString text=subjectTagsListBox->currentText();
	int subject_tag_ID=gt.rules.searchSubjectTag(text);
	if(subject_tag_ID<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected subject tag"));
		return;
	}

	if(QMessageBox::warning( this, QObject::tr("FET"),
		QObject::tr("Are you sure you want to delete this subject tag?\n"),
		QObject::tr("Yes"), QObject::tr("No"), 0, 0, 1 ) == 1)
		return;

	int tmp=gt.rules.removeSubjectTag(text);
	if(tmp)
		subjectTagsListBox->removeItem(subjectTagsListBox->currentItem());
	this->show();
}

void SubjectTagsForm::renameSubjectTag()
{
	if(subjectTagsListBox->currentItem()<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected subject tag"));
		return;
	}
	
	QString initialSubjectTagName=subjectTagsListBox->currentText();

	int subject_tag_ID=gt.rules.searchSubjectTag(initialSubjectTagName);
	if(subject_tag_ID<0){
		QMessageBox::information(this, QObject::tr("FET information"), QObject::tr("Invalid selected subject tag"));
		return;
	}

	bool ok = FALSE;
	QString finalSubjectTagName;
	finalSubjectTagName = QInputDialog::getText( QObject::tr("User input"), QObject::tr("Please enter new subject tag's name") ,
                    QLineEdit::Normal, QString::null, &ok, this );

	if ( ok && !(finalSubjectTagName.isEmpty()) ){
		// user entered something and pressed OK
		if(gt.rules.searchSubjectTag(finalSubjectTagName)>=0){
			QMessageBox::information( this, QObject::tr("Subject tag insertion dialog"),
				QObject::tr("Could not modify item. New name must be a duplicate"));
		}
		else{
			gt.rules.modifySubjectTag(initialSubjectTagName, finalSubjectTagName);
		}
	}

	subjectTagsListBox->clear();
	for(SubjectTag* sbt=gt.rules.subjectTagsList.first(); sbt; sbt=gt.rules.subjectTagsList.next())
		subjectTagsListBox->insertItem(sbt->name);	
}

void SubjectTagsForm::sortSubjectTags()
{
	gt.rules.sortSubjectTagsAlphabetically();

	subjectTagsListBox->clear();
	for(SubjectTag* sbt=gt.rules.subjectTagsList.first(); sbt; sbt=gt.rules.subjectTagsList.next())
		subjectTagsListBox->insertItem(sbt->name);	
}
