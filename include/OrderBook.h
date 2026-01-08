#pragma once
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>

enum class Side { BUY, SELL };
enum class OrderType { MARKET, LIMIT };

struct Order {
    int id;
    int price;
    int quantity;
    Side side;
    OrderType type;

    // Pointers for the Doubly Linked List (O(1) deletion)
    std::shared_ptr<Order> next = nullptr;
    std::weak_ptr<Order> prev; // Weak pointer prevents cyclic reference

    Order(int _id, int _price, int _quantity, Side _side, OrderType _type)
        : id(_id), price(_price), quantity(_quantity), side(_side), type(_type) {}
};

// Represents a specific price level (e.g., all Buy orders at $100)
struct Limit {
    int price;
    int totalVolume = 0; // Total quantity at this price level
    
    std::shared_ptr<Order> head = nullptr;
    std::shared_ptr<Order> tail = nullptr;

    ~Limit() {
        // Prevent recursive destruction stack overflow
        while (head) {
            head = head->next;
        }
    }
    
    void addOrder(std::shared_ptr<Order> order) {
        order->prev.reset(); // Clear previous pointer
        order->next = nullptr;

        if (!head) { // Empty list
            head = order;
            tail = order;
        } else { // Add to end
            tail->next = order;
            order->prev = tail;
            tail = order;
        }
        totalVolume += order->quantity;
    }

    void removeOrder(std::shared_ptr<Order> order) {
        totalVolume -= order->quantity;
        
        if (head == order && tail == order) { // Only one item
            head = nullptr;
            tail = nullptr;
        } else if (head == order) { // Remove head
            head = order->next;
            if(head) head->prev.reset();
        } else if (tail == order) { // Remove tail
            tail = order->prev.lock();
            if(tail) tail->next = nullptr;
        } else { // Remove from middle
            auto p = order->prev.lock();
            if(p) p->next = order->next;
            if(order->next) order->next->prev = p;
        }
        
        // Break pointers to ensure shared_ptr cleanup
        order->next = nullptr;
        order->prev.reset();
    }
};

class OrderBook {
private:
    // Bids: Sorted High -> Low (Best bid is highest price)
    // std::greater<int> makes the map sort in descending order
    std::map<int, std::shared_ptr<Limit>, std::greater<int>> bids;

    // Asks: Sorted Low -> High (Best ask is lowest price)
    std::map<int, std::shared_ptr<Limit>> asks;

    // Fast lookup for cancellations: OrderID -> Order Pointer
    std::unordered_map<int, std::shared_ptr<Order>> orderMap;

public:
    void addOrder(int id, int price, int quantity, Side side, OrderType type);
    void cancelOrder(int id);
    void match(); // The core matching engine logic
    
    // Helper to visualize book
    void printBook();
};