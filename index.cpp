#include "account.h"

int main() {
    priceLogger apple("apple");  
    priceLogger tesla("tesla");  
    account raghav(1000);     
    raghav.startTradingStrategy(apple, "apple");
    raghav.startTradingStrategy(tesla, "tesla");
    std::this_thread::sleep_for(std::chrono::seconds(20));
    raghav.stopTradingStrategy("apple");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    raghav.stopTradingStrategy("tesla");
    std::cout << "Final Profit/Loss: " << raghav.getProfitLoss() << "\n";
    std::cout << "Final Balance: " << raghav.getCurrentBalance() << "\n";
    for (Trade trade : raghav.getTrades()) {
        std::cout << "stock: " << trade.stock << " "
                  << "units " << trade.stockUnits << " "
                  << "price " << trade.stockPrice << " "
                  << "type " << trade.type << "\n";
    }
    return 0;
}
