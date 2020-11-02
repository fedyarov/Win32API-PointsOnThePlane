#include "Source.h"

Graphic::Graphic(int w, int h){
	wndWidth = w - 16;
	wndHeight = h - 39;
	flagXdown = false;
	flagXup = false;
	flagYleft = false;
	flagYright = false;
	X_a = -10;
	X_b = 10;
	Y_a = -10;
	Y_b = 10;
	nX = 1;
	nY = 1;
	numofstepsX = abs(X_a - X_b);
	numofstepsY = abs(Y_a - Y_b);
	scaleX = wndWidth / numofstepsX;
	scaleY = wndHeight / numofstepsY;
	centerX = scaleX * X_b;
	centerY = scaleY * Y_b;
}

void Graphic::PaintGrid(HWND hWnd){
	HDC hDcontext;
	PAINTSTRUCT paintStruct;
	HBRUSH brush;
	HPEN pen;
	int count=0;
	int sizeofnum;
	int prevCenterX, prevCenterY;
	TCHAR buffofnums[10];
	hDcontext = BeginPaint(hWnd, &paintStruct);

	/////////////////// GRID
	pen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	brush = CreateSolidBrush(RGB(200, 200, 200));
	SelectObject(hDcontext, brush);
	SelectObject(hDcontext, pen);
	// X
	prevCenterX = centerX;
	centerX = -X_a * scaleX;
	for (int i = 0; i >X_a ; i-= nX) {
		MoveToEx(hDcontext, centerX +i * scaleX, 0, NULL);
		LineTo(hDcontext, centerX +i * scaleX, wndHeight);
		count+=nX;
	}
	//centerX = count * scaleX;
	for (int i = 0; i < X_b; i+=nX) {
		MoveToEx(hDcontext, centerX + i * scaleX, 0, NULL);
		LineTo(hDcontext, centerX + i * scaleX, wndHeight);
	}
	// Y
	prevCenterY = centerY;
	centerY = wndHeight + Y_a * scaleY;
	count = 0;
	for (int i = 0; i > Y_a; i-=nY) {
		MoveToEx(hDcontext, 0,  centerY - i * scaleY, NULL);
		LineTo(hDcontext, wndWidth,  centerY - i * scaleY);
		count+=nY;
	}
	for (int i = 0; i < Y_b; i+=nY) {
		MoveToEx(hDcontext, 0, centerY - i * scaleY, NULL);
		LineTo(hDcontext, wndWidth, centerY - i * scaleY);
	}

	//////////////////// NUMBERS
	//X

	for (int i = 0; i > X_a; i -= nX) {
		swprintf(buffofnums, 10, L"%d", i);
		sizeofnum = wcslen(buffofnums);
		if (flagXdown) TextOut(hDcontext, centerX + i * scaleX + 5, 0 + 5, buffofnums, sizeofnum);
		else if (flagXup) TextOut(hDcontext, centerX + i * scaleX + 5, wndHeight - 18, buffofnums, sizeofnum);
		else TextOut(hDcontext, centerX +i * scaleX + 5, centerY + 5, buffofnums, sizeofnum);
	}
	int k = 0;
	for (int i = 0; i < X_b; i += nX) {
		if (X_a > 0) swprintf(buffofnums, 10, L"%d",  i + X_a);
		else swprintf(buffofnums, 10, L"%d", i);
		sizeofnum = wcslen(buffofnums);
		
		if (X_a <= 0) {
			if (flagXdown) TextOut(hDcontext, centerX + i * scaleX + 5, 0 + 5, buffofnums, sizeofnum);
			else if (flagXup) TextOut(hDcontext,  centerX + i * scaleX + 5, wndHeight - 18, buffofnums, sizeofnum);
			else TextOut(hDcontext, centerX + i * scaleX + 5, centerY + 5, buffofnums, sizeofnum);
		}
		else {
			if (flagXdown) TextOut(hDcontext, centerX + k * scaleX + 5, 0 + 5, buffofnums, sizeofnum);
			else if (flagXup) TextOut(hDcontext, centerX + k * scaleX - 5, wndHeight - 18, buffofnums, sizeofnum);
			TextOut(hDcontext, k * scaleX + 5, centerY + 5, buffofnums, sizeofnum);
		}
		k+=nX;
		
	}
	//Y
	for (int i = 0; i > Y_a; i -= nY) {
		swprintf(buffofnums, 10, L"%d", i);
		sizeofnum = wcslen(buffofnums);
		if (flagYright) TextOut(hDcontext, 0 +5, centerY - i * scaleY + 5, buffofnums, sizeofnum);
		else if (flagYleft) TextOut(hDcontext,  wndWidth - 30, centerY - i * scaleY + 5, buffofnums, sizeofnum);
		else TextOut(hDcontext, centerX + 5, centerY - i * scaleY + 5, buffofnums, sizeofnum);
	}
	k=0;
	for (int i = 0; i <= Y_b; i += nY) {
		swprintf(buffofnums, 10, L"%d", i);
		sizeofnum = wcslen(buffofnums);
		if (Y_a <= 0) {
			if (flagYright)TextOut(hDcontext, 0 + 5, centerY - i * scaleY + 5, buffofnums, sizeofnum);
			else if (flagYleft) TextOut(hDcontext, wndWidth - 30, centerY - i * scaleY + 5, buffofnums, sizeofnum);
			else TextOut(hDcontext, centerX + 5, centerY - i * scaleY + 5, buffofnums, sizeofnum);
		}
		else {
			if (flagYright)TextOut(hDcontext, 0 + 5, centerY - k * scaleY + 5, buffofnums, sizeofnum);
			else if (flagYleft) TextOut(hDcontext, wndWidth - 30, centerY - k * scaleY + 5, buffofnums, sizeofnum);
			else TextOut(hDcontext, centerX + 5, centerY - k * scaleY + 5, buffofnums, sizeofnum);
		}
		k+=nY;
	}
	
	DeleteObject(brush);
	DeleteObject(pen);

	//////////////////// oX and oY
	pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	brush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hDcontext, brush);
	SelectObject(hDcontext, pen);

	MoveToEx(hDcontext, centerX , 0, NULL);
	LineTo(hDcontext, centerX , wndHeight);
	
	MoveToEx(hDcontext, 0, centerY, NULL);
	LineTo(hDcontext, wndWidth, centerY);

	DeleteObject(brush);
	DeleteObject(pen);

	//////////////////// POINTS
	pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	brush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hDcontext, brush);
	SelectObject(hDcontext, pen);

	int size = X.size();
	for (int i = 0; i < size; i++) {
		X[i] -= prevCenterX - centerX;
		Y[i] += centerY - prevCenterY;
		Ellipse(hDcontext, X[i] - 6, Y[i] - 6, X[i] + 6, Y[i] + 6);
	}

	DeleteObject(brush);
	DeleteObject(pen);
	EndPaint(hWnd, &paintStruct);
}

