#include <iostream>
#include <vector>
#include <memory>
#include <thread>  // this_thread::sleep_for
#include <chrono>  // chrono::milliseconds
#include <sstream>
#include <algorithm>
using namespace std;
#include <map>
#include "header.h"

void PrintText(const string& text, int milliseconds) {
    for (char ch : text) {
        cout << ch << flush;  // Выводим символ без перехода на новую строку
        this_thread::sleep_for(chrono::milliseconds(milliseconds));  // Задержка
    }
    cout << endl;
}
void CreatePlanets(Galaxy& galaxy, const string& playerName) {
    system("clear");
    cout << endl;
    PrintText("ХОД " + playerName, 20);
    cout << endl;
    string temp;

    PrintText("Введите название планет через пробел: ", 20);
    getline(cin, temp);
    stringstream ss(temp);
    string planetName;
    while (ss >> planetName) {
        shared_ptr<CelestialBody> planet = make_shared<Planet>(planetName);
        galaxy.AddCelestialBody(planet);
    }
}

vector<string> ChooseResources(Galaxy& galaxy, int currentPlayer) {

    PrintText("Выберите для своих планет 3 производимых ресурса и введите через пробел: ", 20);
    cout << "Древесина, Каучук, Еда, Сталь, Нефть, Железо, Медикаменты\n==>> ";
    string temp1;
    getline(cin, temp1);
    stringstream ssResources(temp1);
    vector<string> resources;
    string resource;
    while (ssResources >> resource && resources.size() < 3) {
        resources.push_back(resource);
    }

    if (resources.size() != 3) {
        PrintText("Ошибка! Выберите ровно три ресурса!", 20);
        return {}; 
    }

    auto celestialBodies = galaxy.GetCelestialBodies();
    vector<shared_ptr<Planet>> planets;

    for (auto& body : celestialBodies) {
        auto planet = dynamic_pointer_cast<Planet>(body);
        if (planet) {
            planets.push_back(planet);
        }
    }

    int startIndex = (currentPlayer == 1) ? 0 : 3;

    for (int i = 0; i < 3; ++i) { 
        int planetIndex = startIndex + i;
        auto& planet = planets[planetIndex];

        string& res = resources[i];
        int resourceCount = rand() % 300 + 1;  
        planet->SetProduct(res, resourceCount);

        // Спрос для текущего ресурса и двух следующих по порядку
        for (int j = 1; j <= 2; ++j) {
            int demandIndex = (i + j) % 3; 
            string& nextRes = resources[demandIndex];
            int demandCount = rand() % 1000 + 1; 
            planet->SetDemand(nextRes, demandCount);
        }

        planet->SetTechnologyLevel();
    }

    return resources;
}

void CreateRoutes(Galaxy& galaxy, vector<Route>& routes) {
    int countRoutes;
    PrintText("Введите количество маршрутов (максимально 3): ", 20);
    cin >> countRoutes;
    for (int i = 0; i < countRoutes; i++) {
        string planet1, planet2;
        int cost, securityLevel;
        PrintText("Введите названия двух планет для маршрута (через пробел): ", 20);
        cin >> planet1 >> planet2;
        cost = rand() % 1000 + 1;
        securityLevel = rand() % 50 + 1;
        galaxy.AddRoute(planet1, planet2, cost, securityLevel);
    }
}

void ChooseCorporations(vector<shared_ptr<Corporation>>& corporationsPlayer, Galaxy& galaxy) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    PrintText("Введите цифры видов корпораций через пробел, с которыми Игрок будет сотрудничать (максимально 2)", 20);
    cout << "1 - LogisticsCompany — минимизирует транспортные расходы.\n";
    cout << "2 - TechTrader — торгует только высокотехнологичными ресурсами.\n";
    cout << "3 - MinerCompany — добывает ресурсы на астероидах.\n";

    string temp11;
    getline(cin, temp11);
    stringstream ssCorp(temp11);
    string corporation;
    int count = 0;

    while (getline(ssCorp, corporation, ' ') && count < 2) { 
        int corpType = stoi(corporation);
        shared_ptr<Corporation> corp;

        switch (corpType) {
        case 1:
            corp = make_shared<LogisticsCompany>("LogisticsCompany");
            break;
        case 2:
            corp = make_shared<TechTrader>("TechTrader");
            break;
        case 3:
            corp = make_shared<MinerCompany>("MinerCompany");
            break;
        default:
            cout << "Некорректный ввод: " << corporation << ". Корпорация не добавлена." << endl;
            continue;
        }

        if (corp) {
            corporationsPlayer.push_back(corp);
            count++;
            cout << "Корпорация добавлена: " << corp->getName() << endl;
        }
    }
}

