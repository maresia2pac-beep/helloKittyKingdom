#include <iostream>
#include "windows.h"
#include "GameClasses.h"
#include "BaseClasses.h"
#include "Evil.h"
#include <fstream> 
#include <stdexcept>
#include <ctime>

using namespace std;

// === ВСПОМОГАТЕЛЬНЫЕ СТРУКТУРЫ ===
enum class ValueQuality
{
    МУСОР, ОБЫЧНОЕ, РЕДКОЕ, МИФИЧЕСКОЕ, ЛЕГЕНДАРНОЕ 
};

enum class CharacterType {
    UNKNOWN = 0, Lyagushka, Kitty, Kuromi
};

struct Treasure
{
    Treasure() = default;
    string name{ "уровень сладости" };
    ValueQuality quality = ValueQuality::МИФИЧЕСКОЕ;
    unsigned int price{ 0 };
    Treasure(ValueQuality quality)
    {
        switch (quality)
        {
        case ValueQuality::МУСОР: cout << "горькое\n"; break;
        case ValueQuality::ОБЫЧНОЕ: cout << "несладкое\n"; break;
        case ValueQuality::РЕДКОЕ: cout << "сладенькое\n"; break;
        case ValueQuality::МИФИЧЕСКОЕ: cout << "вкусняшка\n"; break;
        case ValueQuality::ЛЕГЕНДАРНОЕ: cout << "конфетка\n"; break;
        default: break;
        }
    }
}; 

struct Сloth : Treasure 
{
    Сloth(ValueQuality quality) : Treasure(quality) {};
    string valueSite[5]{ "бантик", "заколочка", "сумочка", "шарфик", "пояс" };
    string site{NULL};
    unsigned short armor{ 1 };
};

enum class EnemyType {
    БЛУМ = 1, КАНЕКИ_КЕН, СОВУНЬЯ, SHADOW_FIEND, ДЖОКЕРВИКТОР_ДУДКА,
};

// === РЕАЛИЗАЦИЯ МЕХАНИК ===
void TakeDamage(Npc& npc, float damage)
{
    // Т.к. это friend функция, мы можем менять protected поле health напрямую
    if (npc.health > damage) {
        npc.health -= (unsigned int)damage;
    } else {
        npc.health = 0;
    }
}

unsigned short TestChoise(unsigned short maxChoise, string text)
{
    unsigned short choise = 1;
    cin >> choise;
    while (choise > maxChoise || choise < 1)
    {
        cout << text << endl;
        cin >> choise;
    }
    return choise;
};

unique_ptr<Npc> CreateCharacter(CharacterType type)
{
    switch (type)
    {
    case CharacterType::UNKNOWN: return make_unique<Npc>();
    case CharacterType::Lyagushka: return make_unique<Lyagushka>();
    case CharacterType::Kitty:  return make_unique<Kitty>();
    case CharacterType::Kuromi: return make_unique<Kuromi>();
    default: throw invalid_argument("Неизвестный тип персонажа"); 
    }
    return nullptr; 
}

// === ЛОГИКА БИТВЫ ===
void StartBattle(Player* playerWrapper)
{
    Npc* player = playerWrapper->GetCharacter();
    if (!player) return;

    // Генерируем случайного врага
    int enemyType = rand() % 5; 
    string enemyName;
    int hp = 20 + (rand() % 30); // 20-50 хп
    int dmg = 5 + (rand() % 10); // 5-15 урон

    switch(enemyType) {
        case 0: enemyName = "Блум"; hp = 10; dmg = 2; break;
        case 1: enemyName = "Канеки Кен"; hp = 5; dmg = 20; break;
        case 2: enemyName = "Совунья"; hp = 150; dmg = 14; break;
        case 3: enemyName = "Shadow Fiend"; hp = 30; dmg = 5; break;
        case 4: enemyName = "Джокер-Виктор Дудка"; hp = 100; dmg = 1; break;
    }

    Evil enemy(enemyName, hp, dmg);
    
    cout << "На пути появился загадочный силуэт...\n";
    enemy.GetInfo();
    cout << "...\n";

    while (true)
    {
        cout << "\nКак защитить королевство?:о\n1 - Атаковать\n2 - Сбежать\n";
        unsigned short action = TestChoise(2, "Сражайся или беги!");

        if (action == 1)
        {
            // 1. Игрок бьет
            float pDmg = player->GetDamage();
            cout << "-> Вы наносите " << pDmg << " урона по " << enemy.GetName() << "!\n";
            TakeDamage(enemy, pDmg);

            if (enemy.GetHealth() <= 0) {
                cout << "\n*** ПОБЕДА! " << enemy.GetName() << " убежал в страхе! ***\n";
                break;
            }

            // 2. Враг бьет
            float eDmg = enemy.GetDamage();
            cout << "<- " << enemy.GetName() << " атакует вас на " << eDmg << " урона!\n";
            TakeDamage(*player, eDmg); // Разыменовываем указатель, чтобы передать ссылку

            cout << "Ваше настроение: " << player->GetHealth() << "\n";

            if (player->GetHealth() <= 0) {
                cout << "\n*** ВЫ НЕ СМОГЛИ ЗАЩИТИТЬ КОРОЛЕВСТВО СЛАДОСТЕЙ >:(((( ***\n";
                break;
            }
        }
        else if (action == 2)
        {
            if (rand() % 2 == 0) {
                cout << "Вы трусливо избежали драки, никто этого не видел... с:\n";
                break;
            } else {
                cout << "Вы убегаете, но " << enemy.GetName() << " использовал запретную силу. Вы не успели увернуться.";
                float eDmg = enemy.GetDamage();
                TakeDamage(*player, eDmg);
                cout << "Ваше настроение: " << player->GetHealth() << "\n";
                if (player->GetHealth() <= 0) break;
            }
        }
    }
}

