# CLAUDE.md

이 파일은 이 저장소에서 작업할 때 Claude Code (claude.ai/code)에게 가이드를 제공합니다.

## 프로젝트 개요

**주제: 비트코인**

고객 관리 및 상품 관리 프로그램에 네트워크 & GUI 기능이 추가된 Qt 기반 암호화폐 거래 애플리케이션입니다. 실시간 채팅 기능을 포함하며 두 개의 주요 컴포넌트로 구성됩니다:

1. **miniProject2** - 클라이언트 애플리케이션 (GUI)
2. **miniProjcet2_Server** - 서버 애플리케이션 (시스템 트레이 기반)

### 주요 기능

• 네트워크 통신을 이용한 서버 & 클라이언트 프로그래밍
• 채팅 서버/클라이언트 구현 : 채팅 프로토콜 설계
• 채팅 초대, 강제 탈퇴, 일 대 다 채팅, 첨부 파일 전송 등
• Qt의 템플릿을 사용하여 자료 관리 및 검색 구현
• 서버 프로그램 구현(싱글턴 패턴 적용)
• 데이터를 저장하고 서비스를 제공하는 부분
• 쓰레드를 이용한 멀티 태스크 처리 : 멀티 스레드를 사용한 채팅 메시지 로그 기록
• **이메일 인증 시스템**: 회원가입 시 SMTP를 통한 이메일 인증 코드 전송 및 검증
• **차트 시각화**: 암호화폐 가격 데이터의 캔들스틱 및 라인 차트 표시
• **코인 검색**: 실시간 암호화폐 검색 및 선택 기능

## 빌드 시스템

### 클라이언트 (miniProject2)
- **빌드 명령어**: `qmake miniProject2.pro && make` 또는 Qt Creator 사용
- **프로젝트 파일**: `miniProject2/miniProject2.pro`
- **필요한 Qt 모듈**: core, gui, charts, network, widgets
- **C++ 표준**: C++17
- **애플리케이션 아이콘**: Bitcoin.icns (macOS용 아이콘 설정됨)

### 서버 (miniProjcet2_Server)
- **빌드 명령어**: `qmake miniProjcet2_Server.pro && make` 또는 Qt Creator 사용
- **프로젝트 파일**: `miniProjcet2_Server/miniProjcet2_Server.pro`
- **필요한 Qt 모듈**: core, gui, network, widgets
- **C++ 표준**: C++17

## 아키텍처

### 클라이언트 애플리케이션 (MVC 패턴)

**Models** (`models/`):
- `mainmodel.cpp/h` - 핵심 애플리케이션 모델
- `socketManage.cpp/h` - TCP 소켓 관리 및 JSON 메시지 처리용 싱글턴
- `userManage.cpp/h` - 사용자 인증 및 세션 관리
- `sendingManage.cpp/h` - 메시지 전송 조정
- `chattinglistmanager.cpp/h` - 채팅 메시지 목록 관리
- `filesendmanager.cpp/h` - 파일 전송 처리
- `smtpemailsender.cpp/h` - SMTP 이메일 인증 시스템 (회원가입 시 이메일 인증 코드 전송)
- `candlechartdatamanager.cpp/h` - 캔들스틱 차트 데이터 관리
- `linechartdatamanager.cpp/h` - 라인 차트 데이터 관리

**Views** (`views/`):
- `splashview.cpp/h` - 애플리케이션 시작 스플래시 화면
- `loginview.cpp/h` - 사용자 인증 인터페이스
- `signupview.cpp/h` - 사용자 등록 인터페이스 (이메일 인증 기능 포함)
- `homeview.cpp/h` - 메인 애플리케이션 대시보드
- `chattingRoomView.cpp/h` - 실시간 채팅 인터페이스
- `candlechart.cpp/h` - 캔들스틱 차트 시각화 뷰
- `linechartview.cpp/h` - 라인 차트 시각화 뷰
- `chartstoolbox.cpp/h` - 차트 도구 상자 UI
- `coinsearchlineedit.cpp/h` - 코인 검색 입력 필드
- `coinsearchwidget.cpp/h` - 코인 검색 위젯
- `dropwidget.cpp/h` - 파일 드래그 앤 드롭 기능

