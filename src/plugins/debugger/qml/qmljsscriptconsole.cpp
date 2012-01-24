/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#include "qmljsscriptconsole.h"
#include "interactiveinterpreter.h"
#include "qmladapter.h"
#include "debuggerstringutils.h"

#include <texteditor/fontsettings.h>
#include <texteditor/texteditorsettings.h>

#include <extensionsystem/pluginmanager.h>
#include <coreplugin/coreconstants.h>
#include <utils/statuslabel.h>
#include <utils/styledbar.h>

#include <coreplugin/icore.h>
#include <utils/qtcassert.h>

#include <qmljsdebugclient/qdebugmessageclient.h>
#include <debugger/qml/qmlcppengine.h>
#include <debugger/qml/qmlengine.h>
#include <debugger/stackhandler.h>
#include <debugger/stackframe.h>

#include <QtGui/QMenu>
#include <QtGui/QTextBlock>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QCheckBox>

static const char SCRIPT_CONSOLE[] = "ScriptConsole";
static const char SHOW_LOG[] = "showLog";
static const char SHOW_WARNING[] = "showWarning";
static const char SHOW_ERROR[] = "showError";

namespace Debugger {
namespace Internal {

class QmlJSScriptConsolePrivate
{
public:
    QmlJSScriptConsolePrivate()
        : adapter(0),
          prompt(_("> ")),
          startOfEditableArea(-1),
          lastKnownPosition(0),
          inferiorStopped(false)
    {
        scriptHistory.append(QString());
        scriptHistoryIndex = scriptHistory.count();
    }

    void appendToHistory(const QString &script);
    bool canEvaluateScript(const QString &script);

    QmlAdapter *adapter;

    QString prompt;
    int startOfEditableArea;
    int lastKnownPosition;

    QStringList scriptHistory;
    int scriptHistoryIndex;

    InteractiveInterpreter interpreter;

    bool inferiorStopped;

    QFlags<QmlJSScriptConsole::DebugLevelFlag> debugLevel;
};

void QmlJSScriptConsolePrivate::appendToHistory(const QString &script)
{
    scriptHistoryIndex = scriptHistory.count();
    scriptHistory.replace(scriptHistoryIndex - 1,script);
    scriptHistory.append(QString());
    scriptHistoryIndex = scriptHistory.count();
}

bool QmlJSScriptConsolePrivate::canEvaluateScript(const QString &script)
{
    interpreter.clearText();
    interpreter.appendText(script);
    return interpreter.canEvaluate();
}

///////////////////////////////////////////////////////////////////////
//
// QmlJSScriptConsoleWidget
//
///////////////////////////////////////////////////////////////////////

QmlJSScriptConsoleWidget::QmlJSScriptConsoleWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setMargin(0);
    vbox->setSpacing(0);

    QWidget *statusbarContainer = new QWidget;

    QHBoxLayout *hbox = new QHBoxLayout(statusbarContainer);
    hbox->setMargin(0);
    hbox->setSpacing(0);

    //Clear Button
    QToolButton *clearButton = new QToolButton;
    QAction *clearAction = new QAction(tr("Clear Console"), this);
    clearAction->setIcon(QIcon(_(Core::Constants::ICON_CLEAN_PANE)));

    clearButton->setDefaultAction(clearAction);

    //Status Label
    m_statusLabel = new Utils::StatusLabel;

    hbox->addWidget(m_statusLabel, 20, Qt::AlignLeft);
    hbox->addWidget(new Utils::StyledSeparator);
    m_showLog = new QCheckBox(tr("Log"), this);
    m_showWarning = new QCheckBox(tr("Warning"), this);
    m_showError = new QCheckBox(tr("Error"), this);
    connect(m_showLog, SIGNAL(stateChanged(int)), this, SLOT(setDebugLevel()));
    connect(m_showWarning, SIGNAL(stateChanged(int)), this, SLOT(setDebugLevel()));
    connect(m_showError, SIGNAL(stateChanged(int)), this, SLOT(setDebugLevel()));
    hbox->addWidget(m_showLog);
    hbox->addWidget(m_showWarning);
    hbox->addWidget(m_showError);
    hbox->addWidget(new Utils::StyledSeparator);
    hbox->addWidget(clearButton, 0, Qt::AlignRight);