void MakeInvestment(int& money, vector<Route>& routes, Galaxy& galaxy) {
    PrintText("Вы можете инвестировать в улучшение маршрутов или в развитие планет!", 20);
    cout << "1 - Улучшение маршрутов (1456 шекелей за уменьшение на 50 единиц топлива)\n";
    cout << "2 - Развитие планет (778 шекелей за увеличение на 10 единиц для каждого ресурса)\n";
    int investmentChoice;
    cout << "Введите ваш выбор: ";
    cin >> investmentChoice;

    if (investmentChoice == 1) {
        PrintText("Вы улучшили маршруты! Затраты топлива снижены.", 20);
        for (auto& route : routes) {
            route.upgradeRoute();

        }
        money -= 1456;  
    }
    else if (investmentChoice == 2) {
        PrintText("Вы инвестировали в развитие планет!", 20);
        for (auto& body : galaxy.GetCelestialBodies()) {
            shared_ptr<Planet> planet = dynamic_pointer_cast<Planet>(body);
            if (planet) {
                planet->DevelopPlanet(); 
            }
        }
        money -= 778;  
    }
    else {
        PrintText("Неверный выбор. Попробуйте снова.", 20);
    }
}
int CalculatePrice(const string& resource, int demand, int supply) {
    int basePrice = 10;

    if (supply == 0) {
        return basePrice;
    }

    int price = basePrice * demand / supply;
    return max(price, 1);  
}

void TradeBetweenPlanets(vector<Route>& routes, Galaxy& galaxy, int& moneyPlayer, int& fuelPlayer, vector<shared_ptr<Corporation>>& corporations) {
    PrintText("Начало торговли между планетами с корпорациями...", 20);

    for (auto& route : routes) {
        string planet1 = route.planet1;
        string planet2 = route.planet2;

        shared_ptr<Planet> p1 = nullptr;
        shared_ptr<Planet> p2 = nullptr;

        for (auto& body : galaxy.GetCelestialBodies()) {
            shared_ptr<Planet> planet = dynamic_pointer_cast<Planet>(body);
            if (planet && planet->GetName() == planet1) {
                p1 = planet;
            }
            if (planet && planet->GetName() == planet2) {
                p2 = planet;
            }
        }

        if (p1 && p2) {
            for (auto& corp : corporations) {

                if (auto logisticsCompany = dynamic_pointer_cast<LogisticsCompany>(corp)) {

                    route.cost = max(0, route.cost - 5);
                    cout << "Логистическая компания оптимизировала маршрут между " << planet1 << " и " << planet2 << ". Новый расход топлива: " << route.cost << endl;
                }
                else if (auto techTrader = dynamic_pointer_cast<TechTrader>(corp)) {

                    for (auto it = p2->GetDemand().begin(); it != p2->GetDemand().end(); ++it) {
                        auto& resource = it->first;
                        auto& demand = it->second;

                        if (resource == "Сталь" || resource == "Медикаменты" || resource == "Нефть") {
                            auto p1ResourceIt = p1->resources.find(resource);
                            if (p1ResourceIt != p1->resources.end() && p1ResourceIt->second > 0) {
                                int supply = p1ResourceIt->second;
                                int pricePerUnit = CalculatePrice(resource, demand, supply);
                                int tradeAmount = min(demand, supply);
                                int totalPrice = pricePerUnit * tradeAmount;

                                if (moneyPlayer >= totalPrice) {
                                    p1ResourceIt->second -= tradeAmount;
                                    p2->resources[resource] += tradeAmount;
                                    moneyPlayer -= totalPrice;

                                    cout << "TechTrader успешно продал " << tradeAmount << " единиц " << resource << " между " << planet1 << " и " << planet2 << endl;
                                }
                            }
                        }
                    }
                }
                else if (auto minerCompany = dynamic_pointer_cast<MinerCompany>(corp)) {

                    for (auto& body : galaxy.GetCelestialBodies()) {
                        shared_ptr<Asteroid> asteroid = dynamic_pointer_cast<Asteroid>(body);
                        if (asteroid) {
                            if (p1) {
                                minerCompany->MineAsteroid(*asteroid, moneyPlayer, *p1);
                            }
                            else if (p2) {
                                minerCompany->MineAsteroid(*asteroid, moneyPlayer, *p2);
                            }
                        }
                    }
                }

                if (fuelPlayer >= route.cost) {
                    fuelPlayer -= route.cost;
                }
                else {
                    cout << "Недостаточно топлива для маршрута между " << planet1 << " и " << planet2 << "." << endl;
                    continue;
                }

                if (route.securityLevel < 20) {
                    cout << "Уровень безопасности маршрута между " << planet1 << " и " << planet2 << " слишком низкий. Возможны потери ресурсов." << endl;

                    for (auto& resource : p1->resources) {
                        int loss = resource.second * (20 - route.securityLevel) / 100;
                        resource.second = max(0, resource.second - loss);
                        cout << "Потеряно " << loss << " единиц ресурса " << resource.first << " на маршруте." << endl;
                    }
                }
            }
        }

        PrintText("Торговля с корпорациями завершена!", 20);
    }
}
void PrintGameResults(int moneyPlayer1, int moneyPlayer2) {
    PrintText("ИТОГИ ИГРЫ:", 20);
    cout << endl;
    PrintText("Игрок 1: ", 20);
    cout << "Деньги: " << moneyPlayer1 << " шекелей" << endl;
    PrintText("Игрок 2: ", 20);
    cout << "Деньги: " << moneyPlayer2 << " шекелей" << endl;
    if (moneyPlayer1 > moneyPlayer2) {
        PrintText("ПОБЕДИЛ ИГРОК 1!", 20);
    }
    else if (moneyPlayer1 < moneyPlayer2) {
        PrintText("ПОБЕДИЛ ИГРОК 2!", 20);
    }
    else {
        PrintText("НИЧЬЯ! Оба игрока завершили игру с одинаковым количеством топлива.", 20);
    }
}