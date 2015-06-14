/***************************************************************************
                          addspaceconstraintform.cpp  -  description
                             -------------------
    begin                : Thu Jan 8 2004
    copyright            : (C) 2004 by Lalescu Liviu
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

#include "addspaceconstraintform.h"
#include "spaceconstraint.h"

AddSpaceConstraintForm::AddSpaceConstraintForm()
{
}

AddSpaceConstraintForm::~AddSpaceConstraintForm()
{
}

void AddSpaceConstraintForm::addConstraintRoomNotAvailable()
{
	AddConstraintRoomNotAvailableForm* addConstraintRoomNotAvailableForm=new AddConstraintRoomNotAvailableForm();
	addConstraintRoomNotAvailableForm->exec();
}

void AddSpaceConstraintForm::addConstraintRoomTypeNotAllowedSubjects()
{
	AddConstraintRoomTypeNotAllowedSubjectsForm* addConstraintRoomTypeNotAllowedSubjectsForm=new AddConstraintRoomTypeNotAllowedSubjectsForm();
	addConstraintRoomTypeNotAllowedSubjectsForm->exec();
}

void AddSpaceConstraintForm::addConstraintSubjectRequiresEquipments()
{
	AddConstraintSubjectRequiresEquipmentsForm* addConstraintSubjectRequiresEquipmentsForm=new AddConstraintSubjectRequiresEquipmentsForm();
	addConstraintSubjectRequiresEquipmentsForm->exec();
}

void AddSpaceConstraintForm::addConstraintTeacherRequiresRoom()
{
	AddConstraintTeacherRequiresRoomForm* addConstraintTeacherRequiresRoomForm=new AddConstraintTeacherRequiresRoomForm();
	addConstraintTeacherRequiresRoomForm->exec();
}
