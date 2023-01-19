// Copyright (C) 2020 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#pragma once

#include "commandbuilder.h"

namespace IncrediBuild::Internal {

class CMakeCommandBuilder : public CommandBuilder
{
public:
    CMakeCommandBuilder(ProjectExplorer::BuildStep *buildStep) : CommandBuilder(buildStep) {}

private:
    QList<Utils::Id> migratableSteps() const final;
    QString id() const final { return "CMakeCommandBuilder"; }
    QString displayName() const final;
    Utils::FilePath defaultCommand() const final;
    QString defaultArguments() const final;
    QString setMultiProcessArg(QString args) final;
};

} // IncrediBuild::Internal

