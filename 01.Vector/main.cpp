#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <windows.h>

using namespace std;

/*
==========================================================================================================================================
                                                  STL. Контейнер vector
==========================================================================================================================================
                                                              
                                                            STL
  STL (Standard Template Library) — частина стандартної бібліотеки C++: набір готових шаблонних класів і функцій
  для зберігання та обробки даних.
  Мета STL — не писати щоразу власні масиви, списки, сортування і пошук, а брати перевірені, швидкі й однакові для всіх.

  Чотири основні поняття STL:
      КОНТЕЙНЕР — шаблонний клас, що зберігає набір елементів і сам керує їхньою пам'яттю (vector, list, map …);
      ІТЕРАТОР  — об'єкт, що вказує на елемент контейнера і вміє переходити до наступного;
      АЛГОРИТМ  — шаблонна функція, що обробляє елементи через ітератори (sort, find, count …);
      ФУНКТОР   — об'єкт, який можна викликати як функцію; задає алгоритму, ЩО саме робити з елементами.

  Типи контейнерів:
      послідовні     — елементи йдуть у порядку додавання:             vector, list, deque
      асоціативні    — елементи впорядковані за ключем:                map, multimap, set, multiset
      невпорядковані — швидкий пошук за ключем без сортування:         unordered_map, unordered_set
      адаптери       — обмежений інтерфейс поверх іншого контейнера:   stack, queue, priority_queue

                                                          VECTOR
  vector<T> — динамічний масив, який сам збільшується. Елементи лежать у пам'яті підряд, як у звичайному масиві.
  Пам'ять виділяє і звільняє сам vector: new, delete[] і окремий лічильник кількості більше не потрібні.

  ----------------------------------------------------------------------------------------------------------------------------------------

  Основні методи vector<T>:
      push_back(x)        додати в кінець                  pop_back()        видалити останній
      size()   empty()    кількість / чи порожній          clear()           видалити всі
      v[i]                доступ без перевірки межі        v.at(i)           з перевіркою: кидає out_of_range
      front()  back()     перший / останній елемент

  vector можна ініціалізувати списком:  vector<int> scores = { 10, 7, 12 };

  Цикл по діапазону (range-based for) — короткий запис обходу всіх елементів контейнера:
      for (const Student& s : group)   — читаємо;     for (Student& s : group)   — змінюємо.

  Передача vector у функцію або метод:
      const vector<T>&    лише ЧИТАЄМО вектор — без копіювання;
      vector<T> + move    ЗАБИРАЄМО вектор собі: move переносить усю пам'ять разом з елементами за одну дію,
                          без копіювання кожного елемента; вихідний вектор після цього порожній.
  vector можна повертати з функції: його вміст переноситься, а не копіюється.

  Додавання елементів і семантика перенесення:
      push_back(x)          x — звичайна змінна: у вектор потрапляє КОПІЯ, x залишається як був;
      push_back(move(x))    x більше не потрібен: його вміст ПЕРЕНОСИТЬСЯ у вектор;
      push_back(T(...))     тимчасовий об'єкт переноситься автоматично;
      emplace_back(...)     об'єкт створюється одразу всередині вектора з аргументів конструктора.
*/

class Student
{
private:
    string name;
    int age;
    double average;

public:
    Student(const string& name, int age, double average) : name(name), age(age), average(average)
    {}

    string getName() const { return name; }
    int getAge() const { return age; }
    double getAverage() const { return average; }

    void setAverage(double average) { this->average = average; }

    void print() const
    {
        cout << name << ", " << age << " років, середній бал " << average << endl;
    }
};

class Group
{
private:
    string name;
    vector<Student> students;                    // КОМПОЗИЦІЯ: група володіє своїми студентами

public:
    // Вектор за значенням + move: група ЗАБИРАЄ весь список собі без копіювання студентів
    Group(const string& name, vector<Student> students) : name(name), students(move(students))
    {}

    // Вектор за const-посиланням: група лише ЧИТАЄ чужий список і копіює з нього студентів до себе
    void enroll(const vector<Student>& newcomers)
    {
        for (const Student& student : newcomers)
        {
            students.push_back(student);         // student — змінна з іменем: у вектор потрапляє копія
        }
    }

    void addStudent(const string& studentName, int age, double average)
    {
        students.emplace_back(studentName, age, average);   // студент створюється одразу у векторі
    }

    void removeLast()
    {
        if (!students.empty())
        {
            cout << "Вибув: " << students.back().getName() << endl;
            students.pop_back();
        }
    }

    void giveBonus(double bonus)
    {
        for (Student& student : students)        // без const і з & — змінюємо самих студентів
        {
            student.setAverage(student.getAverage() + bonus);
        }
    }

    double average() const
    {
        if (students.empty())
        {
            return 0;
        }
        double sum = 0;
        for (const Student& student : students)
        {
            sum += student.getAverage();
        }
        return sum / static_cast<double>(students.size());
    }

    const Student& at(size_t index) const
    {
        return students.at(index);               // некоректний index — виняток out_of_range
    }

