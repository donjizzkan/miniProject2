#include "filesendmanager.h"
#include <QDateTime>
#include <QRandomGenerator>
#include <QCryptographicHash>
#include <QMimeDatabase>

FileSendManager::FileSendManager(QObject *parent)
    : QObject{parent}
{

}

void FileSendManager::sendFile(QTcpSocket *socket, QString type, QString& chatViewName, QStringList filePaths){
    qDebug() << "FileSendManager sendFile: 파일 전송 시작";
    qDebug() << "전송할 파일 개수:" << filePaths.size();
    
    for (int i = 0; i < filePaths.size(); ++i) {
        if (!sendSingleFileAsJson(socket, filePaths[i], chatViewName, type)) {
            qDebug() << "파일 전송 실패:" << filePaths[i];
        }
    }
}

bool FileSendManager::sendSingleFileAsJson(QTcpSocket *socket, const QString& filePath, const QString& chatViewName, const QString& type) {
    QFile file(filePath);
    QFileInfo fileInfo(filePath);
    
    // 파일이 존재하고 읽을 수 있는지 확인
    if (!file.exists()) {
        qDebug() << "파일이 존재하지 않습니다:" << filePath;
        return false;
    }
    
    // 파일 크기 확인 (파일을 열지 않고도 가능)
    qint64 fileSize = fileInfo.size();
    
    // 현재 시간을 타임스탬프로 생성
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    
    // 고유한 파일 ID 생성
    QString fileId = QString("file_%1_%2").arg(timestamp).arg(QRandomGenerator::global()->bounded(1000, 9999));
    
    // JSON 객체 생성 (메타데이터만 포함)
    QJsonObject sendingObj;
    sendingObj["type"] = type;
    sendingObj["chatViewName"] = chatViewName;
    sendingObj["fileId"] = fileId;
    sendingObj["fileName"] = fileInfo.fileName();
    sendingObj["originalPath"] = filePath;  // 클라이언트의 원본 파일 경로
    sendingObj["fileSize"] = fileSize;
    sendingObj["fileExtension"] = fileInfo.suffix();
    sendingObj["mimeType"] = getMimeType(fileInfo.suffix());
    sendingObj["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    sendingObj["checksum"] = calculateFileChecksum(filePath);  // 파일 무결성 검증용
    
    // JSON을 바이트 배열로 변환하여 전송
    QJsonDocument doc(sendingObj);
    QByteArray sendingArray(doc.toJson(QJsonDocument::Compact));
    sendingArray.append('\n');  // 메시지 구분자 추가
    
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(sendingArray);
        socket->flush();
        qDebug() << "파일 메타데이터 전송 완료:" << fileInfo.fileName() << "(" << fileSize << "bytes)";
        qDebug() << "파일 ID:" << fileId;
        return true;
    } else {
        qDebug() << "소켓이 연결되지 않음";
        return false;
    }
}

// MIME 타입 결정 헬퍼 함수
QString FileSendManager::getMimeType(const QString& extension) {
    QMimeDatabase mimeDb;
    QString fileName = "dummy." + extension.toLower();
    QMimeType mimeType = mimeDb.mimeTypeForFile(fileName, QMimeDatabase::MatchExtension);
    return mimeType.name();
}

// 파일 체크섬 계산 헬퍼 함수 (무결성 검증용)
QString FileSendManager::calculateFileChecksum(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }
    
    QCryptographicHash hash(QCryptographicHash::Md5);
    if (hash.addData(&file)) {
        return QString(hash.result().toHex());
    }
    
    return QString();
}
