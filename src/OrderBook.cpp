#include "OrderBook.h"
#include <iostream>

void OrderBook::addOrder(int id, int price, int quantity, Side side, OrderType type) {
    // Create the incoming order
    auto incomingOrder = std::make_shared<Order>(id, price, quantity, side, type);
    
    // MATCHING LOGIC
    if (side == Side::BUY) {
        // While incoming has quantity AND there are sellers...
        while (incomingOrder->quantity > 0 && !asks.empty()) {
            auto bestAskLimit = asks.begin()->second; // Lowest Sell Price
            
            // If we can't afford the best ask, stop matching
            if (incomingOrder->price < bestAskLimit->price) {
                break; 
            }

            // Match against orders in this limit (FIFO)
            auto currentBookOrder = bestAskLimit->head;
            while (incomingOrder->quantity > 0 && currentBookOrder) {
                int matchQuantity = std::min(incomingOrder->quantity, currentBookOrder->quantity);
                
                // EXECUTE TRADE
                // std::cout << ">> TRADE: " << matchQuantity << " @ $" << bestAskLimit->price << "\n";
                
                incomingOrder->quantity -= matchQuantity;
                currentBookOrder->quantity -= matchQuantity;

                // If book order is filled, remove it
                if (currentBookOrder->quantity == 0) {
                    auto nextOrder = currentBookOrder->next; // Save next before deleting
                    bestAskLimit->removeOrder(currentBookOrder);
                    orderMap.erase(currentBookOrder->id);
                    currentBookOrder = nextOrder;
                } else {
                    currentBookOrder = currentBookOrder->next;
                }
            }

            // If the Price Level is empty, remove it from the map
            if (!bestAskLimit->head) {
                asks.erase(asks.begin());
            }
        }
    } 
    else { // SELL SIDE (Mirror of above)
        while (incomingOrder->quantity > 0 && !bids.empty()) {
            auto bestBidLimit = bids.begin()->second; // Highest Buy Price
            
            if (incomingOrder->price > bestBidLimit->price) {
                break;
            }

            auto currentBookOrder = bestBidLimit->head;
            while (incomingOrder->quantity > 0 && currentBookOrder) {
                int matchQuantity = std::min(incomingOrder->quantity, currentBookOrder->quantity);
                
                std::cout << ">> TRADE: " << matchQuantity << " @ $" << bestBidLimit->price << "\n";
                
                incomingOrder->quantity -= matchQuantity;
                currentBookOrder->quantity -= matchQuantity;

                if (currentBookOrder->quantity == 0) {
                    auto nextOrder = currentBookOrder->next;
                    bestBidLimit->removeOrder(currentBookOrder);
                    orderMap.erase(currentBookOrder->id);
                    currentBookOrder = nextOrder;
                } else {
                    currentBookOrder = currentBookOrder->next;
                }
            }
            if (!bestBidLimit->head) {
                bids.erase(bids.begin());
            }
        }
    }

    // IF quantity remains, ADD to Book (Resting Order)
    if (incomingOrder->quantity > 0) {
        orderMap[id] = incomingOrder;
        
        if (side == Side::BUY) {
            if (bids.find(price) == bids.end()) {
                bids[price] = std::make_shared<Limit>();
                bids[price]->price = price;
            }
            bids[price]->addOrder(incomingOrder);
        } else {
            if (asks.find(price) == asks.end()) {
                asks[price] = std::make_shared<Limit>();
                asks[price]->price = price;
            }
            asks[price]->addOrder(incomingOrder);
        }
    }
}

void OrderBook::cancelOrder(int id) {
    if (orderMap.find(id) == orderMap.end()) return;
    
    auto order = orderMap[id];
    orderMap.erase(id); // Remove from map
    
    // Remove from Limit List
    if (order->side == Side::BUY) {
        if (bids.find(order->price) != bids.end()) {
            auto limit = bids[order->price];
            limit->removeOrder(order);
            if (!limit->head) bids.erase(order->price); // Cleanup empty limit
        }
    } else {
        if (asks.find(order->price) != asks.end()) {
            auto limit = asks[order->price];
            limit->removeOrder(order);
            if (!limit->head) asks.erase(order->price);
        }
    }
}

void OrderBook::printBook() {
    std::cout << "\n--- ORDER BOOK ---\n";
    std::cout << "ASKS:\n";
    for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
        std::cout << "  $" << it->first << " | Vol: " << it->second->totalVolume << "\n";
    }
    std::cout << "BIDS:\n";
    for (auto& [price, limit] : bids) {
        std::cout << "  $" << price << " | Vol: " << limit->totalVolume << "\n";
    }
    std::cout << "------------------\n\n";
}