// === MAIN ===
int main()
{
    setlocale(LC_ALL, "Rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(0)); // Инициализация генератора случайных чисел

    Player* player = new Player();

    cout << "Королевству сладостей нужна помощь Hello Kitty и её друзей.\nНам нужно знать, сможешь ли ты победить злых врагов!!!\nТы впервые тут?\n\t1 - новый персонаж,\n\t2 - загрузить\n";
    if (TestChoise(2, "Наверное ты ошибся, повтори снова") == 1) // Если 1 - новый
    {
        cout << "По одёжке встречают, по уму провожают. Кто ты?\n\t1 - Лягушка\n\t2 - Китти\n\t3 - Куроми\n";
        unique_ptr<Npc> character;
        switch (TestChoise(3, "Ты, по-моему, перепутал."))
        {
            case 1: character = CreateCharacter(CharacterType::Lyagushka); break;
            case 2: character = CreateCharacter(CharacterType::Kitty); break;
            case 3: character = CreateCharacter(CharacterType::Kuromi); break;
        }
        player->Create(move(character));
    }
    else
    {
        // Загрузка
        ifstream loadSystem("save.bin", ios::binary); // Тут было .txt, но в классах у тебя .bin
        if (!loadSystem.is_open())
        {
            cout << "Связь с космосом потеряна. Создаем нового персонажа по умолчанию (Воин).\n";
            player->Create(CreateCharacter(CharacterType::Lyagushka));
        }
        else
        {
            // Упрощенная загрузка для примера. В идеале нужно знать класс перед загрузкой.
            // Пока просто создадим воина и попробуем загрузить в него.
            unique_ptr<Npc> tempChar = make_unique<Lyagushka>(); 
            loadSystem.close(); // Player->Load сам открывает файл, поэтому закроем тут
            if (player->Load(move(tempChar))) {
                cout << "Персонаж загружен!\n";
                player->GetCharacter()->GetInfo();
            }
        }
    }

    // --- ОСНОВНОЙ ЦИКЛ ИГРЫ ---
    bool gameRunning = true;
    while (gameRunning && player->GetCharacter() != nullptr && player->GetCharacter()->GetHealth() > 0)
    {
        cout << "\n----------------------------------\n";
        cout << "\nДля победы нужно испортить настроение врагу\n";
        cout << "Вы в безопасной зоне. Что будем делать?\n";
        cout << "\t1 - Защищать королевство\n";
        cout << "\t2 - Посмотреть статистику\n";
        cout << "\t3 - Сохранить и выйти\n";
        
        unsigned short choice = TestChoise(3, "Нет такого варианта");

        switch (choice)
        {
        case 1:
            StartBattle(player);
            break;
        case 2:
            player->GetCharacter()->GetInfo();
            break;
        case 3:
            if (player->Save()) {
                cout << "Игра сохранена!\n";
            } else {
                cout << "Ошибка сохранения.\n";
            }
            gameRunning = false;
            break;
        }

        if (player->GetCharacter()->GetHealth() <= 0) {
            cout << "\nДостойная смерть — начало бессмертия в легенде.\n";
            gameRunning = false;
        }
    }

    delete player;
    return 0;
}


//всех samurai shaman paladin самурай шамаш шиноби во всех файлах на лягушка китти и куроми
