#pragma once
class Calibrate
{
public:
	Calibrate();
	~Calibrate();

	void MouseHandler(int event, int x, int y, int flags, void* userdata);
	static void mouseWrapper(int event, int x, int y, int flags, void *param);
	void init();
};

