#ifndef SMTPEMAILSENDER_H
#define SMTPEMAILSENDER_H

#include <QObject>
#include <QTcpSocket>
#include <QTextStream>
#include <QTimer>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDateTime>
#include <QRandomGenerator>

class SMTPEmailSender : public QObject
{
    Q_OBJECT
public:
    explicit SMTPEmailSender(QObject *parent = nullptr);

    void setServerSettings(const QString &host, int port, bool useSSL = true);
    void setCredentials(const QString &username, const QString &password);
    void sendVerificationEmail(const QString &recipientEmail, const QString &recipientName = "");

    QString generateVerificationCode();

signals:
    void emailSent(bool success, const QString &message);
    void verificationCodeGenerated(const QString &code);

private slots:
    void socketConnected();
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError error);

private:
    QTcpSocket *socket;
    QTextStream *stream;
    QTimer *timeoutTimer;

    QString smtpHost;
    int smtpPort;
    bool useSSL;
    QString username;
    QString password;
    QString recipientEmail;
    QString recipientName;
    QString verificationCode;

    enum SmtpState {
        Init,
        HandShake,
        Auth,
        User,
        Pass,
        Mail,
        Rcpt,
        Data,
        Body,
        Quit,
        Close
    };

    SmtpState state;

    void nextStep();
    void sendCommand(const QString &command);
    void startConnection();
};
#endif // SMTPEMAILSENDER_H
