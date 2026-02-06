# Limit Order Book & Matching Engine (C++)

A **C++ implementation of a Limit Order Book and Matching Engine**, inspired by real-world trading systems used in stock and crypto exchanges.

This project focuses on **core backend logic**, efficient **data structures**, and **correct order-matching behavior**, rather than UI.

---

## 📌 Overview

The Limit Order Book maintains BUY and SELL orders at different price levels and matches them based on **price priority** and **FIFO (time priority)**.

The system supports:
- Limit orders
- Market orders
- Partial and full fills
- Order cancellation
- Stress testing with large volumes of orders

---

## 🚀 Features

- ✅ BUY and SELL **Limit Orders**
- ✅ **Market Order** execution
- ✅ **Price-Time Priority (FIFO)** matching
- ✅ **Partial & Full Order Matching**
- ✅ **Order Cancellation** using Order ID
- ✅ Automatic cleanup of filled orders
- ✅ Randomized **order book simulation**
- ✅ **Order book integrity validation**

---

## 🧠 How the Matching Engine Works

### BUY Orders
- Matched against the **lowest available SELL price**
- Trade executes if:  
  `BUY price ≥ SELL price`
- Orders at the same price are matched using FIFO

### SELL Orders
- Matched against the **highest available BUY price**
- Trade executes if:  
  `SELL price ≤ BUY price`
- Supports partial fills

---

## 🏗️ Data Structures Used

| Purpose | Data Structure |
|------|---------------|
| BUY order book (highest price first) | `map<int64_t, list<order>, greater<>>` |
| SELL order book (lowest price first) | `map<int64_t, list<order>>` |
| FIFO order handling | `list<order>` |
| Fast cancellation lookup | `unordered_map<int, pair<price, side>>` |

These choices ensure **efficient order lookup, insertion, and deletion**.

---

## 🧪 Simulation & Validation

The program runs a stress simulation with **100,000+ random operations**, including:
- New BUY / SELL orders
- Market orders
- Order cancellations

After simulation, the order book is validated to ensure:
- No orphan orders
- Consistent cancellation mapping
- Correct price–side relationships

---

## ▶️ Build & Run

### Compile
```bash
g++ main.cpp -o main
---

##Project Structure
limit_order_book/
│
├── main.cpp        # Limit order book & matching engine
├── README.md       # Project documentation
└── .gitignore      # Ignores binaries and object files
