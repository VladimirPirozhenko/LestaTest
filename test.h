#include "ParticleSystem.h"

namespace test
{

	
	void render(void); // Only platform::drawPoint should be used
	void update(int dt); // dt in milliseconds
	void on_click(int x, int y); // x, y - in pixels

	void mouseButtonCallback(int button, int state, int x, int y);

	void init(void);
	void term(void);


};

