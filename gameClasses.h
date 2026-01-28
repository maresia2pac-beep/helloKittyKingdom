#pragma once
#include "BaseClasses.h"
#include <string>

class Lyagushka : public virtual Npc //наследование с модификатором доступа public
{
protected:  //модификатор 0 (приватный - защищенный, доступ к полям, только внутри класса)
	unsigned short strenght{ 31 };
	std::string weapons[4] = { "шипучка", "волшебная пыльца", "когти", "сосулька" };
public:
	Lyagushka();
	Lyagushka(std::string name, unsigned int health, float damage);
	void GetWeapons();
	void GetInfo() override;
	void Create() override;
	bool operator == (const Lyagushka& warrior) const;
	Lyagushka& operator = (const Npc& npc);
	bool Save(ofstream& saveSystem) override;
	bool Load(ifstream& loadSystem) override;
	~Lyagushka();
};

class Kitty : public virtual Npc
{
public:
	class Spell
	{
		protected: 
			string name{ "заклинание" };
			unsigned short damage{ 0 };
			unsigned short price{ 0 };
			bool isCurse{ false };
			int timeCast{ 0 };
		public:
			Spell(string name = "заклинание", unsigned short damage = 0, unsigned short price = 0, bool isCurse = false, int timeCast = 0);
			string operator[](unsigned index) const;
			unsigned short CastSpell();
	};
protected:
	unsigned short intellect = 27;
	Spell spells[5] = {
		Spell("колкое оскорбление",20,50,false,3),
		Spell("клонирование",10,30,false,1),
		Spell("культурный шок",10,20,false),
		Spell("политическая беседа",10,25,true,5),
		Spell("обворожительный взгляд",5,10,false,10),
	};
public:
	Kitty();
	Kitty(string name, unsigned int health, float damage);
	void GetInfo() override;
	void Create() override;
	Kitty operator + (const Kitty& wizard) const;
	bool Save(ofstream& saveSystem) override;
	void GetSpellsInfo();
	~Kitty();
};

class Kuromi : public Lyagushka, public Kitty
{
public:
	Kuromi();
	void GetInfo() override;
	void Create() override;
	bool Save(ofstream& saveSystem) override;
	~Kuromi();
};