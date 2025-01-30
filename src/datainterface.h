#ifndef WIDGETRY_DATAINTERFACE_H
#define WIDGETRY_DATAINTERFACE_H

#include <Widgetry/global.h>
#include <Widgetry/userinterface.h>
#include <Widgetry/datainterfacebase.h>

namespace Widgetry {

namespace Ui {
class DataInterface;
}

class DataInterfacePrivate;
class WIDGETRY_EXPORT DataInterface : public UserInterface, public DataInterfaceBase
{
    Q_OBJECT

public:
    DataInterface(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    DataInterface(const QByteArray &id, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~DataInterface();

    bool isOperationSupported(const QString &operation) const override;

public slots:
    void search(const QString &query);
    void filter(const QVariantHash &filters);
    void refresh();
    void addItem();
    void editItem();
    void deleteItems();

    void sync() override;

protected:
    bool handleOperation(Operation *operation) override;
    void handleOperationResult(const Operation &operation) override;

    bool canDeleteObjects(const QList<Jsoner::Object> &objects) override;

    Jsoner::TableModel *createModel(const QStringList &fields);
    QMenu *createContextMenu(bool addDefaultActions = true);

    friend class DataInterfaceBase;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAINTERFACE_H
