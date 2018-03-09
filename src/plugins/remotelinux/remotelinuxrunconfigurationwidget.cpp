/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
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

#include "remotelinuxrunconfigurationwidget.h"

#include "remotelinuxrunconfiguration.h"

#include <projectexplorer/runconfigurationaspects.h>

#include <utils/detailswidget.h>
#include <utils/utilsicons.h>

#include <QCheckBox>
#include <QDir>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

using namespace ProjectExplorer;

namespace RemoteLinux {
namespace Internal {

class RemoteLinuxRunConfigurationWidgetPrivate
{
public:
    RemoteLinuxRunConfigurationWidgetPrivate(RemoteLinuxRunConfiguration *runConfig)
        : runConfiguration(runConfig)
    {
        const auto selectable = Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse;
        localExecutableLabel.setTextInteractionFlags(selectable);
        remoteExecutableLabel.setTextInteractionFlags(selectable);
    }

    RemoteLinuxRunConfiguration * const runConfiguration;
    bool ignoreChange = false;

    QLabel localExecutableLabel;
    QLabel remoteExecutableLabel;
    QCheckBox useAlternateCommandBox;
    QLineEdit alternateCommand;
    QLabel devConfLabel;
    QFormLayout genericWidgetsLayout;
};

} // namespace Internal

using namespace Internal;

RemoteLinuxRunConfigurationWidget::RemoteLinuxRunConfigurationWidget(RemoteLinuxRunConfiguration *runConfiguration)
    : d(new RemoteLinuxRunConfigurationWidgetPrivate(runConfiguration))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);

    auto detailsContainer = new Utils::DetailsWidget(this);
    detailsContainer->setState(Utils::DetailsWidget::NoSummary);

    auto details = new QWidget(this);
    details->setLayout(&d->genericWidgetsLayout);
    detailsContainer->setWidget(details);

    mainLayout->addWidget(detailsContainer);

    d->genericWidgetsLayout.setFormAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    d->localExecutableLabel.setText(d->runConfiguration->localExecutableFilePath());
    d->genericWidgetsLayout.addRow(tr("Executable on host:"), &d->localExecutableLabel);
    d->genericWidgetsLayout.addRow(tr("Executable on device:"), &d->remoteExecutableLabel);
    auto altRemoteExeWidget = new QWidget;
    auto altRemoteExeLayout = new QHBoxLayout(altRemoteExeWidget);
    altRemoteExeLayout->setContentsMargins(0, 0, 0, 0);
    d->alternateCommand.setText(d->runConfiguration->alternateRemoteExecutable());
    altRemoteExeLayout->addWidget(&d->alternateCommand);
    d->useAlternateCommandBox.setText(tr("Use this command instead"));
    d->useAlternateCommandBox.setChecked(d->runConfiguration->useAlternateExecutable());
    altRemoteExeLayout->addWidget(&d->useAlternateCommandBox);
    d->genericWidgetsLayout.addRow(tr("Alternate executable on device:"), altRemoteExeWidget);

    d->runConfiguration->extraAspect<ArgumentsAspect>()
        ->addToMainConfigurationWidget(this, &d->genericWidgetsLayout);
    d->runConfiguration->extraAspect<WorkingDirectoryAspect>()
        ->addToMainConfigurationWidget(this, &d->genericWidgetsLayout);

    connect(d->runConfiguration, &RemoteLinuxRunConfiguration::targetInformationChanged,
            this, &RemoteLinuxRunConfigurationWidget::updateTargetInformation);
    connect(d->runConfiguration, &RemoteLinuxRunConfiguration::deploySpecsChanged,
            this, &RemoteLinuxRunConfigurationWidget::handleDeploySpecsChanged);
    connect(&d->useAlternateCommandBox, &QCheckBox::toggled,
            this, &RemoteLinuxRunConfigurationWidget::handleUseAlternateCommandChanged);
    connect(&d->alternateCommand, &QLineEdit::textEdited,
            this, &RemoteLinuxRunConfigurationWidget::handleAlternateCommandChanged);

    handleDeploySpecsChanged();
    handleUseAlternateCommandChanged();
}

RemoteLinuxRunConfigurationWidget::~RemoteLinuxRunConfigurationWidget()
{
    delete d;
}

void RemoteLinuxRunConfigurationWidget::addFormLayoutRow(QWidget *label, QWidget *field)
{
    d->genericWidgetsLayout.addRow(label, field);
}

void RemoteLinuxRunConfigurationWidget::updateTargetInformation()
{
    setLabelText(d->localExecutableLabel,
            QDir::toNativeSeparators(d->runConfiguration->localExecutableFilePath()),
            tr("Unknown"));
}

void RemoteLinuxRunConfigurationWidget::handleDeploySpecsChanged()
{
    setLabelText(d->remoteExecutableLabel, d->runConfiguration->defaultRemoteExecutableFilePath(),
            tr("Remote path not set"));
}

void RemoteLinuxRunConfigurationWidget::setLabelText(QLabel &label, const QString &regularText,
        const QString &errorText)
{
    const QString errorMessage = QLatin1String("<font color=\"red\">") + errorText
            + QLatin1String("</font>");
    label.setText(regularText.isEmpty() ? errorMessage : regularText);
}

void RemoteLinuxRunConfigurationWidget::handleUseAlternateCommandChanged()
{
    const bool useAltExe = d->useAlternateCommandBox.isChecked();
    d->remoteExecutableLabel.setEnabled(!useAltExe);
    d->alternateCommand.setEnabled(useAltExe);
    d->runConfiguration->setUseAlternateExecutable(useAltExe);
}

void RemoteLinuxRunConfigurationWidget::handleAlternateCommandChanged()
{
    d->runConfiguration->setAlternateRemoteExecutable(d->alternateCommand.text().trimmed());
}

} // namespace RemoteLinux
