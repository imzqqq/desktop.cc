#pragma once

#include "fileutils.h"
#include "theme/theme.h"
#include "utils_global.h"

#include <QIcon>
#include <QPair>
#include <QVector>

QT_FORWARD_DECLARE_CLASS(QColor)
QT_FORWARD_DECLARE_CLASS(QPixmap)
QT_FORWARD_DECLARE_CLASS(QString)

namespace Utils {

using IconMaskAndColor = QPair<FilePath, Theme::Color>;
using IconStringMaskAndColor = QPair<QString, Theme::Color>;

// Returns a recolored icon with shadow and custom disabled state for a
// series of grayscalemask|Theme::Color mask pairs
class QTCREATOR_UTILS_EXPORT Icon : public QVector<IconMaskAndColor>
{
public:
    enum IconStyleOption {
        None = 0,
        Tint = 1,
        DropShadow = 2,
        PunchEdges = 4,

        ToolBarStyle = Tint | DropShadow | PunchEdges,
        MenuTintedStyle = Tint | PunchEdges
    };

    Q_DECLARE_FLAGS(IconStyleOptions, IconStyleOption)

    Icon();
    Icon(std::initializer_list<IconMaskAndColor> args, IconStyleOptions style = ToolBarStyle);
    Icon(std::initializer_list<IconStringMaskAndColor> args, IconStyleOptions style = ToolBarStyle);
    Icon(const FilePath &imageFileName);
    Icon(const QString &imageFileName);

    Icon(const Icon &other) = default;

    QIcon icon() const;
    // Same as icon() but without disabled state.
    QPixmap pixmap(QIcon::Mode iconMode = QIcon::Normal) const;

    // Try to avoid it. it is just there for special API cases in Qt Creator
    // where icons are still defined as filename.
    FilePath imageFilePath() const;

    // Returns either the classic or a themed icon depending on
    // the current Theme::FlatModeIcons flag.
    static QIcon sideBarIcon(const Icon &classic, const Icon &flat);
    // Like sideBarIcon plus added action mode for the flat icon
    static QIcon modeIcon(const Icon &classic, const Icon &flat, const Icon &flatActive);

    // Combined icon pixmaps in Normal and Disabled states from several Icons
    static QIcon combinedIcon(const QList<QIcon> &icons);
    static QIcon combinedIcon(const QList<Icon> &icons);

private:
    IconStyleOptions m_style = None;
};

} // namespace Utils

Q_DECLARE_OPERATORS_FOR_FLAGS(Utils::Icon::IconStyleOptions)
