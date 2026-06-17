package webull.trading.bot;

import com.webull.openapi.core.common.dict.ComboType;
import com.webull.openapi.core.common.dict.EntrustType;
import com.webull.openapi.core.common.dict.InstrumentSuperType;
import com.webull.openapi.core.common.dict.OrderSide;
import com.webull.openapi.core.common.dict.OrderTIF;
import com.webull.openapi.core.common.dict.OrderType;
import com.webull.openapi.trade.response.v2.AccountBalanceInfo;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import webull.trading.bot.commands.Trading;
import webull.trading.bot.user.User;

public class Application {
    private static final Logger logger = LoggerFactory.getLogger(Application.class);

    public static void main(String[] args) {
        System.setProperty("org.slf4j.simpleLogger.log.com.webull.openapi", "WARN");

        User    user    = new User("NotPooh555", "secret.json");
        Trading trading = new Trading(
            user.get_id(),
            user.get_api_key(),
            user.get_secret(),
            "us",
            "us-openapi-alb.uat.webullbroker.com"
        );

        // trading.connect_events(user.get_id());
        
        String account_id = user.get_id();
        
        AccountBalanceInfo balance = trading.get_account_balance(account_id);

        if (balance != null) {
            StringBuilder sb = new StringBuilder();
            
            sb.append("\n==================================================\n")
                .append("             COMPLETE ACCOUNT PROFILE             \n")
                .append("==================================================\n")
                .append(" [OVERVIEW]\n")
                .append(String.format(" Total Asset Currency:      %s\n", balance.getTotalAssetCurrency()))
                .append(String.format(" Net Liquidation Value:     %s\n", balance.getTotalNetLiquidationValue()))
                .append(String.format(" Total Cash Balance:        %s\n", balance.getTotalCashBalance()))
                .append(String.format(" Total Market Value:        %s\n", balance.getTotalMarketValue()))
                .append(String.format(" Total Unrealized P&L:      %s\n", balance.getTotalUnrealizedProfitLoss()))
                .append(String.format(" Total Day P&L:             %s\n", balance.getTotalDayProfitLoss()))
                .append(String.format(" Day Trades Left:           %s\n", balance.getDayTradesLeft()))
                .append(String.format(" Maintenance Margin:        %s\n", balance.getMaintenanceMargin()))
                .append(String.format(" Open Margin Calls:         %s\n", balance.getOpenMarginCalls()));

            if (balance.getAccountCurrencyAssets() != null) {
                for (com.webull.openapi.trade.response.v2.AccountAssetInfo asset : balance.getAccountCurrencyAssets()) {
                    sb.append("--------------------------------------------------\n")
                        .append(String.format(" [ASSET DETAILS - %s]\n", asset.getCurrency()))
                        .append(String.format(" Net Liquidation Value:   %s\n", asset.getNetLiquidationValue()))
                        .append(String.format(" Cash Balance:            %s\n", asset.getCashBalance()))
                        .append(String.format(" Market Value:            %s\n", asset.getMarketValue()))
                        .append(String.format(" Settled Cash:            %s\n", asset.getSettledCash()))
                        .append(String.format(" Unsettled Cash:          %s\n", asset.getUnsettledCash()))
                        .append(String.format(" Buying Power:            %s\n", asset.getBuyingPower()))
                        .append(String.format(" Option Buying Power:     %s\n", asset.getOptionBuyingPower()))
                        .append(String.format(" Day Buying Power:        %s\n", asset.getDayBuyingPower()))
                        .append(String.format(" Overnight Buying Power:  %s\n", asset.getOvernightBuyingPower()))
                        .append(String.format(" Night Trading BP:        %s\n", asset.getNightTradingBuyingPower()))
                        .append(String.format(" Unrealized P&L:          %s\n", asset.getUnrealizedProfitLoss()))
                        .append(String.format(" Day P&L:                 %s\n", asset.getDayProfitLoss()))
                        .append(String.format(" Available Withdrawal:    %s\n", asset.getAvailableWithdrawal()))
                        .append(String.format(" Held Amount:             %s\n", asset.getHeldAmount()))
                        .append(String.format(" Frozen Amount:           %s\n", asset.getFrozenAmount()))
                        .append(String.format(" Interests Unpaid:        %s\n", asset.getInterestsUnpaid()));
                }
            }
            sb.append("==================================================");
            
            logger.info(sb.toString());
        }

        String client_order_id = trading.place_order(account_id, 
            ComboType.NORMAL.name(), 
            "AAPL", 
            InstrumentSuperType.EQUITY.name(), 
            "US", 
            OrderType.LIMIT.name(), 
            "1", 
            "180", 
            "CORE", 
            OrderSide.BUY.name(), 
            OrderTIF.DAY.name(), 
            EntrustType.QTY.name()
        );
        
        try {
            logger.info("Waiting for order to transition out of PENDING_SUBMIT...");
            Thread.sleep(2000); 
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        if (client_order_id != null) {
            trading.modify_order(account_id, client_order_id, "179", "2");
            
            try {
                Thread.sleep(2000);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }

            trading.cancel_order(account_id, client_order_id);
        }
    }
}