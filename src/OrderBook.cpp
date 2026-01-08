#include "OrderBook.h"
#include <iostream>

void OrderBook::addOrder(int id, int price, int quantity, Side side, OrderType type) {
    // 1. Create the order
    auto order = std::make_shared<Order>(id, price, quantity, side, type);
    
    // 2. Add to fast lookup map
    orderMap[id] = order;

    // 3. Logic to determine if we Match or Add to Book (We will expand this later)
    // For now, let's just add it to the book structure to test storage.
    
    if (side == Side::BUY) {
        if (bids.find(price) == bids.end()) {
            bids[price] = std::make_shared<Limit>();
            bids[price]->price = price;
        }
        bids[price]->addOrder(order);
    } else {
        if (asks.find(price) == asks.end()) {
            asks[price] = std::make_shared<Limit>();
            asks[price]->price = price;
        }
        asks[price]->addOrder(order);
    }
}

void OrderBook::cancelOrder(int id) {
    // To be implemented: Remove from Linked List and Map
}

void OrderBook::printBook() {
    std::cout << "--- ASKS ---\n";
    for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
        std::cout << "$" << it->first << " | Vol: " << it->second->totalVolume << "\n";
    }
    std::cout << "--- BIDS ---\n";
    for (auto& [price, limit] : bids) {
        std::cout << "$" << price << " | Vol: " << limit->totalVolume << "\n";
    }
}