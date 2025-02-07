#ifndef WIDGETRY_ABSTRACTDATAEDIT_H
#define WIDGETRY_ABSTRACTDATAEDIT_H

#include <Widgetry/global.h>

#include <Jsoner/object.h>

#include <QtWidgets/qwidget.h>

#include <QtCore/qscopedpointer.h>

class QDialog;

namespace Widgetry {

class DataInterface;
typedef std::function<void(const Jsoner::Object &, int)> DataEditFinishedCallback;

class AbstractDataEditPrivate;
class WIDGETRY_EXPORT AbstractDataEdit
{
public:
    enum Operation {
        ShowOperation,
        AddOperation,
        EditOperation
    };

    enum EditType {
        WindowEdit,
        WidgetEdit,
        CustomEdit
    };

    enum EditCode {
        Accepted = 1,
        Rejected = 0
    };

    explicit AbstractDataEdit();
    virtual ~AbstractDataEdit();

    Jsoner::Object object() const;
    Operation operation() const;
    void setObject(const Jsoner::Object &object, Operation operation);

    bool isComplete() const;
    QString completionError() const;

    bool isReadOnly() const;
    void setReadOnly(bool r = true);

    virtual QWidget *editWidget() const;
    virtual EditType editType() const;

    static QDialog *dialogFromEdit(AbstractDataEdit *edit, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    static AbstractDataEdit *editFromDialog(QDialog *dialog);

public:
    void show();
    void show(const Jsoner::Object &object);
    void add(const Jsoner::Object &object);
    void edit(const Jsoner::Object &object);
    virtual void clear();

    virtual void exec(const DataEditFinishedCallback &onFinished);

protected:
    enum FieldMemberType {
        PropertyMember,
        SignalMember,
        DeduceMember
    };

    AbstractDataEdit(AbstractDataEditPrivate *d);

    template<typename Edit, typename Signal> void initEditing(Edit *edit, Signal &&signal)
    { QObject::connect(edit, signal, edit, [this](int result) { finishEditing(result); }); }

    virtual void render(const Jsoner::Object &object, Operation operation) = 0;
    virtual void extract(Jsoner::Object &object, Operation operation) const = 0;
    virtual bool validateInput() = 0;
    virtual void makeWriteable(bool writeable) = 0;

    void setCompletionError(const QString &str);
    void clearCompletionError();

    virtual void finishEditing(int result = Accepted);

    QScopedPointer<AbstractDataEditPrivate> d_ptr;

    friend class DataEditDialogHelper;
    friend class DataWindow;
};

class WIDGETRY_EXPORT AbstractDataEditFactory
{
public:
    virtual ~AbstractDataEditFactory() = default;

    virtual AbstractDataEdit *create(const Jsoner::Object &object, AbstractDataEdit::Operation operation, QWidget *parent = nullptr);

protected:
    virtual AbstractDataEdit *createEdit(QWidget *parent = nullptr) = 0;
};

template<typename Edit>
class WIDGETRY_EXPORT DataEditFactory : public AbstractDataEditFactory
{
public:
    virtual ~DataEditFactory()
    { cleanupOldInvisibleEdits(); }

    DataEditFactory<Edit> *mainField(const QString &field) { m_field = field; return this; }
    DataEditFactory<Edit> *maxCount(int count) { m_maxCount = count; return this; }

    AbstractDataEdit *create(const Jsoner::Object &object, AbstractDataEdit::Operation operation, QWidget *parent = nullptr) override
    {
        AbstractDataEdit *edit = nullptr;

        if (m_maxCount == 1) {
            if (!m_edits.isEmpty()) {
                edit = m_edits.values().first();
            } else {
                edit = createEdit(parent);
                m_edits.insert(QVariant(), edit);
            }

            edit->setObject(object, operation);
            return edit;
        }

        const QVariant key = object.variant(m_field);
        if (m_edits.contains(key)) {
            edit = m_edits.value(key);
            edit->setObject(object, operation);
            return edit;
        }

        if (m_edits.size() >= m_maxCount) {
            cleanupOldInvisibleEdits(false);
            if (m_edits.size() >= m_maxCount) {
                return nullptr; // Still full after cleanup
            }
        }

        edit = createEdit(parent);
        edit->setObject(object, operation);
        m_edits.insert(key, edit);
        return edit;
    }

private:
    AbstractDataEdit *createEdit(QWidget *parent) override
    {
        Edit *edit = new Edit(parent);
        if (edit->editType() == AbstractDataEdit::WindowEdit) {
            return edit;
        } else {
            QDialog *dialog = AbstractDataEdit::dialogFromEdit(edit, parent);
            return AbstractDataEdit::editFromDialog(dialog);
        }
    }

    void cleanupOldInvisibleEdits(bool all = true)
    {
        for (auto it = m_edits.begin(); it != m_edits.end();) {
            if (!it.value()->editWidget()->isVisible()) {
                delete it.value();
                it = m_edits.erase(it);

                if (!all)
                    return; // Delete only one per call
            } else {
                ++it;
            }
        }
    }

    QString m_field{"id"}; // Default id
    int m_maxCount{1}; // Default limit
    QHash<QVariant, AbstractDataEdit *> m_edits;
};

} // namespace Widgetry

inline size_t qHash(const QVariant &key, size_t seed = 0)
{ return qHash(key.toString()); }

#endif // WIDGETRY_ABSTRACTDATAEDIT_H
