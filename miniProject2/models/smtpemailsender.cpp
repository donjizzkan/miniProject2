#include "smtpemailsender.h"
#include <QSslSocket>
#include <QDebug>

SMTPEmailSender::SMTPEmailSender(QObject *parent)
    : QObject{parent}
    , socket(nullptr)
    , stream(nullptr)
    , timeoutTimer(new QTimer(this))
    , smtpPort(587)
    , useSSL(true)
    , state(Init)
{
    timeoutTimer->setSingleShot(true);
    timeoutTimer->setInterval(30000); // 30초 타임아웃

    connect(timeoutTimer, &QTimer::timeout, this, [this]() {
        emit emailSent(false, "Connection timeout");
        if (socket) socket->abort();
    });
}

void SMTPEmailSender::setServerSettings(const QString &host, int port, bool ssl)
{
    smtpHost = host;
    smtpPort = port;
    useSSL = ssl;
}

void SMTPEmailSender::setCredentials(const QString &user, const QString &pass)
{
    username = user;
    password = pass;
}

QString SMTPEmailSender::generateVerificationCode()
{
    // 6자리 랜덤 숫자 생성
    int code = QRandomGenerator::global()->bounded(100000, 999999);
    verificationCode = QString::number(code);
    emit verificationCodeGenerated(verificationCode);
    return verificationCode;
}

void SMTPEmailSender::sendVerificationEmail(const QString &email, const QString &name)
{
    recipientEmail = email;
    recipientName = name.isEmpty() ? email : name;

    if (verificationCode.isEmpty()) {
        generateVerificationCode();
    }

    startConnection();
}

void SMTPEmailSender::startConnection()
{
    if (socket) {
        socket->deleteLater();
    }

    if (useSSL) {
        socket = new QSslSocket(this);
    } else {
        socket = new QTcpSocket(this);
    }

    stream = new QTextStream(socket);

    connect(socket, &QTcpSocket::connected, this, &SMTPEmailSender::socketConnected);
    connect(socket, &QTcpSocket::readyRead, this, &SMTPEmailSender::socketReadyRead);
    // connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
    //         this, &SMTPEmailSender::socketError);

    state = Init;
    timeoutTimer->start();

    if (useSSL) {
        static_cast<QSslSocket*>(socket)->connectToHostEncrypted(smtpHost, smtpPort);
    } else {
        socket->connectToHost(smtpHost, smtpPort);
    }
}

void SMTPEmailSender::socketConnected()
{
    qDebug() << "Connected to SMTP server";
}

void SMTPEmailSender::socketReadyRead()
{
    QString response = socket->readAll();
    qDebug() << "Server response:" << response;

    if (response.startsWith("220") && state == Init) {
        state = HandShake;
        sendCommand("EHLO localhost");
    }
    else if (response.startsWith("250") && state == HandShake) {
        state = Auth;
        sendCommand("AUTH LOGIN");
    }
    else if (response.startsWith("334") && state == Auth) {
        state = User;
        sendCommand(username.toUtf8().toBase64());
    }
    else if (response.startsWith("334") && state == User) {
        state = Pass;
        sendCommand(password.toUtf8().toBase64());
    }
    else if (response.startsWith("235") && state == Pass) {
        state = Mail;
        sendCommand(QString("MAIL FROM:<%1>").arg(username));
    }
    else if (response.startsWith("250") && state == Mail) {
        state = Rcpt;
        sendCommand(QString("RCPT TO:<%1>").arg(recipientEmail));
    }
    else if (response.startsWith("250") && state == Rcpt) {
        state = Data;
        sendCommand("DATA");
    }
    else if (response.startsWith("354") && state == Data) {
        state = Body;

        // 이메일 본문 작성
        QString emailBody = QString(
                                "From: %1\r\n"
                                "To: %2\r\n"
                                "Subject: 이메일 인증 코드\r\n"
                                "Content-Type: text/html; charset=UTF-8\r\n"
                                "\r\n"
                                "<html><body>"
                                "<h2>이메일 인증</h2>"
                                "<p>안녕하세요 %3님,</p>"
                                "<p>요청하신 인증 코드는 다음과 같습니다:</p>"
                                "<h3 style='color: #2196F3; font-size: 24px;'>%4</h3>"
                                "<p>이 코드는 5분간 유효합니다.</p>"
                                "<p>감사합니다.</p>"
                                "</body></html>"
                                "\r\n.\r\n"
                                ).arg(username, recipientEmail, recipientName, verificationCode);

        sendCommand(emailBody);
    }
    else if (response.startsWith("250") && state == Body) {
        state = Quit;
        sendCommand("QUIT");
    }
    else if (response.startsWith("221") && state == Quit) {
        timeoutTimer->stop();
        socket->disconnectFromHost();
        emit emailSent(true, "이메일이 성공적으로 발송되었습니다.");
    }
    else if (response.startsWith("5") || response.startsWith("4")) {
        timeoutTimer->stop();
        emit emailSent(false, QString("SMTP 오류: %1").arg(response.trimmed()));
        socket->abort();
    }
}

void SMTPEmailSender::socketError(QAbstractSocket::SocketError error)
{
    timeoutTimer->stop();
    QString errorMsg = QString("네트워크 오류: %1").arg(socket->errorString());
    emit emailSent(false, errorMsg);
    qDebug() << "Socket error:" << error << errorMsg;
}

void SMTPEmailSender::sendCommand(const QString &command)
{
    qDebug() << "Sending command:" << command;
    *stream << command << "\r\n";
    stream->flush();
}
