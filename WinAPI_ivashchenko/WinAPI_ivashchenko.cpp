// WinAPI_ivashchenko.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WinAPI_ivashchenko.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

HWND mainWnd;
HWND ball;
HWND* upObs;
HWND* lowObs;

int ballSpeed = -2;
int minSpeed = -5;
int maxSpeed = 5;
int obsSpeed = 5;

int obsWidth = 50;
int obsHeight = 150;

int ballX = 200;
int ballY = 200;

int* obsX;
int* random;

int score = 0;

bool ballGoingUp = false;
bool scored = false;
bool bitmap=false;
bool stretch =false;

COLORREF colours[16] = { 0 };
TCHAR file[260];

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
ATOM				MyRegisterClassBall(HINSTANCE hInstance);
ATOM				MyRegisterClassObs(HINSTANCE hInstance);

BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void Initialize();
void IsBall(int i);
void Randomize(int i);
void Reset();
void Bitmap();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WINAPI_IVASHCHENKO, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance);
	MyRegisterClassBall(hInstance);
	MyRegisterClassObs(hInstance);

	Initialize();
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPI_IVASHCHENKO));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	free(upObs);
	free(lowObs);
	free(obsX);
	free(random);

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI_IVASHCHENKO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	//light blue window
	wcex.hbrBackground = CreateSolidBrush(RGB(135,206,250));
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WINAPI_IVASHCHENKO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
ATOM MyRegisterClassObs(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI_IVASHCHENKO));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0,0,255));
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDI_WINAPI_IVASHCHENKO);
	wcex.lpszClassName =L"o";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

