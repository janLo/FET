/***************************************************************************
                          addactivityform.cpp  -  description
                             -------------------
    begin                : Wed Apr 23 2003
    copyright            : (C) 2003 by Lalescu Liviu
    email                : Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QMessageBox>

#include <cstdio>

#include <QDialog>

#include "longtextmessagebox.h"

#include "addactivityform.h"
#include "teacher.h"
#include "subject.h"
#include "studentsset.h"

#include "activityplanningform.h"

#include <QtGui>

#include <QLineEdit>

#include <QAbstractItemView>
#include <QModelIndex>

#include <QList>

/*QWidget* AddActivityForm::subTab(int i)
{
	assert(i>=0 && i<subactivitiesTabWidget->count());
	return subactivitiesTabWidget->widget(i);
}*/

QSpinBox* AddActivityForm::dur(int i)
{
	assert(i>=0 && i<durList.count());
	assert(i<MAX_SPLIT_OF_AN_ACTIVITY);
	return durList.at(i);
}

QCheckBox* AddActivityForm::activ(int i)
{
	assert(i>=0 && i<activList.count());
	assert(i<MAX_SPLIT_OF_AN_ACTIVITY);
	return activList.at(i);
}

AddActivityForm::AddActivityForm(const QString& teacherName, const QString& studentsSetName, const QString& subjectName, const QString& activityTagName)
{
    setupUi(this);

	splitSpinBox->setMaxValue(MAX_SPLIT_OF_AN_ACTIVITY);

	durList.clear();
	durList.append(duration1SpinBox);
	durList.append(duration2SpinBox);
	durList.append(duration3SpinBox);
	durList.append(duration4SpinBox);
	durList.append(duration5SpinBox);
	durList.append(duration6SpinBox);
	durList.append(duration7SpinBox);
	durList.append(duration8SpinBox);
	durList.append(duration9SpinBox);
	durList.append(duration10SpinBox);
	durList.append(duration11SpinBox);
	durList.append(duration12SpinBox);
	durList.append(duration13SpinBox);
	durList.append(duration14SpinBox);
	durList.append(duration15SpinBox);
	durList.append(duration16SpinBox);
	durList.append(duration17SpinBox);
	durList.append(duration18SpinBox);
	durList.append(duration19SpinBox);
	durList.append(duration20SpinBox);
	durList.append(duration21SpinBox);
	durList.append(duration22SpinBox);
	durList.append(duration23SpinBox);
	durList.append(duration24SpinBox);
	durList.append(duration25SpinBox);
	durList.append(duration26SpinBox);
	durList.append(duration27SpinBox);
	durList.append(duration28SpinBox);
	durList.append(duration29SpinBox);
	durList.append(duration30SpinBox);
	durList.append(duration31SpinBox);
	durList.append(duration32SpinBox);
	durList.append(duration33SpinBox);
	durList.append(duration34SpinBox);
	durList.append(duration35SpinBox);
	
	for(int i=0; i<MAX_SPLIT_OF_AN_ACTIVITY; i++)
		dur(i)->setMaxValue(gt.rules.nHoursPerDay);

	activList.clear();
	activList.append(active1CheckBox);
	activList.append(active2CheckBox);
	activList.append(active3CheckBox);
	activList.append(active4CheckBox);
	activList.append(active5CheckBox);
	activList.append(active6CheckBox);
	activList.append(active7CheckBox);
	activList.append(active8CheckBox);
	activList.append(active9CheckBox);
	activList.append(active10CheckBox);
	activList.append(active11CheckBox);
	activList.append(active12CheckBox);
	activList.append(active13CheckBox);
	activList.append(active14CheckBox);
	activList.append(active15CheckBox);
	activList.append(active16CheckBox);
	activList.append(active17CheckBox);
	activList.append(active18CheckBox);
	activList.append(active19CheckBox);
	activList.append(active20CheckBox);
	activList.append(active21CheckBox);
	activList.append(active22CheckBox);
	activList.append(active23CheckBox);
	activList.append(active24CheckBox);
	activList.append(active25CheckBox);
	activList.append(active26CheckBox);
	activList.append(active27CheckBox);
	activList.append(active28CheckBox);
	activList.append(active29CheckBox);
	activList.append(active30CheckBox);
	activList.append(active31CheckBox);
	activList.append(active32CheckBox);
	activList.append(active33CheckBox);
	activList.append(active34CheckBox);
	activList.append(active35CheckBox);

    connect(clearStudentsPushButton, SIGNAL(clicked()), this, SLOT(clearStudents()));
    connect(clearTeacherPushButton, SIGNAL(clicked()), this, SLOT(clearTeachers()));
    connect(subgroupsCheckBox, SIGNAL(toggled(bool)), this, SLOT(showSubgroupsChanged()));
    connect(groupsCheckBox, SIGNAL(toggled(bool)), this, SLOT(showGroupsChanged()));
    connect(yearsCheckBox, SIGNAL(toggled(bool)), this, SLOT(showYearsChanged()));
    connect(splitSpinBox, SIGNAL(valueChanged(int)), this, SLOT(splitChanged()));
    connect(subjectsComboBox, SIGNAL(activated(QString)), this, SLOT(subjectChanged(QString)));
    connect(closePushButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(addActivityPushButton, SIGNAL(clicked()), this, SLOT(addActivity()));
    connect(allTeachersListBox, SIGNAL(selected(QString)), this, SLOT(addTeacher()));
    connect(selectedTeachersListBox, SIGNAL(selected(QString)), this, SLOT(removeTeacher()));
    connect(allStudentsListBox, SIGNAL(selected(QString)), this, SLOT(addStudents()));
    connect(selectedStudentsListBox, SIGNAL(selected(QString)), this, SLOT(removeStudents()));
    connect(helpPushButton, SIGNAL(clicked()), this, SLOT(help()));
    connect(clearActivityTagPushButton, SIGNAL(clicked()), this, SLOT(clearActivityTags()));
    connect(allActivityTagsListBox, SIGNAL(selected(QString)), this, SLOT(addActivityTag()));
    connect(selectedActivityTagsListBox, SIGNAL(selected(QString)), this, SLOT(removeActivityTag()));
    connect(minDayDistanceSpinBox, SIGNAL(valueChanged(int)), this, SLOT(minDaysChanged()));

	/*setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	QDesktopWidget* desktop=QApplication::desktop();
	int xx=desktop->width()/2 - frameGeometry().width()/2;
	int yy=desktop->height()/2 - frameGeometry().height()/2;
	move(xx, yy);*/
	centerWidgetOnScreen(this);
	
	QSize tmp3=subjectsComboBox->minimumSizeHint();
	Q_UNUSED(tmp3);
						
	//updatePreferredDaysComboBox();
	//updatePreferredHoursComboBox();
	selectedStudentsListBox->clear();
	updateStudentsListBox();
	updateTeachersListBox();
	updateSubjectsComboBox();
	updateActivityTagsListBox();

	minDayDistanceSpinBox->setMaxValue(gt.rules.nDaysPerWeek);
	minDayDistanceSpinBox->setMinValue(0);
	minDayDistanceSpinBox->setValue(1);
	
	int nSplit=splitSpinBox->value();
	for(int i=0; i<MAX_SPLIT_OF_AN_ACTIVITY; i++)
		if(i<nSplit)
			subactivitiesTabWidget->setTabEnabled(i, true);
		else
			subactivitiesTabWidget->setTabEnabled(i, false);
			
	minDayDistanceTextLabel->setEnabled(nSplit>=2);
	minDayDistanceSpinBox->setEnabled(nSplit>=2);
	percentageTextLabel->setEnabled(nSplit>=2 && minDayDistanceSpinBox->value()>0);
	percentageLineEdit->setEnabled(nSplit>=2 && minDayDistanceSpinBox->value()>0);
	percentTextLabel->setEnabled(nSplit>=2 && minDayDistanceSpinBox->value()>0);
	forceAdjacentCheckBox->setEnabled(nSplit>=2 && minDayDistanceSpinBox->value()>0);
	
	subactivitiesTabWidget->setCurrentIndex(0);
	
	nStudentsSpinBox->setMinValue(-1);
	nStudentsSpinBox->setMaxValue(MAX_ROOM_CAPACITY);
	nStudentsSpinBox->setValue(-1);

	//addActivityPushButton->setText(tr("Add"));
	
	addActivityPushButton->setDefault(true);
	addActivityPushButton->setFocus();
	
	//selectedTeachersListBox->clear();
	if(teacherName!="")
		selectedTeachersListBox->insertItem(teacherName);
	//selectedStudentsListBox->clear();
	if(studentsSetName!="")
		selectedStudentsListBox->insertItem(studentsSetName);
	if(subjectName!=""){
		int pos=-1;
		for(int i=0; i<subjectsComboBox->count(); i++){
			if(subjectsComboBox->itemText(i)==subjectName){
				pos=i;
				break;
			}
		}
		assert(pos>=0);
		subjectsComboBox->setCurrentItem(pos);
	}
	else{
		//begin trick to pass a Qt 4.6.0 bug: the first entry is not highlighted with mouse until you move to second entry and then back up
		if(subjectsComboBox->view()){
			/*QModelIndex mi=subjectsComboBox->view()->currentIndex();
			subjectsComboBox->view()->setCurrentIndex(mi);*/
			subjectsComboBox->view()->setCurrentIndex(QModelIndex());
		}
		//end trick
		subjectsComboBox->setCurrentItem(-1);
	}
	//selectedActivityTagsListBox->clear();
	if(activityTagName!="")
		selectedActivityTagsListBox->insertItem(activityTagName);	
}

AddActivityForm::~AddActivityForm()
{
}

void AddActivityForm::updateTeachersListBox()
{
	allTeachersListBox->clear();
	for(int i=0; i<gt.rules.teachersList.size(); i++){
		Teacher* tch=gt.rules.teachersList[i];
		allTeachersListBox->insertItem(tch->name);
	}
		
	selectedTeachersListBox->clear();

	activityChanged();
}

void AddActivityForm::addTeacher()
{
	if(allTeachersListBox->currentItem()<0 || (uint)(allTeachersListBox->currentItem())>=allTeachersListBox->count())
		return;
	
	for(uint i=0; i<selectedTeachersListBox->count(); i++)
		if(selectedTeachersListBox->text(i)==allTeachersListBox->currentText())
			return;
			
	selectedTeachersListBox->insertItem(allTeachersListBox->currentText());

	activityChanged();
}

void AddActivityForm::removeTeacher()
{
	if(selectedTeachersListBox->count()<=0 || selectedTeachersListBox->currentItem()<0 ||
	 (uint)(selectedTeachersListBox->currentItem())>=selectedTeachersListBox->count())
		return;
		
	selectedTeachersListBox->removeItem(selectedTeachersListBox->currentItem());

	activityChanged();
}

void AddActivityForm::addStudents()
{
	if(allStudentsListBox->currentItem()<0 || (uint)(allStudentsListBox->currentItem())>=allStudentsListBox->count())
		return;
	
	assert(canonicalStudentsSetsNames.count()==(int)(allStudentsListBox->count()));
	QString sn=canonicalStudentsSetsNames.at(allStudentsListBox->currentItem());

	for(uint i=0; i<selectedStudentsListBox->count(); i++)
		if(selectedStudentsListBox->text(i)==sn)
			return;
			
	selectedStudentsListBox->insertItem(sn);
	
	activityChanged();
}

void AddActivityForm::removeStudents()
{
	if(selectedStudentsListBox->count()<=0 || selectedStudentsListBox->currentItem()<0 ||
	 (uint)(selectedStudentsListBox->currentItem())>=selectedStudentsListBox->count())
		return;
		
	selectedStudentsListBox->removeItem(selectedStudentsListBox->currentItem());

	activityChanged();
}

void AddActivityForm::addActivityTag()
{
	if(allActivityTagsListBox->currentItem()<0 || (uint)(allActivityTagsListBox->currentItem())>=allActivityTagsListBox->count())
		return;
	
	for(uint i=0; i<selectedActivityTagsListBox->count(); i++)
		if(selectedActivityTagsListBox->text(i)==allActivityTagsListBox->currentText())
			return;
			
	selectedActivityTagsListBox->insertItem(allActivityTagsListBox->currentText());

	activityChanged();
}

void AddActivityForm::removeActivityTag()
{
	if(selectedActivityTagsListBox->count()<=0 || selectedActivityTagsListBox->currentItem()<0 ||
	 (uint)(selectedActivityTagsListBox->currentItem())>=selectedActivityTagsListBox->count())
		return;
		
	selectedActivityTagsListBox->removeItem(selectedActivityTagsListBox->currentItem());

	activityChanged();
}


void AddActivityForm::updateSubjectsComboBox()
{
	subjectsComboBox->clear();
	for(int i=0; i<gt.rules.subjectsList.size(); i++){
		Subject* sbj=gt.rules.subjectsList[i];
		subjectsComboBox->insertItem(sbj->name);
	}

	subjectChanged(subjectsComboBox->currentText());
}

void AddActivityForm::updateActivityTagsListBox()
{
	allActivityTagsListBox->clear();
	for(int i=0; i<gt.rules.activityTagsList.size(); i++){
		ActivityTag* at=gt.rules.activityTagsList[i];
		allActivityTagsListBox->insertItem(at->name);
	}
		
	selectedActivityTagsListBox->clear();

	activityChanged();

/*	activityTagsComboBox->clear();
	activityTagsComboBox->insertItem("");
	for(int i=0; i<gt.rules.activityTagsList.size(); i++){
		ActivityTag* sbt=gt.rules.activityTagsList[i];
		activityTagsComboBox->insertItem(sbt->name);
	}
		
	activityTagsComboBox->setCurrentItem(0);

	activityTagChanged(activityTagsComboBox->currentText());*/
}

void AddActivityForm::showYearsChanged()
{
	updateStudentsListBox();
}

void AddActivityForm::showGroupsChanged()
{
	updateStudentsListBox();
}

void AddActivityForm::showSubgroupsChanged()
{
	updateStudentsListBox();
}

void AddActivityForm::updateStudentsListBox()
{
	const int INDENT=2;

	bool showYears=yearsCheckBox->isChecked();
	bool showGroups=groupsCheckBox->isChecked();
	bool showSubgroups=subgroupsCheckBox->isChecked();

	allStudentsListBox->clear();
	canonicalStudentsSetsNames.clear();
	for(int i=0; i<gt.rules.yearsList.size(); i++){
		StudentsYear* sty=gt.rules.yearsList[i];
		if(showYears){
			allStudentsListBox->insertItem(sty->name);
			canonicalStudentsSetsNames.append(sty->name);
		}
		for(int j=0; j<sty->groupsList.size(); j++){
			StudentsGroup* stg=sty->groupsList[j];
			if(showGroups){
				QString begin=QString("");
				QString end=QString("");
				begin=QString(INDENT, ' ');
				if(LANGUAGE_STYLE_RIGHT_TO_LEFT==true)
					end=QString(INDENT, ' ');
				allStudentsListBox->insertItem(begin+stg->name+end);
				canonicalStudentsSetsNames.append(stg->name);
			}
			for(int k=0; k<stg->subgroupsList.size(); k++){
				StudentsSubgroup* sts=stg->subgroupsList[k];
				if(showSubgroups){
					QString begin=QString("");
					QString end=QString("");
					begin=QString(2*INDENT, ' ');
					if(LANGUAGE_STYLE_RIGHT_TO_LEFT==true)
						end=QString(2*INDENT, ' ');
					allStudentsListBox->insertItem(begin+sts->name+end);
					canonicalStudentsSetsNames.append(sts->name);
				}
			}
		}
	}
	
	//selectedStudentsListBox->clear();

	activityChanged();
}

/*void AddActivityForm::updatePreferredDaysComboBox()
{
	for(int j=0; j<8; j++){
		prefDay(j)->clear();
		prefDay(j)->insertItem(tr("Any"));
		for(int i=0; i<gt.rules.nDaysPerWeek; i++)
			prefDay(j)->insertItem(gt.rules.daysOfTheWeek[i]);
	}
}

void AddActivityForm::updatePreferredHoursComboBox()
{
	for(int j=0; j<8; j++){
		prefHour(j)->clear();
		prefHour(j)->insertItem(tr("Any"));
		for(int i=0; i<gt.rules.nHoursPerDay; i++)
			prefHour(j)->insertItem(gt.rules.hoursOfTheDay[i]);
	}
}*/

void AddActivityForm::subjectChanged(const QString& dummy)
{
	Q_UNUSED(dummy);
	//if(dummy=="")
	//	;

	activityChanged();
}

/*void AddActivityForm::activityTagChanged(const QString& dummy)
{
	Q_UNUSED(dummy);
	//if(dummy=="")
	//	;

	activityChanged();
}*/

void AddActivityForm::splitChanged()
{
	int nSplit=splitSpinBox->value();
	
	//if(nSplit>=2){
		//addActivityPushButton->setText(tr("Add current activities"));
		//addActivityPushButton->setText(tr("Add activities"));
		//addActivityPushButton->setText(tr("Add"));
		//currentActivityTextLabel->setText(tr("Current activities"));
	//}
	//else{
		//addActivityPushButton->setText(tr("Add current activity"));
		//addActivityPushButton->setText(tr("Add activity"));
		//addActivityPushButton->setText(tr("Add"));
		//currentActivityTextLabel->setText(tr("Current activity"));
	//}

	//minDayDistanceTextLabel->setEnabled(nSplit>=2);
	//minDayDistanceSpinBox->setEnabled(nSplit>=2);

	minDayDistanceTextLabel->setEnabled(nSplit>=2);
	minDayDistanceSpinBox->setEnabled(nSplit>=2);
	percentageTextLabel->setEnabled(nSplit>=2 && minDayDistanceSpinBox->value()>0);
	percentageLineEdit->setEnabled(nSplit>=2 && minDayDistanceSpinBox->value()>0);
	percentTextLabel->setEnabled(nSplit>=2 && minDayDistanceSpinBox->value()>0);
	forceAdjacentCheckBox->setEnabled(nSplit>=2 && minDayDistanceSpinBox->value()>0);

	for(int i=0; i<MAX_SPLIT_OF_AN_ACTIVITY; i++)
		if(i<nSplit)
			subactivitiesTabWidget->setTabEnabled(i, true);
		else
			subactivitiesTabWidget->setTabEnabled(i, false);

	activityChanged();
}

void AddActivityForm::activityChanged()
{
/*	QString s;
	//s+=tr("Current activity:");s+="\n";
	if(selectedTeachersListBox->count()==0){
		if(splitSpinBox->value()==1)
			s+=tr("No teachers for this activity");
		else
			s+=tr("No teachers for these activities");
			
		s+="\n";
	}
	else
		for(uint i=0; i<selectedTeachersListBox->count(); i++){
			s+=tr("Teacher=%1").arg(selectedTeachersListBox->text(i));
			//s+=selectedTeachersListBox->text(i);
			s+="\n";
		}

	s+=tr("Subject=%1").arg(subjectsComboBox->currentText());
	s+="\n";
	if(activityTagsComboBox->currentText()!=""){
		s+=tr("Activity tag=%1").arg(activityTagsComboBox->currentText());
		s+="\n";
	}
	if(selectedStudentsListBox->count()==0){
		if(splitSpinBox->value()==1)
			s+=tr("No students for this activity");
		else
			s+=tr("No students for these activities");
			
		s+="\n";
	}
	else
		for(uint i=0; i<selectedStudentsListBox->count(); i++){
			s+=tr("Students=%1").arg(selectedStudentsListBox->text(i));
			s+="\n";
		}

	if(nStudentsSpinBox->value()>=0){
		s+=tr("Number of students=%1").arg(nStudentsSpinBox->value());
		s+="\n";
	}
	else{
		s+=tr("Number of students: automatically computed from component students sets");
		s+="\n";
	}

	if(splitSpinBox->value()==1){
		s+=tr("Duration=%1").arg(dur(0)->value());
		s+="\n";
		if(activ(0)->isChecked()){
			s+=tr("Active activity");
			s+="\n";
		}
		else{
			s+=tr("Non-active activity");
			s+="\n";
		}
	}
	else{
		s+=tr("This larger activity will be split into %1 smaller activities per week").arg(splitSpinBox->value());
		s+="\n";
		if(minDayDistanceSpinBox->value()>0){
			percentageTextLabel->setEnabled(true);
			percentageLineEdit->setEnabled(true);
			percentTextLabel->setEnabled(true);
			forceAdjacentCheckBox->setEnabled(true);
			
			s+=tr("The distance between any pair of activities must be at least %1 days").arg(minDayDistanceSpinBox->value());
			s+="\n";
			
			s+=tr("Weight percentage of added min days constraint: %1\%").arg(percentageLineEdit->text());
			s+="\n";
			
			if(forceAdjacentCheckBox->isChecked()){
				s+=tr("If activities on same day, then place activities consecutive, in a bigger duration lesson");
				s+="\n";
			}
		}
		else{
			percentageTextLabel->setDisabled(true);
			percentageLineEdit->setDisabled(true);
			percentTextLabel->setDisabled(true);
			forceAdjacentCheckBox->setDisabled(true);
		}
		s+="\n";

		for(int i=0; i<splitSpinBox->value(); i++){
			s+=tr("Component %1:").arg(i+1);
			s+="\n";
			s+=tr("Duration=%1").arg(dur(i)->value());
			s+="\n";
			if(activ(i)->isChecked()){
				s+=tr("Active activity");
				s+="\n";
			}
			else{
				s+=tr("Non-active activity");
				s+="\n";
			}
			s+="\n";
		}
	}

	currentActivityTextEdit->setText(s);*/
}

SecondMinDaysDialog::SecondMinDaysDialog(QWidget* p, int minD, double w) :QDialog(p)
{
	weight=-1;

	QString l=tr
	 ("You selected min days between activities %1 (above 1) and weight %2 (under 100.0). "
	  "Would you like to add also a second constraint to ensure that almost certainly the "
	  "distance between activities is at least %3 (%1-1) days? If yes, please select weight (recommended "
	  "95.0%-100.0%) and click Yes. If no, please click No (only one constraint will be added)").arg(minD).arg(w).arg(minD-1);
	l+="\n\n";
	l+=tr("(Yes means to add an additional constraint min %1 days between activities, weight 0.0%-100.0%. "
	  "If you say Yes, you will have 2 constraints min days added for current activities. "
	  "Adding the second constraint might lead to impossible timetables if the condition is "
	  "too tight, but you can remove the second constraint at any time).").arg(minD-1);
	l+="\n\n";
	l+=tr("Note: 95% is usually enough for min days constraints referring to same activities. "
	  "The weights are cumulated if referring to the same activities. If you have 2 constraints with say 95%"
	  " (say min n days and min n-1 days), "
	  "the min n days constraint is skipped with probability 5%, then min n-1 days constraint is skipped with "
	  "probability 0.25%=5%*5%, so you'll get in 99.75% cases the min n-1 days constraint respected.");
	l+="\n\n";
	l+=tr("Recommended answer is Yes, 95% (or higher).");
	l+="\n\n";

	setWindowTitle(tr("Add a second constraint or not?"));
				
	QVBoxLayout* vl=new QVBoxLayout(this);
				
	/*QLabel* la=new QLabel(this);
	la->setWordWrap(true);
	la->setText(l);*/
	QTextEdit* la=new QTextEdit();
	la->setText(l);
	la->setReadOnly(true);

	vl->addWidget(la);
				
	QPushButton* yes=new QPushButton(tr("Yes"));
	yes->setDefault(true);				
				
	QPushButton* no=new QPushButton(tr("No"));

	QLabel* percLabel=new QLabel(this);
	percLabel->setText("Percentage");
	percText.setText("95.0");
	QHBoxLayout* hl2=new QHBoxLayout(vl);
	hl2->addWidget(percLabel);
	hl2->addWidget(&percText);

	QHBoxLayout* hl=new QHBoxLayout(vl);
	hl->addStretch(1);
	hl->addWidget(yes);
	hl->addWidget(no);
				
	connect(yes, SIGNAL(clicked()), this, SLOT(yesPressed()));
	connect(no, SIGNAL(clicked()), this, SLOT(reject()));
	
	int ww=this->sizeHint().width();
	if(ww>1000)
		ww=1000;
	if(ww<590)
		ww=590;

	int hh=this->sizeHint().height();
	if(hh>650)
		hh=650;
	if(hh<380)
		hh=380;
		
	this->setGeometry(0, 0, ww, hh);
	centerWidgetOnScreen(this);
}

SecondMinDaysDialog::~SecondMinDaysDialog()
{
}

void SecondMinDaysDialog::yesPressed()
{
	double wt;
	QString tmp=percText.text();
	sscanf(tmp, "%lf", &wt);
	if(wt<0.0 || wt>100.0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage) - must be >=0 and <=100.0"));
		return;
	}
	weight=wt;
	accept();
}

