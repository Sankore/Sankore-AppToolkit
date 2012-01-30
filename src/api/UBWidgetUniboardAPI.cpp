#include "UBWidgetUniboardAPI.h"
#include <QLocale>

UBWidgetUniboardAPI::UBWidgetUniboardAPI(QObject *parent):QObject(parent)
{

}

UBWidgetUniboardAPI::~UBWidgetUniboardAPI()
{

}

void UBWidgetUniboardAPI::setTool(const QString& toolString)
{
    QString msg = QString(tr("%0 called (toolString=%1)")).arg("setTool").arg(toolString);
    emit functionCalled(msg);
}

void UBWidgetUniboardAPI::setPenColor(const QString& penColor)
{
    QString msg = QString(tr("%0 called (penColor=%1)")).arg("setPenColor").arg(penColor);
    emit functionCalled(msg);
}

void UBWidgetUniboardAPI::setMarkerColor(const QString& penColor)
{
    QString msg = QString(tr("%0 called (penColor=%1)")).arg("setMarkerColor").arg(penColor);
    emit functionCalled(msg);
}

QString UBWidgetUniboardAPI::pageThumbnail(const int pageNumber)
{
    QString msg = QString(tr("%0 called (pageNumber=%1)")).arg("pageThumbnail").arg(pageNumber);
    emit functionCalled(msg);
    return "";
}

void UBWidgetUniboardAPI::zoom(const qreal factor, const qreal x, const qreal y)
{
    QString msg = QString(tr("%0 called (factor=%1, x=%2, y=%3)")).arg("zoom").arg(factor).arg(x).arg(y);
    emit functionCalled(msg);
}

void UBWidgetUniboardAPI::move(const qreal x, const qreal y)
{
    QString msg = QString(tr("%0 called (x=%1, y=%2)")).arg("move").arg(x).arg(y);
    emit functionCalled(msg);
}

void UBWidgetUniboardAPI::moveTo(const qreal x, const qreal y)
{
    QString msg = QString(tr("%0 called (x=%1, y=%2)")).arg("moveTo").arg(x).arg(y);
    emit functionCalled(msg);
}

void UBWidgetUniboardAPI::drawLineTo(const qreal x, const qreal y, const qreal pWidth)
{
    QString msg = QString(tr("%0 called (x=%1, y=%2, pWidth=%3)")).arg("drawLineTo").arg(x).arg(y).arg(pWidth);
    emit functionCalled(msg);
}

void UBWidgetUniboardAPI::eraseLineTo(const qreal x, const qreal y, const qreal pWidth)
{
    QString msg = QString(tr("%0 called (x=%1, y=%2, pWidth=%3)")).arg("eraseLineTo").arg(x).arg(y).arg(pWidth);
    emit functionCalled(msg);
}

void UBWidgetUniboardAPI::clear()
{
    QString msg = QString(tr("%0 called")).arg("clear");
    emit functionCalled(msg);
}

void UBWidgetUniboardAPI::setBackground(bool pIsDark, bool pIsCrossed)
{
    QString msg = QString(tr("%0 called (pIsDark=%1, pIsCrossed=%2)")).arg("setBackground").arg(pIsDark).arg(pIsCrossed);
    emit functionCalled(msg);
}

void UBWidgetUniboardAPI::addObject(QString pUrl, int width, int height, int x, int y, bool background)
{
    QString msg = QString(tr("%0 called (pUlr=%1, width=%2, height=%3, x=%4, y=%5, background=%6)")).arg("addObject").arg(pUrl).arg(width).arg(height).arg(x).arg(y).arg(background);
    emit functionCalled(msg);
}

void UBWidgetUniboardAPI::resize(qreal width, qreal height)
{
    QString msg = QString(tr("%0 called (width=%1, height=%2)")).arg("resize").arg(width).arg(height);
    emit functionCalled(msg);
}

QString UBWidgetUniboardAPI::locale()
{
    QString msg = QString(tr("%0 called")).arg("locale");
    emit functionCalled(msg);
    return QLocale().name();
}

void UBWidgetUniboardAPI::setPreference(const QString& key, QString value)
{
    QString msg = QString(tr("%0 called (key=%1, value=%2)")).arg("setPreference").arg(key).arg(value);
    emit functionCalled(msg);
}

QString UBWidgetUniboardAPI::preference(const QString& key, const QString& pDefault)
{
    QString msg = QString(tr("%0 called (key=%1, pDefault=%2)")).arg("preference").arg(key).arg(pDefault);
    emit functionCalled(msg);
    return "";
}

QStringList UBWidgetUniboardAPI::preferenceKeys()
{
    QStringList qsl;

    qsl << "test key 1";
    qsl << "test key 2";
    qsl << "test key 3";

    QString msg = QString(tr("%0 called")).arg("preferenceKeys");
    emit functionCalled(msg);

    return qsl;
}

void UBWidgetUniboardAPI::showMessage(const QString& message)
{
    QString msg = QString(tr("%0 called (message=%1)")).arg("showMessage").arg(message);
    emit functionCalled(msg);
}

void UBWidgetUniboardAPI::centerOn(const qreal x, const qreal y)
{
    QString msg = QString(tr("%0 called (x=%1, y=%2)")).arg("centerOn").arg(x).arg(y);
    emit functionCalled(msg);
}

void UBWidgetUniboardAPI::addText(const QString& text, const qreal x, const qreal y, const int height, const QString& font, bool bold, bool italic)
{
    QString msg = QString(tr("%0 called (text=%1, x=%2, y=%3, height=%4, font=%5, bold=%6, italic=%7)")).arg("addText").arg(text).arg(x).arg(y).arg(height).arg(font).arg(bold).arg(italic);
    emit functionCalled(msg);
}

void UBWidgetUniboardAPI::returnStatus(const QString& method, const QString& status)
{
    QString msg = QString(tr("%0 called (method=%1, status=%2)")).arg("returnStatus").arg(method).arg(status);
    emit functionCalled(msg);
}

void UBWidgetUniboardAPI::usedMethods(QStringList methods)
{
    QString meth = "";
    foreach(QString method, methods)
    {
        meth += QString("%0 ").arg(method);
    }

    QString msg = QString(tr("%0 called (methods=%1)")).arg("usedMethods").arg(meth);
    emit functionCalled(msg);
}

void UBWidgetUniboardAPI::response(bool correct)
{
    QString msg = QString(tr("%0 called (correct=%1)")).arg("response").arg(correct);
    emit functionCalled(msg);
}
