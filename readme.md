# Stock Trading Simulation

This is a C++ program that simulates a stock trading strategy using a moving average approach. The simulation includes mock price updates for stocks and an account that can execute trading strategies based on these prices.

## Features

- **Price Logger**: Simulates real-time stock price updates using a random number generator.
- **Trading Strategy**: Implements a moving average strategy to decide when to buy or sell stocks.
- **Multithreading**: Utilizes C++11 threads for concurrent price updates and trading actions.
- **Trade Tracking**: Records all trades made, including stock symbol, units traded, price, and trade type.

## How It Works

1. **Price Logger**: Each stock (e.g., Apple and Tesla) has a `priceLogger` that updates its price every 5 seconds.
2. **Account**: The `account` class manages the trading strategy, current balance, and recorded trades.
3. **Trading Strategy**:
    - When the stock price crosses above a moving average, it triggers a buy signal.
    - When the stock price crosses below the moving average, it triggers a sell signal.
4. **Final Report**: After stopping the trading strategies, the final balance and profit/loss are displayed along with a list of all trades made.

## Usage

1. **Start the Trading Simulation**:
   - The simulation initializes with a predefined amount of initial investment (e.g., $1000).
   - It starts trading strategies for multiple stocks, such as Apple and Tesla.

2. **Stopping the Trading**:
   - After a specified time (e.g., 20 seconds), the trading strategies for each stock can be stopped.
   - The program displays the final profit/loss and balance after stopping the trading strategies.

3. **Viewing Trades**:
   - The program outputs all trades made during the simulation, showing details such as stock, units, price, and type (BUY/SELL).

## Dependencies

This code requires a C++11 compatible compiler. You can compile and run the code using any standard C++ development environment or command line.

## How to Compile and Run

1. Clone the repository or download the code files.
2. Open a terminal and navigate to the directory containing the files.
3. Compile the code with the following command:

   ```bash
   g++ -std=c++11 -o stock_trading_simulation stock_trading_simulation.cpp -lpthread
