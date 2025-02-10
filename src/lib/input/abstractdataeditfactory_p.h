#ifndef WIDGETRY_ABSTRACTDATAEDITFACTORY_P_H
#define WIDGETRY_ABSTRACTDATAEDITFACTORY_P_H

#include "abstractdataeditfactory.h"

namespace Widgetry {

class AbstractDataEditFactoryPrivate
{
public:
    AbstractDataEdit *editForKey(const QVariant &key) const;

    bool canCreateEdit() const;
    void registerEdit(AbstractDataEdit *edit);

    int cleanupInvisibleEdits();

public:
    QString field{"id"}; // Default id
    int maxCount{1}; // Default limit
    bool allowDialogCreation = true;

    QVector<AbstractDataEdit *> edits;
};

} // namespace Widgetry

#endif // WIDGETRY_ABSTRACTDATAEDITFACTORY_P_H