//////////////////////// SET
void Graphic::set_scaleX() {
	int size = X.size();
	double prevX[100];
	for (int i = 0; i < size; i++) {
		prevX[i] = return_translate_X(i);
	}

	if (X_a > X_b) {
		int p;
		p = X_a;
		X_a = X_b;
		X_b = p;
	}
	if (X_a <= 0) {
		if (X_b <= 0) {
			flagYleft = true;
			flagYright = false;
		}
		else {
			flagYleft = false;
			flagYright = false;
		}
		numofstepsX = (double)abs(X_a - X_b);
		scaleX = wndWidth / (double)abs(X_a - X_b);

		centerX = -X_a * scaleX;
	}
	else if (X_a > 0 && X_b > 0) {
		flagYright = true;
		flagYleft = false;
		numofstepsX = (double)(X_b - X_a);
		scaleX = wndWidth / (double)(X_b - X_a);

		centerX = -X_a * scaleX;
	}

	if (!((int)numofstepsX % 2)) {
		if (numofstepsX < 40) nX = 1;
		else if ((nX = numofstepsX / 40 + 1) % 2) nX += 1;
	}
	else {
		if ((int)numofstepsX < 40) nX = 1;
		else if (!((nX = numofstepsX / 40 + 1) % 2)) nX += 1;
	}

	///////// new coordinates
	for (int i = 0; i < size; i++) {
		X[i] = centerX + prevX[i] * scaleX;
	}
	
}

