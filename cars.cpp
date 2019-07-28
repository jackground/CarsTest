/**
 * Cars data construction file
 *
 * Copyright (c) 2019, Jack Hawley.
 */

#include <cstdint>
#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#define LOG(x) (std::cout << x << std::endl)

using namespace std;

enum CarKind { FERRARI, LAMBORGHINI, MCLAREN, PORSCHE };

struct AgentInfo {
  int agentId;
  std::map<CarKind, uint8_t> expertise;
  uint8_t serviceTime;  // Service time is in hours
  double rating;
};

struct CustomerInfo {
  time_t arrivalTime;
  CarKind interest;
  bool saleClosed;
};

extern std::map<CarKind, double> PRICES;

std::vector<AgentInfo> agentInfos(uint16_t count);
std::vector<CustomerInfo> customerInfos(uint16_t count);

void printCustomerInfos(const vector<CustomerInfo> &);
void printAgentInfos(const vector<AgentInfo> &);

enum HOURS { OPEN = 8, CLOSE = 17 };

std::map<CarKind, double> PRICES = {{FERRARI, 256550},
                                    {LAMBORGHINI, 203295},
                                    {MCLAREN, 288845},
                                    {PORSCHE, 294250}};

time_t update(time_t time, uint32_t inc) {
  /**
   * Increment time, reset to opening time if inc = 0.
   * - time: Time in seconds since epoch.
   * - inc: Increment in seconds.
   */
  time += inc;
  std::tm *tm = std::localtime(&time);
  if (tm->tm_hour > CLOSE) {
    tm->tm_yday++;
  }
  if (tm->tm_hour > CLOSE || inc == 0) {
    tm->tm_hour = OPEN;
    tm->tm_min = 0;
    tm->tm_sec = 0;
  }

  return std::mktime(tm);
}

std::vector<AgentInfo> agentInfos(uint16_t count) {
  std::vector<AgentInfo> vectorOfAgents;

  for (uint16_t i = 0; i < count; ++i) {
    vectorOfAgents.push_back({std::rand() % 900000 + 100000,
                              []() {
                                std::map<CarKind, uint8_t> expertise;
                                for (auto c : PRICES) {
                                  expertise[c.first] =
                                      std::rand() % PRICES.size();
                                }

                                return expertise;
                              }(),
                              static_cast<uint8_t>(std::rand() % 3 + 3),
                              static_cast<double>(std::rand()) / RAND_MAX});
  }

  return vectorOfAgents;
}

std::vector<CustomerInfo> customerInfos(uint16_t count) {
  std::vector<CustomerInfo> vectorOfCustomers;

  // Start with today, randomly schedule each 2 hours.
  time_t next = update(std::time(nullptr), 0);
  for (uint16_t i = 0; i < count; ++i) {
    vectorOfCustomers.push_back(
        {next = update(next, std::rand() % 2 * 3600),
         static_cast<CarKind>(std::rand() % PRICES.size()),
         std::rand() > 0.7 * RAND_MAX});
  }
  return vectorOfCustomers;
}

int main() {
  std::vector<AgentInfo> vectorOfAgents = agentInfos(5);
  // printAgentInfos(vectorOfAgents);

  /* TEST CODE CUSTOMERS */

  // struct CustomerInfo {
  //   time_t arrivalTime;
  //   CarKind interest;
  //   bool saleClosed;
  // };

  vector<CustomerInfo> customerInfo = customerInfos(20);
  printCustomerInfos(customerInfo);

  return 0;
}

void printCustomerInfos(const vector<CustomerInfo> &customerInfo) {
  for (auto i : customerInfo) {
    static int num = 1;
    cout << "CUSTOMER " << num << ":" << endl;
    cout << "Arrival Time: " << i.arrivalTime << endl;
    cout << "Car Kind: " << i.interest << endl;
    cout << "Closed: " << boolalpha << i.saleClosed << endl;
    cout << endl;
    num += 1;
  }
}

void printAgentInfos(const vector<AgentInfo> &vectorOfAgents) {
  for (auto i : vectorOfAgents) {
    static int num = 1;
    std::cout << "AGENT " << num << ":" << std::endl;
    std::cout << "Agent ID: " << i.agentId << std::endl;
    std::cout << "Agent Expertise:" << std::endl;
    for (auto j : i.expertise) {
      cout << j.first << ": " << unsigned(j.second) << endl;
    }
    std::cout << "Agent Service Time: " << unsigned(i.serviceTime) << std::endl;
    std::cout << "Agent Rating: " << i.rating * 100 << std::endl;
    std::cout << std::endl;
    num++;
  }
}

void printTime(time_t *arrTime) {
  char buff[20];
  time_t ptime = time(arrTime);
  strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&ptime));
}