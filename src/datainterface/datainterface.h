#ifndef WIDGETRY_DATAINTERFACE_H
#define WIDGETRY_DATAINTERFACE_H

#include <Widgetry/global.h>
#include <Widgetry/userinterface.h>

namespace Jsoner {
class Object;
class Array;
class TableModel;
}

namespace Widgetry {

namespace Ui {
class DataInterface;
}

class DataInterfaceForge;
class AbstractDataController;
class DataQuery;
class DataResponse;

class DataInterfacePrivate;
class WIDGETRY_EXPORT DataInterface : public UserInterface
{
    Q_OBJECT

public:
    DataInterface(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    DataInterface(const QByteArray &id, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~DataInterface();

    bool isOperationSupported(const QString &operation) const override;
    QStringList availableOperations() const override;

    DataInterfaceForge *forge() const;

    virtual AbstractDataController *dataController() const;
    void setDataController(AbstractDataController *controller);

public slots:
    void search(const QString &query);
    void filter(const QVariantHash &filters);
    void refresh();
    void showCurrentItem();
    void addItem();
    void editCurrentItem();
    void deleteSelectedItems();

protected:
    Jsoner::Object currentObject() const;
    Jsoner::Array selectedObjects() const;

    virtual void showObject(const Jsoner::Object &object);
    virtual Jsoner::Object addObject(const Jsoner::Object &object);
    virtual Jsoner::Object editObject(const Jsoner::Object &object);

    void showContextMenu(const Jsoner::Array &objects, const QPoint &pos);
    virtual bool prepareContextMenu(const Jsoner::Array &objects, QMenu *menu);

    bool handleOperation(Operation *operation) override;
    bool handleSearch(const QVariantHash &parameters);
    bool handleFilter(const QVariantHash &parameters);
    bool handleRefresh(const QVariantHash &parameters);
    bool handleShowItem(const QVariantHash &parameters);
    bool handleAddItem(const QVariantHash &parameters);
    bool handleEditItem(const QVariantHash &parameters);
    bool handleDeleteItems(const QVariantHash &parameters);

    void handleOperationResult(const Operation &operation) override;

    virtual DataQuery generateQuery() const;

    Jsoner::TableModel *createModel(const QStringList &fields);
    QMenu *createContextMenu(bool addDefaultActions = true);

protected slots:
    virtual void handleFetchedObjects(const Jsoner::Array &objects, const DataResponse &response);
    virtual void handleFetchedObject(const Jsoner::Object &object, const DataResponse &response, int targetRequestType);
    virtual void handleAddedObject(const Jsoner::Object &object, const DataResponse &response);
    virtual void handleEditedObject(const Jsoner::Object &object, const DataResponse &response);
    virtual void handleDeletedObjects(const Jsoner::Array &objects, const DataResponse &response);
    virtual void handleError(int requestType, const Jsoner::Array &objects, const DataResponse &error);

    friend class DataInterfaceForge;

private:
    Ui::DataInterface *ui;

    static QStringList s_availableOperations;

    friend class DataInterfacePrivate;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAINTERFACE_H
