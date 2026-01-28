	#pragma once
	#include "BaseClasses.h"

	class Evil : public Npc {
	public:
		Evil(string name);
		Evil(string name, int health);
		Evil(string name, int health, int damage);

		void GetInfo() override;
	};