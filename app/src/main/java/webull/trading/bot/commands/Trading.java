package webull.trading.bot.commands;

import com.webull.openapi.core.http.HttpApiConfig;
import com.webull.openapi.trade.TradeClientV2;
import com.webull.openapi.core.utils.GUID;
import com.webull.openapi.trade.events.subscribe.*;
import com.webull.openapi.trade.events.subscribe.message.*;
import com.webull.openapi.trade.request.v2.TradeOrder;
import com.webull.openapi.trade.request.v2.TradeOrderItem;
import com.webull.openapi.trade.response.v2.AccountBalanceInfo;
import com.webull.openapi.trade.response.v2.TradeOrderResponse;

import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Trading {
    private static final Logger logger = LoggerFactory.getLogger(Trading.class);
    
    private final HttpApiConfig     config;
    private final ITradeEventClient event_client;
    private final TradeClientV2     client;

    public Trading(
        String account_id,
        String api_key,
        String secret,
        String region,
        String endpoint) {
        this.config = HttpApiConfig.builder()
            .appKey(api_key)
            .appSecret(secret)
            .regionId(region)
            .endpoint(endpoint)
            .build();
        this.event_client = ITradeEventClient.builder()
            .appKey(api_key)
            .appSecret(secret)
            .regionId(region)
            .onMessage(response->
                logger.info(
                    "[Streaming] Order update: {}",
                    response.getPayload()
                )
            )
            .build();

        try {
            this.client = new TradeClientV2(this.config);
        } catch (Exception e) {
            throw new IllegalStateException("Failed to initialize Webull client. Check endpoint, app key, secret, and account permissions.", e);
        }

        logger.info("[Trading] Trading command manager initialized");
    }

    public String place_order(
        String account_id, 
        String combo_type, 
        String symbol, 
        String instrument_type, 
        String market, 
        String order_type, 
        String quantity, 
        String limit_price, 
        String support_trading_session, 
        String side, 
        String time_in_force, 
        String entrust_type) {
        final String order_id = GUID.get();

        TradeOrder           trade_order = new TradeOrder();
        List<TradeOrderItem> orders_list = new ArrayList<>();
        TradeOrderItem       single_order = new TradeOrderItem();

        single_order.setClientOrderId(order_id);
        single_order.setComboType(combo_type);
        single_order.setSymbol(symbol);
        single_order.setInstrumentType(instrument_type);
        single_order.setMarket(market);
        single_order.setOrderType(order_type);
        single_order.setQuantity(quantity);
        single_order.setLimitPrice(limit_price);
        single_order.setSupportTradingSession(support_trading_session);
        single_order.setSide(side);
        single_order.setTimeInForce(time_in_force);
        single_order.setEntrustType(entrust_type);
        
        orders_list.add(single_order);

        trade_order.setNewOrders(orders_list);

        try {
            TradeOrderResponse response = this.client.placeOrder(account_id, trade_order);
            
            logger.info(
                "\n========================================\n" +
                "             ORDER PLACED              \n" +
                "========================================\n" +
                "Order ID:        {}\n" +
                "Client Order ID: {}\n" +
                "Combo Order ID:  {}\n" +
                "========================================",
                response.getOrderId(),
                response.getClientOrderId(),
                response.getComboOrderId() != null ? response.getComboOrderId() : "N/A"
        );   

            return order_id;
        } catch (Exception e) {
            logger.error("[Trading] Place order failed", e);
            return null;
        } 
    }   

    public void modify_order(
        String account_id, 
        String client_order_id, 
        String new_limit_price, 
        String new_quantity) {
        TradeOrder           replace_order   = new TradeOrder();
        List<TradeOrderItem> modified_orders = new ArrayList<>();
        TradeOrderItem       modified_item   = new TradeOrderItem();
        
        modified_item.setClientOrderId(client_order_id);
        modified_item.setLimitPrice(new_limit_price);
        modified_item.setQuantity(new_quantity);
        modified_orders.add(modified_item);

        replace_order.setModifyOrders(modified_orders);

        try {
            TradeOrderResponse response = this.client.replaceOrder(account_id, replace_order);
            
            logger.info(
                "\n========================================\n" +
                "            ORDER MODIFIED              \n" +
                "========================================\n" +
                "Order ID:        {}\n" +
                "Client Order ID: {}\n" +
                "========================================",
                response.getOrderId(),
                response.getClientOrderId()
            );
        } catch (Exception e) {
            logger.error("[Trading] Modify order failed", e);
        } 
    }

    public void cancel_order(
        String account_id, 
        String client_order_id) {
        TradeOrder cancel_order = new TradeOrder();
        
        cancel_order.setClientOrderId(client_order_id);

        try {
            TradeOrderResponse response = this.client.cancelOrder(account_id, cancel_order);
            
            logger.info(
                "\n========================================\n" +
                "            ORDER CANCELLED             \n" +
                "========================================\n" +
                "Order ID:        {}\n" +
                "Client Order ID: {}\n" +
                "========================================",
                response.getOrderId(),
                response.getClientOrderId()
            );
        } catch (Exception e) {
            logger.error("[Trading] Cancel order failed", e);
        } 
    }

    public void connect_events(String account_id) {
        try {
            SubscribeRequest request      = new SubscribeRequest(account_id);
            ISubscription    subscription = this.event_client.subscribe(request);

            subscription.blockingAwait();

            logger.info("[Trading] Event stream connected");
        } catch (Exception e) {
            logger.warn("[Trading] Event stream unavailable: {}", e.getMessage());
        }
    }

    public AccountBalanceInfo get_account_balance(String account_id) {
        try {
            AccountBalanceInfo balance = this.client.balanceAccount(account_id);           
            return balance;
        } catch (Exception e) {
            logger.error("[Trading] Failed to fetch complete account balance", e);
            return null;
        }
    }
}