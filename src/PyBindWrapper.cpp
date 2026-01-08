#include <pybind11/pybind11.h>
#include "../include/OrderBook.h"

namespace py = pybind11;

PYBIND11_MODULE(lob_python, m) {
    m.doc() = "LOBMatcher: A high-frequency limit order book engine";

    // Bind the Enums
    py::enum_<Side>(m, "Side")
        .value("BUY", Side::BUY)
        .value("SELL", Side::SELL)
        .export_values();

    py::enum_<OrderType>(m, "OrderType")
        .value("LIMIT", OrderType::LIMIT)
        .value("MARKET", OrderType::MARKET)
        .export_values();

    // Bind the OrderBook Class
    py::class_<OrderBook>(m, "OrderBook")
        .def(py::init<>()) // Constructor
        .def("add_order", &OrderBook::addOrder, 
             "Add an order to the book",
             py::arg("id"), py::arg("price"), py::arg("quantity"), py::arg("side"), py::arg("type"))
        .def("cancel_order", &OrderBook::cancelOrder, "Cancel an order by ID")
        .def("print_book", &OrderBook::printBook, "Print the current book state to console");
}