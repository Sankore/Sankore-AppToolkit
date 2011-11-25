#ifndef OPTIONWIDGET_H
#define OPTIONWIDGET_H

#include <QWidget>

class OptionWidget : public QWidget
{
    Q_OBJECT
public:
    OptionWidget(QWidget* parent=0, const char* name="OptionWidget");
    ~OptionWidget();
    QString name(){return mName;}

protected:
    QString mName;
};

#endif // OPTIONWIDGET_H
