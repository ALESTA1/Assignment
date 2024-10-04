#include "priceLogger.h"

struct Trade {
    std::string stock;
    double stockUnits;
    double stockPrice;
    std::string type;  
};

// Assumed one-time initial investment
class account {
public:
    account(double initialInvestment_) : initialInvestment(initialInvestment_) {
        currentBalance = initialInvestment;
    }

    ~account() {
        stopTrading(); 
    }

    double getCurrentBalance() { 
        std::lock_guard<std::mutex> lock(accountMutex);
        return currentBalance; 
    }

    double getProfitLoss() { 
        std::lock_guard<std::mutex> lock(accountMutex);
        return currentBalance - initialInvestment; 
    }

    void startTradingStrategy(priceLogger& logger, const std::string& stock) {
        std::lock_guard<std::mutex> lock(accountMutex);
        if (strategyThreads.find(stock) != strategyThreads.end()) {
            std::cout << "Trading strategy for " << stock << " is already running.\n";
            return;
        }
        stopFlags[stock] = false;
        strategyThreads[stock] = std::thread(&account::tradingStrategy, this, std::ref(logger), stock);
    }

    void stopTradingStrategy(const std::string& stock) {
        std::lock_guard<std::mutex> lock(accountMutex);
        if (stopFlags.find(stock) != stopFlags.end()) {
            stopFlags[stock] = true;  
            if (strategyThreads[stock].joinable()) {
                strategyThreads[stock].join();  
                strategyThreads.erase(stock);    
                stopFlags.erase(stock);         
                std::cout << "Stopped trading strategy for " << stock << ".\n";
            }
        }
    }

    void stopTrading() {
        std::lock_guard<std::mutex> lock(accountMutex);
        for (auto& [stock, stopFlag] : stopFlags) {
            stopFlag = true;  
        }
        for (auto& [stock, thread] : strategyThreads) {
            if (thread.joinable()) {
                thread.join();  
            }
        }
    }

    std::vector<Trade> getTrades() { return tradesMade; }

private:
    double currentBalance;
    double initialInvestment;
    std::vector<Trade> tradesMade;
    std::map<std::string, double> positions;  
    std::map<std::string, std::thread> strategyThreads;  
    std::map<std::string, bool> stopFlags;  
    std::mutex accountMutex;

    void tradingStrategy(priceLogger& logger, const std::string& stock) {
        double investThreshold = 50.0;  
        double sellThreshold = 70.0;    
        double stockUnitsToTrade = 10;  

        while (!stopFlags[stock]) {
            double currentPrice = logger.getCurrentPrice();
            std::vector<double> recentPrices = logger.getRecentPrices();
            
            double movingAverage = calculateMovingAverage(recentPrices);

            if (currentPrice < movingAverage * 0.95) { 
                invest(stock, stockUnitsToTrade, currentPrice);  
            } else if (currentPrice > movingAverage * 1.05 && positions[stock] >= stockUnitsToTrade) { 
                sell(stock, stockUnitsToTrade, currentPrice);  
            }
            std::this_thread::sleep_for(std::chrono::seconds(5));  
        }
    }

    double calculateMovingAverage(const std::vector<double>& prices) {
        if (prices.empty()) return 0.0;
        double sum = std::accumulate(prices.begin(), prices.end(), 0.0);
        return sum / prices.size();
    }

    void invest(const std::string& stock, double stockUnits, double stockPrice) {
        std::lock_guard<std::mutex> lock(accountMutex);
        double totalCost = stockUnits * stockPrice;
        if (totalCost > currentBalance) {
            std::cout << "Insufficient funds to buy " << stockUnits << " units of " << stock << "\n";
            return;
        }
        tradesMade.push_back({stock, stockUnits, stockPrice, "BUY"});
        positions[stock] += stockUnits;  
        currentBalance -= totalCost; 
    }

    void sell(const std::string& stock, double stockUnits, double stockPrice) {
        std::lock_guard<std::mutex> lock(accountMutex);
        if (positions[stock] < stockUnits) {
            std::cout << "Insufficient stock holdings to sell " << stockUnits << " units of " << stock << "\n";
            return;
        }
        tradesMade.push_back({stock, stockUnits, stockPrice, "SELL"});
        positions[stock] -= stockUnits;  
        currentBalance += stockUnits * stockPrice;  
    }
};
