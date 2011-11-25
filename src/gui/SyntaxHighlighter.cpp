#include <QTextEdit>
#include "SyntaxHighlighter.h"
#include "core/globalDefs.h"

SyntaxHighlighter::SyntaxHighlighter(QObject *parent):QSyntaxHighlighter(parent)
{
    QSyntaxHighlighter::setDocument(dynamic_cast<QTextEdit*>(parent)->document());
}

SyntaxHighlighter::~SyntaxHighlighter()
{

}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    // Will be overloaded by the children
    Q_UNUSED(text);
}

// -------------------------------------------------------------------------------
JSSyntaxHighlighter::JSSyntaxHighlighter(QObject *parent):SyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // Keywords
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns     << "\\bbreak\\b"
                        << "\\bcase\\b"
                        << "\\bcatch\\b"
                        << "\\bcontinue\\b"
                        << "\\bdebugger\\b"
                        << "\\bdefault\\b"
                        << "\\bdelete\\b"
                        << "\\bdo\\b"
                        << "\\belse\\b"
                        << "\\bfinally\\b"
                        << "\\bfor\\b"
                        << "\\bfunction\\b"
                        << "\\bif\\b"
                        << "\\bin\\b"
                        << "\\binstanceof\\b"
                        << "\\bnew\\b"
                        << "\\breturn\\b"
                        << "\\bswitch\\b"
                        << "\\bthis\\b"
                        << "\\bthrow\\b"
                        << "\\btry\\b"
                        << "\\btypeof\\b"
                        << "\\bvar\\b"
                        << "\\bvoid\\b"
                        << "\\bwhile\\b"
                        << "\\bwith\\b"
                        ;
    foreach (const QString &pattern, keywordPatterns) {
     rule.pattern = QRegExp(pattern);
     rule.format = keywordFormat;
     highlightingRules.append(rule);
    }

    // Class
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    // Single Line Comment
    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // Multi Line Comment
    multiLineCommentFormat.setForeground(Qt::darkGreen);

    // Quotation
    quotationFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

//    functionFormat.setFontItalic(true);
//    functionFormat.setForeground(Qt::blue);
//    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
//    rule.format = functionFormat;
//    highlightingRules.append(rule);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

JSSyntaxHighlighter::~JSSyntaxHighlighter()
{

}

void JSSyntaxHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
     startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0)
    {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if(endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
        }
         setFormat(startIndex, commentLength, multiLineCommentFormat);
         startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}

// -------------------------------------------------------------------------------
XMLSyntaxHighlighter::XMLSyntaxHighlighter(QObject *parent):SyntaxHighlighter(parent)
{
    fmtSyntaxChar.setForeground(Qt::blue);
    fmtElementName.setForeground(Qt::darkRed);
    fmtComment.setForeground(Qt::darkGreen);
    fmtAttributeName.setForeground(Qt::red);
    fmtAttributeValue.setForeground(Qt::blue);
    fmtError.setForeground(Qt::darkMagenta);
    fmtOther.setForeground(Qt::black);
}

XMLSyntaxHighlighter::~XMLSyntaxHighlighter()
{

}

