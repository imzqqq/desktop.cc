#pragma once

#include "environmentfwd.h"
#include "utils_global.h"

#include <QModelIndex>
#include <QPersistentModelIndex>
#include <QTimer>
#include <QValidator>

namespace Utils {

class QTCREATOR_UTILS_EXPORT NameValueValidator : public QValidator
{
    Q_OBJECT
public:
    NameValueValidator(QWidget *parent,
                       Utils::NameValueModel *model,
                       QTreeView *view,
                       const QModelIndex &index,
                       const QString &toolTipText);

    QValidator::State validate(QString &in, int &pos) const override;

    void fixup(QString &input) const override;

private:
    const QString m_toolTipText;
    Utils::NameValueModel *m_model;
    QTreeView *m_view;
    QPersistentModelIndex m_index;
    mutable QTimer m_hideTipTimer;
};
} // namespace Utils
