#ifndef WIDGETRY_DATABROWSER_H
#define WIDGETRY_DATABROWSER_H

#include <Widgetry/global.h>
#include <Widgetry/widget.h>
#include <Widgetry/abstractdataedit.h>

#include <DataGate/abstractdataclient.h>

namespace DataGate {
class AbstractDataManager;
class DataResponse;
}

namespace Widgetry {

namespace Ui {
class DataBrowser;
}

class AbstractDataBrowserHandler;

class AbstractDataEditFactory;

class WIDGETRY_EXPORT DataBrowser : public Widgetry::Widget, public DataGate::AbstractDataClient
{
    Q_OBJECT

public:
    DataBrowser(const QByteArray &id, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    DataBrowser(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~DataBrowser();

    QVariant parameter(const QString &name) const override;
    void setParameter(const QString &name, const QVariant &value) override;

    Q_SLOT void search(const QString &query);
    Q_SLOT void filter(const QVariantHash &filters);
    Q_SLOT void reload();
    Q_SLOT void refresh();

    Jsoner::Object currentObject() const;
    Jsoner::Array selectedObjects() const;

    Q_SLOT void showCurrentItem();
    void showItem(const Jsoner::Object &object);

    Q_SLOT void addNewItem();
    void addItem(const Jsoner::Object &object);

    Q_SLOT void editCurrentItem();
    void editItem(const Jsoner::Object &object);

    Q_SLOT void deleteSelectedItems();
    void deleteItems(const Jsoner::Array &objects);

    QStringList availableOperations() const override;

    Q_SLOT void sync() override;

    void loadSettings(QSettings *settings) override;
    void saveSettings(QSettings *settings) const override;

    void registerHandler(AbstractDataBrowserHandler *handler);

    AbstractDataEdit *filterEdit() const;
    void setFilterEdit(AbstractDataEdit *edit);

    AbstractDataEditFactory *editFactory() const;
    void setEditFactory(AbstractDataEditFactory *factory);

    DataGate::AbstractDataManager *dataManager() const;
    void setDataManager(DataGate::AbstractDataManager *manager);

    DataEditFinishedCallback editCallback(AbstractDataEdit::Operation operation);

protected:
    void prepareUi() override;
    void translateUi(bool full = true) override;

    QVariant handleOperationRequest(const WidgetOperation &operation, bool &success) override;
    void handleOperationResult(const WidgetOperation &operation, const QVariantHash &result, bool success) override;

    DataGate::DataRequest newRequest(int type);
    DataGate::DataRequest newRequest() override final;

private:
    Ui::DataBrowser *ui;

    friend class DataBrowserPrivate;
    friend class DataBrowserBlueprintPrivate;
};

} // namespace Widgetry

#endif // WIDGETRY_DATABROWSER_H