    void print() const
    {
        cout << "Група " << name << ", студентів: " << students.size() << endl;
        for (size_t i = 0; i < students.size(); i++)    // size() повертає size_t — беззнаковий тип
        {
            cout << "  " << i + 1 << ". ";
            students[i].print();
        }
    }
};

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    vector<Student> applicants = {               // список зарахованих на курс
        Student("Олена Коваль", 20, 11.5),
        Student("Ігор Мельник", 35, 9.8),
        Student("Марія Бондар", 27, 10.2)
    };

    Group group("ПВ-221", move(applicants));     // група забирає весь список собі
    cout << "У списку зарахованих залишилось: " << applicants.size() << endl;   // 0
    group.print();

    cout << "--- Переведення з іншої групи ---" << endl;
    vector<Student> transfer = { Student("Андрій Ткач", 42, 8.7) };
    group.enroll(transfer);                      // transfer лише читаємо
    cout << "У списку переведених залишилось: " << transfer.size() << endl;     // 1
    group.addStudent("Софія Лисенко", 19, 11.9);
    group.print();

    cout << "--- Бонус за олімпіаду ---" << endl;
    group.giveBonus(0.5);
    cout << "Середній бал групи: " << group.average() << endl;

    cout << "--- Остання студентка перевелася на вечірню форму ---" << endl;
    group.removeLast();
    group.print();

    cout << "--- Доступ з перевіркою ---" << endl;
    try
    {
        group.at(1).print();
        group.at(10).print();                    // такого номера немає — виняток
    }
    catch (const out_of_range&)                  // ім'я змінної не потрібне: e.what() не використовуємо
    {
        cout << "Помилка: немає студента з таким номером" << endl;
    }

    return 0;
}

/*
==========================================================================================================================================
                                                  ІНСАЙТИ / ЛАЙФХАКИ / BEST PRACTICES
==========================================================================================================================================

  1. vector — контейнер за замовчуванням. Якщо немає особливої причини брати інший, беріть vector:
     завдяки розміщенню елементів підряд він найшвидший для обходу.

  2. Найчастіша помилка в циклі по діапазону — забути &:  for (Student s : students)  копіює КОЖНОГО студента,
     і зміни потрапляють у копію, а не у вектор.

  3. Як vector росте: коли місце закінчується, він виділяє більшу ділянку пам'яті, переносить туди елементи
     і звільняє стару. size() — скільки елементів є, capacity() — під скільки вже виділено пам'ять.
     Якщо кількість відома заздалегідь, викличте reserve(n), і переїздів не буде.

  4. Порівняйте з динамічним масивом: у Group немає ні new, ні delete[], ні поля count, ні деструктора, ні конструктора
     копіювання — vector робить усе це сам. Клас, який складається з таких «розумних» полів, правильно копіюється,
     переноситься і знищується без жодного рядка власного коду.

  5. Після move вектор гарантовано порожній, тому applicants.size() у прикладі безпечно повертає 0.
     Але загальне правило — після move об'єкт більше не використовують (хіба що присвоюють йому нове значення):
     для багатьох класів його стан після перенесення не визначений.

  6. Вектор може зберігати розумні покажчики:  vector<unique_ptr<IWeapon>> — так у реальних проєктах
     зберігають об'єкти різних класів однієї ієрархії. Пам'ять звільниться сама, коли знищиться вектор.

==========================================================================================================================================
                                                              ПРАКТИКА
==========================================================================================================================================

  Завдання. Рюкзак героя.
  Клас Item уже готовий — скопіюйте його у свій файл:

      class Item
      {
      private:
          string name;
          int price;

      public:
          Item(const string& name, int price) : name(name), price(price)
          {}

          string getName() const { return name; }
          int getPrice() const { return price; }

          void setPrice(int price) { this->price = price; }

          void print() const
          {
              cout << name << " — " << price << " золота" << endl;
          }
      };

    1. Напишіть клас Backpack (за зразком Group):
        поле vector<Item> items;
        конструктор Backpack(vector<Item> items) — забирає список собі через move;
        метод void addItem(const string& name, int price) — додає предмет через emplace_back;
        метод int totalPrice() const — загальна вартість (як average у Group);
        метод void applyDiscount() — зменшує ціну кожного предмета вдвічі (як giveBonus);
        метод const Item& at(size_t index) const — через items.at(index);
        метод void print() const — виводить усі предмети і рядок «Разом: 520 золота».
    2. У main:
        - створіть vector<Item> loot: «Зілля лікування» 50, «Сталевий меч» 300, «Карта підземелля» 20;
        - створіть Backpack з цього списку через move і виведіть, скільки елементів залишилось у loot;
        - додайте «Щит» 150 через addItem і виведіть рюкзак;
        - торговець оголосив знижку: викличте applyDiscount() і виведіть рюкзак ще раз;
        - у try спробуйте вивести предмет at(7); у catch (const out_of_range&) виведіть «У рюкзаку немає такої комірки».
*/
