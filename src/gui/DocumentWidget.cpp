#include "DocumentWidget.h"
#include "core/globalDefs.h"

DocumentWidget::DocumentWidget(const QString& docName, const char *name, QWidget *parent):QWidget(parent)
{
    setObjectName(name);
    mName = docName;

    mpLayout = new QVBoxLayout();
    setLayout(mpLayout);
}

DocumentWidget::~DocumentWidget()
{
    DELETEPTR(mpWidget);
    DELETEPTR(mpLayout);
}

QWidget* DocumentWidget::widget()
{
    return mpWidget;
}

QString DocumentWidget::name()
{
    return mName;
}

void DocumentWidget::save()
{
    // Will be overloaded by the children
}
