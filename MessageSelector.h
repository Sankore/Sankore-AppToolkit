#ifndef MESSAGESELECTOR_H
#define MESSAGESELECTOR_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringList>

class MessageSelector : public QGroupBox
{
    Q_OBJECT
public:
    MessageSelector(const QString & label, QWidget* parent=0, const char* name="MessageSelector");
    ~MessageSelector();
    void populateMessages(QStringList msgList);

signals:
    void sendMessage(int msgID, const QString& label);

public slots:
    void onLoaded();

private slots:
    void onSendMessage();

private:
    QComboBox* mpCombo;
    QPushButton* mpButton;
    QVBoxLayout* mpLayout;
    QHBoxLayout* mpButtonLayout;
    QHBoxLayout* mpCBLayout;
};

#endif // MESSAGESELECTOR_H
