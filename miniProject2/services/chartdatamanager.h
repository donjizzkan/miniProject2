#ifndef CHARTDATAMANAGER_H
#define CHARTDATAMANAGER_H

#include <QObject>
#include <QMutex>

class ChartDataManager : public QObject
{
    Q_OBJECT

public:
    // 싱글톤 접근 메서드 - devwooms
    static ChartDataManager* getInstance();
    // 싱글톤 해제 (옵션) - devwooms
    static void destroyInstance();

signals:
    // void stockDataUpdated(const QList<StockData>& stocks);

private:
    // 생성자를 private으로 (싱글톤 패턴) - devwooms
    explicit ChartDataManager(QObject* parent = nullptr);
    ~ChartDataManager();

    // 싱글톤 인스턴스 - devwooms
    static ChartDataManager* m_instance;
    static QMutex m_mutex;  // 쓰레드 안전성을 위한 뮤텍스

};

#endif // CHARTDATAMANAGER_H
