#pragma once

#include "core_global.h"
#include "generatedfile.h"

#include <coreplugin/iwizardfactory.h>

#include <QList>
#include <QVariantMap>

QT_BEGIN_NAMESPACE
class QWizard;
class QWizardPage;
QT_END_NAMESPACE

namespace Utils { class Wizard; }

namespace Core {

class BaseFileWizard;

class CORE_EXPORT WizardDialogParameters
{
public:
    using WizardPageList = QList<QWizardPage *>;

    enum DialogParameterEnum {
        ForceCapitalLetterForFileName = 0x01
    };
    Q_DECLARE_FLAGS(DialogParameterFlags, DialogParameterEnum)

    explicit WizardDialogParameters(const QString &defaultPath, Utils::Id platform,
                                    const QSet<Utils::Id> &requiredFeatures, DialogParameterFlags flags,
                                    const QVariantMap &extraValues)
        : m_defaultPath(defaultPath),
          m_selectedPlatform(platform),
          m_requiredFeatures(requiredFeatures),
          m_parameterFlags(flags),
          m_extraValues(extraValues)
    {}

    QString defaultPath() const
    { return m_defaultPath; }

    Utils::Id selectedPlatform() const
    { return m_selectedPlatform; }

    QSet<Utils::Id> requiredFeatures() const
    { return m_requiredFeatures; }

    DialogParameterFlags flags() const
    { return m_parameterFlags; }

    QVariantMap extraValues() const
    { return m_extraValues; }

private:
    QString m_defaultPath;
    Utils::Id m_selectedPlatform;
    QSet<Utils::Id> m_requiredFeatures;
    DialogParameterFlags m_parameterFlags;
    QVariantMap m_extraValues;
};

class CORE_EXPORT BaseFileWizardFactory : public IWizardFactory
{
    Q_OBJECT

    friend class BaseFileWizard;

public:
    static QString buildFileName(const QString &path, const QString &baseName, const QString &extension);

protected:
    virtual BaseFileWizard *create(QWidget *parent, const WizardDialogParameters &parameters) const = 0;

    virtual GeneratedFiles generateFiles(const QWizard *w,
                                         QString *errorMessage) const = 0;

    virtual bool writeFiles(const GeneratedFiles &files, QString *errorMessage) const;

    virtual bool postGenerateFiles(const QWizard *w, const GeneratedFiles &l, QString *errorMessage) const;

    static QString preferredSuffix(const QString &mimeType);

    enum OverwriteResult { OverwriteOk,  OverwriteError,  OverwriteCanceled };
    static OverwriteResult promptOverwrite(GeneratedFiles *files,
                                           QString *errorMessage);
    static bool postGenerateOpenEditors(const GeneratedFiles &l, QString *errorMessage = nullptr);

private:
    // IWizard
    Utils::Wizard *runWizardImpl(const QString &path, QWidget *parent, Utils::Id platform,
                                 const QVariantMap &extraValues) override;
};

} // namespace Core

Q_DECLARE_OPERATORS_FOR_FLAGS(Core::GeneratedFile::Attributes)
Q_DECLARE_OPERATORS_FOR_FLAGS(Core::WizardDialogParameters::DialogParameterFlags)
