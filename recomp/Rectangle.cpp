#include "Rectangle.h"

namespace badEngine {

	bool Rectangle::intersectsEnhanced(const Rectangle& a, const Rectangle& b, Rectangle* output) {

		float dx = center(a.x, a.w) - center(b.x, b.w);
		float dy = center(a.y, a.h) - center(b.y, b.h);

		float overlapX = overlap(a.w, b.w, dx);
		float overlapY = overlap(a.h, b.h, dy);

		if (overlapX < 0.0f && overlapY < 0.0f)
			return false;

		if (output)
			*output = { dx,dy,overlapX, overlapY };

		return true;
	}
	bool Rectangle::intersectsEnhanced(const Rectangle& other, Rectangle* output)const {
		return Rectangle::intersectsEnhanced(*this, other, output);
	}

}