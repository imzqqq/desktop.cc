// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "resourceeditor.h"
#include "resourceeditorconstants.h"
#include "resourceeditortr.h"
#include "resourcenode.h"

#include <coreplugin/icore.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/editormanager/editormanager.h>

#include <extensionsystem/iplugin.h>
#include <extensionsystem/pluginmanager.h>

#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/projecttree.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projectnodes.h>

#include <utils/algorithm.h>
#include <utils/parameteraction.h>
#include <utils/qtcassert.h>
#include <utils/stringutils.h>

#include <QAction>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>

using namespace ProjectExplorer;
using namespace Utils;
using namespace Core;

namespace ResourceEditor::Internal {

const char resourcePrefix[] = ":";
const char urlPrefix[] = "qrc:";

class PrefixLangDialog final : public QDialog
{
public:
    PrefixLangDialog(const QString &title, const QString &prefix, const QString &lang)
        : QDialog(ICore::dialogParent())
    {
        setWindowTitle(title);
        auto layout = new QFormLayout(this);
        m_prefixLineEdit = new QLineEdit(this);
        m_prefixLineEdit->setText(prefix);
        layout->addRow(Tr::tr("Prefix:"), m_prefixLineEdit);

        m_langLineEdit = new QLineEdit(this);
        m_langLineEdit->setText(lang);
        layout->addRow(Tr::tr("Language:"), m_langLineEdit);

        QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok
                                                         | QDialogButtonBox::Cancel,
                                                         Qt::Horizontal,
                                                         this);

        layout->addWidget(buttons);

        connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

    QString prefix() const
    {
        return m_prefixLineEdit->text();
    }

    QString lang() const
    {
        return m_langLineEdit->text();
    }

private:
    QLineEdit *m_prefixLineEdit;
    QLineEdit *m_langLineEdit;
};

// ResourceEditorPlugin

class ResourceEditorPlugin final : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "ResourceEditor.json")

    void initialize() final;

    void extensionsInitialized() final;

    void onUndo();
    void onRedo();
    void onRefresh();

    void addPrefixContextMenu();
    void renamePrefixContextMenu();
    void removePrefixContextMenu();
    void renameFileContextMenu();
    void removeFileContextMenu();
    void removeNonExisting();

    void openEditorContextMenu();

    void copyPathContextMenu();
    void copyUrlContextMenu();

    void updateContextActions(Node *node);

    // project tree's folder context menu
    QAction *m_addPrefix = nullptr;
    QAction *m_removePrefix = nullptr;
    QAction *m_renamePrefix = nullptr;
    QAction *m_removeNonExisting = nullptr;

    QAction *m_renameResourceFile = nullptr;
    QAction *m_removeResourceFile = nullptr;

    QAction *m_openInEditor = nullptr;
    QMenu *m_openWithMenu = nullptr;

    // file context menu
    ParameterAction *m_copyPath = nullptr;
    ParameterAction *m_copyUrl = nullptr;
};

