#ifndef LINEDATA_H
#define LINEDATA_H

class LineData
{
public:
    LineData();
    LineData(double tradePrice, double timestamp);
    double getTradePrice() const { return m_tradePrice; }
    double getTimestamp() const { return m_timestamp; }

private:
    double m_tradePrice;
    double m_timestamp;
};

#endif // LINEDATA_H
