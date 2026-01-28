#pragma once
using namespace std;
#include <fstream>
#include <iostream>

class Npc
{
protected:
	string name{"персонаж"};
	unsigned int health{10};
	float damage{5};
	unsigned short lvl{1};

public:
	string GetName() const { return name; }
	unsigned int GetHealth() const { return health; }
	float GetDamage() const { return damage; }
	unsigned short GetLvl() const { return lvl; }

	friend void TakeDamage(Npc &, float damage);

	virtual void GetInfo()
	{
		cout << "Имя - " << name << endl;
		cout << "Настроение - " << health << endl;
		cout << "Урон - " << damage << endl;
	}
	virtual void Create() {};

	// ИЗМЕНЕНИЕ: принимаем поток по ссылке, файл не открываем
	virtual bool Save(ofstream &saveSystem)
	{
		if (saveSystem.is_open())
		{
			size_t nameLength = name.length();
			saveSystem.write(reinterpret_cast<const char *>(&nameLength), sizeof(nameLength));
			saveSystem.write(name.c_str(), nameLength);
			saveSystem.write(reinterpret_cast<const char *>(&health), sizeof(health));
			saveSystem.write(reinterpret_cast<const char *>(&damage), sizeof(damage));
			saveSystem.write(reinterpret_cast<const char *>(&lvl), sizeof(lvl));
			return true;
		}
		return false;
	};

	// ИЗМЕНЕНИЕ: принимаем поток по ссылке
	virtual bool Load(ifstream &loadSystem)
	{
		if (loadSystem.is_open())
		{
			size_t nameLength;
			loadSystem.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));

			// Защита от битого файла (если длина имени неадекватная)
			if (nameLength > 1000)
				return false;

			char *buffer = new char[nameLength + 1];
			loadSystem.read(buffer, nameLength);
			buffer[nameLength] = '\0';
			name = string(buffer);
			delete[] buffer;

			loadSystem.read(reinterpret_cast<char *>(&health), sizeof(health));
			loadSystem.read(reinterpret_cast<char *>(&damage), sizeof(damage));
			loadSystem.read(reinterpret_cast<char *>(&lvl), sizeof(lvl));
			return loadSystem.good();
		}
		return false;
	};

	virtual ~Npc() = default;
};

class Player
{
private:
	unique_ptr<Npc> currentCharacter{nullptr};

public:
	void Create(unique_ptr<Npc> player)
	{
		currentCharacter = move(player);
		currentCharacter->Create();
	}

	// Метод перегружен, чтобы можно было создать без аргументов, если персонаж уже есть
	void Create()
	{
		if (currentCharacter != nullptr)
			currentCharacter->Create();
	}

	// Здесь открываем файл ОДИН РАЗ
	bool Save()
	{
		if (!currentCharacter)
			return false;
		ofstream saveSystem("save.bin", ios::binary);
		if (!saveSystem.is_open())
			return false;

		// Передаем поток внутрь
		bool result = currentCharacter->Save(saveSystem);
		saveSystem.close();
		return result;
	}

	// Здесь открываем файл ОДИН РАЗ
	bool Load(unique_ptr<Npc> player)
	{
		ifstream loadSystem("save.bin", ios::binary);
		if (!loadSystem.is_open())
			return false;

		// Передаем поток внутрь
		if (player->Load(loadSystem))
		{
			currentCharacter = move(player);
			loadSystem.close();
			return true;
		}
		loadSystem.close();
		return false;
	}

	Npc *GetCharacter()
	{
		return currentCharacter.get();
	}
};