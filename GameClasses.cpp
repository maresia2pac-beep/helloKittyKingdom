#pragma once
#include "GameClasses.h"

Lyagushka::Lyagushka()
{
	name = "Лягушка";
	health = 35;
	damage = 10;
};

Lyagushka::Lyagushka(std::string name, unsigned int health, float damage)
{
	cout << "кастомный конструктор лягушки" << endl;
	this->name = name;
	this->health = health;
	this->damage = damage;
};

void Lyagushka::GetWeapons()
{
	cout << name << " начинает плеваться " << weapons[lvl - 1];
};

void Lyagushka::GetInfo()
{
	Npc::GetInfo();
	cout << "Сила - " << strenght << endl;
	cout << "Доступная способность - ";
	for (int i = 0; i < lvl; i++)
	{
		cout << weapons[i] << endl;
	}
};
void Lyagushka::Create()
{
	cout << "Вы создали лягушку" << endl;
	cout << "Введите имя персонажа\t";
	cin >> name;
	GetInfo();
	GetWeapons();
};

bool Lyagushka::operator==(const Lyagushka &lyagushka) const
{
	return (lyagushka.damage == this->damage) &&
		   (lyagushka.health == this->health) &&
		   (lyagushka.strenght == this->strenght);
};

Lyagushka &Lyagushka::operator=(const Npc &npc)
{
	if (this != &npc)
	{
		this->name = npc.GetName();
		this->health = npc.GetHealth();
		this->damage = npc.GetDamage();
		this->lvl = npc.GetLvl();
	}
	return *this;
};

bool Lyagushka::Save(ofstream &saveSystem)
{
	// Сначала сохраняем базу (Npc), передавая тот же файл
	if (!Npc::Save(saveSystem))
		return false;

	// Потом дописываем свое
	saveSystem.write(reinterpret_cast<const char *>(&strenght), sizeof(strenght));
	return true;
};

bool Lyagushka::Load(ifstream &loadSystem)
{
	// Сначала читаем базу
	if (!Npc::Load(loadSystem))
		return false;

	// Потом читаем свое
	loadSystem.read(reinterpret_cast<char *>(&strenght), sizeof(strenght));
	return loadSystem.good();
};

Lyagushka::~Lyagushka()
{
	if (health <= 0)
		cout << name << " расстроен." << endl;
};

Kitty::Kitty()
{
	name = "kitty";
	health = 23;
	damage = 15;
};

Kitty::Kitty(string name, unsigned int health, float damage)
{
	cout << "кастомный конструктор kitty" << endl;
	this->name = name;
	this->health = health;
	this->damage = damage;
};

void Kitty::GetInfo()
{
	Npc::GetInfo();
	cout << "Интеллект - " << intellect << endl;
	cout << "Доступные знания - ";
	GetSpellsInfo();
};

void Kitty::GetSpellsInfo()
{
	for (int i = 0; i < 5; i++)
	{
		cout << i + 1 << " знание:\n";
		for (int j = 0; j < 5; j++)
		{
			cout << spells[i][j] << endl;
		}
		cout << endl;
	}
}

void Kitty::Create()
{
	cout << "Вы создали kitty" << endl;
	cout << "Введите имя персонажа\t";
	cin >> name;
	GetInfo();
};

Kitty Kitty::operator+(const Kitty &kitty) const
{
	return Kitty(
		this->name,
		(this->health + kitty.health),
		(this->damage + kitty.damage));
};

bool Kitty::Save(ofstream &saveSystem)
{
	if (Npc::Save(saveSystem))
	{
		// Файл уже открыт, просто пишем
		saveSystem.write(reinterpret_cast<const char *>(&intellect), sizeof(intellect));
		// Цикл заклинаний (пока пустой у тебя)
		return true;
	}
	return false;
};

Kitty::~Kitty()
{
	if (health <= 0)
		cout << name << " отправился к праотцам" << endl;
};

Kitty::Spell::Spell(string name, unsigned short damage,
					 unsigned short price, bool isCurse, int timeCast)
{
	this->name = name;
	this->damage = damage;
	this->price = price;
	this->isCurse = isCurse;
	this->timeCast = timeCast;
}

string Kitty::Spell::operator[](unsigned index) const
{
	switch (index)
	{
	case 0:
		return "Название - " + name;
		break;
	case 1:
		return "Урон - " + to_string(damage);
		break;
	case 2:
		return "Стоимость писпользования - " + to_string(price) + " маны";
		break;
	case 3:
	{
		if (isCurse)
		{
			return "периодичный урон";
		}
		else
		{
			return "произносимый заговор";
		}
		break;
	}
	case 4:
		return isCurse ? "Длительность дебафа - " + to_string(timeCast) : "Длительность использования - " + to_string(timeCast);
		break;
	default:
		return "такое свойство остутствует";
		break;
	}
}

unsigned short Kitty::Spell::CastSpell()
{
	cout << "Ты прочёл заговор " << name << " на противнике" << endl;
	return damage;
}

Kuromi::Kuromi()
{
	name = "Куроми";
	health = 25;
	damage = 12;
	strenght = 27;
};

void Kuromi::GetInfo()
{
	Lyagushka::GetInfo();
	cout << "Интеллект - " << intellect << endl;
	cout << "Доступные заклинания в книге заклинаний - ";
	GetSpellsInfo();
};

void Kuromi::Create()
{
	cout << "Вы создали куроми" << endl;
	cout << "Введите имя персонажа\t";
	cin >> name;
	GetInfo();
	GetWeapons();
};

bool Kuromi::Save(ofstream &saveSystem)
{
	// Куроми наследует Warrior, вызываем его сохранение
	if (Lyagushka::Save(saveSystem))
	{
		saveSystem.write(reinterpret_cast<const char *>(&intellect), sizeof(intellect));
		return true;
	}
	return false;
};

Kuromi::~Kuromi()
{
	if (health <= 0)
		cout << name << " расстроен и плачет" << endl;
};