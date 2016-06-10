 /*
*  Highlighter.cpp
*  ===============
*
*/

 #include <QtWidgets>

 #include "Highlighter.h"

 Highlighter::Highlighter(QTextDocument *parent)
     : QSyntaxHighlighter(parent)
 {
     HighlightingRule rule;

     keywordFormat.setForeground(Qt::blue);
     keywordFormat.setFontWeight(QFont::Bold);
     QStringList keywordPatterns;
     keywordPatterns << "\\band\\b" << "\\barray\\b" << "\\basm\\b"
                     << "\\bbegin\\b" << "\\bcase\\b" << "\\bconst\\b"
                     << "\\bconstructor\\b" << "\\bdestructor\\b" << "\\bdiv\\b"
                     << "\\bdo\\b" << "\\bdownto\\b" << "\\belse\\b"
                     << "\\bend\\b" << "\\bexports\\b" << "\\bfile\\b"
                     << "\\bfor\\b" << "\\bfunction\\b" << "\\bgoto\\b"
                     << "\\bif\\b" << "\\bimplementation\\b" << "\\bin\\b"
                     << "\\binherited\\b" << "\\binline\\b" << "\\binterface\\b"
                     << "\\blabel\\b" << "\\blibrary\\b" << "\\bmod\\b"
                     << "\\bnil\\b" << "\\bnot\\b" << "\\bobject\\b"
					    << "\\bof\\b" << "\\bor\\b" << "\\bpacked\\b"
                     << "\\bprocedure\\b" << "\\bprogram\\b" << "\\brecord\\b"
                     << "\\brepeat\\b" << "\\bset\\b" << "\\bshl\\b"
                     << "\\bshr\\b" << "\\bstring\\b" << "\\bthen\\b"
                     << "\\bto\\b" << "\\btype\\b" << "\\bunit\\b"
                     << "\\buntil\\b" << "\\buses\\b" << "\\bvar\\b"
					 << "\\bwhile\\b" << "\\bwith\\b" << "\\bxor\\b" << "\\bwrite\\b";
					 
     foreach (QString pattern, keywordPatterns) {
         rule.pattern = QRegExp(pattern);
		 rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
         rule.format = keywordFormat;
         highlightingRules.append(rule);
     }

     classFormat.setFontWeight(QFont::Bold);
     classFormat.setForeground(Qt::darkMagenta);
     rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
     rule.format = classFormat;
     highlightingRules.append(rule);
 
     singleLineCommentFormat.setForeground(Qt::red);
     rule.pattern = QRegExp("/[^\n]*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     multiLineCommentFormat.setForeground(Qt::red);

     quotationFormat.setForeground(Qt::darkGreen);
     rule.pattern = QRegExp("\'.*\'");
     rule.format = quotationFormat;
     highlightingRules.append(rule);
	 
	 commentStartExpression = QRegExp("\\(\\*"); 
     commentEndExpression = QRegExp("\\*\\)");

		 
	 
 }

 void Highlighter::highlightBlock(const QString &text)
 {
     foreach (HighlightingRule rule, highlightingRules) {
         QRegExp expression(rule.pattern);
         int index = text.indexOf(expression);
         while (index >= 0) {
             int length = expression.matchedLength();
             setFormat(index, length, rule.format); 
             index = text.indexOf(expression, index + length);
         }
     }
     setCurrentBlockState(0);

     int startIndex = 0;
     if (previousBlockState() != 1)
         startIndex = text.indexOf(commentStartExpression);

     while (startIndex >= 0) {
         int endIndex = text.indexOf(commentEndExpression, startIndex);
         int commentLength;
         if (endIndex == -1) {
             setCurrentBlockState(1);
             commentLength = text.length() - startIndex;
         } else {
             commentLength = endIndex - startIndex
                             + commentEndExpression.matchedLength();
         }
         setFormat(startIndex, commentLength, multiLineCommentFormat);
         startIndex = text.indexOf(commentStartExpression,
                                                 startIndex + commentLength);
     }
	 
 }
 
 
 
 
 
 
 
 
 
