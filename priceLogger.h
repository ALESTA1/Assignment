#include <bits/stdc++.h>
#include <thread>
#include <random>


class priceLogger {
public:
    priceLogger(std::string stock_) : stock(stock_), stopFlag(false) {
        updatePrice();
        workerThread = std::thread(&priceLogger::priceUpdater, this);
    }

    ~priceLogger() {
        stopFlag = true;
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }

    double getCurrentPrice() {
        std::lock_guard<std::mutex> lock(priceMutex);
        return currentPrice;
    }

    std::vector<double> getRecentPrices() {
        std::lock_guard<std::mutex> lock(priceMutex);
        return prices;
    }

private:
    std::atomic<bool> stopFlag; 
    double currentPrice;
    std::string stock;
    std::vector<double> prices;
    std::thread workerThread;
    std::mutex priceMutex;

    // This function serves as a mock API call to get the price of this stock
    void updatePrice() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 100.0);
        double random_number = dis(gen);

        std::lock_guard<std::mutex> lock(priceMutex); 
        currentPrice = random_number;
        prices.push_back(random_number);
        if (prices.size() > 50) { 
            prices.erase(prices.begin()); 
        }
    }

    void priceUpdater() {
        while (!stopFlag) {
            updatePrice();
            std::this_thread::sleep_for(std::chrono::seconds(5)); // Update every 5 seconds
        }
    }
};