    m_console = new QmlJSScriptConsole(this);
    connect(m_console, SIGNAL(evaluateExpression(QString)), this,
            SIGNAL(evaluateExpression(QString)));
    connect(m_console, SIGNAL(updateStatusMessage(const QString &, int)), m_statusLabel,
            SLOT(showStatusMessage(const QString &, int)));
    connect(clearAction, SIGNAL(triggered()), m_console, SLOT(clear()));
    vbox->addWidget(statusbarContainer);
    vbox->addWidget(m_console);

    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(_(SCRIPT_CONSOLE));
    m_showLog->setChecked(settings->value(_(SHOW_LOG), true).toBool());
    m_showWarning->setChecked(settings->value(_(SHOW_WARNING), true).toBool());
    m_showError->setChecked(settings->value(_(SHOW_ERROR), true).toBool());
    settings->endGroup();
}

QmlJSScriptConsoleWidget::~QmlJSScriptConsoleWidget()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(_(SCRIPT_CONSOLE));
    settings->setValue(_(SHOW_LOG), QVariant(m_showLog->isChecked()));
    settings->setValue(_(SHOW_WARNING), QVariant(m_showWarning->isChecked()));
    settings->setValue(_(SHOW_ERROR), QVariant(m_showError->isChecked()));
    settings->endGroup();
}

void QmlJSScriptConsoleWidget::setEngine(DebuggerEngine *engine)
{
    if (m_console->engine())
        disconnect(m_console->engine(), SIGNAL(stateChanged(Debugger::DebuggerState)),
                   this, SLOT(engineStateChanged(Debugger::DebuggerState)));

    QmlEngine *qmlEngine = qobject_cast<QmlEngine *>(engine);
    QmlCppEngine *qmlCppEngine = qobject_cast<QmlCppEngine *>(engine);
    if (qmlCppEngine)
        qmlEngine = qobject_cast<QmlEngine *>(qmlCppEngine->qmlEngine());

    //Supports only QML Engine
    if (qmlEngine) {
        connect(qmlEngine, SIGNAL(stateChanged(Debugger::DebuggerState)),
                this, SLOT(engineStateChanged(Debugger::DebuggerState)));

        engineStateChanged(qmlEngine->state());
    }

    m_console->setEngine(qmlEngine);
}

void QmlJSScriptConsoleWidget::appendResult(const QString &result)
{
    m_console->appendResult(result);
}

void QmlJSScriptConsoleWidget::setDebugLevel()
{
    QFlags<QmlJSScriptConsole::DebugLevelFlag> level;

    if (m_showLog->isChecked())
        level |= QmlJSScriptConsole::Log;

    if (m_showWarning->isChecked())
        level |= QmlJSScriptConsole::Warning;

    if (m_showError->isChecked())
        level |= QmlJSScriptConsole::Error;

    m_console->setDebugLevel(level);
}

void QmlJSScriptConsoleWidget::engineStateChanged(Debugger::DebuggerState state)
{
    if (state == InferiorRunOk || state == InferiorStopOk) {
        setEnabled(true);
        m_console->setInferiorStopped(state == InferiorStopOk);
    } else {
        setEnabled(false);
    }
}

///////////////////////////////////////////////////////////////////////
//
// QmlJSScriptConsole
//
///////////////////////////////////////////////////////////////////////

QmlJSScriptConsole::QmlJSScriptConsole(QWidget *parent)
    : QPlainTextEdit(parent),
      d(new QmlJSScriptConsolePrivate())
{
    connect(this, SIGNAL(cursorPositionChanged()), SLOT(onCursorPositionChanged()));

    setFrameStyle(QFrame::NoFrame);
    setUndoRedoEnabled(false);
    setBackgroundVisible(false);
    const TextEditor::FontSettings &fs = TextEditor::TextEditorSettings::instance()->fontSettings();
    setFont(fs.font());

    displayPrompt();
}

QmlJSScriptConsole::~QmlJSScriptConsole()
{
    delete d;
}

void QmlJSScriptConsole::setPrompt(const QString &prompt)
{
    d->prompt = prompt;
}

