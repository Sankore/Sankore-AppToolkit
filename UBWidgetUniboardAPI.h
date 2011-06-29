#ifndef UBWIDGETUNIBOARDAPI_H
#define UBWIDGETUNIBOARDAPI_H

#include <QObject>
#include <QString>
#include <QStringList>

class UBWidgetUniboardAPI : public QObject
{
    Q_OBJECT
public:
    UBWidgetUniboardAPI(QObject* parent=0);
    ~UBWidgetUniboardAPI();

signals:
    void functionCalled(const QString& infos);

public slots:
    void setTool(const QString& toolString);
    void setPenColor(const QString& penColor);
    void setMarkerColor(const QString& penColor);
    QString pageThumbnail(const int pageNumber);
    void zoom(const qreal factor, const qreal x, const qreal y);
    void move(const qreal x, const qreal y);
    void moveTo(const qreal x, const qreal y);
    void drawLineTo(const qreal x, const qreal y, const qreal pWidth);
    void eraseLineTo(const qreal x, const qreal y, const qreal pWidth);
    void clear();
    void setBackground(bool pIsDark, bool pIsCrossed);
    void addObject(QString pUrl, int width = 0, int height = 0, int x = 0, int y = 0, bool background = false);
    void resize(qreal width, qreal height);
    void setPreference(const QString& key, QString value);
    QString preference(const QString& key, const QString& pDefault = QString());
    QStringList preferenceKeys();
    void showMessage(const QString& message);
    void centerOn(const qreal x, const qreal y);
    void addText(const QString& text, const qreal x, const qreal y, const int height = -1, const QString& font = "", bool bold = false, bool italic = false);

    // Documented API methods
    void returnStatus(const QString& method, const QString& status);
    void usedMethods(QStringList methods);
    void response(bool correct);
    QString locale();
};

#endif // UBWIDGETUNIBOARDAPI_H
