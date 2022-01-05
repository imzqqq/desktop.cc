#pragma once

#include "treemodel.h"

#include "utils_global.h"

#include <QJsonValue>
#include <QCoreApplication>

namespace Utils {

class QTCREATOR_UTILS_EXPORT JsonTreeItem : public TypedTreeItem<JsonTreeItem>
{
    Q_DECLARE_TR_FUNCTIONS(JsonTreeModelItem)
public:
    JsonTreeItem() = default;
    JsonTreeItem(const QString &displayName, const QJsonValue &value);

    QVariant data(int column, int role) const override;
    bool canFetchMore() const override;
    void fetchMore() override;

private:
    bool canFetchObjectChildren() const;
    bool canFetchArrayChildren() const;

    QString m_name;
    QJsonValue m_value;
};

} // namespace Utils
