/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.  For licensing terms and
** conditions see http://www.qt.io/terms-conditions.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef CPPTOOLS_INTERNAL_MODELMANAGERTESTHELPER_H
#define CPPTOOLS_INTERNAL_MODELMANAGERTESTHELPER_H

#include "cpptools_global.h"
#include "cppmodelmanager.h"

#include <projectexplorer/project.h>

#include <QObject>

namespace CppTools {
namespace Tests {

class CPPTOOLS_EXPORT TestProject: public ProjectExplorer::Project
{
    Q_OBJECT

public:
    TestProject(const QString &name, QObject *parent);
    ~TestProject() override;

    QString displayName() const override { return m_name; }
    Core::IDocument *document() const override { return 0; }
    ProjectExplorer::IProjectManager *projectManager() const override { return 0; }
    ProjectExplorer::ProjectNode *rootProjectNode() const override { return 0; }

    QStringList files(FilesMode fileMode) const override { Q_UNUSED(fileMode); return QStringList(); }

private:
    QString m_name;
};

class CPPTOOLS_EXPORT ModelManagerTestHelper: public QObject
{
    Q_OBJECT

public:
    typedef ProjectExplorer::Project Project;

    explicit ModelManagerTestHelper(QObject *parent = 0,
                                    bool testOnlyForCleanedProjects = true);
    ~ModelManagerTestHelper() override;

    void cleanup();

    Project *createProject(const QString &name);

    QSet<QString> updateProjectInfo(const ProjectInfo &projectInfo);

    void resetRefreshedSourceFiles();
    QSet<QString> waitForRefreshedSourceFiles();
    void waitForFinishedGc();

signals:
    void aboutToRemoveProject(ProjectExplorer::Project *project);
    void projectAdded(ProjectExplorer::Project*);

public slots:
    void sourceFilesRefreshed(const QSet<QString> &files);
    void gcFinished();

private:
    bool m_gcFinished;
    bool m_refreshHappened;
    bool m_testOnlyForCleanedProjects;
    QSet<QString> m_lastRefreshedSourceFiles;
};

} // namespace Tests
} // namespace CppTools

#endif // CPPTOOLS_INTERNAL_MODELMANAGERTESTHELPER_H
