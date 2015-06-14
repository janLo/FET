/***************************************************************************
                          modifystudentsyearform.cpp  -  description
                             -------------------
    begin                : Feb 8, 2005
    copyright            : (C) 2005 by Lalescu Liviu
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

#include "modifystudentsyearform.h"

#include <QMessageBox>

ModifyStudentsYearForm::ModifyStudentsYearForm(const QString& initialYearName, int initialNumberOfStudents)
{
    setupUi(this);

    connect(okPushButton, SIGNAL(clicked()), this /*ModifyStudentsYearForm_template*/, SLOT(ok()));
    connect(cancelPushButton, SIGNAL(clicked()), this /*ModifyStudentsYearForm_template*/, SLOT(cancel()));


	//setWindowFlags(Qt::Window);
	/*setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	QDesktopWidget* desktop=QApplication::desktop();
	int xx=desktop->width()/2 - frameGeometry().width()/2;
	int yy=desktop->height()/2 - frameGeometry().height()/2;
	move(xx, yy);*/
	centerWidgetOnScreen(this);
	
	numberSpinBox->setMaxValue(MAX_ROOM_CAPACITY);
	numberSpinBox->setMinValue(0);
	numberSpinBox->setValue(0);

	this->_initialYearName=initialYearName;
	this->_initialNumberOfStudents=initialNumberOfStudents;
	numberSpinBox->setValue(initialNumberOfStudents);
	nameLineEdit->setText(initialYearName);
	nameLineEdit->selectAll();
	nameLineEdit->setFocus();
}

ModifyStudentsYearForm::~ModifyStudentsYearForm()
{
}

void ModifyStudentsYearForm::cancel()
{
	this->close();
}

void ModifyStudentsYearForm::ok()
{
	if(nameLineEdit->text().isEmpty()){
		QMessageBox::information(this, tr("FET information"), tr("Incorrect name"));
		return;
	}
	if(this->_initialYearName!=nameLineEdit->text() && gt.rules.searchStudentsSet(nameLineEdit->text())!=NULL){
		QMessageBox::information(this, tr("FET information"), tr("Name existing - please choose another"));
		return;
	}
	bool t=gt.rules.modifyYear(this->_initialYearName, nameLineEdit->text(), numberSpinBox->value());
	assert(t);

	this->close();
}
