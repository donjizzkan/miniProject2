#ifndef CANDLEDATA_H
#define CANDLEDATA_H

class CandleData
{
public:
    CandleData();
    CandleData(double openingPrice, double highPrice, double lowPrice, double tradePrice, double timestamp);

    double getOpeningPrice() const { return m_openingPrice; }
    double getHighPrice() const { return m_highPrice; }
    double getLowPrice() const { return m_lowPrice; }
    double getTradePrice() const { return m_tradePrice; }
    double getTimestamp() const { return m_timestamp; }

private:
    double m_openingPrice;
    double m_highPrice;
    double m_lowPrice;
    double m_tradePrice;
    double m_timestamp;
};

#endif // CANDLEDATA_H
