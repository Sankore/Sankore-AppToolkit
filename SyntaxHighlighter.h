#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>

struct HighlightingRule
 {
     QRegExp pattern;
     QTextCharFormat format;
 };

class SyntaxHighlighter : public QSyntaxHighlighter
{
public:
    SyntaxHighlighter(QObject* parent=0);
    ~SyntaxHighlighter();

protected:
    virtual void highlightBlock(const QString &text);

    QVector<HighlightingRule> highlightingRules;
};

// -------------------------------------------------------------------------------
// Javascript one
class JSSyntaxHighlighter : public SyntaxHighlighter
{
public:
    JSSyntaxHighlighter(QObject* parent=0);
    ~JSSyntaxHighlighter();

protected:
    void highlightBlock(const QString &text);

private:
    QRegExp commentStartExpression;
    QRegExp commentEndExpression;
    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat attributeFormat;
};

// -------------------------------------------------------------------------------
// XML - HTML one
enum ParsingState
{
    NoState = 0,
    ExpectElementNameOrSlash,
    ExpectElementName,
    ExpectAttributeOrEndOfElement,
    ExpectEqual,
    ExpectAttributeValue
};

enum BlockState
{
    NoBlock = -1,
    InComment,
    InElement
};

enum HighlightType
{
    SyntaxChar,
    ElementName,
    Comment,
    AttributeName,
    AttributeValue,
    Error,
    Other
};

class XMLSyntaxHighlighter : public SyntaxHighlighter
{
public:
    XMLSyntaxHighlighter(QObject* parent=0);
    ~XMLSyntaxHighlighter();
    void setHighlightColor(HighlightType type, QColor color, bool foreground = true);
    void setHighlightFormat(HighlightType type, QTextCharFormat format);


protected:
    void highlightBlock(const QString &text);
    int  processDefaultText(int i, const QString& rstrText);

private:
    QTextCharFormat fmtSyntaxChar;
    QTextCharFormat fmtElementName;
    QTextCharFormat fmtComment;
    QTextCharFormat fmtAttributeName;
    QTextCharFormat fmtAttributeValue;
    QTextCharFormat fmtError;
    QTextCharFormat fmtOther;

    ParsingState state;
};

#endif // SYNTAXHIGHLIGHTER_H
