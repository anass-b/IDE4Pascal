#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QString>

class Document : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER _text NOTIFY textChanged)
    Q_PROPERTY(QString remoteText MEMBER _remoteText)
public:
    explicit Document(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    void setText(const QString& text);

    void setRemoteText(const QString& remoteText);
    const QString& getRemoteText() const;

signals:
    void textChanged(const QString& text);

private:
    QString _text;
    QString _remoteText;
};

#endif // DOCUMENT_H