void ResourceEditorPlugin::initialize()
{
    setupResourceEditor(this);

    Context projectTreeContext(ProjectExplorer::Constants::C_PROJECT_TREE);
    ActionContainer *folderContextMenu =
            ActionManager::actionContainer(ProjectExplorer::Constants::M_FOLDERCONTEXT);
    ActionContainer *fileContextMenu =
            ActionManager::actionContainer(ProjectExplorer::Constants::M_FILECONTEXT);
    Command *command = nullptr;

    m_addPrefix = new QAction(Tr::tr("Add Prefix..."), this);
    command = ActionManager::registerAction(m_addPrefix, Constants::C_ADD_PREFIX, projectTreeContext);
    folderContextMenu->addAction(command, ProjectExplorer::Constants::G_FOLDER_FILES);
    connect(m_addPrefix, &QAction::triggered, this, &ResourceEditorPlugin::addPrefixContextMenu);

    m_renamePrefix = new QAction(Tr::tr("Change Prefix..."), this);
    command = ActionManager::registerAction(m_renamePrefix, Constants::C_RENAME_PREFIX, projectTreeContext);
    folderContextMenu->addAction(command, ProjectExplorer::Constants::G_FOLDER_FILES);
    connect(m_renamePrefix, &QAction::triggered, this, &ResourceEditorPlugin::renamePrefixContextMenu);

    m_removePrefix = new QAction(Tr::tr("Remove Prefix..."), this);
    command = ActionManager::registerAction(m_removePrefix, Constants::C_REMOVE_PREFIX, projectTreeContext);
    folderContextMenu->addAction(command, ProjectExplorer::Constants::G_FOLDER_FILES);
    connect(m_removePrefix, &QAction::triggered, this, &ResourceEditorPlugin::removePrefixContextMenu);

    m_removeNonExisting = new QAction(Tr::tr("Remove Missing Files"), this);
    command = ActionManager::registerAction(m_removeNonExisting, Constants::C_REMOVE_NON_EXISTING, projectTreeContext);
    folderContextMenu->addAction(command, ProjectExplorer::Constants::G_FOLDER_FILES);
    connect(m_removeNonExisting, &QAction::triggered, this, &ResourceEditorPlugin::removeNonExisting);

    m_renameResourceFile = new QAction(Tr::tr("Rename..."), this);
    command = ActionManager::registerAction(m_renameResourceFile, Constants::C_RENAME_FILE, projectTreeContext);
    folderContextMenu->addAction(command, ProjectExplorer::Constants::G_FOLDER_FILES);
    connect(m_renameResourceFile, &QAction::triggered, this, &ResourceEditorPlugin::renameFileContextMenu);

    m_removeResourceFile = new QAction(Tr::tr("Remove File..."), this);
    command = ActionManager::registerAction(m_removeResourceFile, Constants::C_REMOVE_FILE, projectTreeContext);
    folderContextMenu->addAction(command, ProjectExplorer::Constants::G_FOLDER_FILES);
    connect(m_removeResourceFile, &QAction::triggered, this, &ResourceEditorPlugin::removeFileContextMenu);

    m_openInEditor = new QAction(Tr::tr("Open in Editor"), this);
    command = ActionManager::registerAction(m_openInEditor, Constants::C_OPEN_EDITOR, projectTreeContext);
    folderContextMenu->addAction(command, ProjectExplorer::Constants::G_FOLDER_FILES);
    connect(m_openInEditor, &QAction::triggered, this, &ResourceEditorPlugin::openEditorContextMenu);

    m_openWithMenu = new QMenu(Tr::tr("Open With"), folderContextMenu->menu());
    folderContextMenu->menu()->insertMenu(
                folderContextMenu->insertLocation(ProjectExplorer::Constants::G_FOLDER_FILES),
                m_openWithMenu);

    m_copyPath = new ParameterAction(Tr::tr("Copy Path"), Tr::tr("Copy Path \"%1\""), ParameterAction::AlwaysEnabled, this);
    command = ActionManager::registerAction(m_copyPath, Constants::C_COPY_PATH, projectTreeContext);
    command->setAttribute(Command::CA_UpdateText);
    fileContextMenu->addAction(command, ProjectExplorer::Constants::G_FILE_OTHER);
    connect(m_copyPath, &QAction::triggered, this, &ResourceEditorPlugin::copyPathContextMenu);

    m_copyUrl = new ParameterAction(Tr::tr("Copy URL"), Tr::tr("Copy URL \"%1\""), ParameterAction::AlwaysEnabled, this);
    command = ActionManager::registerAction(m_copyUrl, Constants::C_COPY_URL, projectTreeContext);
    command->setAttribute(Command::CA_UpdateText);
    fileContextMenu->addAction(command, ProjectExplorer::Constants::G_FILE_OTHER);
    connect(m_copyUrl, &QAction::triggered, this, &ResourceEditorPlugin::copyUrlContextMenu);

    m_addPrefix->setEnabled(false);
    m_removePrefix->setEnabled(false);
    m_renamePrefix->setEnabled(false);
    m_removeNonExisting->setEnabled(false);
    m_renameResourceFile->setEnabled(false);
    m_removeResourceFile->setEnabled(false);

    connect(ProjectTree::instance(), &ProjectTree::currentNodeChanged,
            this, &ResourceEditorPlugin::updateContextActions);
}

