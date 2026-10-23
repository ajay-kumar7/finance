#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib> 
#include <ctime>
#include <chrono> 
enum OrderType {BUY,SELL};
struct Order{
    int id;
    OrderType type;
    double price;
    int quantity;
};

class OrderBook{
private:
    std::map<double,std::vector<Order>,std::greater<double>>bids; //higher bbids first
    std::map<double,std::vector<Order>> asks;

public:
    void addOrder(int id, OrderType type, double price, int quantity){
        Order obj;
        obj.id=id;
        obj.type=type;
        obj.price=price;
        obj.quantity=quantity;
        if (obj.type==BUY) bids[price].push_back(obj);
        else asks[price].push_back(obj);
        // std::cout <<"Order added: "<<id<<"\n";
    }

    void match(){
        while (!bids.empty()&&!asks.empty()){
            auto bestBid=bids.begin();
            auto bestAsk=asks.begin();
            // buyer >seller
            if(bestBid->first>=bestAsk->first){
                Order &buyorder=bestBid->second.front();
                Order &sellorder =bestAsk->second.front();
                int tradeQty =std::min(buyorder.quantity, sellorder.quantity);
                // std::cout<<"TRADE!"<<tradeQty<<"shares at "<<bestAsk->first<<"\n";
                buyorder.quantity-=tradeQty;
                sellorder.quantity-=tradeQty;
                if (buyorder.quantity== 0)bestBid->second.erase(bestBid->second.begin());
                if (sellorder.quantity==0) bestAsk->second.erase(bestAsk->second.begin());
                if (bestBid->second.empty())  bids.erase(bestBid);
                if (bestAsk->second.empty())asks.erase(bestAsk);
            } 
            else{
                break; 
            }
        }
    }
    void printBook(){
        std::cout << "\n-------- order book\n";
        //std::cout<<"sellers(ask)\n";
        if(asks.empty()){
            //std::cout<<"(no asks)\n";
        }else{
            for(auto it =asks.rbegin();it!=asks.rend();it++){
                double price = it->first;
                for(const auto& order :it->second){
                    std::cout<<"[ask] $"<<price<<" x "<<order.quantity<<" (id: "<<order.id<<")\n";
                }
            }
        }
        std::cout <<"-------- bids (buyers) \n";
        if(bids.empty()){
            // std::cout << "(no bids)\n";
        }else{
            for(auto it=bids.begin();it!= bids.end();it++) {
                double price = it->first;
                for (const auto& order : it->second) {
                    // std::cout<<"  [bid]$"<<price<<" x "<<order.quantity<<"(ID:"<<order.id<<")\n";
                }
            }
        }
        std::cout << "---------\n";
    }
};
int main() {
    OrderBook book;
    int n_orders=100000;
    //timer start
    auto start=std::chrono::high_resolution_clock::now();
    std::srand(0);// Fixed seed so results are consistent
    for(int i=0; i<n_orders; ++i) {
        OrderType type=(std::rand()%2==0)?BUY:SELL;
        double price=100.0+(std::rand()%5);
        int qty=1+(std::rand() % 100);
        
        book.addOrder(i, type, price, qty);
        book.match();
    }
    // 4. STOP THE TIMER
    auto end=std::chrono::high_resolution_clock::now();
    // 5. Calculate Duration
    // We convert the time difference into Milliseconds (ms)
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    std::cout << "------------------------------------------------\n";
    std::cout << "Time Taken:   "<< duration.count()<< " ms\n";   
    // Calculate Speed (Orders per Second)
    double seconds=duration.count() / 1000.0;
    double speed =n_orders/seconds;
    std::cout << "Throughput:  " << (long)speed << "orders/second\n";
    std::cout << "------------------------------------------------\n";
    return 0;
}
