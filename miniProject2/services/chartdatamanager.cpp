#include "chartdatamanager.h"

// 정적 멤버 초기화
ChartDataManager* ChartDataManager::m_instance = nullptr;
QMutex ChartDataManager::m_mutex;

ChartDataManager::ChartDataManager(QObject *parent)
    : QObject{parent}
{

}

ChartDataManager::~ChartDataManager() {

}


ChartDataManager* ChartDataManager::getInstance() {
    // 쓰레드 안전한 싱글톤 (Double-Checked Locking) - devwooms
    // 쓰레드는 동시에 실행될 경우 뭐가 먼저 실행될지 모르기 때문에 - devwooms
    // locking을 mutex로 하고 확인해서 가져온다 - devwooms
    if (m_instance == nullptr) {
        QMutexLocker locker(&m_mutex);
        if (m_instance == nullptr) {
            m_instance = new ChartDataManager();
        }
    }
    return m_instance;
}

