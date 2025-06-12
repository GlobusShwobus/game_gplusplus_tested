#pragma once
#include <bitset>
namespace badEngine {

	//bit event state system
	namespace BESS {
		enum Events {
			BESSE_DELETE_ENTITY = 0,
			BESSE_CHECK_ANIM = 2,
			BESSE_CHECK_DIR = 3,
		};
		struct EventList {
			std::bitset<128> eventList;
		};


		enum StateDir {
			right, down,left,up,none
		};
		enum StateMovement {
			walk, idle, jump
		};
		struct StateMachine {

		};



		/*
		FOOD FOR THOUGHT
		template <typename... Enums>
void setFlags(std::bitset<128>& b, Enums... values) {
    (b.set(values), ...); // Fold expression (C++17+)
}

OR 

void setFlags(std::bitset<128>& b, std::initializer_list<int> flags) {
	for (int f : flags) b.set(f);
}
		*/
	}


}
