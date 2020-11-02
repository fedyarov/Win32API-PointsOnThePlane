#pragma once

class Graphic{
	std::vector<int> X;
	std::vector<int> Y;
	int X_a, X_b, Y_a, Y_b;
	int nX, nY;
	double scaleX, scaleY;
	double numofstepsX, numofstepsY;
	int centerX, centerY;
	int wndWidth, wndHeight;
	bool flagXdown, flagXup, flagYleft, flagYright;
public:
	Graphic(int, int);

	int return_X(int);
	int return_Y(int);
	double return_translate_X(int);
	double return_translate_Y(int);
	int return_size();
	int return_X_a();
	int return_X_b();
	int return_Y_a();
	int return_Y_b();
	int return_nX();
	int return_nY();

	void PaintGrid(HWND);
	void set_new_Xwnd();
	void set_new_Ywnd();

	void set_X_a(int);
	void set_X_b(int);
	void set_Y_a(int);
	void set_Y_b(int);

	void set_scaleX();
	void set_scaleY();

	void set_point(int, int);
	void delete_point(int, int);
};

