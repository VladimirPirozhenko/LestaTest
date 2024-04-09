#include "ParticleRenderer.h"

namespace test
{
	void render(void); // Only platform::drawPoint should be used
	void update(int dt); // dt in milliseconds
	void on_click(int x, int y); // x, y - in pixels
	void init(void);
	void term(void);
};

