#include "OrderBook.h"

int main() {
    OrderBook book;
    
    // Test adding orders
    book.addOrder(1, 100, 10, Side::BUY, OrderType::LIMIT);
    book.addOrder(2, 100, 20, Side::BUY, OrderType::LIMIT);
    book.addOrder(3, 99, 15, Side::BUY, OrderType::LIMIT);
    
    book.addOrder(4, 101, 10, Side::SELL, OrderType::LIMIT);
    book.addOrder(5, 102, 30, Side::SELL, OrderType::LIMIT);

    book.printBook();
    return 0;
}