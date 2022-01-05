#pragma once

#include <QDialog>

#include <QPair>
#include <QSize>
#include <QVector>

QT_FORWARD_DECLARE_CLASS(QLineEdit)

namespace Utils { class PathChooser; }

namespace ImageViewer {
namespace Internal {

struct ExportData;

class MultiExportDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MultiExportDialog(QWidget *parent = nullptr);

    QString exportFileName() const;
    void setExportFileName(QString);

    void accept() override;

    void setSizes(const QVector<QSize> &);
    QVector<QSize> sizes() const;

    QVector<ExportData> exportData() const;

    static QVector<QSize> standardIconSizes();

    QSize svgSize() const { return m_svgSize; }
    void setSvgSize(const QSize &svgSize) { m_svgSize = svgSize; }

public slots:
    void setStandardIconSizes();
    void setGeneratedSizes();
    void suggestSizes();

private:
    QString sizesSpecification() const;

    Utils::PathChooser *m_pathChooser;
    QLineEdit *m_sizesLineEdit;
    QSize m_svgSize;

};

} // namespace Internal
} // namespace ImageViewer
