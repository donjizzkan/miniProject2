#include "filesendmanager.h"

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
    
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "파일을 열 수 없습니다:" << filePath;
        return false;
    }
    
    // 파일 내용을 Base64로 인코딩
    QByteArray fileData = file.readAll();
    QByteArray base64Data = fileData.toBase64();
    
    file.close();
    
    // JSON 객체 생성
    QJsonObject sendingObj;
    sendingObj["type"] = type;
    sendingObj["chatViewName"] = chatViewName;
    sendingObj["fileName"] = fileInfo.fileName();
    sendingObj["fileSize"] = fileData.size();
    sendingObj["fileExtension"] = fileInfo.suffix();
    sendingObj["fileData"] = QString::fromUtf8(base64Data);
    
    // JSON을 바이트 배열로 변환하여 전송
    QJsonDocument doc(sendingObj);
    QByteArray sendingArray(doc.toJson(QJsonDocument::Compact));
    
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(sendingArray);
        socket->flush();
        qDebug() << "파일 전송 완료:" << fileInfo.fileName() << "(" << fileData.size() << "bytes)";
        return true;
    } else {
        qDebug() << "소켓이 연결되지 않음";
        return false;
    }
}
