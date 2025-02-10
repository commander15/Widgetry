#ifndef WIDGETRY_ABSTRACTDATAEDITFACTORY_H
#define WIDGETRY_ABSTRACTDATAEDITFACTORY_H

#include <Widgetry/global.h>
#include <Widgetry/abstractdataedit.h>

namespace Widgetry {

class AbstractDataEditFactoryPrivate;
class WIDGETRY_EXPORT AbstractDataEditFactory
{
public:
    explicit AbstractDataEditFactory();
    virtual ~AbstractDataEditFactory();

    void setMainField(const QString &field);
    void setMaxCount(int count);

    bool isDialogCreationSupported() const;
    void enableDialogCreation();
    void disableDialogCreation();

    virtual AbstractDataEdit *create(const Jsoner::Object &object, AbstractDataEdit::Operation operation, QWidget *parent = nullptr);

protected:
    virtual AbstractDataEdit *createEdit(QWidget *parent = nullptr) = 0;

private:
    QScopedPointer<AbstractDataEditFactoryPrivate> d_ptr;

    friend class AbstractDataEditPrivate;
};

} // namespace Widgetry

#endif // WIDGETRY_ABSTRACTDATAEDITFACTORY_H
