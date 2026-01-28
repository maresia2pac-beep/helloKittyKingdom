#pragma once
#include "Evil.h"
#include <string>

Evil::Evil(std::string name) {
		this->name = name;
		health = 10;
		damage = 5;
}
Evil::Evil(string name, int health) : Evil(name) {
		this->health = health;
	}
Evil::Evil(string name, int health, int damage) : Evil(name, health) {
		this->damage = damage;
}

void Evil::GetInfo() {
	cout << endl;
	cout << "Âàñ àòàêóåò " << name << endl;
	if (health >= 0 && damage < 0) {
		cout << "Çäîðîâüå - " << health << endl;
	}
	if (health < 0 && damage >= 0) {
		cout << "Íàñòðîåíèå - " << damage << endl;
	}
	if (health >= 0 && damage >= 0) {
		cout << "Çäîðîâüå - " << health << endl;
		cout << "Íàñòðîåíèå - " << damage << endl;
	}
}
