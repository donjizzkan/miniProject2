#include "loginview.h"
#include "sendingManage.h"
#include "socketManage.h"
#include "ui_loginview.h"
#include <QDebug>      // 디버그 출력을 위해 추가
#include <QMessageBox> // QMessageBox 사용을 위해 추가

LoginView::LoginView(QWidget *parent) : QWidget(parent), ui(new Ui::LoginView) {

  ui->setupUi(this);

  //==========================
  //        소켓 연결
  //==========================

  // socket Manage에서 생성된 socket 객체 가져오기
  QTcpSocket *socket = SocketManage::instance().socket();

  // 소켓 연결 시도 (LoginView 생성 시점에 한 번만 시도)
  // 이미 연결되어 있다면 다시 연결 시도하지 않음
  if (socket->state() == QAbstractSocket::UnconnectedState) {
    // 192.168.2.235 => devwooms
    // 192.168.56.1 => donjizzkan
    socket->connectToHost("192.168.56.1", 51234);
    // 서버 연결 완료 시그널은 SocketManage에서 처리하므로 여기서 직접 연결할
    // 필요 없음 connect(socket, &QTcpSocket::connected, this, [](){
    //     qDebug("서버 연결 완료");
    // });
  }

  //==========================
  // 통과 신호(로그인 응답) 받으면 메인화면으로
  //==========================
  // SocketManage에서 방출하는 loginResponseReceived 시그널을 받도록 변경
  connect(&SocketManage::instance(), &SocketManage::loginResponseReceived, this,
          [this](const QJsonObject &obj) {
            qDebug() << "LoginView: loginResponseReceived 시그널 받음";
            // 이 obj는 이미 SocketManage에서 파싱된 JSON 객체임
            if (obj.value("type").toString() ==
                "response") { // 타입이 'response'인지 다시 확인 (안전성)
              QString result = obj.value("result").toString();
              // 로그인 성공/실패에 따라 UI 전환
              if (result == "success") {
                // 해당 로그인 정보에 해당하는 이름 받아 옴
                QString senderName = obj.value("name").toString();
                qDebug() << senderName << "으로 로그인 성공";
                // sendingManage에 있는 이름 setter로 내 이름 저장
                // 파일이나 메세지 전송시 저장된 senderName 불러옴
                sendingManage::instance()->setSenderName(senderName);
                emit goToMain(); // 메인 화면으로 이동 시그널 방출
              } else {
                QMessageBox::warning(this, "로그인 실패",
                                     "ID 또는 PW가 틀렸습니다!");
                qDebug() << "로그인 실패: ID 또는 PW 불일치";
              }
            }
          });

  // UI 버튼 연결
  connect(ui->pw_LineEdit, &QLineEdit::returnPressed, this,
          [=]() { ui->login_Button->click(); });
  connect(ui->signup_Button, &QPushButton::clicked, this,
          &LoginView::goToSignup);
  connect(ui->login_Button, &QPushButton::clicked, this, &LoginView::doSignIn);
}

LoginView::~LoginView() { delete ui; }

QString LoginView::getInsertedID() {
  QString getID;
  getID = ui->id_LineEdit->text();
  return getID;
}
QString LoginView::getInsertedPW() {
  QString getPW;
  getPW = ui->pw_LineEdit->text();
  return getPW;
}
