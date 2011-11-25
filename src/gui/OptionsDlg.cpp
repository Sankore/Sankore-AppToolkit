#include <QHeaderView>

#include "OptionsDlg.h"
#include "core/globalDefs.h"

OptionsDlg::OptionsDlg(QWidget *parent, const char *name):QDialog(parent)
  , mpLayout(NULL)
  , mpHLayout(NULL)
  , mpTree(NULL)
  , mpStack(NULL)
  , mpButtons(NULL)
{
    setObjectName(name);
    setModal(true);
    resize(DLG_WIDTH, DLG_HEIGHT);

    mpLayout = new QVBoxLayout();
    setLayout(mpLayout);

    mpHLayout = new QHBoxLayout();
    mpLayout->addLayout(mpHLayout, 1);

    mpTree = new QTreeWidget(this);
    mpTree->header()->hide();
    mpHLayout->addWidget(mpTree, 0);

    mpStack = new QStackedWidget(this);
    mpHLayout->addWidget(mpStack, 1);

    mpButtons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    mpLayout->addWidget(mpButtons);

    connect(mpButtons, SIGNAL(accepted()), this, SLOT(onOk()));
    connect(mpButtons, SIGNAL(rejected()), this, SLOT(onCancel()));
}

OptionsDlg::~OptionsDlg()
{
    DELETEPTR(mpButtons);
    DELETEPTR(mpStack);
    DELETEPTR(mpTree);
    DELETEPTR(mpHLayout);
    DELETEPTR(mpLayout);
}

void OptionsDlg::onOk()
{
    accept();
}

void OptionsDlg::onCancel()
{
    reject();
}
