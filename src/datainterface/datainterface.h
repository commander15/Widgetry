#ifndef WIDGETRY_DATAINTERFACE_H
#define WIDGETRY_DATAINTERFACE_H

#include <Widgetry/global.h>
#include <Widgetry/userinterface.h>
#include <Widgetry/abstractdatacontroller.h>

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

typedef void(AbstractDataController::*DataControllerRawMethod)(const DataQuery &, const DataQueryProgressCallback &, const DataQueryResponseCallback &);

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

    virtual void beginRequest();
    virtual void monitorRequest(int processed, int total);
    virtual void endRequest();

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

protected:
    void showResponseMessage(const QString &title, const DataResponse &response);
    void executeDataRequest(DataControllerRawMethod method, const DataQuery &query);
    void executeDataRequest(DataControllerRawMethod method, const DataQuery &query, const DataQueryResponseCallback &callback);

    friend class DataInterfaceForge;

private:
    Q_DISABLE_COPY(DataInterface)

    Ui::DataInterface *ui;

    static QStringList s_availableOperations;

    friend class DataInterfacePrivate;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAINTERFACE_H