ATOM MyRegisterClassBall(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI_IVASHCHENKO));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255,0,0));
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDI_WINAPI_IVASHCHENKO);
	wcex.lpszClassName = L"b";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  // HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   mainWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 600, 400, NULL, NULL, hInstance, NULL);
   //◾at the start main window appears in the middle of the screen
    RECT rc;
	GetWindowRect ( mainWnd, &rc ) ;
	int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right)/2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom)/2;
	SetWindowPos( mainWnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	if (!mainWnd)
   {
      return FALSE;
   }

   //transparency 20%
	SetWindowLong(mainWnd, GWL_EXSTYLE, GetWindowLong(mainWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(mainWnd, 0, (255 * 80) / 100, LWA_ALPHA);

	ShowWindow(mainWnd, nCmdShow);
	UpdateWindow(mainWnd);

   //creating a ball
	ball = CreateWindowW(L"b", szTitle, WS_CHILD | WS_CLIPSIBLINGS,
		ballX, ballY,15,15, mainWnd, nullptr, hInst, nullptr);
	SetWindowLong(ball, GWL_STYLE, 0);
	SetMenu(ball, NULL);
	ShowWindow(ball, nCmdShow);

	HRGN regionb = CreateEllipticRgn(0, 0,15,15);
	SetWindowRgn(ball, regionb, true);

	//creating obstacles
	for (int i = 0; i < 1; i++) 
	{
		upObs[i] = CreateWindowW(L"o", szTitle, WS_CHILD | WS_CLIPSIBLINGS,
			obsX[i], 0, obsWidth, obsHeight + random[i], mainWnd, nullptr, hInst, nullptr);
		SetWindowLong(upObs[i], GWL_STYLE, 0);
		SetMenu(upObs[i], NULL);
		ShowWindow(upObs[i], nCmdShow);

		lowObs[i] = CreateWindowW(L"o", szTitle, WS_CHILD | WS_CLIPSIBLINGS,
			obsX[i], obsHeight + 100 + random[i], obsWidth, obsHeight - random[i], mainWnd, nullptr, hInst, nullptr);
		SetWindowLong(lowObs[i], GWL_STYLE, 0);
		SetMenu(lowObs[i], NULL);
		ShowWindow(lowObs[i], nCmdShow);
	}

	if (!ball)
	{
		return FALSE;
	}
	for (int i = 0; i < 1; i++) {
		if (!upObs[i] | !lowObs[i]) {
			return FALSE;
		}
	}

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		SetTimer(mainWnd, 1, 5, NULL);
		SetTimer(mainWnd, 2, 30, NULL);
	}
	break;
	case WM_TIMER:
	{
		switch (wParam) {
		case 1:
		{
			for (int i = 0; i < 1; i++) 
			{
				IsBall(i);
				obsX[i] -= obsSpeed;

				if (obsX[i] < -obsWidth) 
				{
					obsX[i] = 600;
					Randomize(i);
					scored = false;
				}

				MoveWindow(upObs[i], obsX[i], 0, obsWidth, obsHeight + random[i], TRUE);
				MoveWindow(lowObs[i], obsX[i], obsHeight + 100 + random[i], obsWidth, obsHeight - random[i], TRUE);

				InvalidateRect(upObs[i], NULL, TRUE);
				InvalidateRect(lowObs[i], NULL, TRUE);
				UpdateWindow(upObs[i]);
				UpdateWindow(lowObs[i]);
			}
		}
		break;
		case 2:
		{
			for (int i = 0; i < 1; i++)
				IsBall(i);

			if (ballGoingUp==true) 
			{
				if (ballSpeed < 0)
					ballSpeed = 1;

				else 
				{ 
					if (ballSpeed < maxSpeed)
						ballSpeed++;
					else 
						ballGoingUp = false;
				}
			}
			else if (ballSpeed > minSpeed)
					ballSpeed--;
	
			ballY -= ballSpeed;
			MoveWindow(ball, ballX, ballY, 15, 15, TRUE);
			InvalidateRect(ball, NULL, TRUE);
			UpdateWindow(ball);
		}
		break;
		}
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_NEWGAME:
			Reset();
			break;

		//background color
		case IDM_COLOR:
		{
			CHOOSECOLOR color;
			color.lStructSize = sizeof(CHOOSECOLOR);
			color.Flags = CC_ANYCOLOR | CC_FULLOPEN;
			color.hwndOwner = mainWnd;
			color.lpCustColors = colours;
			if (ChooseColor(&color)) 
			{
				SetClassLong(mainWnd, GCLP_HBRBACKGROUND, (LONG)CreateSolidBrush(color.rgbResult));
				InvalidateRect(mainWnd, NULL, true);
				UpdateWindow(mainWnd);
			}
		}
			break;
		//bitmap
		case IDM_BACKGROUND:
		{

			OPENFILENAME openFile;

			ZeroMemory(&openFile, sizeof(openFile));
			openFile.lStructSize = sizeof(openFile);
			openFile.hwndOwner = hWnd;
			openFile.lpstrFile = file;
			openFile.lpstrFile[0] = '\0';
			openFile.nMaxFile = sizeof(file);
			openFile.nFilterIndex = 1;
			openFile.lpstrFileTitle = NULL;
			openFile.nMaxFileTitle = 0;
			openFile.lpstrInitialDir = NULL;
			openFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			openFile.lpstrFilter = _T("Bitmaps\0*.BMP\0");

			if (GetOpenFileName(&openFile)) 
			{
				Bitmap();
				bitmap=true;
			}
		}
			break;
		case IDM_TILE:
		{
			stretch=false;
			Bitmap();
		}
		break;
		case IDM_STRETCH:
		{
			stretch=true;
			Bitmap();
		}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT p;
		BeginPaint(hWnd, &p);
		EndPaint(hWnd, &p);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZING:
	{
		//prevent a size changing, but allow to change the window position
		RECT *rc = (RECT*)lParam;
		rc->right = rc->left + 600;
		rc->bottom = rc->top + 400;
	}
	break;
	case WM_LBUTTONDOWN:
		ballGoingUp = true;
		break;
	case WM_RBUTTONDOWN:
	{
		POINT p;
		HMENU m;
		p.x = LOWORD(lParam);
		p.y = HIWORD(lParam);
		ClientToScreen(hWnd, &p);
		m = LoadMenu(NULL, MAKEINTRESOURCE(IDR_CONTEXT_MENU));
		m = GetSubMenu(m, 0);
		TrackPopupMenu(m, TPM_RIGHTBUTTON, p.x, p.y, 0, hWnd, NULL);
		DestroyMenu(m);
	}
		break;
		// Process other window messages.  
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void IsBall(int i) 
{
	TCHAR s[256];
	if (ballX + 15 >= obsX[i] && ballX <= obsX[i] + obsWidth) 
	{ 
		if (ballY <= obsHeight + random[i] || ballY + 15 >= obsHeight + 100 + random[i])
			Reset();
		else 
			scored = false;
	}
	else 
		if (ballY <= 0 || ballY + 2*15 >= 400) 
			Reset();
		
	if (ballX > obsX[i] + obsWidth && !scored) 
	{
		score++;
		scored = true;
	}
	//printing score in the title
	_stprintf_s(s,256, L"YOUR SCORE: %d", score);
	SetWindowText(mainWnd, s);
}

void Bitmap() 
{
	HBITMAP bm;
	if(stretch)
		bm = (HBITMAP)LoadImage(NULL, file, IMAGE_BITMAP, 600, 400, LR_LOADFROMFILE);
	else
		bm = (HBITMAP)LoadImage(NULL, file, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	SetClassLongPtr(mainWnd, GCLP_HBRBACKGROUND, (LONG)CreatePatternBrush(bm));
	InvalidateRect(mainWnd, NULL, true);
	UpdateWindow(mainWnd);
}

void Reset() 
{
	ballY = 200;
	ballX = 200;
	for (int i = 0; i < 1; i++)
		obsX[i] = 600 + 600 * i;

	Randomize(0);
	score = 0;
	scored = false;
	ballSpeed = -3;
}

void Randomize(int a) 
{
	if (a == 0)
		for (int i = 0; i < 1; i++) 
			random[i] = (rand() % (obsHeight));
	else 
		random[a] = (rand() % (obsHeight));
}

void Initialize() 
{
	upObs = (HWND*)calloc(1, sizeof(HWND));
	lowObs = (HWND*)calloc(1, sizeof(HWND));

	obsX = (int*)calloc(1, sizeof(int));
	for (int i = 0; i < 1; i++)
		obsX[i] = 600 + 600 * i;

	random = (int*)calloc(1, sizeof(int));
	srand(time(NULL));
	Randomize(0);
}