**Services** (`services/`):
- `chartdatamanager.cpp/h` - 암호화폐 가격 데이터 관리

**Data Models** (`data/`):
- `candledata.cpp/h` - 캔들스틱 차트 데이터 구조
- `linedata.cpp/h` - 라인 차트 데이터 구조

**Networks** (`networks/`):
- `socketnetwork.cpp` - 네트워크 통신 레이어

### 서버 애플리케이션

**Models** (`models/`):
- `clientHandler.cpp/h` - 개별 클라이언트 연결 관리
- `usermanage.cpp/h` - 서버 측 사용자 관리 및 인증

**Networks** (`networks/`):
- `servermanager.cpp/h` - 싱글턴 TCP 서버 관리자
- `clientsetup.cpp/h` - 클라이언트 연결 설정 및 구성

**Views** (`views/`):
- `mainwindow.cpp/h` - 서버 관리 인터페이스 (테스트용)

### 주요 디자인 패턴

1. **싱글턴 패턴**: `SocketManage` (클라이언트)와 `ServerManager` (서버)에서 단일 인스턴스 보장
2. **MVC 패턴**: 모델, 뷰, 컨트롤러 간 명확한 분리
3. **Signal/Slot 패턴**: 애플리케이션 전반에 걸친 Qt의 이벤트 기반 통신

### 통신 프로토콜

- **전송 방식**: Qt의 `QTcpSocket`과 `QTcpServer`를 통한 TCP 소켓
- **메시지 형식**: 모든 클라이언트-서버 통신에 JSON 객체 사용
- **주요 메시지 타입**: 로그인 응답, 채팅 메시지, 파일 전송, 채팅 로그

### SMTP 이메일 인증 시스템

- **프로토콜**: SMTP over TCP (기본 포트 587, SSL/TLS 지원)
- **인증 코드**: 6자리 랜덤 숫자 생성 (`100000-999999` 범위)
- **상태 머신**: Init → HandShake → Auth → User → Pass → Mail → Rcpt → Data → Body → Quit → Close
- **타임아웃**: 30초 연결 타임아웃 설정
- **보안**: QCryptographicHash 및 SSL 소켓 지원

### 데이터 저장

- **서버**: `DB/` 디렉토리의 JSON 파일들 (`userInfo.json`, `test.json`)
- **클라이언트**: 암호화폐 데이터 캐싱용 로컬 JSON 파일 (`btc_price.json`)

## 에셋 및 리소스

### 아이콘 및 이미지
- **애플리케이션 아이콘**: `assets/Bitcoin.icns` (macOS), `assets/Bitcoin.ico` (Windows)
- **암호화폐 아이콘**: Bitcoin, Ethereum, Dogecoin, XRP, PEPE 등의 코인 이미지
- **UI 요소**: 파일 추가 아이콘, 경고 아이콘, 닫기 버튼 SVG
- **애니메이션**: 비트코인 로딩 애니메이션 (`bitAnimation.gif`)

### 리소스 관리
- `resource.qrc`: 모든 에셋 파일들의 Qt 리소스 매핑
- `/assets` 및 `/charts/test` 프리픽스로 리소스 그룹화

## 개발 참고사항

- 프로덕션에서 서버는 시스템 트레이 애플리케이션으로 실행
- 클라이언트는 메인 윈도우 전에 2초 지연 스플래시 화면 표시
- `.ts` 번역 파일을 통한 한국어 현지화 지원
- 임베디드 에셋 및 아이콘용 리소스 파일 (`.qrc`)
- **SMTP 이메일 기능**: 회원가입 시 6자리 랜덤 인증 코드 생성 및 이메일 전송

## Git 컨벤션

- `[큰분류] / [이름] / [작은분류]` 패턴 따름