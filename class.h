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

class CelestialBody {
protected:
    string name;
public:
    CelestialBody(string name) {
        this->name = name;
    }
    virtual void getInfo() {
        cout << endl << "Название: " << name << endl;
    }
    string GetName() {
        return name;
    }
};
class Planet : public CelestialBody {
public:
    int coefficient;
    map <string, int> resources;
    map <string, int> demand;
    string technologyLevel;

    Planet(string& name) : CelestialBody(name), technologyLevel("Неопределённый") {}
    void SetTechnologyLevel() {
        for (auto& res : resources) {
            if (res.first == "Древесина" || res.first == "Еда") {
                technologyLevel = "Недоразвитая";
                break;
            }
            else if (res.first == "Каучук" || res.first == "Сталь" || res.first == "Железо") {
                technologyLevel = "Развивающаяся";
            }
            else {
                technologyLevel = "Развитая";

            }
        }
    }
    void SetProduct(string& name, int& count) {
        resources[name] = count;
    }
    void SetDemand(string& name, int& count) {
        demand[name] = count;
    }
    void GenerateResources() {
        for (auto element : resources) {
            element.second *= coefficient;
        }
    }
    void DevelopPlanet() {
        for (auto& res : resources) {
            res.second += 50; 
        }
    }
    void getInfo() {
        cout << endl << "---------------PLANET INFORMATION---------------" << endl;
        cout << "Название: " << name;
        cout << endl << "Технологический уровень: " << technologyLevel;
        cout << endl << "### Ресурсы ###" << endl;
        for (auto& element : resources) {
            cout << element.first << " : " << element.second << " единиц | ";
        }
        cout << endl << "### Cпрос ###" << endl;
        for (auto& element : demand) {
            cout << element.first << " : " << element.second << " единиц | ";
            cout << endl;
        }
        cout << endl;
    }
    const map<string, int>& GetResources() {
        return resources;
    }
    map<string, int>& GetDemand() {
        return demand;
    }
};
class Asteroid : CelestialBody {
private:
    int priceMining;
    string nameResource;
    int countResource;

public:
    Asteroid(string& name, int& priceMining, int& countResources, int& cost, string& nameResource) : CelestialBody(name), countResource(cost), nameResource(nameResource) {
        priceMining = cost;
    }

};

class Corporation {
protected:
    string name;
public:
    Corporation(string name) {
        this->name = name;
    }
    virtual ~Corporation() = default;
    string getName() {
        return name;
    }

};
class LogisticsCompany : public Corporation {
public:
    LogisticsCompany(string name) : Corporation(name) {}

};

class TechTrader : public Corporation {
public:
    TechTrader(string name) : Corporation(name) {}

};

class MinerCompany : public Corporation {
public:
    MinerCompany(string name) : Corporation(name) {}

    void MineAsteroid(Asteroid& asteroid, int& moneyPlayer, Planet& nearestPlanet) {
        int miningCost = 1000;  
        int minedResources = 100; 
        vector<string> possibleResources = { "Сталь", "Железо" };

        // Рандомный выбор ресурса
        string resourceName = possibleResources[rand() % possibleResources.size()];

        if (moneyPlayer >= miningCost) {
            moneyPlayer -= miningCost;
            nearestPlanet.resources[resourceName] += minedResources;
            cout << "MinerCompany добыла " << minedResources << " единиц ресурса \"" << resourceName
                << "\" с астероида и доставила на планету "
                << nearestPlanet.GetName() << "." << endl;
        }
        else {
            cout << "Недостаточно средств для добычи ресурса \"" << resourceName << "\" с астероида !!!" << endl;
        }
    }
};

class Galaxy {
private:
    string name = "Галактика Андромеды";
    vector <shared_ptr<CelestialBody>> celectialBody; //shared ptr умный указатель объект класса
    map < pair<string, string>, pair<int, int> > routes;
public:
    void AddCelestialBody(shared_ptr<CelestialBody>& object) {
        celectialBody.push_back(object);
    }
    void AddRoute(string& planet1, string& planet2, int cost, int securityLevel) {
        routes[{planet1, planet2}] = { cost, securityLevel };
    }
    void getInfo() {
        cout << endl << "---------------GALAXY INFORMATION---------------" << endl;
        cout << "Название: " << name;
        for (auto& element : celectialBody) {
            element->getInfo();
        }
        cout << endl << "Маршруты:" << endl;

        for (auto it = routes.begin(); it != routes.end(); it++) {
            auto& planets = it->first;
            auto& route = it->second;
            cout << endl << planets.first << " <-> " << planets.second << " стоит " << route.first << " с уровнем безопасности " << route.second << endl;
        }
    }
    const vector<shared_ptr<CelestialBody>>& GetCelestialBodies() const {
        return celectialBody;
    }
};

class Route {
public:
    string planet1;
    string planet2;
    int cost;
    int securityLevel;

    Route(const string& p1, const string& p2, int c, int s)
        : planet1(p1), planet2(p2), cost(c), securityLevel(s) {}

    void upgradeRoute() {
        cost -= 50; 
    }
};
