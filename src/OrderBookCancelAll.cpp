/**
 * @brief This module implements simulator of exchange
 * @file OrderBook.cpp
 *
 */


#include "OrderBook.hpp"
#include "Matcher.hpp"
#include "Trader.hpp"


namespace pyxchange
{


namespace py = boost::python;


namespace format
{

const boost::format traderCanceledAll( "%|| cancelled all %|| bid orders, %|| ask orders" );

} /* namespace message */


/**
 * @brief FIXME
 *
 */
void OrderBook::cancelAllOrders( const MatcherConstPtr& matcher, const TraderPtr& trader )
{
    const size_t numBid = cancelAllOrders<BidOrderContainer>( bidOrders, matcher, trader, side::bid_ );
    const size_t numAsk = cancelAllOrders<AskOrderContainer>( askOrders, matcher, trader, side::ask_ );

    if( numBid || numAsk )
    {
        logger.info( boost::format( format::traderCanceledAll ) % trader->getName() % numBid % numAsk );
    }
}


/**
 * @brief FIXME
 *
 */
template<typename OrderContainer>
size_t OrderBook::cancelAllOrders( typename OrderContainer::type& orders, const MatcherConstPtr& matcher,
                                   const TraderPtr& trader, const side_t side_ )
{
    typename OrderContainer::price_set priceLevels;

          auto &idx = orders.template get<tags::idxTrader>();
          auto it   = idx.lower_bound( trader );
    const auto end  = idx.upper_bound( trader );

    size_t n = 0;

    while( it != end )
    {
        const auto& order = *it;

        priceLevels.insert( order->price );

        idx.erase( it++ );

        ++n;
    }

    aggregateAllPriceLevels<OrderContainer>( orders, priceLevels, side_ );

    return n;
}


template size_t OrderBook::cancelAllOrders<BidOrderContainer>(
    BidOrderContainer::type& orders, const MatcherConstPtr& matcher,
    const TraderPtr& trader, const side_t side_ );


template size_t OrderBook::cancelAllOrders<AskOrderContainer>(
    AskOrderContainer::type& orders, const MatcherConstPtr& matcher,
    const TraderPtr& trader, const side_t side_ );


} /* namespace pyxchange */



/* EOF */

