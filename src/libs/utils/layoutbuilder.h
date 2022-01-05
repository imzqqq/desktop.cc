#pragma once

#include "utils_global.h"

#include <QList>
#include <QString>
#include <QVariant>

QT_BEGIN_NAMESPACE
class QLayout;
class QWidget;
QT_END_NAMESPACE

namespace Utils {

class BaseAspect;
class BoolAspect;

class QTCREATOR_UTILS_EXPORT LayoutBuilder
{
public:
    enum LayoutType {
        HBoxLayout,
        VBoxLayout,
        FormLayout,
        GridLayout,
    };

    enum class AlignmentType {
        DefaultAlignment,
        AlignAsFormLabel,
    };

    enum class SpecialType {
        NotSpecial,
        Space,
        Stretch,
        Break,
        Title,
    };

    class QTCREATOR_UTILS_EXPORT LayoutItem
    {
    public:
        LayoutItem();
        LayoutItem(QLayout *layout);
        LayoutItem(QWidget *widget);
        LayoutItem(BaseAspect *aspect); // Remove
        LayoutItem(BaseAspect &aspect);
        LayoutItem(const QString &text);
        LayoutItem(const LayoutBuilder &builder);

        QLayout *layout = nullptr;
        QWidget *widget = nullptr;
        BaseAspect *aspect = nullptr;

        QString text; // FIXME: Use specialValue for that
        int span = 1;
        AlignmentType align = AlignmentType::DefaultAlignment;
        SpecialType specialType = SpecialType::NotSpecial;
        QVariant specialValue;
    };

    using LayoutItems = QList<LayoutItem>;

    explicit LayoutBuilder(LayoutType layoutType, const LayoutItems &items = {});

    LayoutBuilder(const LayoutBuilder &) = delete;
    LayoutBuilder(LayoutBuilder &&) = default;
    LayoutBuilder &operator=(const LayoutBuilder &) = delete;
    LayoutBuilder &operator=(LayoutBuilder &&) = default;

    ~LayoutBuilder();

    LayoutBuilder &addItem(const LayoutItem &item);
    LayoutBuilder &addItems(const LayoutItems &items);

    LayoutBuilder &finishRow();
    LayoutBuilder &addRow(const LayoutItem &item);
    LayoutBuilder &addRow(const LayoutItems &items);

    LayoutType layoutType() const { return m_layoutType; }

    void attachTo(QWidget *w, bool withMargins = true);
    QWidget *emerge(bool withMargins = true);

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

    class QTCREATOR_UTILS_EXPORT AlignAsFormLabel : public LayoutItem
    {
    public:
        AlignAsFormLabel(const LayoutItem &item);
    };

    class QTCREATOR_UTILS_EXPORT Stretch : public LayoutItem
    {
    public:
        explicit Stretch(int stretch = 1);
    };

    class QTCREATOR_UTILS_EXPORT Break : public LayoutItem
    {
    public:
        Break();
    };

    class QTCREATOR_UTILS_EXPORT Title : public LayoutItem
    {
    public:
        explicit Title(const QString &title, BoolAspect *check = nullptr);
    };

protected:
    explicit LayoutBuilder(); // Adds to existing layout.

    void doLayout(QWidget *parent);

    LayoutItems m_items;
    LayoutType m_layoutType;
    bool m_withMargins = false;
};

class QTCREATOR_UTILS_EXPORT LayoutExtender : public LayoutBuilder
{
public:
    explicit LayoutExtender(QLayout *layout);
    ~LayoutExtender();

private:
    QLayout *m_layout = nullptr;
};

namespace Layouting {

class QTCREATOR_UTILS_EXPORT Group : public LayoutBuilder::LayoutItem
{
public:
    Group(std::initializer_list<LayoutBuilder::LayoutItem> items);
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

using Stretch = LayoutBuilder::Stretch;
using Space = LayoutBuilder::Space;
using Span = LayoutBuilder::Span;
using AlignAsFormLabel = LayoutBuilder::AlignAsFormLabel;
using Break = LayoutBuilder::Break;
using Title = LayoutBuilder::Title;

}
} // namespace Utils
