#ifndef WIDGETRY_DATAINTERFACE_H
#define WIDGETRY_DATAINTERFACE_H

#include <Widgetry/global.h>
#include <Widgetry/userinterface.h>
#include <Widgetry/abstractdataedit.h>

#include <DataGate/abstractdatacontroller.h>

namespace Jsoner {
class Object;
class Array;
}

namespace Widgetry {

namespace Ui {
class DataInterface;
}

class DataInterfaceForge;

typedef void(DataGate::AbstractDataController::*DataControllerRawMethod)(const DataGate::DataRequest &, const DataGate::DataRequestCallback &, const DataGate::DataResponseCallback &);

class DataInterfacePrivate;
class WIDGETRY_EXPORT DataBrowser : public UserInterface
{
    Q_OBJECT

public:
    DataBrowser(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    DataBrowser(const QByteArray &id, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~DataBrowser();

    bool isOperationSupported(const QString &operation) const override;
    QStringList availableOperations() const override;

    QStringList crudOperations() const;

    Jsoner::Object currentObject() const;
    Jsoner::Array selectedObjects() const;

    DataGate::AbstractDataController *dataController() const;
    DataInterfaceForge *forge() const;

public slots:
    void search(const QString &query);
    void filter(const QVariantHash &filters);
    void refresh();

    void showCurrentItem();
    void showItem(const Jsoner::Object &object);

    void addNewItem();
    void addItem(const Jsoner::Object &object);

    void editCurrentItem();
    void editItem(const Jsoner::Object &object);

    void deleteSelectedItems();
    void deleteItems(const Jsoner::Array &objects);

protected:
    void prepareUi() override;
    void translateUi(bool full = true) override;

    virtual void showObject(const Jsoner::Object &object);
    virtual void addObject(const Jsoner::Object &object, const DataEditFinishedCallback &callback);
    virtual void editObject(const Jsoner::Object &object, const DataEditFinishedCallback &callback);

    void showContextMenu(const Jsoner::Array &objects, const QPoint &pos);
    virtual bool prepareContextMenu(const Jsoner::Array &objects, QMenu *menu);

    virtual void beginRequest();
    virtual void monitorRequest(int processed, int total);
    virtual void endRequest();

    bool handleOperation(WidgetOperation *operation) override;
    bool handleSearch(const QVariantHash &parameters);
    bool handleFilter(const QVariantHash &parameters);
    bool handleRefresh(const QVariantHash &parameters);
    bool handleShowItem(const QVariantHash &parameters);
    bool handleAddItem(const QVariantHash &parameters);
    bool handleEditItem(const QVariantHash &parameters);
    bool handleDeleteItems(const QVariantHash &parameters);
    void handleOperationResult(const WidgetOperation &operation) override;

    virtual DataGate::DataRequest prepareQuery(const DataGate::DataRequest &query) const;

    DataEditFinishedCallback editCallback(AbstractDataEdit::Operation operation) const;

protected:
    virtual void showMaxWindowMessage(int activeEdits, int maxActiveEdits);
    void showResponseMessage(const QString &title, const DataGate::DataResponse &response);
    void showResponseMessage(const QString &title, const QString &text, const DataGate::DataResponse &response);

    void preFetch(const DataGate::DataRequest &query, const std::function<void(const Jsoner::Object &object)> &callback);
    void executeDataRequest(DataControllerRawMethod method, const DataGate::DataRequest &query);
    void executeDataRequest(DataControllerRawMethod method, const DataGate::DataRequest &query, const DataGate::DataResponseCallback &callback);

    friend class DataInterfaceForge;

private:
    Q_DISABLE_COPY(DataBrowser)

    Ui::DataInterface *ui;

    static QStringList s_availableOperations;
    static QStringList s_permissions;

    friend class DataInterfacePrivate;
    friend class AbstractDataEditFactory;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAINTERFACE_H
