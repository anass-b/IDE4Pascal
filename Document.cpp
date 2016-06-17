#include "Document.h"

void Document::setText(const QString& text)
{
    if (text == _text)
        return;
    _text = text;
    _remoteText = _text;
    emit textChanged(_text);
}

void Document::setRemoteText(const QString& remoteText)
{
    if (remoteText == _remoteText)
        return;
    _remoteText = remoteText;
}

const QString& Document::getRemoteText() const
{
    return _remoteText;
}
