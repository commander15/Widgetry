#ifndef WIDGETRY_LINEEDIT_P_H
#define WIDGETRY_LINEEDIT_P_H

#include "lineedit.h"

#include <QtCore/qstringlistmodel.h>

#include <QtGui/qvalidator.h>

#include <QtWidgets/qcompleter.h>

namespace Widgetry {

class LineEditPrivate
{
public:
    LineEditPrivate(LineEdit *q);
    virtual ~LineEditPrivate() = default;

    virtual void initEdit();
    virtual void retranslateEdit();

    QRegularExpression regularExpression() const;
    void setRegularExpression(const QRegularExpression &regex);

    QStringList completions() const;
    void setCompletions(const QStringList &completions);

    LineEdit *q_ptr;
    int maxCompletionCount;

protected:
    QCompleter *completer();
    virtual void createCompleter();

private:
    QRegularExpressionValidator *m_validator;
    QStringListModel *m_completionModel;
    QCompleter *m_completer;

    friend class LineEdit;
};

}

#endif // WIDGETRY_LINEEDIT_P_H