void XMLSyntaxHighlighter::highlightBlock(const QString &text)
{
    int i = 0;
    int pos = 0;
    int brackets = 0;

    state = (previousBlockState() == InElement ? ExpectAttributeOrEndOfElement : NoState);

    if (previousBlockState() == InComment)
    {
            // search for the end of the comment
            QRegExp expression("[^-]*-([^-][^-]*-)*->");
            pos = expression.indexIn(text, i);

            if (pos >= 0)
            {
                    // end comment found
                    const int iLength = expression.matchedLength();
                    setFormat(0, iLength - 3, fmtComment);
                    setFormat(iLength - 3, 3, fmtSyntaxChar);
                    i += iLength; // skip comment
            }
            else
            {
                    // in comment
                    setFormat(0, text.length(), fmtComment);
                    setCurrentBlockState(InComment);
                    return;
            }
    }

    for (; i < text.length(); i++)
    {
            switch (text.at(i).toAscii())
            {
            case '<':
                    brackets++;
                    if (brackets == 1)
                    {
                            setFormat(i, 1, fmtSyntaxChar);
                            state = ExpectElementNameOrSlash;
                    }
                    else
                    {
                            // wrong bracket nesting
                            setFormat(i, 1, fmtError);
                    }
                    break;

            case '>':
                    brackets--;
                    if (brackets == 0)
                    {
                            setFormat(i, 1, fmtSyntaxChar);
                    }
                    else
                    {
                            // wrong bracket nesting
                            setFormat( i, 1, fmtError);
                    }
                    state = NoState;
                    break;

            case '/':
                    if (state == ExpectElementNameOrSlash)
                    {
                            state = ExpectElementName;
                            setFormat(i, 1, fmtSyntaxChar);
                    }
                    else
                    {
                            if (state == ExpectAttributeOrEndOfElement)
                            {
                                    setFormat(i, 1, fmtSyntaxChar);
                            }
                            else
                            {
                                    processDefaultText(i, text);
                            }
                    }
                    break;

            case '=':
                    if (state == ExpectEqual)
                    {
                            state = ExpectAttributeValue;
                            setFormat(i, 1, fmtOther);
                    }
                    else
                    {
                            processDefaultText(i, text);
                    }
                    break;

            case '\'':
            case '\"':
                    if (state == ExpectAttributeValue)
                    {
                            // search attribute value
                            QRegExp expression("\"[^<\"]*\"|'[^<']*'");
                            pos = expression.indexIn(text, i);

                            if (pos == i) // attribute value found ?
                            {
                                    const int iLength = expression.matchedLength();

                                    setFormat(i, 1, fmtOther);
                                    setFormat(i + 1, iLength - 2, fmtAttributeValue);
                                    setFormat(i + iLength - 1, 1, fmtOther);

                                    i += iLength - 1; // skip attribute value
                                    state = ExpectAttributeOrEndOfElement;
                            }
                            else
                            {
                                    processDefaultText(i, text);
                            }
                    }
                    else
                    {
                            processDefaultText(i, text);
                    }
                    break;

            case '!':
                    if (state == ExpectElementNameOrSlash)
                    {
                            // search comment
                            QRegExp expression("<!--[^-]*-([^-][^-]*-)*->");
                            pos = expression.indexIn(text, i - 1);

                            if (pos == i - 1) // comment found ?
                            {
                                    const int iLength = expression.matchedLength();

                                    setFormat(pos, 4, fmtSyntaxChar);
                                    setFormat(pos + 4, iLength - 7, fmtComment);
                                    setFormat(iLength - 3, 3, fmtSyntaxChar);
                                    i += iLength - 2; // skip comment
                                    state = NoState;
                                    brackets--;
                            }
                            else
                            {
                                    // Try find multiline comment
                                    QRegExp expression("<!--"); // search comment start
                                    pos = expression.indexIn(text, i - 1);

                                    //if (pos == i - 1) // comment found ?
                                    if (pos >= i - 1)
                                    {
                                            setFormat(i, 3, fmtSyntaxChar);
                                            setFormat(i + 3, text.length() - i - 3, fmtComment);
                                            setCurrentBlockState(InComment);
                                            return;
                                    }
                                    else
                                    {
                                            processDefaultText(i, text);
                                    }
                            }
                    }
                    else
                    {
                            processDefaultText(i, text);
                    }

                    break;

            default:
                    const int iLength = processDefaultText(i, text);
                    if (iLength > 0)
                            i += iLength - 1;
                    break;
            }
    }

    if (state == ExpectAttributeOrEndOfElement)
    {
            setCurrentBlockState(InElement);
    }
}

int XMLSyntaxHighlighter::processDefaultText(int i, const QString& text)
{
    // length of matched text
    int iLength = 0;

    switch(state)
    {
    case ExpectElementNameOrSlash:
    case ExpectElementName:
    {
        // search element name
        QRegExp expression("([A-Za-z_:]|[^\\x00-\\x7F])([A-Za-z0-9_:.-]|[^\\x00-\\x7F])*");
        const int pos = expression.indexIn(text, i);

        if (pos == i) // found ?
        {
            iLength = expression.matchedLength();

            setFormat(pos, iLength, fmtElementName);
            state = ExpectAttributeOrEndOfElement;
        }
        else
        {
            setFormat(i, 1, fmtOther);
        }
    }
    break;

    case ExpectAttributeOrEndOfElement:
    {
        // search attribute name
        QRegExp expression("([A-Za-z_:]|[^\\x00-\\x7F])([A-Za-z0-9_:.-]|[^\\x00-\\x7F])*");
        const int pos = expression.indexIn(text, i);

        if (pos == i) // found ?
        {
            iLength = expression.matchedLength();

            setFormat(pos, iLength, fmtAttributeName);
            state = ExpectEqual;
        }
        else
        {
            setFormat(i, 1, fmtOther);
        }
    }
    break;

    default:
        setFormat(i, 1, fmtOther);
        break;
    }
    return iLength;
}

void XMLSyntaxHighlighter::setHighlightColor(HighlightType type, QColor color, bool foreground)
{
    QTextCharFormat format;
    if (foreground)
        format.setForeground(color);
    else
        format.setBackground(color);
    setHighlightFormat(type, format);
}

void XMLSyntaxHighlighter::setHighlightFormat(HighlightType type, QTextCharFormat format)
{
    switch (type)
    {
        case SyntaxChar:
            fmtSyntaxChar = format;
            break;
        case ElementName:
            fmtElementName = format;
            break;
        case Comment:
            fmtComment = format;
            break;
        case AttributeName:
            fmtAttributeName = format;
            break;
        case AttributeValue:
            fmtAttributeValue = format;
            break;
        case Error:
            fmtError = format;
            break;
        case Other:
            fmtOther = format;
            break;
    }
    rehighlight();
}
