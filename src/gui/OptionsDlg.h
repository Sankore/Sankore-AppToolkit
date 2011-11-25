#ifndef OPTIONSDLG_H
#define OPTIONSDLG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QTreeWidget>

#define DLG_WIDTH   640
#define DLG_HEIGHT  480

class OptionsDlg : public QDialog
{
    Q_OBJECT
public:
    OptionsDlg(QWidget* parent=0, const char* name="OptionsDlg");
    ~OptionsDlg();

private slots:
    void onOk();
    void onCancel();

private:
    QVBoxLayout* mpLayout;
    QHBoxLayout* mpHLayout;
    QTreeWidget* mpTree;
    QStackedWidget* mpStack;
    QDialogButtonBox* mpButtons;
};

#endif // OPTIONSDLG_H
