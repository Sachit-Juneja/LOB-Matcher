import unittest
import lob_python as lob

class TestOrderBook(unittest.TestCase):
    def setUp(self):
        self.book = lob.OrderBook()

    def test_add_order(self):
        # Test that we can add an order without crashing
        self.book.add_order(1, 100, 10, lob.Side.BUY, lob.OrderType.LIMIT)
        bids = self.book.get_bids()
        self.assertEqual(len(bids), 1)
        self.assertEqual(bids[0], [100, 10])

    def test_execution(self):
        # Test that a crossing order executes trade and reduces volume
        self.book.add_order(1, 100, 10, lob.Side.SELL, lob.OrderType.LIMIT)
        self.book.add_order(2, 100, 5, lob.Side.BUY, lob.OrderType.LIMIT) # Should eat 5
        
        asks = self.book.get_asks()
        self.assertEqual(asks[0][1], 5) # 10 - 5 = 5 remaining

if __name__ == '__main__':
    unittest.main()