void ResourceEditorPlugin::addPrefixContextMenu()
{
    auto topLevel = dynamic_cast<ResourceTopLevelNode *>(ProjectTree::currentNode());
    QTC_ASSERT(topLevel, return);
    PrefixLangDialog dialog(Tr::tr("Add Prefix"), QString(), QString());
    if (dialog.exec() != QDialog::Accepted)
        return;

    QString prefix = dialog.prefix();
    if (prefix.isEmpty())
        return;
    topLevel->addPrefix(prefix, dialog.lang());
}

void ResourceEditorPlugin::removePrefixContextMenu()
{
    auto rfn = dynamic_cast<ResourceFolderNode *>(ProjectTree::currentNode());
    QTC_ASSERT(rfn, return);
    if (QMessageBox::question(ICore::dialogParent(),
                              Tr::tr("Remove Prefix"),
                              Tr::tr("Remove prefix %1 and all its files?").arg(rfn->displayName()))
            == QMessageBox::Yes) {
        ResourceTopLevelNode *rn = rfn->resourceNode();
        rn->removePrefix(rfn->prefix(), rfn->lang());
    }
}

void ResourceEditorPlugin::removeNonExisting()
{
    auto topLevel = dynamic_cast<ResourceTopLevelNode *>(ProjectTree::currentNode());
    QTC_ASSERT(topLevel, return);
    topLevel->removeNonExistingFiles();
}

void ResourceEditorPlugin::renameFileContextMenu()
{
    ProjectExplorerPlugin::initiateInlineRenaming();
}

void ResourceEditorPlugin::removeFileContextMenu()
{
    auto rfn = dynamic_cast<ResourceTopLevelNode *>(ProjectTree::currentNode());
    QTC_ASSERT(rfn, return);
    FilePath path = rfn->filePath();
    FolderNode *parent = rfn->parentFolderNode();
    QTC_ASSERT(parent, return);
    if (parent->removeFiles({path}) != RemovedFilesFromProject::Ok)
        QMessageBox::warning(ICore::dialogParent(),
                             Tr::tr("File Removal Failed"),
                             Tr::tr("Removing file %1 from the project failed.").arg(path.toUserOutput()));
}

void ResourceEditorPlugin::openEditorContextMenu()
{
    EditorManager::openEditor(ProjectTree::currentNode()->filePath());
}

void ResourceEditorPlugin::copyPathContextMenu()
{
    auto node = dynamic_cast<ResourceFileNode *>(ProjectTree::currentNode());
    QTC_ASSERT(node, return);
    setClipboardAndSelection(QLatin1String(resourcePrefix) + node->qrcPath());
}

void ResourceEditorPlugin::copyUrlContextMenu()
{
    auto node = dynamic_cast<ResourceFileNode *>(ProjectTree::currentNode());
    QTC_ASSERT(node, return);
    setClipboardAndSelection(QLatin1String(urlPrefix) + node->qrcPath());
}

void ResourceEditorPlugin::renamePrefixContextMenu()
{
    auto node = dynamic_cast<ResourceFolderNode *>(ProjectTree::currentNode());
    QTC_ASSERT(node, return);

    PrefixLangDialog dialog(Tr::tr("Rename Prefix"), node->prefix(), node->lang());
    if (dialog.exec() != QDialog::Accepted)
        return;

    QString prefix = dialog.prefix();
    if (prefix.isEmpty())
        return;

    node->renamePrefix(prefix, dialog.lang());
}

