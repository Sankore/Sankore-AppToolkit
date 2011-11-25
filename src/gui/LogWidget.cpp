#include <QDebug>
#include <QTreeWidgetItem>
#include <QTime>
#include <QStringList>
#include <QBrush>
#include <QHeaderView>

#include "LogWidget.h"
#include "core/globalDefs.h"

LogWidget::LogWidget(QWidget *parent, const char *name):QTreeWidget(parent)
{
    setObjectName(name);
    setAlternatingRowColors(true);
    setColumnCount(3);
    setColumnWidth(0, 150);
    setColumnWidth(1, 400);
    QStringList qslHeader;
    qslHeader << tr("Time");
    qslHeader << tr("Message");
    qslHeader << tr("Direction");
    setHeaderLabels(qslHeader);
}

LogWidget::~LogWidget()
{

}

void LogWidget::LogMessage(const QString &msg, eMsgDirection direction)
{
    QTreeWidgetItem* pRoot = invisibleRootItem();
    QStringList qslLogElems;
    qslLogElems << QTime::currentTime().toString("hh:mm:ss.zzz");
    qslLogElems << msg;

    switch(direction)
    {
        case eMsgDirection_QtToJS:
            qslLogElems << tr("Sankore to Widget");
            break;

        case eMsgDirection_JSToQt:
            qslLogElems << tr("Widget to Sankore");
            break;
    default:
        break;
    }

    QTreeWidgetItem* pCrntItem = new QTreeWidgetItem(qslLogElems);
    pRoot->addChild(pCrntItem);
    scrollToItem(pCrntItem);
}
