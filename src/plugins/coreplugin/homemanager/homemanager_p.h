#pragma once

#include "documentmodel.h"
#include "homearea.h"
#include "homemanager.h"
#include "homeview.h"
#include "ieditor.h"
#include "ieditorfactory.h"

#include <coreplugin/idocument.h>

#include <QList>
#include <QObject>
#include <QPointer>
#include <QSet>
#include <QString>
#include <QVariant>

QT_BEGIN_NAMESPACE
class QAction;
class QTimer;
QT_END_NAMESPACE

namespace Core {

class HomeManager;

namespace Internal {

class HomeWindow;
class MainWindow;
class OpenEditorsViewFactory;
class OpenEditorsWindow;

enum MakeWritableResult { OpenedWithVersionControl, MadeWritable, SavedAs, Failed };

class HomeManagerPrivate : public QObject
{
    Q_OBJECT

    friend class Core::HomeManager;

public:
    enum class CloseFlag {
        CloseWithAsking,
        CloseWithoutAsking,
        Suspend
    };

    static HomeManagerPrivate *instance();

    static void extensionsInitialized(); // only use from MainWindow

    static HomeArea *mainEditorArea();
    static HomeView *currentEditorView();
    static void setCurrentEditor(IEditor *editor, bool ignoreNavigationHistory = false);
    static IEditor *openEditor(HomeView *view,
                               const Utils::FilePath &filePath,
                               Utils::Id editorId = {},
                               HomeManager::OpenEditorFlags flags = HomeManager::NoFlags,
                               bool *newEditor = nullptr);
    static IEditor *openEditorAt(HomeView *view,
                                 const Utils::Link &filePath,
                                 Utils::Id editorId = {},
                                 HomeManager::OpenEditorFlags flags = HomeManager::NoFlags,
                                 bool *newEditor = nullptr);
    static IEditor *openEditorWith(const Utils::FilePath &filePath, Utils::Id editorId);
    static IEditor *duplicateEditor(IEditor *editor);
    static IEditor *activateEditor(HomeView *view, IEditor *editor,
                                   HomeManager::OpenEditorFlags flags = HomeManager::NoFlags);
    static IEditor *activateEditorForDocument(HomeView *view, IDocument *document,
                                              HomeManager::OpenEditorFlags flags = {});
    static bool activateEditorForEntry(HomeView *view, DocumentModel::Entry *entry,
                                       HomeManager::OpenEditorFlags flags = HomeManager::NoFlags);
    /* closes the document if there is no other editor on the document visible */
    static void closeEditorOrDocument(IEditor *editor);
    static bool closeEditors(const QList<IEditor *> &editors, CloseFlag flag);

    static HomeView *viewForEditor(IEditor *editor);
    static void setCurrentView(HomeView *view);
    static void activateView(HomeView *view);

    static MakeWritableResult makeFileWritable(IDocument *document);
    static void doEscapeKeyFocusMoveMagic();

    static Utils::Id getOpenWithEditorId(const QString &fileName, bool *isExternalEditor = nullptr);

    static void saveSettings();
    static void readSettings();
    static void setAutoSaveEnabled(bool enabled);
    static bool autoSaveEnabled();
    static void setAutoSaveInterval(int interval);
    static int autoSaveInterval();
    static void setAutoSuspendEnabled(bool enabled);
    static bool autoSuspendEnabled();
    static void setAutoSuspendMinDocumentCount(int count);
    static int autoSuspendMinDocumentCount();
    static void setWarnBeforeOpeningBigFilesEnabled(bool enabled);
    static bool warnBeforeOpeningBigFilesEnabled();
    static void setBigFileSizeLimit(int limitInMB);
    static int bigFileSizeLimit();
    static void setMaxRecentFiles(int count);
    static int maxRecentFiles();

    static HomeWindow *createEditorWindow();
    static void splitNewWindow(Internal::HomeView *view);
    static void closeView(Internal::HomeView *view);
    static const QList<IEditor *> emptyView(Internal::HomeView *view);
    static void deleteEditors(const QList<IEditor *> &editors);

    static void updateActions();

    static void updateWindowTitleForDocument(IDocument *document, QWidget *window);

    static void vcsOpenCurrentEditor();
    static void makeCurrentEditorWritable();

    static void setPlaceholderText(const QString &text);
    static QString placeholderText();

public slots:
    static bool saveDocument(Core::IDocument *document);
    static bool saveDocumentAs(Core::IDocument *document);

    static void split(Qt::Orientation orientation);
    static void removeAllSplits();
    static void gotoPreviousSplit();
    static void gotoNextSplit();

    void handleDocumentStateChange();
    void editorAreaDestroyed(QObject *area);

signals:
    void placeholderTextChanged(const QString &text);

private:
    static void gotoNextDocHistory();
    static void gotoPreviousDocHistory();

    static void gotoLastEditLocation();

    static void autoSave();
    static void handleContextChange(const QList<Core::IContext *> &context);

