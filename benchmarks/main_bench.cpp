#include <benchmark/benchmark.h>
#include "../include/OrderBook.h"

// Benchmark 1: Measure adding passive orders (No matching)
static void BM_AddPassiveOrder(benchmark::State& state) {
    OrderBook book;
    int id = 1;
    
    for (auto _ : state) {
        // We pause timing to construct the object (setup overhead)
        state.PauseTiming();
        int price = 100 + (id % 10);
        state.ResumeTiming();

        // The Code We Are Measuring
        book.addOrder(id++, price, 100, Side::BUY, OrderType::LIMIT);
    }
}

// Benchmark 2: Measure Matching (Aggressive Orders)
static void BM_MatchOrder(benchmark::State& state) {
    OrderBook book;
    // Setup: Fill book with 1000 sells
    for(int i = 0; i < 1000; ++i) {
        book.addOrder(i, 100, 10, Side::SELL, OrderType::LIMIT);
    }

    int id = 2000;
    for (auto _ : state) {
        // Measure adding a BUY that instantly matches
        book.addOrder(id++, 100, 10, Side::BUY, OrderType::LIMIT);
    }
}

BENCHMARK(BM_AddPassiveOrder);
BENCHMARK(BM_MatchOrder);

BENCHMARK_MAIN();