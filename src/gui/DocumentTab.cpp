#include <QFileInfo>
#include <QDebug>

#include "DocumentTab.h"
#include "core/globalDefs.h"
#include "TextDocument.h"
#include "WebDocument.h"

DocumentTab::DocumentTab(QWidget *parent, const char *name):QTabWidget(parent)
{
    setObjectName(name);
    setTabsClosable(true);

    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequest(int)));
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

        connect(pWidget, SIGNAL(modified()), this, SLOT(onDocModified()));
    }

    // Show this widget
    setCurrentWidget(mWidgets[name]);
}

void DocumentTab::removePage(const QString &name)
{
    // Disconnect the slot
    disconnect(mWidgets[name], SIGNAL(modified()), this, SLOT(onDocModified()));

    // remove the widget
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
            "csv" == qsExtension ||
            "h" == qsExtension ||
            "c" == qsExtension ||
            "cpp" == qsExtension)
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

void DocumentTab::onTabCloseRequest(int index)
{
    // Get the tab name and call RemovePage
    QString qsTab = tabText(index);
    removePage(qsTab);
    removeTab(index);
}

void DocumentTab::clear()
{
    mWidgets.clear();
    QTabWidget::clear();
}

void DocumentTab::saveCurrentDoc()
{
    if(count())
    {
        DocumentWidget* pCrntWidget = dynamic_cast<DocumentWidget*>(currentWidget());
        if(NULL != pCrntWidget)
        {
            pCrntWidget->save();

            QFileInfo fi(pCrntWidget->name());
            setTabText(currentIndex(), fi.fileName());
        }
    }
}

void DocumentTab::onDocModified()
{
    DocumentWidget* pCrntWidget = dynamic_cast<DocumentWidget*>(currentWidget());
    if(NULL != pCrntWidget)
    {
        QFileInfo fi(pCrntWidget->name());
        QString qsNewTitle = QString("%0*").arg(fi.fileName());
        setTabText(currentIndex(), qsNewTitle);
    }
}

void DocumentTab::onFileRenamed(const QString &oldPath, const QString &newPath)
{
    QFileInfo fiOld(oldPath);
    QFileInfo fiNew(newPath);

    QString oldName = fiOld.fileName();

    if(mWidgets.contains(oldName))
    {
        removeTab(indexOf(mWidgets[oldName]));
        removePage(oldName);
        addPage(newPath);
    }
}

void DocumentTab::onFileDeleted(const QString &path)
{
    QFileInfo fi(path);
    QString qsName = fi.fileName();
    if(mWidgets.contains(qsName))
    {
        removeTab(indexOf(mWidgets[qsName]));
        removePage(qsName);
    }
}
