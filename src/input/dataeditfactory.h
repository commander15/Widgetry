#ifndef WIDGETRY_DATAEDITFACTORY_H
#define WIDGETRY_DATAEDITFACTORY_H

#include <Widgetry/global.h>
#include <Widgetry/abstractdataeditfactory.h>

namespace Widgetry {

template<typename Edit>
class WIDGETRY_EXPORT DataEditFactory : public AbstractDataEditFactory
{
public:
    virtual ~DataEditFactory()
    { cleanupOldInvisibleEdits(); }

    DataEditFactory<Edit> *allowDialogCreation() { enableDialogCreation(); return this; }
    DataEditFactory<Edit> *mainField(const QString &field) { m_field = field; return this; }
    DataEditFactory<Edit> *maxCount(int count) { m_maxCount = count; return this; }

    AbstractDataEdit *create(const Jsoner::Object &object, AbstractDataEdit::Operation operation, QWidget *parent = nullptr) override
    {
        AbstractDataEdit *edit = nullptr;

        if (m_maxCount == 1) {
            if (!m_edits.isEmpty()) {
                edit = m_edits.values().first();
            } else {
                edit = AbstractDataEditFactory::create(object, operation, parent);
                m_edits.insert(QVariant(), edit);
            }

            return edit;
        }

        const QVariant key = object.variant(m_field);
        if (m_edits.contains(key)) {
            edit = m_edits.value(key);
            return edit;
        }

        if (m_edits.size() >= m_maxCount) {
            cleanupOldInvisibleEdits(false);
            if (m_edits.size() >= m_maxCount) {
                return nullptr; // Still full after cleanup
            }
        }

        edit = AbstractDataEditFactory::create(object, operation, parent);
        m_edits.insert(key, edit);
        return edit;
    }

private:
    AbstractDataEdit *createEdit(QWidget *parent) override
    {
        return new Edit(parent);
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

}

#endif // WIDGETRY_DATAEDITFACTORY_H
