#include "DocumentTab.h"
#include "globalDefs.h"

DocumentTab::DocumentTab(QWidget *parent, const char *name):QTabWidget(parent)
{
    setObjectName(name);
}

DocumentTab::~DocumentTab()
{

}

void DocumentTab::addPage(const QString &name, QTextEdit *textEditor)
{
    if(NULL != textEditor)
    {
        mpPages[name] = textEditor;
        addTab(textEditor, name);
    }
}

void DocumentTab::removePage(const QString &name)
{
    // TODO : Implement this
    Q_UNUSED(name);
}
