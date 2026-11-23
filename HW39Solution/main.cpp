#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <windows.h>

using namespace std;

/*
==========================================================================================================================================
                                  ДОМАШНЄ ЗАВДАННЯ 39 тема: контейнер vector. Ітератори
==========================================================================================================================================
*/

class Card
{
private:
    string name;
    int cost;      // вартість (мана)
    int power;     // сила

public:
    Card(const string& name, int cost, int power) : name(name), cost(cost), power(power)
    {}

    string getName() const { return name; }
    int getCost() const { return cost; }
    int getPower() const { return power; }

    void print() const
    {
        cout << name << " (" << cost << "/" << power << ")" << endl;
    }
};

class Deck
{
private:

    //  2. Чому верх колоди — це КІНЕЦЬ вектора, а не початок? Що відбувається з іншими елементами вектора під час вставки на початок?
    vector<Card> cards;                          // верх колоди — кінець вектора

public:
    //  1. Чому конструктор Deck приймає vector за значенням і робить move? Що зміниться, якщо в main передати starter без move?
    explicit Deck(vector<Card> cards) : cards(move(cards))
    {}

    size_t size() const
    {
        return cards.size();
    }

    Card draw()
    {
        if (cards.empty())
        {
            throw out_of_range("Колода порожня");
        }
        Card top = cards.back();                 // запам'ятовуємо верхню карту
        cards.pop_back();                        // і прибираємо її з колоди
        return top;
    }

    void putOnBottom(const Card& card)
    {
        cards.insert(cards.begin(), card);       // усі інші карти зсуваються на одну позицію
    }

    void insertAt(size_t position, const Card& card)
    {
        if (position > cards.size())
        {
            throw out_of_range("Немає такої позиції");
        }
        cards.insert(cards.begin() + position, card);
    }

    //  3. Чому в removeExpensive не можна видаляти елементи в звичайному циклі for з ++it? Що повертає erase і навіщо нам це значення ?
    int removeExpensive(int maxCost)
    {
        int removed = 0;
        auto it = cards.begin();
        while (it != cards.end())
        {
            if (it->getCost() > maxCost)
            {
                cout << "Вилучено: " << it->getName() << " (" << it->getCost() << ")" << endl;
                it = cards.erase(it);            // erase повертає ітератор на наступну карту
                removed++;
            }
            else
            {
                ++it;
            }
        }
        return removed;
    }

    //  4. Чим end() відрізняється від останнього елемента? Чому для виводу від верху колоди зручні rbegin() і rend()?
    void print() const
    {
        cout << "Колода, карт: " << cards.size() << " (від верху до низу):" << endl;
        for (auto r = cards.rbegin(); r != cards.rend(); ++r)
        {
            cout << "  ";
            r->print();
        }
    }
};

void printHand(const vector<Card>& hand)
{
    cout << "Рука, карт: " << hand.size() << endl;
    for (const Card& card : hand)
    {
        cout << "  ";
        card.print();
    }
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // 1
    vector<Card> starter = {
        Card("Гоблін", 1, 2),
        Card("Дракон", 8, 9),
        Card("Лучник", 2, 3),
        Card("Велетень", 7, 8),
        Card("Лицар", 4, 5),
        Card("Маг", 3, 4)
    };
    //  1. Чому конструктор Deck приймає vector за значенням і робить move? Що зміниться, якщо в main передати starter без move?
    Deck deck(move(starter));
    cout << "У starter залишилось: " << starter.size() << endl;     // 0
    deck.print();

    // 2
    vector<Card> hand;
    hand.push_back(deck.draw());
    hand.push_back(deck.draw());
    printHand(hand);
    deck.print();

    // 3
    deck.putOnBottom(Card("Фенікс", 5, 4));
    deck.insertAt(2, Card("Капкан", 2, 0));
    deck.print();

    // 4
    int removed = deck.removeExpensive(5);
    cout << "Вилучено карт: " << removed << endl;
    deck.print();

    // 5
    while (deck.size() > 0)
    {
        hand.push_back(deck.draw());
    }
    printHand(hand);

    // 6
    try
    {
        deck.draw();
    }
    catch (const out_of_range& e)
    {
        cout << "Помилка: " << e.what() << endl;
    }

    try
    {
        deck.insertAt(10, Card("Привид", 3, 3));
    }
    catch (const out_of_range& e)
    {
        cout << "Помилка: " << e.what() << endl;
    }

    return 0;
}

/*

  1. Чому конструктор Deck приймає vector за значенням і робить move? Що зміниться, якщо в main передати starter без move?
  2. Чому верх колоди — це КІНЕЦЬ вектора, а не початок? Що відбувається з іншими елементами вектора під час
     вставки на початок?
  3. Чому в removeExpensive не можна видаляти елементи в звичайному циклі for з ++it? Що повертає erase
     і навіщо нам це значення?
  4. Чим end() відрізняється від останнього елемента? Чому для виводу від верху колоди зручні rbegin() і rend()?

  ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

  1. 

  2. 

  3. 

  4. 

*/
