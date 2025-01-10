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

int main() {
    setlocale(LC_ALL, "rus");
    Galaxy galaxy;

    PrintText("ДОБРО ПОЖАЛОВАТЬ В ИГРУ <ИМПЕРИЯ ЗВЕЗД>!!!", 20);
    cout << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    PrintText("ПРАВИЛА ОЧЕНЬ ПРОСТЫ: СОЗДАЙТЕ ПЛАНЕТЫ, МАРШРУТЫ МЕЖДУ НИМИ, ГАЛАКТИЧЕСКИЕ КОРПОРАЦИИ, А ЗАТЕМ ЗАРАБАТЫВАЙТЕ НА ТОРГОВЛЕ!", 20);
    cout << endl;
    PrintText("ПОБЕДИТ ИГРОК, КОТОРЫЙ СМОГ ДОСТИЧЬ МАКСИМАЛЬНОЙ ПРИБЫЛИ.", 20);
    cout << endl << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    PrintText("УДАЧИ!!!", 20);

    cout << endl << endl << "Нажмите клавишу Enter для продолжения...";
    cin.ignore();

    system("clear");
    PrintText("Количество игроков для игры: 2", 20);
    PrintText("Количество раундов: 3", 20);
    cout << endl << endl;
    PrintText("Каждому игроку в начале игры выдается 10000 шекелей, 600 единиц топлива и 3 планеты, каждой из которых нужно выбрать главный производимый ресурс (Одна планета принадлежит Игроку);", 20);
    PrintText("Создать маршруты между этими планетами, которые будут рандомно затрачивать топливо для пути и формировать уровень безопасности;", 20);
    PrintText("Также необходимо выбрать два вида корпорации, с которыми игрок будет сотрудничать.", 20);


    cout << endl << endl << "Нажмите клавишу Enter для продолжения...";
    cin.ignore();

    int countPlayers = 2;
    int countRounds = 3;
    int moneyPlayer1 = 10000;
    int moneyPlayer2 = 10000;
    int fuelPlayer1 = 600;
    int fuelPlayer2 = 600;

    vector<shared_ptr<Corporation>> corporationsPlayer1;
    vector<shared_ptr<Corporation>> corporationsPlayer2;

    CreatePlanets(galaxy, "Игрок 1");
    vector<string> resources1 = ChooseResources(galaxy, 1);
    if (resources1.empty()) return 0;  
    vector<Route> routes1; 
    system("clear");
    galaxy.getInfo();
    cout << endl;
    PrintText("Далее Вам будет необходимо построить маршруты между планетами, поэтому выбирайте с умом!", 20);
    cout << endl << endl << "Нажмите клавишу Enter для продолжения...";
    cin.ignore();
    cout << endl;
    CreateRoutes(galaxy, routes1); 
    vector<int> CorporationPlayer1;
    ChooseCorporations(corporationsPlayer1, galaxy);
    galaxy.getInfo();

    cout << endl << endl << "Нажмите клавишу Enter для продолжения...";
    cin.ignore();

    CreatePlanets(galaxy, "Игрок 2");
    vector<string> resources2 = ChooseResources(galaxy, 2);
    if (resources2.empty()) return 0;
    vector<Route> routes2; 
    system("clear");
    galaxy.getInfo();
    cout << endl;
    PrintText("Далее Вам будет необходимо построить маршруты между планетами, поэтому выбирайте с умом!", 20);
    cout << endl << endl << "Нажмите клавишу Enter для продолжения...";
    cin.ignore();
    cout << endl;
    CreateRoutes(galaxy, routes2); 
    vector<int> CorporationPlayer2;
    ChooseCorporations(corporationsPlayer2, galaxy);
    galaxy.getInfo();

    cout << endl << endl << "Нажмите клавишу Enter для продолжения...";
    cin.ignore();

    system("clear");
    PrintText("Галактика обновлена!", 1);
    this_thread::sleep_for(chrono::milliseconds(2000));


    // Основной цикл игры
    for (int round = 1; round <= countRounds; round++) {
        system("clear");
        PrintText("РАУНД: ", 20);
        cout << round << endl;
        PrintText("ХОД ИГРОКА 1 ", 20);
        TradeBetweenPlanets(routes1, galaxy, moneyPlayer1, fuelPlayer1, corporationsPlayer1);
        MakeInvestment(moneyPlayer1, routes1, galaxy); 
        cout << endl << endl << "Нажмите клавишу Enter для продолжения...";
        cin.ignore();

        system("clear");
        PrintText("ХОД ИГРОКА 2 ", 20);
        TradeBetweenPlanets(routes2, galaxy, moneyPlayer2, fuelPlayer2, corporationsPlayer2);
        MakeInvestment(moneyPlayer2, routes2, galaxy); 

        this_thread::sleep_for(chrono::milliseconds(1000));
        cout << endl;
        PrintText("Раунд завершён...", 20);
        this_thread::sleep_for(chrono::milliseconds(1000));
        PrintText("Вывод информации о планете...", 20);
        this_thread::sleep_for(chrono::milliseconds(1000));
        galaxy.getInfo();
        cout << endl;
        PrintText("Количество шекелей у Игрока 1: ", 20);
        cout << moneyPlayer1;
        cout << endl;
        PrintText("Количество шекелей у Игрока 2: ", 20);
        cout << moneyPlayer2;
        string temp;
        cout << endl << endl << "Введите любую клавишу для продолжения...";
        cin >> temp;
    }
    PrintGameResults(moneyPlayer1, moneyPlayer2);
    return 0;
}