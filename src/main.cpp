#include "OrderBook.h"
#include <iostream>

int main() {
    OrderBook book;

    std::cout << "1. Adding Liquidity (Passive Orders)...\n";
    book.addOrder(1, 100, 10, Side::SELL, OrderType::LIMIT);
    book.addOrder(2, 101, 20, Side::SELL, OrderType::LIMIT);
    book.addOrder(3, 99, 15, Side::BUY, OrderType::LIMIT);
    book.printBook();

    std::cout << "2. Executing Trade (Aggressive Buy at 100)...\n";
    // Should match Order #1 (10 units @ 100)
    book.addOrder(4, 100, 5, Side::BUY, OrderType::LIMIT); 
    book.printBook();

    std::cout << "3. Sweeping the Book (Large Buy at 102)...\n";
    // Should eat remaining 5 units of Order #1, then 20 units of Order #2
    book.addOrder(5, 102, 30, Side::BUY, OrderType::LIMIT);
    book.printBook();

    return 0;
}