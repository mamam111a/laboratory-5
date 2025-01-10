#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <thread>  // this_thread::sleep_for
#include <chrono>  // chrono::milliseconds
#include <sstream>
#include <algorithm>
using namespace std;
#include <map>
#include "class.h"
void PrintText(const string& text, int milliseconds);
void CreatePlanets(Galaxy& galaxy, const string& playerName);
vector<string> ChooseResources(Galaxy& galaxy, int currentPlayer);
void CreateRoutes(Galaxy& galaxy, vector<Route>& routes);
void ChooseCorporations(vector<shared_ptr<Corporation>>& corporationsPlayer, Galaxy& galaxy);
void MakeInvestment(int& money, vector<Route>& routes, Galaxy& galaxy);
int CalculatePrice(const string& resource, int demand, int supply);
void TradeBetweenPlanets(vector<Route>& routes, Galaxy& galaxy, int& moneyPlayer, int& fuelPlayer, vector<shared_ptr<Corporation>>& corporations);
void PrintGameResults(int moneyPlayer1, int moneyPlayer2);

