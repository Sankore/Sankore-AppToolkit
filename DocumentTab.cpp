#include <QFileInfo>
#include <QDebug>

#include "DocumentTab.h"
#include "globalDefs.h"
#include "TextDocument.h"
#include "WebDocument.h"

DocumentTab::DocumentTab(QWidget *parent, const char *name):QTabWidget(parent)
{
    setObjectName(name);
}

DocumentTab::~DocumentTab()
{
    if(!mWidgets.empty())
    {
        mWidgets.clear();
    }
}

void DocumentTab::addPage(const QString &path)
{
    QFileInfo fi(path);
    QString name = fi.fileName();

    if(!mWidgets.contains(name))
    {
        // Get the widget type and create the widget
        eDocType docType = getDocType(name);

        DocumentWidget* pWidget = NULL;
        switch(docType)
        {
            case eDocType_Text:
                pWidget = new TextDocument(path, "TextDocument", this);
            break;
            case eDocType_Any:
            default:
            pWidget = new WebDocument(path, "WebDocument", this);
            break;
        }

        // Then add it to the widget map mWidgets
        mWidgets[name] = pWidget;

        // Finally add a new tab
        addTab(pWidget, name);
    }

    // Show this widget
    setCurrentWidget(mWidgets[name]);
}

void DocumentTab::removePage(const QString &name)
{
    mWidgets.remove(name);
}

eDocType DocumentTab::getDocType(const QString &docName)
{
    eDocType retType = eDocType_Any;

    QFileInfo fi(docName);
    QString qsExtension = fi.suffix();
    if(     "css" == qsExtension ||
            "js" == qsExtension ||
            "xml" == qsExtension ||
            "html" == qsExtension ||
            "xhtml" == qsExtension ||
            "txt" == qsExtension ||
            "rtf" == qsExtension ||
            "csv" == qsExtension)
    {
        retType = eDocType_Text;
    }

    return retType;
}

void DocumentTab::onFileDoubleClicked(const QString &path)
{
    if("" != path)
    {
        addPage(path);
    }
}
