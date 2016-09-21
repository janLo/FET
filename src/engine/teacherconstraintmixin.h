#ifndef TEACHERCONSTRAINTMIXIN_H
#define TEACHERCONSTRAINTMIXIN_H

#include <QString>

class Rules;

class TeacherConstraintMixin
{
private:
    QString _teacherName;
    int _teacherId;

protected:
    bool fetchTeacherId(Rules const &rules);

public:
    TeacherConstraintMixin();

    TeacherConstraintMixin(QString const &teacherName);

    virtual ~TeacherConstraintMixin();

    QString teacherName() const {
        return _teacherName;
    }

    int teacherId() const {
        return _teacherId;
    }

    void teacherName(QString const &teacherName) {
        _teacherName = teacherName;
    }
};

#endif // TEACHERCONSTRAINTMIXIN_H
