#ifndef WIDGETRY_DATAEDITFACTORY_H
#define WIDGETRY_DATAEDITFACTORY_H

#include <Widgetry/global.h>
#include <Widgetry/abstractdataeditfactory.h>

namespace Widgetry {

template<typename Edit>
class WIDGETRY_EXPORT DataEditFactory : public AbstractDataEditFactory
{
public:
    DataEditFactory<Edit> *allowDialogCreation() { enableDialogCreation(); return this; }
    DataEditFactory<Edit> *disallowDialogCreation() { disableDialogCreation(); return this; }
    DataEditFactory<Edit> *mainField(const QString &field) { setMainField(field); return this; }
    DataEditFactory<Edit> *maxCount(int count) { setMaxCount(count); return this; }

private:
    AbstractDataEdit *createEdit(QWidget *parent) override
    { return new Edit(parent); }
};

}

#endif // WIDGETRY_DATAEDITFACTORY_H