    static void copyFilePathFromContextMenu();
    void copyLocationFromContextMenu();
    static void copyFileNameFromContextMenu();
    static void saveDocumentFromContextMenu();
    static void saveDocumentAsFromContextMenu();
    static void revertToSavedFromContextMenu();
    static void closeEditorFromContextMenu();
    static void closeOtherDocumentsFromContextMenu();

    static void closeAllEditorsExceptVisible();
    static void revertToSaved(IDocument *document);
    static void autoSuspendDocuments();

    static void openTerminal();
    static void findInDirectory();

    static void togglePinned();

    static void removeCurrentSplit();

    static void setCurrentEditorFromContextChange();

    static OpenEditorsWindow *windowPopup();
    static void showPopupOrSelectDocument();

    static HomeFactoryList findFactories(Utils::Id editorId, const QString &fileName);
    static IEditor *createEditor(IEditorFactory *factory, const QString &fileName);
    static void addEditor(IEditor *editor);
    static void removeEditor(IEditor *editor, bool removeSusependedEntry);
    static IEditor *placeEditor(HomeView *view, IEditor *editor);
    static void restoreEditorState(IEditor *editor);
    static int visibleDocumentsCount();
    static HomeArea *findEditorArea(const HomeView *view, int *areaIndex = nullptr);
    static IEditor *pickUnusedEditor(Internal::HomeView **foundView = nullptr);
    static void addDocumentToRecentFiles(IDocument *document);
    static void updateAutoSave();
    static void updateMakeWritableWarning();
    static void setupSaveActions(IDocument *document, QAction *saveAction,
                                 QAction *saveAsAction, QAction *revertToSavedAction);
    static void updateWindowTitle();
    static bool skipOpeningBigTextFile(const Utils::FilePath &filePath);

private:
    explicit HomeManagerPrivate(QObject *parent);
    ~HomeManagerPrivate() override;
    void init();

    EditLocation m_globalLastEditLocation;
    QList<EditLocation> m_globalHistory;
    QList<HomeArea *> m_editorAreas;
    QPointer<IEditor> m_currentEditor;
    QPointer<IEditor> m_scheduledCurrentEditor;
    QPointer<HomeView> m_currentView;
    QTimer *m_autoSaveTimer = nullptr;

    // actions
    QAction *m_revertToSavedAction;
    QAction *m_saveAction;
    QAction *m_saveAsAction;
    QAction *m_closeCurrentEditorAction;
    QAction *m_closeAllEditorsAction;
    QAction *m_closeOtherDocumentsAction;
    QAction *m_closeAllEditorsExceptVisibleAction;
    QAction *m_gotoNextDocHistoryAction;
    QAction *m_gotoPreviousDocHistoryAction;
    QAction *m_goBackAction;
    QAction *m_goForwardAction;
    QAction *m_gotoLastEditAction;
    QAction *m_splitAction;
    QAction *m_splitSideBySideAction;
    QAction *m_splitNewWindowAction;
    QAction *m_removeCurrentSplitAction;
    QAction *m_removeAllSplitsAction;
    QAction *m_gotoPreviousSplitAction;
    QAction *m_gotoNextSplitAction;

    QAction *m_copyFilePathContextAction;
    QAction *m_copyLocationContextAction; // Copy path and line number.
    QAction *m_copyFileNameContextAction;
    QAction *m_saveCurrentEditorContextAction;
    QAction *m_saveAsCurrentEditorContextAction;
    QAction *m_revertToSavedCurrentEditorContextAction;

    QAction *m_closeCurrentEditorContextAction;
    QAction *m_closeAllEditorsContextAction;
    QAction *m_closeOtherDocumentsContextAction;
    QAction *m_closeAllEditorsExceptVisibleContextAction;
    QAction *m_openGraphicalShellAction;
    QAction *m_openGraphicalShellContextAction;
    QAction *m_openTerminalAction;
    QAction *m_findInDirectoryAction;
    QAction *m_filePropertiesAction = nullptr;
    QAction *m_pinAction = nullptr;
    DocumentModel::Entry *m_contextMenuEntry = nullptr;
    IEditor *m_contextMenuEditor = nullptr;

    OpenEditorsWindow *m_windowPopup = nullptr;

    QMap<QString, QVariant> m_editorStates;
    OpenEditorsViewFactory *m_openEditorsFactory = nullptr;

    HomeManager::WindowTitleHandler m_titleAdditionHandler;
    HomeManager::WindowTitleHandler m_sessionTitleHandler;
    HomeManager::WindowTitleHandler m_titleVcsTopicHandler;

    struct Settings
    {
        IDocument::ReloadSetting reloadSetting = IDocument::AlwaysAsk;

        bool autoSaveEnabled = true;
        int autoSaveInterval = 5;

        bool autoSuspendEnabled = true;
        int autoSuspendMinDocumentCount = 30;

        bool warnBeforeOpeningBigFilesEnabled = true;
        int bigFileSizeLimitInMB = 5;
        int maxRecentFiles = 8;
    };

    Settings m_settings;
    QString m_placeholderText;
    QList<std::function<bool(IEditor *)>> m_closeEditorListeners;
};

} // Internal
} // Core
