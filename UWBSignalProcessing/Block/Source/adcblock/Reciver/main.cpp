#include "network.h"
#include "vector"
#include <iostream>

using namespace std;

struct DataSend {
  uint8_t data1;
  uint16_t data2;

} __attribute__((packed));

int main() {
  UtilityNetwork::ReciverUDP reciverUDP(8000);
  reciverUDP.AddCallback([&]() {
    std::string answerStr = reciverUDP.GetData();
    cout << answerStr << endl;
    reciverUDP.SendAnswer(std::string("Hi"));
  });
  reciverUDP.Start();
  this_thread::sleep_for(chrono::minutes(1));
  reciverUDP.Stop();
  return 0;
}
