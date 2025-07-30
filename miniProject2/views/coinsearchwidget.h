#ifndef COINSEARCHWIDGET_H
#define COINSEARCHWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableWidgetItem>

struct CoinData {
    QString symbol;
    QString koreanName;
    QString englishName;
    QString iconPath;
};

class CoinSearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CoinSearchWidget(QWidget *parent = nullptr);
    
    void updateSearchText(const QString& text);

private:
    QTableWidget* resultTableWidget;
    QList<CoinData> coinDataList;

    void setupUI();
    void connectSignal();
    void initializeCoinData();
    void addCoinToTable(const CoinData& coin);

signals:
    void coinSelected(const QString& symbol, const QString& koreanName, const QString& englishName);

private slots:
    void onSearchTextChanged(const QString& text);
    void filterResults(const QString& searchText);
    void onCoinSelected(QTableWidgetItem* item);
};

#endif // COINSEARCHWIDGET_H
