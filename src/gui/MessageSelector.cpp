#include "MessageSelector.h"
#include "core/globalDefs.h"

MessageSelector::MessageSelector(const QString &label, QWidget *parent, const char *name):QGroupBox(label, parent)
  , mpCombo(NULL)
  , mpButton(NULL)
  , mpLayout(NULL)
  , mpButtonLayout(NULL)
  , mpCBLayout(NULL)
{
    setObjectName(name);
    mpLayout = new QVBoxLayout();
    setLayout(mpLayout);

    mpCBLayout = new QHBoxLayout();
    mpLayout->addLayout(mpCBLayout);

    //mpCBLayout->addStretch(1);
    mpCombo = new QComboBox(this);
    mpCBLayout->addWidget(mpCombo);
    mpCBLayout->addStretch(1);

    mpButtonLayout = new QHBoxLayout();

    mpLayout->addLayout(mpButtonLayout);

    //mpButtonLayout->addStretch(1);
    mpButton = new QPushButton(tr("Send"), this);
    mpButtonLayout->addWidget(mpButton, 0);
    mpButtonLayout->addStretch(1);

    mpButton->setEnabled(false);
    mpCombo->setEnabled(false);
    connect(mpButton, SIGNAL(clicked()), this, SLOT(onSendMessage()));
}

MessageSelector::~MessageSelector()
{
    DELETEPTR(mpCBLayout);
    DELETEPTR(mpButtonLayout);
    DELETEPTR(mpCombo);
    DELETEPTR(mpButton);
    DELETEPTR(mpLayout);
}

void MessageSelector::populateMessages(QStringList msgList)
{
    foreach(QString eachMsg, msgList)
    {
        mpCombo->addItem(eachMsg);
    }
}

void MessageSelector::onSendMessage()
{
    emit sendMessage(mpCombo->currentIndex(), mpCombo->currentText());
}

void MessageSelector::onLoaded()
{
    mpButton->setEnabled(true);
    mpCombo->setEnabled(true);
}
