#pragma once

namespace badEngine{

	//state machine
	namespace StateM {

		struct State {
			bool isMoving     = false;
			bool isFacingLeft = false;
		};

		static bool isFacingLeft(float x) {
			return x < 0.0f;
		}
		static bool isMoving(float x, float y) {
			return x != 0.0f || y != 0.0f;
		}

	}

}