void AddActivityForm::addActivity()
{
	double weight;
	QString tmp=percentageLineEdit->text();
	sscanf(tmp, "%lf", &weight);
	if(percentageLineEdit->isEnabled() && (weight<0.0 || weight>100.0)){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage) for added constraint min days between activities"));
		return;
	}

	//teachers
	QStringList teachers_names;
	if(selectedTeachersListBox->count()<=0){
		int t=QMessageBox::question(this, tr("FET question"),
		 tr("Do you really want to add an activity without teacher(s)?"),
		 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		if(t==QMessageBox::No)
			return;
	}
	else{
		for(uint i=0; i<selectedTeachersListBox->count(); i++){
			assert(gt.rules.searchTeacher(selectedTeachersListBox->text(i))>=0);
			teachers_names.append(selectedTeachersListBox->text(i));
		}
	}

	//subject
	QString subject_name=subjectsComboBox->currentText();
	int subject_index=gt.rules.searchSubject(subject_name);
	if(subject_index<0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid subject"));
		return;
	}

	//activity tags
	/*QString activity_tag_name=activityTagsComboBox->currentText();
	int activity_tag_index=gt.rules.searchActivityTag(activity_tag_name);
	if(activity_tag_index<0 && activity_tag_name!=""){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid activity tag"));
		return;
	}*/
	QStringList activity_tags_names;
	for(uint i=0; i<selectedActivityTagsListBox->count(); i++){
		assert(gt.rules.searchActivityTag(selectedActivityTagsListBox->text(i))>=0);
		activity_tags_names.append(selectedActivityTagsListBox->text(i));
	}

	//students
	QStringList students_names;
	if(selectedStudentsListBox->count()<=0){
		int t=QMessageBox::question(this, tr("FET question"),
		 tr("Do you really want to add an activity without student set(s)?"),
		 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		if(t==QMessageBox::No)
			return;
	}
	else{
		for(uint i=0; i<selectedStudentsListBox->count(); i++){
			assert(gt.rules.searchStudentsSet(selectedStudentsListBox->text(i))!=NULL);
			students_names.append(selectedStudentsListBox->text(i));
		}
	}

	if(splitSpinBox->value()==1){ //indivisible activity
		int duration=duration1SpinBox->value();
		if(duration<0){
			QMessageBox::warning(this, tr("FET information"),
				tr("Invalid duration"));
			return;
		}

		bool active=false;
		if(active1CheckBox->isChecked())
			active=true;

		int activityid=0; //We set the id of this newly added activity = (the largest existing id + 1)
		for(int i=0; i<gt.rules.activitiesList.size(); i++){
			Activity* act=gt.rules.activitiesList[i];
			if(act->id > activityid)
				activityid = act->id;
		}
		activityid++;
		Activity a(gt.rules, activityid, 0, teachers_names, subject_name, activity_tags_names, students_names,
			duration, duration, /*parity,*/ active, (nStudentsSpinBox->value()==-1), nStudentsSpinBox->value());

		bool already_existing=false;
		for(int i=0; i<gt.rules.activitiesList.size(); i++){
			Activity* act=gt.rules.activitiesList[i];
			if((*act)==a)
				already_existing=true;
		}

		if(already_existing){
			int t=QMessageBox::question(this, tr("FET question"), 
				//tr("This activity already exists. Insert it again?"),
				tr("A similar activity already exists. Do you want to insert current activity?"),
				tr("Yes"),tr("No"));
			assert(t==0 || t==1 ||t==-1);
			if(t==1) //no pressed
				return;
			if(t==-1) //Esc pressed
				return;
		}

		bool tmp=gt.rules.addSimpleActivity(activityid, 0, teachers_names, subject_name, activity_tags_names,
			students_names,	duration, duration, /*parity,*/ active, /*preferred_day, preferred_hour,*/
			(nStudentsSpinBox->value()==-1), nStudentsSpinBox->value());
		if(tmp)
			QMessageBox::information(this, tr("FET information"), tr("Activity added"));
		else
			QMessageBox::critical(this, tr("FET information"), tr("Activity NOT added - please report error"));
	}
	else{ //split activity
		if(minDayDistanceSpinBox->value()>0 && splitSpinBox->value()>gt.rules.nDaysPerWeek){
			int t=LongTextMessageBox::largeConfirmation(this, tr("FET confirmation"),
			 tr("Possible incorrect setting. Are you sure you want to add current activity? See details below:")+"\n\n"+
			 tr("You want to add a container activity split into more than the number of days per week and also add a constraint min days between activities."
			  " This is a very bad practice from the way the algorithm of generation works (it slows down the generation and makes it harder to find a solution).")+
			 "\n\n"+
			 tr("The best way to add the activities would be:")+
			 "\n\n"+
			 tr("1. If you add 'force consecutive if same day', then couple extra activities in pairs to obtain a number of activities equal to the number of days per week"
			  ". Example: 7 activities with duration 1 in a 5 days week, then transform into 5 activities with durations: 2,2,1,1,1 and add a single container activity with these 5 components"
			  " (possibly raising the weight of added constraint min days between activities up to 100%)")+
			  "\n\n"+
			 tr("2. If you don't add 'force consecutive if same day', then add a larger activity splitted into a number of"
			  " activities equal with the number of days per week and the remaining components into other larger splitted activity."
			  " For example, suppose you need to add 7 activities with duration 1 in a 5 days week. Add 2 larger container activities,"
			  " first one splitted into 5 activities with duration 1 and second one splitted into 2 activities with duration 1"
			  " (possibly raising the weight of added constraints min days between activities for each of the 2 containers up to 100%)")+
		  	 "\n\n"+
			 tr("Do you want to add current activities as they are now (not recommended) or cancel and edit them as instructed?")
			  ,
			 tr("Yes"), tr("No"), QString(), 0, 1);

			if(t==1)
				return;
		}

		int totalduration;
		int durations[MAX_SPLIT_OF_AN_ACTIVITY];
		//int parities[8];
		//int preferred_days[8];
		//int preferred_hours[8];
		bool active[MAX_SPLIT_OF_AN_ACTIVITY];
		int nsplit=splitSpinBox->value();

		totalduration=0;
		for(int i=0; i<nsplit; i++){
			durations[i]=dur(i)->value();
			/*parities[i]=PARITY_WEEKLY;
			if(par(i)->isChecked())
				parities[i]=PARITY_FORTNIGHTLY;*/
			active[i]=false;
			if(activ(i)->isChecked())
				active[i]=true;
			//preferred_days[i]=prefDay(i)->currentItem()-1;
			//preferred_hours[i]=prefHour(i)->currentItem()-1;

			totalduration+=durations[i];
		}

		//the group id of this split activity and the id of the first partial activity
		//it is the maximum already existing id + 1
		int firstactivityid=0;
		for(int i=0; i<gt.rules.activitiesList.size(); i++){
			Activity* act=gt.rules.activitiesList[i];
			if(act->id > firstactivityid)
				firstactivityid = act->id;
		}
		firstactivityid++;

		int minD=minDayDistanceSpinBox->value();
		bool tmp=gt.rules.addSplitActivity(firstactivityid, firstactivityid,
			teachers_names, subject_name, activity_tags_names, students_names,
			nsplit, totalduration, durations,
			/*parities,*/ active, minD, /*percentageSpinBox->value()*/weight, forceAdjacentCheckBox->isChecked(), /*preferred_days, preferred_hours,*/
			(nStudentsSpinBox->value()==-1), nStudentsSpinBox->value());
		if(tmp){
			if(minD>1 && weight<100.0){
				SecondMinDaysDialog second(this, minD, weight);
				int code=second.exec();

				if(code==QDialog::Accepted){
					assert(second.weight>=0 && second.weight<=100.0);
					//int acts[MAX_CONSTRAINT_MIN_DAYS_BETWEEN_ACTIVITIES];
					QList<int> acts;
					for(int i=0; i<nsplit; i++){
						//acts[i]=firstactivityid+i;
						acts.append(firstactivityid+i);
					}
					TimeConstraint* c=new ConstraintMinDaysBetweenActivities(second.weight, forceAdjacentCheckBox->isChecked(), nsplit, acts, minD-1);
					bool tmp=gt.rules.addTimeConstraint(c);
					assert(tmp);
				}
				
			/*
			    bool ok;
				
				QString s=tr
				 ("You selected min days between activities %1 (above 1) and weight %2 (under 100.0).\n"
				  "Would you like to add also a second constraint to ensure that almost certainly the\n"
				  "distance between activities is at least 1 day? If yes, please select weight (recommended\n"
				  "95%-100%) and click OK. If no, please click Cancel (only one constraint will be added)\n\n"
				  "(OK means to add an additional constraint min 1 day between activities, weight 0%-100%.\n"
				  "If you say OK, you will have 2 constraints min days added for current activities.\n"
				  "Adding the second constraint might lead to impossible timetables if the condition is\n"
				  "too tight, but you can remove the second constraint at any time).\n\n"
				  "Note: 95% is usually enough for min days constraints referring to same activities.\n"
				  "The weights are cumulated (only in such cases). If you have 2 constraints with say 95%\n"
				  "first constraint is skipped with probability 5%, then second constraint is skipped with\n"
				  "probability 0.25%=5%*5%, so you'll get in 99.75% cases second constraint respected\n\n"
				  "Recommended answer is Yes (OK), 95%.").arg(minD).arg(weight);
				
			    double d = QInputDialog::getDouble(this, tr("FET question"),
				 s, 95.0, 0.0, 100.0, 10, &ok);

 				if(ok){ //yes
					int acts[MAX_CONSTRAINT_MIN_DAYS_BETWEEN_ACTIVITIES];
					for(int i=0; i<nsplit; i++)
						acts[i]=firstactivityid+i;
					TimeConstraint* c=new ConstraintMinDaysBetweenActivities(d, forceAdjacentCheckBox->isChecked(), nsplit, acts, 1);
					bool tmp=gt.rules.addTimeConstraint(c);
					assert(tmp);
				}*/
			
				/*int t=QMessageBox::question(this, tr("FET question"), tr("You selected min days between activities %1 (above 1) and weight %2 (under 100.0). Would you"
				 " like to add also a second constraint to ensure that always the distance between activities is at least 1 day?\n\n"
				 "(This means to add an additional constraint min 1 day between activities, weight 100.0%. If you say yes, you will have 2 constraints"
				 " min days added for these activities. Adding the second constraint might lead to impossible timetables"
				 " if the condition is too tight, but you can remove the second constraint at any time).\n\n"
				 "Safest answer is No, recommended answer is Yes.").arg(minD).arg(weight),
				 tr("Yes"), tr("No"), QString(),
 				 0, 1 );
														 				 	
 				if(t==0){ //yes
					int acts[MAX_CONSTRAINT_MIN_DAYS_BETWEEN_ACTIVITIES];
					for(int i=0; i<nsplit; i++)
						acts[i]=firstactivityid+i;
					TimeConstraint* c=new ConstraintMinDaysBetweenActivities(100.0, forceAdjacentCheckBox->isChecked(), nsplit, acts, 1);
					bool tmp=gt.rules.addTimeConstraint(c);
					assert(tmp);
				}*/
			}
		
			QMessageBox::information(this, tr("FET information"), tr("Split activity added."
			 " Please note that FET currently cannot check for duplicates when adding split activities"
			 ". It is advisable to check the statistics after adding all the activities"));
		}
		else
			QMessageBox::critical(this, tr("FET information"), tr("Split activity NOT added - error???"));
	}

	PlanningChanged::increasePlanningCommunicationSpinBox();
}

void AddActivityForm::clearTeachers()
{
	selectedTeachersListBox->clear();
	activityChanged();
}

void AddActivityForm::clearStudents()
{
	selectedStudentsListBox->clear();
	activityChanged();
}

void AddActivityForm::clearActivityTags()
{
	selectedActivityTagsListBox->clear();
	activityChanged();
}

void AddActivityForm::help()
{
	QString s;
	
	s=tr("This help by Liviu Lalescu, modified 25 September 2008");
	
	s+="\n\n";
	 
	s+=tr("Comment added on 25 September 2008:");
	s+=" ";
	s+=tr("A first notice, because many users didn't care about it: "
	 "If you use a 5 days week: "
	 "when adding an activity split into only 2 components "
	 "per week, the best practice is to add min days between activities to be 2. "
	 "If you split an activity into 3 components per week - please read FAQ question Q1-5-September-2008");
	s+="\n\n";
	
	s+=tr("You can select a teacher from all the teachers with the mouse or with keyboard tab/up/down, then "
	 "double click it or press Enter to add it to the selected teachers for current activity. "
	 "You can then choose to remove a teacher from the selected teachers. You can highlight it "
	 "with arrows or mouse, then double click or press Enter to remove the teacher from the selected teachers.");
	 
	s+="\n\n";
	
	s+=tr("The same procedure (double click or Enter) applies to adding a students set or removing a students set.");
	
	s+="\n\n";
	
	s+=tr("You can check/uncheck show years, show groups or show subgroups.");
	s+="\n\n";
	
	 s+=tr("If you split a larger activity into more activities per week, you have a multitude of choices:\n"
	 "You can choose the minimum distance in days between each pair of activities."
	 " Please note that a minimum distance of 1 means that the activities must not be in the same day, "
	 "a minimum distance of 2 means that the activities must be separated by one day (distance from Monday"
	 " to Wednesday for instance is 2 days), etc.");

	s+="\n\n";
	 
	 s+=tr("Modification on 14 June 2008:");
	 s+=" ";
	 s+=tr("If you have for instance an activity with 2 lessons per week and you want to spread them to at "
	 "least 2 days distance, you can add a constraint min days with min days = 2 and weight 95% "
	 "(or higher). If you want also to ensure that activities will "
	 "be separated by at least one day, you can use this feature: "
	 "add a constraint min days with minimum days 2 and weight 95% or lower, and after that you'll get "
	 "the possibility to add another constraint with min 1 days and weight 95% or higher. "
	 "It works if you first select in the dialog the min days >= 2 and click Add activities. Or you can add manually the constraints "
	 "(difficult this way). "
	 "Important: it is best practice to consider both constraints to have 95% weight. The combination assures that "
	 "the resultant is 99.75% weight");

	s+="\n\n";
	 
	s+=tr("Please note that the min days distance is a time constraint and you can only see/modify it in the "
	 "time constraints dialogs, not in the modify activity dialog. Additionally, you can see the constraints "
	 "for each activity in the details text box of each activity");

	s+="\n\n";
	 
	 s+=tr("If you choose a value greater or equal with 1 for min days, a time constraint min days between activities will be added automatically "
	 "(you can see this constraint in the time constraints list or you can see this constraint in the "
	 "detailed description of the activity). You can select a weight percentage for this constraint. "
	 "If you select 100%, the constraint must be respected all the time. If you select 95%, there is a small chance "
	 "that the timetable will not respect this constraint. Recommended values are 95.0%-100.0% (maybe you could try "
	 "with 95%, then 99.75%, or even 100.0%, but the generation time might be larger). Generally, 99.75% might be a good value. "
	 "Note: if you put a value less than 100% and the constraint is too tough, FET is able to find that this constraint "
	 "is impossible and will break it. 99.75% might be better than 95% but possibly slower. The percentage is subjective "
	 "(if you put 95% you may get 6 soft conflicts and if you put 99.75% you may get 3 soft conflicts). "
	 "Starting with FET-5.3.6, it is possible to change this value for all constraints in one click, in constraint min days"
	 " between activities dialog.");

	s+="\n\n";

	s+=tr("There is another option, if the activities are in the same day, force consecutive activities. You can select "
	 "this option for instance if you have 5 lessons of math in 5 days, and there is no timetable which respects "
	 "fully the days separation. Then, you can set the weight percent of the min days constraint to 95% and "
	 "add consecutive if same day. You will have as results say 3 lessons with duration 1 and a 2 hours lesson in another day. "
	 "Please be careful: if the activities are on the same day, even if the constraint has 0% weight, then the activities are forced to be "
	 "consecutive.");

	s+="\n\n";

	s+=tr("Current algorithm cannot schedule 3 activities in the same day if consecutive is checked, so "
	 "you will get no solution in such extreme cases (for instance, if you have 3 lessons and a teacher which works only 1 day per week, "
	 "and select 'force consecutive if same day', you will get an imposssible timetable. But these are extremely unlikely cases. "
	 "If you encounter such cases, please contact the author, I'll try to fix this problem).");

	s+="\n\n";
	 
	 s+=tr("Note: You cannot add 'consecutive if same day' with min days=0. If you want this, you have to add "
	 "min days at least 1 (and any weight percentage).");

	s+="\n\n";
	 
	 s+=tr("Starting with version 5.0.0, it is possible to add activities with no students or no teachers");

	s+="\n\n";
	 
	 s+=tr("Addition 14 June 2008:");
	 s+=" ";
	 s+=tr("If you select a number of min days above 1, you will get the possibility "
	 "to add a second constraint min days between activities, with min days 1 and a percentage of your choice. Just click "
	 "Add activities");
	 
	//show the message in a dialog
	QDialog dialog;
	
	dialog.setWindowTitle(tr("FET - help on adding activity(ies)"));

	QVBoxLayout* vl=new QVBoxLayout(&dialog);
	QTextEdit* te=new QTextEdit();
	te->setPlainText(s);
	te->setReadOnly(true);
	QPushButton* pb=new QPushButton(tr("OK"));

	QHBoxLayout* hl=new QHBoxLayout(0);
	hl->addStretch(1);
	hl->addWidget(pb);

	vl->addWidget(te);
	vl->addLayout(hl);
	connect(pb, SIGNAL(clicked()), &dialog, SLOT(close()));

	/*
	dialog.setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	QRect rect = QApplication::desktop()->availableGeometry(&dialog);
	int xx=rect.width()/2 - 350;
	int yy=rect.height()/2 - 250;
	dialog.setGeometry(xx, yy, 700, 500);*/
	dialog.setGeometry(0,0,700,500);
	centerWidgetOnScreen(&dialog);

	dialog.exec();
}

void AddActivityForm::minDaysChanged()
{
	/*if(splitSpinBox->value()<2){
		QMessageBox::critical(this, tr("FET critical"), tr("You found a bug in FET (file %1, line %2). "
		 "The problem is that the split has value less than 2, but the min days button can be modified. Please report this bug. Operation will now continue")
		 .arg(__FILE__)
		 .arg(__LINE__));
	}
	else
		assert(splitSpinBox->value()>=2);*/
	percentageTextLabel->setEnabled(splitSpinBox->value()>=2 && minDayDistanceSpinBox->value()>0);
	percentageLineEdit->setEnabled(splitSpinBox->value()>=2 && minDayDistanceSpinBox->value()>0);
	percentTextLabel->setEnabled(splitSpinBox->value()>=2 && minDayDistanceSpinBox->value()>0);
	forceAdjacentCheckBox->setEnabled(splitSpinBox->value()>=2 && minDayDistanceSpinBox->value()>0);
}
