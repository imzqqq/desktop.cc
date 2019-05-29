/****************************************************************************
**
** Copyright (c) 2018 Artur Shepilko
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#pragma once

#include <coreplugin/iversioncontrol.h>

QT_BEGIN_NAMESPACE
class QVariant;
QT_END_NAMESPACE

namespace Fossil {
namespace Internal {

class FossilClient;

//Implements just the basics of the Version Control Interface
//FossilClient handles all the work
class FossilControl: public Core::IVersionControl
{
    Q_OBJECT

public:
    explicit FossilControl(FossilClient *fossilClient);

    QString displayName() const final;
    Core::Id id() const final;

    bool isVcsFileOrDirectory(const Utils::FilePath &filePath) const final;
    bool managesDirectory(const QString &filename, QString *topLevel = 0) const final;
    bool managesFile(const QString &workingDirectory, const QString &fileName) const final;
    bool isConfigured() const final;
    bool supportsOperation(Operation operation) const final;
    bool vcsOpen(const QString &fileName) final;
    bool vcsAdd(const QString &filename) final;
    bool vcsDelete(const QString &filename) final;
    bool vcsMove(const QString &from, const QString &to) final;
    bool vcsCreateRepository(const QString &directory) final;
    bool vcsAnnotate(const QString &file, int line) final;
    Core::ShellCommand *createInitialCheckoutCommand(const QString &sourceUrl,
                                                     const Utils::FilePath &baseDirectory,
                                                     const QString &localName,
                                                     const QStringList &extraArgs) final;

    // To be connected to the VcsTask's success signal to emit the repository/
    // files changed signals according to the variant's type:
    // String -> repository, StringList -> files
    void changed(const QVariant &);

private:
    FossilClient *const m_client;
};

} // namespace Internal
} // namespace Fossil
