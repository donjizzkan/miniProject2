# miniProject2

## 주제 : 비트코인

고객 관리 및 상품 관리 프로그램에 네트워크 & GUI 기능 추가
• 네트워크 통신을 이용한 서버 & 클라이언트 프로그래밍
• 채팅 서버/클라이언트 구현 : 채팅 프로토콜 설계
• 채팅 초대, 강제 탈퇴, 일 대 다 채팅, 첨부 파일 전송 등
• Qt의 템플릿을 사용하여 자료 관리 및 검색 구현
• 서버 프로그램 구현(싱글턴 패턴 적용)
• 데이터를 저장하고 서비스를 제공하는 부분
• 클라이언트 프로그램 구현
• 쓰레드를 이용한 멀티 태스크 처리 : 멀티 스레드를 사용한 채팅 메시지 로그 기록

## Git 컨벤션
- [큰분류] / [이름] / [작은분류]


  📊 Qt 기반 암호화폐 거래 시스템 구조도

  ┌─────────────────────────────────────────────────────────────────────────────────┐
  │                           🏗️ Qt 기반 암호화폐 거래 시스템                              │
  │                              (Client-Server Architecture)                       │
  └─────────────────────────────────────────────────────────────────────────────────┘

  ┌─────────────────────────────────────┐    TCP/JSON     ┌─────────────────────────────────────┐
  │            📱 CLIENT SIDE            │ ◄──────────────► │            🖥️ SERVER SIDE            │
  │         (miniProject2)              │                 │       (miniProjcet2_Server)         │
  │                                     │                 │                                     │
  │  ┌─────────────────────────────────┐ │                 │  ┌─────────────────────────────────┐ │
  │  │         🎨 VIEWS (UI)           │ │                 │  │        📊 MODELS               │ │
  │  │  ┌─────────────────────────────┐│ │                 │  │  ┌─────────────────────────────┐│ │
  │  │  │ • SplashView                ││ │                 │  │  │ • ClientHandler             ││ │
  │  │  │ • LoginView                 ││ │                 │  │  │ • UserManage                ││ │
  │  │  │ • SignupView (📧 SMTP)      ││ │                 │  │  │   - Email Verification      ││ │
  │  │  │ • HomeView                  ││ │                 │  │  │   - User Authentication     ││ │
  │  │  │ • ChattingRoomView          ││ │                 │  │  │   - Report System           ││ │
  │  │  │ • CandleChart               ││ │                 │  │  └─────────────────────────────┘│ │
  │  │  │ • LineChartView             ││ │                 │  └─────────────────────────────────┘ │
  │  │  │ • ChartsToolBox             ││ │                 │                                     │
  │  │  │ • CoinSearchWidget          ││ │                 │  ┌─────────────────────────────────┐ │
  │  │  └─────────────────────────────┘│ │                 │  │        🌐 NETWORKS             │ │
  │  └─────────────────────────────────┘ │                 │  │  ┌─────────────────────────────┐│ │
  │                                     │                 │  │  │ • ServerManager (Singleton) ││ │
  │  ┌─────────────────────────────────┐ │                 │  │  │ • ClientSetup               ││ │
  │  │        🔧 MODELS               │ │                 │  │  │ • TCP Server (Port 51234)  ││ │
  │  │  ┌─────────────────────────────┐│ │                 │  │  │ • Multi-threaded            ││ │
  │  │  │ • MainModel                 ││ │                 │  │  └─────────────────────────────┘│ │
  │  │  │ • SocketManage (Singleton)  ││ │                 │  └─────────────────────────────────┘ │
  │  │  │ • UserManage                ││ │                 │                                     │
  │  │  │ • SendingManage             ││ │                 │  ┌─────────────────────────────────┐ │
  │  │  │ • ChattingListManager       ││ │                 │  │        💾 DATABASE             │ │
  │  │  │ • FileSendManager           ││ │                 │  │  ┌─────────────────────────────┐│ │
  │  │  │ • CandleChartDataManager    ││ │                 │  │  │ DB/ (JSON Files)            ││ │
  │  │  │ • LineChartDataManager      ││ │                 │  │  │ • userInfo.json             ││ │
  │  │  │ • SMTPEmailSender           ││ │                 │  │  │ • test.json (Chat logs)     ││ │
  │  │  └─────────────────────────────┘│ │                 │  │  │ • report.json               ││ │
  │  └─────────────────────────────────┘ │                 │  │  └─────────────────────────────┘│ │
  │                                     │                 │  └─────────────────────────────────┘ │
  │  ┌─────────────────────────────────┐ │                 │                                     │
  │  │        📊 SERVICES             │ │                 │  ┌─────────────────────────────────┐ │
  │  │  ┌─────────────────────────────┐│ │                 │  │        🎨 VIEWS               │ │
  │  │  │ • ChartDataManager          ││ │                 │  │  ┌─────────────────────────────┐│ │
  │  │  └─────────────────────────────┘│ │                 │  │  │ • MainWindow (Test UI)     ││ │
  │  └─────────────────────────────────┘ │                 │  │  │ • System Tray Application   ││ │
  │                                     │                 │  │  └─────────────────────────────┘│ │
  │  ┌─────────────────────────────────┐ │                 │  └─────────────────────────────────┘ │
  │  │        🗂️ DATA MODELS          │ │                 │                                     │
  │  │  ┌─────────────────────────────┐│ │                 └─────────────────────────────────────┘
  │  │  │ • CandleData                ││ │
  │  │  │ • LineData                  ││ │                 ┌─────────────────────────────────────┐
  │  │  └─────────────────────────────┘│ │                 │        📧 EMAIL VERIFICATION        │
  │  └─────────────────────────────────┘ │                 │                                     │
  │                                     │                 │  ┌─────────────────────────────────┐ │
  │  ┌─────────────────────────────────┐ │                 │  │ Server-side SMTP Processing     │ │
  │  │        🌐 NETWORKS             │ │                 │  │ • Environment Variables         │ │
  │  │  ┌─────────────────────────────┐│ │                 │  │ • 6-digit Random Codes         │ │
  │  │  │ • SocketNetwork             ││ │                 │  │ • 5-minute Expiration          │ │
  │  │  └─────────────────────────────┘│ │                 │  │ • Secure Authentication        │ │
  │  └─────────────────────────────────┘ │                 │  └─────────────────────────────────┘ │
  │                                     │                 └─────────────────────────────────────┘
  └─────────────────────────────────────┘

  ┌─────────────────────────────────────────────────────────────────────────────────┐
  │                           🔄 COMMUNICATION FLOW                                │
  │                                                                                 │
  │  📱 Client ──JSON──► 🖥️ Server ──Process──► 💾 Database ──Response──► 📱 Client  │
  │                                                                                 │
  │  📨 Message Types:                                                             │
  │  • login / signup / email_verification / verify_email_code                    │
  │  • messagesend / filesend / givemelog / trade / report                        │
  │                                                                                 │
  │  🔒 Security Features:                                                         │
  │  • Server-side Email Authentication • Environment Variables                    │
  │  • SSL/TLS Support • User Report System • Banned User Detection              │
  └─────────────────────────────────────────────────────────────────────────────────┘

  📋 PPT용 간단 버전

  ┌──────────────────────────────────────────────────────────────┐
  │               🏗️ Qt 암호화폐 거래 시스템                      │
  └──────────────────────────────────────────────────────────────┘

  📱 CLIENT (miniProject2)          🖥️ SERVER (miniProjcet2_Server)
  ┌─────────────────────────┐       ┌─────────────────────────┐
  │  🎨 Views (UI Layer)    │       │  🔧 Business Logic      │
  │  • Login/Signup         │       │  • User Management      │
  │  • Trading Dashboard    │◄─────►│  • Email Verification   │
  │  • Real-time Charts     │ TCP   │  • Chat Processing      │
  │  • Chat Rooms           │ JSON  │  • Trade Processing     │
  │                         │       │  • Report System        │
  │  🔧 Models & Services   │       │                         │
  │  • Socket Management    │       │  🌐 Network Layer       │
  │  • Data Management      │       │  • Multi-threaded       │
  │  • Chart Processing     │       │  • Broadcasting         │
  │                         │       │                         │
  │  📊 Data Structures     │       │  💾 Data Storage        │
  │  • Candle/Line Data     │       │  • JSON Files           │
  │  • User Information     │       │  • User Database        │
  └─────────────────────────┘       └─────────────────────────┘

  🔑 Key Features: Real-time Trading • Secure Chat • Email Auth • Charts

  🎯 핵심 기능 다이어그램

  ┌─────────────────────────────────────────────────────────────┐
  │                    🚀 핵심 기능                             │
  └─────────────────────────────────────────────────────────────┘

  📧 Email Authentication    💬 Real-time Chat       📈 Trading System
  ┌─────────────────────┐   ┌─────────────────────┐  ┌─────────────────────┐
  │ Client → Server     │   │ Multi-user Support  │  │ Buy/Sell Orders     │
  │ Server → SMTP       │   │ File Transfer       │  │ Real-time Prices    │
  │ Code Verification   │   │ Chat History        │  │ Portfolio Tracking  │
  │ Environment Vars    │   │ Report System       │  │ Transaction History │
  └─────────────────────┘   └─────────────────────┘  └─────────────────────┘

  📊 Chart Visualization    🔒 Security Features     🗄️ Data Management
  ┌─────────────────────┐   ┌─────────────────────┐  ┌─────────────────────┐
  │ Candlestick Charts  │   │ User Authentication │  │ JSON Database       │
  │ Line Charts         │   │ Server-side Validation│ │ Real-time Updates   │
  │ Real-time Updates   │   │ Report & Ban System │  │ Multi-threaded      │
  │ Multiple Cryptocoin │   │ SSL/TLS Support     │  │ File Operations     │
  └─────────────────────┘   └─────────────────────┘  └─────────────────────┘
