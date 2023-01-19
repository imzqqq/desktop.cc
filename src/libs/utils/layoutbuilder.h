// Copyright (C) 2020 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#pragma once

#include "utils_global.h"

#include <QList>
#include <QString>
#include <QVariant>

#include <optional>

QT_BEGIN_NAMESPACE
class QLayout;
class QSplitter;
class QTabWidget;
class QWidget;
QT_END_NAMESPACE

namespace Utils {
class BaseAspect;
class BoolAspect;
} // Utils

namespace Utils::Layouting {

enum AttachType {
    WithMargins,
    WithoutMargins,
    WithFormAlignment, // Handle Grid similar to QFormLayout, i.e. use special alignment for the first column on Mac
};

class LayoutBuilder;

// LayoutItem

class QTCREATOR_UTILS_EXPORT LayoutItem
{
public:
    enum class AlignmentType {
        DefaultAlignment,
        AlignAsFormLabel,
    };

    enum class SpecialType {
        NotSpecial,
        Space,
        Stretch,
        Break,
        HorizontalRule,
    };

    using Setter = std::function<void(QObject *target)>;
    LayoutItem();
    LayoutItem(QLayout *layout);
    LayoutItem(QWidget *widget);
    LayoutItem(BaseAspect *aspect); // Remove
    LayoutItem(BaseAspect &aspect);
    LayoutItem(const QString &text);
    LayoutItem(const LayoutBuilder &builder);
    LayoutItem(const Setter &setter) { this->setter = setter; }

    QLayout *layout = nullptr;
    QWidget *widget = nullptr;
    BaseAspect *aspect = nullptr;

    QString text; // FIXME: Use specialValue for that
    int span = 1;
    AlignmentType align = AlignmentType::DefaultAlignment;
    Setter setter;
    SpecialType specialType = SpecialType::NotSpecial;
    QVariant specialValue;
};

class QTCREATOR_UTILS_EXPORT Space : public LayoutItem
{
public:
    explicit Space(int space);
};

class QTCREATOR_UTILS_EXPORT Span : public LayoutItem
{
public:
    Span(int span, const LayoutItem &item);
};

class QTCREATOR_UTILS_EXPORT Stretch : public LayoutItem
{
public:
    explicit Stretch(int stretch = 1);
};

class QTCREATOR_UTILS_EXPORT Tab : public LayoutItem
{
public:
    Tab(const QString &tabName, const LayoutBuilder &item);
};

class QTCREATOR_UTILS_EXPORT Break : public LayoutItem
{
public:
    Break();
};

class QTCREATOR_UTILS_EXPORT HorizontalRule : public LayoutItem
{
public:
    HorizontalRule();
};

class QTCREATOR_UTILS_EXPORT Group : public LayoutItem
{
public:
    Group(std::initializer_list<LayoutItem> items);
};

class QTCREATOR_UTILS_EXPORT PushButton : public LayoutItem
{
public:
    PushButton(std::initializer_list<LayoutItem> items);
};

class QTCREATOR_UTILS_EXPORT Splitter : public LayoutItem
{
public:
    Splitter(std::initializer_list<LayoutItem> items);
    Splitter(QSplitter *splitter, std::initializer_list<LayoutItem> items);
};

class QTCREATOR_UTILS_EXPORT TabWidget : public LayoutItem
{
public:
    TabWidget(std::initializer_list<Tab> tabs);
    TabWidget(QTabWidget *tabWidget, std::initializer_list<Tab> tabs);
};

// Singleton items.

QTCREATOR_UTILS_EXPORT extern Break br;
QTCREATOR_UTILS_EXPORT extern Stretch st;
QTCREATOR_UTILS_EXPORT extern Space empty;
QTCREATOR_UTILS_EXPORT extern HorizontalRule hr;

// "Properties"

QTCREATOR_UTILS_EXPORT LayoutItem::Setter title(const QString &title,
                                                BoolAspect *checker = nullptr);

QTCREATOR_UTILS_EXPORT LayoutItem::Setter text(const QString &text);
QTCREATOR_UTILS_EXPORT LayoutItem::Setter tooltip(const QString &toolTip);
QTCREATOR_UTILS_EXPORT LayoutItem::Setter onClicked(const std::function<void()> &func,
                                                    QObject *guard = nullptr);


// Convenience

QTCREATOR_UTILS_EXPORT QWidget *createHr(QWidget *parent = nullptr);


// LayoutBuilder

class QTCREATOR_UTILS_EXPORT LayoutBuilder
{
public:
    enum LayoutType {
        HBoxLayout,
        VBoxLayout,
        FormLayout,
        GridLayout,
        StackLayout,
    };

    using LayoutItems = QList<LayoutItem>;

    explicit LayoutBuilder(LayoutType layoutType, const LayoutItems &items = {});

    LayoutBuilder(const LayoutBuilder &) = delete;
    LayoutBuilder(LayoutBuilder &&) = default;
    LayoutBuilder &operator=(const LayoutBuilder &) = delete;
    LayoutBuilder &operator=(LayoutBuilder &&) = default;

    ~LayoutBuilder();

    LayoutBuilder &setSpacing(int spacing);

    LayoutBuilder &addItem(const LayoutItem &item);
    LayoutBuilder &addItems(const LayoutItems &items);

    LayoutBuilder &finishRow();
    LayoutBuilder &addRow(const LayoutItem &item);
    LayoutBuilder &addRow(const LayoutItems &items);

    LayoutType layoutType() const { return m_layoutType; }

    void attachTo(QWidget *w, Layouting::AttachType attachType = Layouting::WithMargins) const;
    QWidget *emerge(Layouting::AttachType attachType = Layouting::WithMargins);

protected:
    friend class LayoutItem;

    explicit LayoutBuilder(); // Adds to existing layout.

    QLayout *createLayout() const;
    void doLayout(QWidget *parent, Layouting::AttachType attachType) const;

    LayoutItems m_items;
    LayoutType m_layoutType;
    std::optional<int> m_spacing;
};

class QTCREATOR_UTILS_EXPORT LayoutExtender : public LayoutBuilder
{
public:
    explicit LayoutExtender(QLayout *layout, Layouting::AttachType attachType);
    ~LayoutExtender();

private:
    QLayout *m_layout = nullptr;
    Layouting::AttachType m_attachType = {};
};

class QTCREATOR_UTILS_EXPORT Column : public LayoutBuilder
{
public:
    Column() : LayoutBuilder(VBoxLayout) {}
    Column(std::initializer_list<LayoutItem> items) : LayoutBuilder(VBoxLayout, items) {}
};

class QTCREATOR_UTILS_EXPORT Row : public LayoutBuilder
{
public:
    Row() : LayoutBuilder(HBoxLayout) {}
    Row(std::initializer_list<LayoutItem> items) : LayoutBuilder(HBoxLayout, items) {}
};

class QTCREATOR_UTILS_EXPORT Grid : public LayoutBuilder
{
public:
    Grid() : LayoutBuilder(GridLayout) {}
    Grid(std::initializer_list<LayoutItem> items) : LayoutBuilder(GridLayout, items) {}
};

class QTCREATOR_UTILS_EXPORT Form : public LayoutBuilder
{
public:
    Form() : LayoutBuilder(FormLayout) {}
    Form(std::initializer_list<LayoutItem> items) : LayoutBuilder(FormLayout, items) {}
};

class QTCREATOR_UTILS_EXPORT Stack : public LayoutBuilder
{
public:
    Stack() : LayoutBuilder(StackLayout) {}
    Stack(std::initializer_list<LayoutItem> items) : LayoutBuilder(StackLayout, items) {}
};

} // Utils::Layouting
