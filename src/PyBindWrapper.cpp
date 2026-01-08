#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // VITAL: Allows C++ vector to become Python list
#include "../include/OrderBook.h"

namespace py = pybind11;

PYBIND11_MODULE(lob_python, m) {
    m.doc() = "LOBMatcher: A high-frequency limit order book engine";

    py::enum_<Side>(m, "Side")
        .value("BUY", Side::BUY)
        .value("SELL", Side::SELL)
        .export_values();

    py::enum_<OrderType>(m, "OrderType")
        .value("LIMIT", OrderType::LIMIT)
        .value("MARKET", OrderType::MARKET)
        .export_values();

    py::class_<OrderBook>(m, "OrderBook")
        .def(py::init<>())
        .def("add_order", &OrderBook::addOrder)
        .def("cancel_order", &OrderBook::cancelOrder)
        .def("print_book", &OrderBook::printBook)
        // --- NEW GETTERS ---
        .def("get_bids", &OrderBook::getBids, "Get top 10 bids as list of [price, vol]")
        .def("get_asks", &OrderBook::getAsks, "Get top 10 asks as list of [price, vol]");
}