void ResourceEditorPlugin::updateContextActions(Node *node)
{
    const bool isResourceNode = dynamic_cast<const ResourceTopLevelNode *>(node);
    m_addPrefix->setEnabled(isResourceNode);
    m_addPrefix->setVisible(isResourceNode);

    bool enableRename = false;
    bool enableRemove = false;

    if (isResourceNode) {
        FolderNode *parent = node ? node->parentFolderNode() : nullptr;
        enableRename = parent && parent->supportsAction(Rename, node);
        enableRemove = parent && parent->supportsAction(RemoveFile, node);
    }

    m_renameResourceFile->setEnabled(isResourceNode && enableRename);
    m_renameResourceFile->setVisible(isResourceNode && enableRename);
    m_removeResourceFile->setEnabled(isResourceNode && enableRemove);
    m_removeResourceFile->setVisible(isResourceNode && enableRemove);

    m_openInEditor->setEnabled(isResourceNode);
    m_openInEditor->setVisible(isResourceNode);

    const bool isResourceFolder = dynamic_cast<const ResourceFolderNode *>(node);
    m_removePrefix->setEnabled(isResourceFolder);
    m_removePrefix->setVisible(isResourceFolder);

    m_renamePrefix->setEnabled(isResourceFolder);
    m_renamePrefix->setVisible(isResourceFolder);

    m_removeNonExisting->setEnabled(isResourceNode);
    m_removeNonExisting->setVisible(isResourceNode);

    if (isResourceNode)
        EditorManager::populateOpenWithMenu(m_openWithMenu, node->filePath());
    else
        m_openWithMenu->clear();
    m_openWithMenu->menuAction()->setVisible(!m_openWithMenu->actions().isEmpty());

    const bool isResourceFile = dynamic_cast<const ResourceFileNode *>(node);
    m_copyPath->setEnabled(isResourceFile);
    m_copyPath->setVisible(isResourceFile);
    m_copyUrl->setEnabled(isResourceFile);
    m_copyUrl->setVisible(isResourceFile);
    if (isResourceFile) {
        auto fileNode = dynamic_cast<const ResourceFileNode *>(node);
        QTC_ASSERT(fileNode, return);
        QString qrcPath = fileNode->qrcPath();
        m_copyPath->setParameter(QLatin1String(resourcePrefix) + qrcPath);
        m_copyUrl->setParameter(QLatin1String(urlPrefix) + qrcPath);
    }
}

void ResourceEditorPlugin::extensionsInitialized()
{
    ProjectTree::registerTreeManager([](FolderNode *folder, ProjectTree::ConstructionPhase phase) {
        switch (phase) {
        case ProjectTree::AsyncPhase: {
            QList<FileNode *> toReplace;
            folder->forEachNode([&toReplace](FileNode *fn) {
                if (fn->fileType() == FileType::Resource)
                    toReplace.append(fn);
            }, {}, [](const FolderNode *fn) {
                return dynamic_cast<const ResourceTopLevelNode *>(fn) == nullptr;
            });
            for (FileNode *file : std::as_const(toReplace)) {
                FolderNode *const pn = file->parentFolderNode();
                QTC_ASSERT(pn, continue);
                const FilePath path = file->filePath();
                auto topLevel = std::make_unique<ResourceTopLevelNode>(path, pn->filePath());
                topLevel->setEnabled(file->isEnabled());
                topLevel->setIsGenerated(file->isGenerated());
                pn->replaceSubtree(file, std::move(topLevel));
            }
            break;
        }
        case ProjectTree::FinalPhase: {
            folder->forEachNode({}, [](FolderNode *fn) {
                if (auto topLevel = dynamic_cast<ResourceTopLevelNode *>(fn))
                    topLevel->setupWatcherIfNeeded();
            });
            break;
        }
        }
    });
}

} // ResourceEditor::Internal

#include "resourceeditorplugin.moc"