void Graphic::set_scaleY() {
	int size = Y.size();
	double prevY[100];
	for (int i = 0; i < size; i++) {
		prevY[i] = return_translate_Y(i);
	}

	if (Y_a > Y_b) {
		int p;
		p = Y_a;
		Y_a = Y_b;
		Y_b = p;
	}
	if (Y_a < 0) {
		if (Y_b < 0) {
			flagXdown = true;
			flagXup = false;
		}
		else {
			flagXdown = false;
			flagXup = false;
		}
		numofstepsY = (double)abs(Y_a - Y_b);
		scaleY = wndHeight / (double)abs(Y_a - Y_b);

		centerY = wndHeight + Y_a * scaleY;
	}
	else if (Y_a >= 0 && Y_b >= 0) {
		flagXup = true;
		flagXdown = false;
		numofstepsY = (double)(Y_b - Y_a);
		scaleY = wndHeight / (double)(Y_b - Y_a);

		centerY = Y_b * scaleY;
	}

	if (!((int)numofstepsY % 2)) {
		if (numofstepsY < 20) nY = 1;
		else if ((nY = numofstepsY / 20 + 1) % 2) nY += 1;
	}
	else {
		if ((int)numofstepsY < 20) nY = 1;
		else if (!((nY = numofstepsY / 20 + 1) % 2)) nY += 1;
	}

	//////////////// new coordinates
	for (int i = 0; i < size; i++) {
		Y[i] = centerY - prevY[i] * scaleY;
	}

}

void Graphic::set_point(int x, int y) {

	X.push_back(x);
	Y.push_back(y);
}

void Graphic::delete_point(int x, int y) {
	int size = X.size();
	if (size != 0) {
		for (int i = 0; i < size; i++) {
			if (X[i] >= x-6 && X[i] <= x+6 && Y[i] >= y-6 && Y[i] <= y+6) {
				X.erase(X.begin() + i);
				Y.erase(Y.begin() + i);
				break;
			}
		}
	};
}

void Graphic::set_X_a(int a) {
	this->X_a = a;
}

void Graphic::set_X_b(int b) {
	this->X_b = b;
}

void Graphic::set_Y_a(int a) {
	this->Y_a = a;
}

void Graphic::set_Y_b(int b) {
	this->Y_b = b;
}

///////////////////// RETURN
int Graphic::return_size() {
	return X.size();
}

int Graphic::return_X(int index) {
	return X[index];
}

int Graphic::return_Y(int index) {
	return Y[index];
}

double Graphic::return_translate_X(int index) {
	if (X[index] >= centerX) {
		return (X[index] - centerX) / scaleX;
	}
	else {
		return -((centerX - X[index]) / scaleX);
	}
}

double Graphic::return_translate_Y(int index) {
	if (Y[index] <= centerY) {
		return (centerY - Y[index]) / scaleY;
	}
	else {
		return -((Y[index] - centerY) / scaleY);
	}
}

void Graphic::set_new_Xwnd() {
	int size = X.size();
	for (int i = 0; i < size; i++) {
		X[i] = centerX + return_translate_X(i) * scaleX;
	}
}

void Graphic::set_new_Ywnd() {
	int size = Y.size();
	for (int i = 0; i < size; i++) {
		Y[i] = centerY + return_translate_Y(i) * scaleY;
	}
}


int Graphic::return_X_a() {
	return X_a;
}

int Graphic::return_X_b() {
	return X_b;
}

int Graphic::return_Y_a() {
	return Y_a;
}

int Graphic::return_Y_b() {
	return Y_b;
}

int Graphic::return_nX() {
	return nX;
}

int Graphic::return_nY() {
	return nY;
}


