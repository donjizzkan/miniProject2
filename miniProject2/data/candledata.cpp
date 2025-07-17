#include "candledata.h"

CandleData::CandleData() {}
CandleData::CandleData(
    double openingPrice,
    double highPrice,
    double lowPrice,
    double tradePrice,
    double timestamp) :
    m_openingPrice(openingPrice),
    m_highPrice(highPrice),
    m_lowPrice(lowPrice),
    m_tradePrice(tradePrice),
    m_timestamp(timestamp){

}
