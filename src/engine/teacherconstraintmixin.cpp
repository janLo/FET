#include "teacherconstraintmixin.h"

#include "rules.h"


bool TeacherConstraintMixin::fetchTeacherId(Rules const &rules)
{
    _teacherId = rules.teachersHash.value(_teacherName , -1);
    assert(_teacherId >= 0);
    return true;
}

TeacherConstraintMixin::TeacherConstraintMixin() { }

TeacherConstraintMixin::TeacherConstraintMixin(const QString &teacherName)
    : _teacherName(teacherName),
      _teacherId(-1)
{ }

TeacherConstraintMixin::~TeacherConstraintMixin()
{ }
