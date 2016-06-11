/**
 * @brief This module implements simulator of exchange
 * @file MatcherApi.cpp
 *
 */


#include "matcher/Matcher.hpp"
#include "orderbook/OrderBook.hpp"
#include "client/Client.hpp"
#include "client/Trader.hpp"
#include "utils/Constants.hpp"
#include "utils/Exception.hpp"
#include "utils/Json.hpp"


namespace pyxchange
{

namespace py = boost::python;


/**
 * @brief FIXME
 *
 */
void Matcher::handleCreateOrder( const TraderPtr& trader, const boost::python::dict& decoded )
{
    trader->matcher->orderbook->createOrder( trader, decoded );
}


/**
 * @brief FIXME
 *
 */
void Matcher::handleCancelOrder( const TraderPtr& trader, const boost::python::dict& decoded )
{
    trader->matcher->orderbook->cancelOrder( trader, decoded );
}


/**
 * @brief FIXME
 *
 */
void Matcher::handleCancelAll( const TraderPtr& trader )
{
    trader->matcher->orderbook->cancelAllOrders( trader );
}


/**
 * @brief FIXME
 *
 */
ClientPtr Matcher::makeClient( const MatcherPtr& matcher, const std::string& name, const py::object& transport )
{
    const ClientPtr& client = std::make_shared<Client>( matcher,
        ( boost::format( format::f1::client ) % name ).str(), transport );

    matcher->clients->insert( client );
    matcher->orderbook->aggregateAllPriceLevels( client );

    return client;
}


/**
 * @brief FIXME
 *
 */
void Matcher::removeClient( const ClientPtr& client )
{
    const auto& it = findClient( client );

    clients->erase( it );

    logger.info( boost::format( format::f1::logRemoveClient ) % client->toString() );
}


} /* namespace pyxchange */


/* EOF */

