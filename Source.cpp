#include "Source.h"

#define IDB_BUTTONOPTIONS 1

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ModalDialogOptionsProc(HWND, UINT, WPARAM, LPARAM);

Graphic graphic(1100, 600);
TCHAR FileName[260] = L"";
OPENFILENAME ofn;

int WINAPI WinMain(HINSTANCE hInst,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

	TCHAR szClassName[] = L"Мой класс";
	HWND hMainWnd{};
	MSG msg;
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.lpszClassName = szClassName;
	wc.cbWndExtra = NULL;
	wc.cbClsExtra = NULL;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hInstance = hInst;

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Не получилось зарегистроровать класс", L"Ошибка", MB_OK);
		return NULL;
	}

	hMainWnd = CreateWindow(szClassName, L"Полноценная оконная процедура", WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE, CW_USEDEFAULT,
		NULL, 1100, 600, (HWND)NULL, NULL, HINSTANCE(hInst), NULL);

	if (!hMainWnd) {
		MessageBox(NULL, L"Не получилось создать окно", L"Ошибка", MB_OK);
		return NULL;
	};

	//////////////// STRUCT FOR FILE
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hMainWnd;
	ofn.lpstrFile = FileName;
	ofn.nMaxFile = sizeof(FileName);
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	ShowWindow(hMainWnd, nCmdShow);
	UpdateWindow(hMainWnd);
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	HDC hDeviceContext;
	PAINTSTRUCT paintStruct;
	RECT rectPlace, windowRect;
	HFONT hFont;
	static HWND hButton{};
	static HINSTANCE hInst;

	GetWindowRect(hWindow, &windowRect);

	switch (uMessage) {
		case WM_CREATE:
			hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		
			hButton = CreateWindow(L"button", L"Опции", WS_CHILD | WS_VISIBLE | WS_BORDER,
				10, 500, 100, 50, hWindow, (HMENU) IDB_BUTTONOPTIONS, hInst, NULL);
			ShowWindow(hButton, SW_SHOWNORMAL);

			break;
		case WM_PAINT:
			graphic.PaintGrid(hWindow);
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam)){
				case IDB_BUTTONOPTIONS:
					if (DialogBox(hInst, MAKEINTRESOURCE(DLG_OPTIONS),
						hWindow, (DLGPROC)ModalDialogOptionsProc) == IDOK){
					}
					else {
						graphic.PaintGrid(hWindow);
						InvalidateRect(hWindow, NULL, TRUE); // Перерисовка окна
						break;
					}
				}
			return 0L;
		case WM_LBUTTONUP:
			WORD xPos, yPos;
			TCHAR szBuf[80];

			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);

			graphic.set_point( (int)xPos, (int)yPos);
			InvalidateRect(hWindow, NULL, TRUE); // Перерисовка окна
			break;
		case WM_RBUTTONUP:
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);

			graphic.delete_point((int)xPos, (int)yPos);
			InvalidateRect(hWindow, NULL, TRUE); // Перерисовка окна
			break;
		case WM_DESTROY:
			PostQuitMessage(NULL);
			break;
		default:
			return DefWindowProc(hWindow, uMessage, wParam, lParam);
	};
	return NULL;
}

BOOL CALLBACK ModalDialogOptionsProc(HWND hWindowDlg, UINT uMessage, WPARAM wParam, LPARAM lParam){
	TCHAR buff[256];
	TCHAR vector_buff[1024*10];
	DWORD iNumRead = 0, iNumWrite = 0; // Получает кол-во считанных байт
	DWORD dwBytesToWrite = 0;

	int size = 0;
	switch (uMessage){
		// Инициализация диалоговой панели
		case WM_INITDIALOG:{
			memset(vector_buff, 0, 1024 * 10);
			size = graphic.return_size();
			for (int i = 0; i < size; i++) {
				swprintf(buff, 100, L"(%.1lf,%.1lf)", graphic.return_translate_X(i), graphic.return_translate_Y(i));
				wcscat_s(vector_buff, buff);
			};
			SetDlgItemText(hWindowDlg, IDC_EDIT7, vector_buff);

			swprintf(buff, 100, L"%d", graphic.return_nX());
			SetDlgItemText(hWindowDlg, IDC_EDIT1, buff);
			swprintf(buff, 100, L"%d", graphic.return_nY());
			SetDlgItemText(hWindowDlg, IDC_EDIT2, buff);
			swprintf(buff, 100, L"%d", graphic.return_X_a());
			SetDlgItemText(hWindowDlg, IDC_EDIT3, buff);
			swprintf(buff, 100, L"%d", graphic.return_X_b());
			SetDlgItemText(hWindowDlg, IDC_EDIT4, buff);
			swprintf(buff, 100, L"%d", graphic.return_Y_a());
			SetDlgItemText(hWindowDlg, IDC_EDIT5, buff);
			swprintf(buff, 100, L"%d", graphic.return_Y_b());
			SetDlgItemText(hWindowDlg, IDC_EDIT6, buff);

			SetDlgItemText(hWindowDlg, IDC_EDIT8, ofn.lpstrFile);

			return TRUE;
		}
		 break;
		case WM_COMMAND:{
			switch (wParam){
				case IDB_BEGINX:
					GetDlgItemText(hWindowDlg, IDC_EDIT3, buff, sizeof(buff));
					graphic.set_X_a(_wtof(buff));
					graphic.set_scaleX();
					//graphic.set_new_Xwnd();
					break;
				case IDB_ENDX:
					GetDlgItemText(hWindowDlg, IDC_EDIT4, buff, sizeof(buff));
					graphic.set_X_b(_wtof(buff));
					graphic.set_scaleX();
					//graphic.set_new_Xwnd();
					break;
				case IDB_BEGINY:
					GetDlgItemText(hWindowDlg, IDC_EDIT5, buff, sizeof(buff));
					graphic.set_Y_a(_wtof(buff));
					graphic.set_scaleY();
					//graphic.set_new_Ywnd();
					break;
				case IDB_ENDY:
					GetDlgItemText(hWindowDlg, IDC_EDIT6, buff, sizeof(buff));
					graphic.set_Y_b(_wtof(buff));
					graphic.set_scaleY();
					//graphic.set_new_Ywnd();
					break;
				case IDB_SETFILE:
					if (GetOpenFileName(&ofn) == TRUE) {
						SetDlgItemText(hWindowDlg, IDC_EDIT8, ofn.lpstrFile);

						ifstream out(ofn.lpstrFile, ios::binary);

						int i = 0;
						int x, y;
						while (out.read((char*)& x, sizeof(x))) {
							out.read((char*)& y, sizeof(y));
							graphic.set_point(x, y);
							i++;
						}
						out.close();

						MessageBox(hWindowDlg, L"Файл успешно открыт", L"Информация", MB_OK);
					}
					break;
				case IDB_SAVE: {
					ofstream in(ofn.lpstrFile, ios::binary);
					int size = graphic.return_size();
					int tempX, tempY;
					for (int i = 0; i < size; i++) {
						tempX = graphic.return_X(i);
						tempY = graphic.return_Y(i);
						in.write((char*)& tempX, sizeof(tempX));
						in.write((char*)& tempY, sizeof(tempY));
					}
					in.close();

					MessageBox(hWindowDlg, L"Файл успешно сохранен", L"Информация", MB_OK);

					break;
				}
				case IDOK:
					EndDialog(hWindowDlg, 0);
					return TRUE;
				case IDCANCEL:{
					EndDialog(hWindowDlg, 0);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}