QString QmlJSScriptConsole::prompt() const
{
    return d->prompt;
}

void QmlJSScriptConsole::setInferiorStopped(bool inferiorStopped)
{
    d->inferiorStopped = inferiorStopped;
    onSelectionChanged();
}

void QmlJSScriptConsole::setEngine(QmlEngine *eng)
{
    if (d->adapter) {
        disconnect(engine()->stackHandler(), SIGNAL(currentIndexChanged()), this, SLOT(onSelectionChanged()));
        disconnect(d->adapter, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
        disconnect(d->adapter->messageClient(), SIGNAL(message(QtMsgType,QString)),
                this, SLOT(insertDebugOutput(QtMsgType,QString)));
        d->adapter = 0;
    }

    if (eng) {
        d->adapter = eng->adapter();
        connect(eng->stackHandler(), SIGNAL(currentIndexChanged()), this, SLOT(onSelectionChanged()));
        connect(d->adapter, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
        connect(d->adapter->messageClient(), SIGNAL(message(QtMsgType,QString)),
                this, SLOT(insertDebugOutput(QtMsgType,QString)));
    }

    clear();
}

DebuggerEngine *QmlJSScriptConsole::engine()
{
    if (d->adapter) {
        return d->adapter->debuggerEngine();
    }
    return 0;
}

void QmlJSScriptConsole::appendResult(const QString &message, const QColor &color)
{
    QTextCharFormat resultFormat;
    resultFormat.setForeground(color);
    QTextCursor cur = textCursor();

    cur.setPosition(d->startOfEditableArea - d->prompt.length());
    cur.insertText(message, resultFormat);
    cur.insertText(_("\n"));

    QList<QTextEdit::ExtraSelection> selections = extraSelections();
    QTextEdit::ExtraSelection sel;
    sel.format = resultFormat;
    sel.cursor = cur;
    selections.append(sel);

    setExtraSelections(selections);

    d->startOfEditableArea += message.length() + 1; //1 for new line character
}

void QmlJSScriptConsole::setDebugLevel(QFlags<DebugLevelFlag> level)
{
    d->debugLevel = level;
}

void QmlJSScriptConsole::clear()
{
    QPlainTextEdit::clear();
    displayPrompt();
}

void QmlJSScriptConsole::onStateChanged(QmlJsDebugClient::QDeclarativeDebugQuery::State state)
{
    QDeclarativeDebugExpressionQuery *query = qobject_cast<QDeclarativeDebugExpressionQuery *>(sender());

    if (query && state != QDeclarativeDebugQuery::Error) {
        QString result(query->result().toString());
        if (result == _("<undefined>") && d->inferiorStopped) {
            //don't give up. check if we can still evaluate using javascript engine
            emit evaluateExpression(getCurrentScript());
        } else {
            appendResult(result);
        }
    } else {
        QPlainTextEdit::appendPlainText(QString());
        moveCursor(QTextCursor::EndOfLine);
    }
    delete query;
}

void QmlJSScriptConsole::onSelectionChanged()
{
    if (d->adapter) {
        QString status(tr("Context: "));
        if (!d->inferiorStopped) {
            status.append(d->adapter->currentSelectedDisplayName());
        } else {
            status.append(engine()->stackHandler()->currentFrame().function);
        }
        emit updateStatusMessage(status, 0);
    }
}

void QmlJSScriptConsole::insertDebugOutput(QtMsgType type, const QString &debugMsg)
{
    QColor color;
    switch (type) {
    case QtDebugMsg:
        if (!(d->debugLevel & Log))
            return;
        color = QColor(Qt::darkBlue);
        break;
    case QtWarningMsg:
        if (!(d->debugLevel & Warning))
            return;
        color = QColor(Qt::darkYellow);
        break;
    case QtCriticalMsg:
        if (!(d->debugLevel & Error))
            return;
        color = QColor(Qt::darkRed);
        break;
    default:
        color = QColor(Qt::black);
    }
    appendResult(debugMsg, color);
}

void QmlJSScriptConsole::keyPressEvent(QKeyEvent *e)
{
    bool keyConsumed = false;
    switch (e->key()) {

    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (isEditableArea()) {
            handleReturnKey();
            keyConsumed = true;
        }
        break;

    case Qt::Key_Backspace: {
        QTextCursor cursor = textCursor();
        bool hasSelection = cursor.hasSelection();
        int selectionStart = cursor.selectionStart();
        if ((hasSelection && selectionStart < d->startOfEditableArea)
                || (!hasSelection && selectionStart == d->startOfEditableArea)) {
            keyConsumed = true;
        }
        break;
    }

    case Qt::Key_Delete:
        if (textCursor().selectionStart() < d->startOfEditableArea) {
            keyConsumed = true;
        }
        break;

    case Qt::Key_Tab:
    case Qt::Key_Backtab:
        keyConsumed = true;
        break;

    case Qt::Key_Left:
        if (textCursor().position() == d->startOfEditableArea) {
            keyConsumed = true;
        } else if (e->modifiers() & Qt::ControlModifier && isEditableArea()) {
            handleHomeKey();
            keyConsumed = true;
        }
        break;

    case Qt::Key_Up:
        if (isEditableArea()) {
            handleUpKey();
            keyConsumed = true;
        }
        break;

    case Qt::Key_Down:
        if (isEditableArea()) {
            handleDownKey();
            keyConsumed = true;
        }
        break;

    case Qt::Key_Home:
        if (isEditableArea()) {
            handleHomeKey();
            keyConsumed = true;
        }
        break;

    case Qt::Key_C:
    case Qt::Key_Insert: {
        //Fair to assume that for any selection beyond startOfEditableArea
        //only copy function is allowed.
        QTextCursor cursor = textCursor();
        bool hasSelection = cursor.hasSelection();
        int selectionStart = cursor.selectionStart();
        if (hasSelection && selectionStart < d->startOfEditableArea) {
            if (!(e->modifiers() & Qt::ControlModifier))
                keyConsumed = true;
        }
        break;
    }

    default: {
        QTextCursor cursor = textCursor();
        bool hasSelection = cursor.hasSelection();
        int selectionStart = cursor.selectionStart();
        if (hasSelection && selectionStart < d->startOfEditableArea) {
            keyConsumed = true;
        }
        break;
    }
    }

    if (!keyConsumed)
        QPlainTextEdit::keyPressEvent(e);
}

void QmlJSScriptConsole::contextMenuEvent(QContextMenuEvent *event)
{
    QTextCursor cursor = textCursor();
    Qt::TextInteractionFlags flags = textInteractionFlags();
    bool hasSelection = cursor.hasSelection();
    int selectionStart = cursor.selectionStart();
    bool canBeEdited = true;
    if (hasSelection && selectionStart < d->startOfEditableArea) {
        canBeEdited = false;
    }

    QMenu *menu = new QMenu();
    QAction *a;

    if ((flags & Qt::TextEditable) && canBeEdited) {
        a = menu->addAction(tr("Cut"), this, SLOT(cut()));
        a->setEnabled(cursor.hasSelection());
    }


    a = menu->addAction(tr("Copy"), this, SLOT(copy()));
    a->setEnabled(cursor.hasSelection());

    if ((flags & Qt::TextEditable) && canBeEdited) {
        a = menu->addAction(tr("Paste"), this, SLOT(paste()));
        a->setEnabled(canPaste());
    }

    menu->addSeparator();
    a = menu->addAction(tr("Select All"), this, SLOT(selectAll()));
    a->setEnabled(!document()->isEmpty());

    menu->addSeparator();
    menu->addAction(tr("Clear"), this, SLOT(clear()));

    menu->exec(event->globalPos());


    delete menu;
}

void QmlJSScriptConsole::mouseReleaseEvent(QMouseEvent *e)
{
    QPlainTextEdit::mouseReleaseEvent(e);
    QTextCursor cursor = textCursor();
    if (e->button() == Qt::LeftButton && !cursor.hasSelection() && !isEditableArea()) {
        cursor.setPosition(d->lastKnownPosition);
        setTextCursor(cursor);
    }
}

void QmlJSScriptConsole::onCursorPositionChanged()
{
    if (!isEditableArea()) {
        setTextInteractionFlags(Qt::TextSelectableByMouse);
    } else {
        d->lastKnownPosition = textCursor().position();
        setTextInteractionFlags(Qt::TextEditorInteraction);
    }
}

void QmlJSScriptConsole::displayPrompt()
{
    d->startOfEditableArea = textCursor().position() + d->prompt.length();
    QTextCursor cur = textCursor();
    cur.insertText(d->prompt);
    cur.movePosition(QTextCursor::EndOfWord);
    setTextCursor(cur);
}

void QmlJSScriptConsole::handleReturnKey()
{
    QString currentScript = getCurrentScript();
    bool scriptEvaluated = false;

    //Check if string is only white spaces
    if (currentScript.trimmed().isEmpty()) {
        scriptEvaluated = true;
    }

    if (!scriptEvaluated) {
        //check if it can be evaluated
        if (d->canEvaluateScript(currentScript)) {

            //Select the engine for evaluation based on
            //inferior state
            if (!d->inferiorStopped) {

                if (d->adapter) {
                    QDeclarativeEngineDebug *engineDebug = d->adapter->engineDebugClient();
                    int id = d->adapter->currentSelectedDebugId();
                    if (engineDebug && id != -1) {
                        QDeclarativeDebugExpressionQuery *query =
                                engineDebug->queryExpressionResult(id, currentScript, this);
                        connect(query, SIGNAL(stateChanged(QmlJsDebugClient::QDeclarativeDebugQuery::State)),
                                this, SLOT(onStateChanged(QmlJsDebugClient::QDeclarativeDebugQuery::State)));
                        scriptEvaluated = true;
                    }
                }
            }

            if (!scriptEvaluated) {
                emit evaluateExpression(currentScript);
                scriptEvaluated = true;
            }

            if (scriptEvaluated) {
                d->appendToHistory(currentScript);
            }
        }
    }

    if (!scriptEvaluated) {
        QPlainTextEdit::appendPlainText(QString());
        moveCursor(QTextCursor::EndOfLine);
    } else {
        QTextCursor cur = textCursor();
        cur.movePosition(QTextCursor::End);
        cur.insertText(_("\n"));
        setTextCursor(cur);
        displayPrompt();
    }

}

void QmlJSScriptConsole::handleUpKey()
{
    //get the current script and update in script history
    QString currentScript = getCurrentScript();
    d->scriptHistory.replace(d->scriptHistoryIndex - 1,currentScript);

    if (d->scriptHistoryIndex > 1)
        d->scriptHistoryIndex--;

    replaceCurrentScript(d->scriptHistory.at(d->scriptHistoryIndex - 1));
}

void QmlJSScriptConsole::handleDownKey()
{
    //get the current script and update in script history
    QString currentScript = getCurrentScript();
    d->scriptHistory.replace(d->scriptHistoryIndex - 1,currentScript);

    if (d->scriptHistoryIndex < d->scriptHistory.count())
        d->scriptHistoryIndex++;

    replaceCurrentScript(d->scriptHistory.at(d->scriptHistoryIndex - 1));
}

void QmlJSScriptConsole::handleHomeKey()
{
    QTextCursor cursor = textCursor();
    cursor.setPosition(d->startOfEditableArea);
    setTextCursor(cursor);
}

QString QmlJSScriptConsole::getCurrentScript() const
{
    QTextCursor cursor = textCursor();
    cursor.setPosition(d->startOfEditableArea);
    while (cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor)) ;
    QString script = cursor.selectedText();
    cursor.clearSelection();
    //remove trailing white space
    int end = script.size() - 1;
    while (end > 0 && script[end].isSpace())
        end--;
    return script.left(end + 1);
}

void QmlJSScriptConsole::replaceCurrentScript(const QString &script)
{
    QTextCursor cursor = textCursor();
    cursor.setPosition(d->startOfEditableArea);
    while (cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor)) ;
    cursor.deleteChar();
    cursor.insertText(script);
    setTextCursor(cursor);
}

bool QmlJSScriptConsole::isEditableArea() const
{
    return textCursor().position() >= d->startOfEditableArea;
}

} //Internal
} //Debugger
