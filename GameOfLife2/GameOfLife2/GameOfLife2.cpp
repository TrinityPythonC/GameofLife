// GameOfLife2.cpp : Defines the entry point for the application.
//


/* From the book "101 Basic Games" by David H. Ahl 1978
*  This game was made by John Conway (University of Cambridge, England)
*  Martin Gardner described this game in "Scientific American", October 1970

Conway's genetic laws are delightfully simple. First note that each cell of the checkerboard
(assumed to be an infinite plane) has eight neighboring cells, four adjacent orthogonally, four adjacent diagonally.
The rules are:

1. Survivals. Every counter with two or three neighboring counters survives for the next generation.

2. Deaths. Each counter with four or more neighbors dies(is removed) from overpopulation.
   Every counter with one neighbor or none dies from isolation.

3. Births. Each empty cell adjacent to exactly three neighbors - no more, no fewer - is a birth cell.
   A counter is placed on it at the next move.
   It is important to understand that all births and deaths occur simultaneously.
   Together they constitute a single generation or , as we shall call it, a "move" in the complete "life history"
   of the initial configuration
*/

// To make the menu   ...  just double click on Resorce File .rc and click on Menu. An IDE appears for adding to the menu.


#include "framework.h"
#include "GameOfLife2.h"
#include <ctime>
#include <vector>
#include <string>

using namespace std;

#define MAX_LOADSTRING 100
#define TIMER_RAY1 1 // use a different number for each timer 

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

int CellSize = 20; // width (and height) in pixels of each cell
int WindowSize = 800; // width (and height) of (square) window
int TableSize = WindowSize / CellSize - 2; // number of cells that fit in the width of the window
int msWait = 400; // medium speed
int Table[1000][1000];
int OLDTable[1000][1000];
int iMenuCELLSIZESelection = ID_CELLSIZE_MEDIUM;  // the number of the selected menu item for CELLSIZE
int iMenuPATTERNSelection = ID_PATTERN_RANDOM;  // the number of the selected menu item for PATTERN
int iMenuSPEEDSelection = ID_SPEED_MEDIUM;  // the number of the selected menu item for PATTERN
bool ClearTheScreenFlag = false;
vector <string> PatternVector = {"*"};

void ClearTable()
{
    for (int i = 0; i < 1000; i++)
        for (int j = 0; j < 1000; j++)
            Table[i][j] = 0;
}

void CreatePattern()
{
    string mystring;
    ClearTable();
    for (int i = 0; i < PatternVector.size(); i++)
        for (int j = 0; j < PatternVector[i].size(); j++)
        {
            mystring = PatternVector[i];
            if (mystring[j] == ' ')
            {
                Table[j+TableSize/2][i + TableSize / 2] = 0;
            }
            else
            {
                Table[j + TableSize / 2][i + TableSize / 2] = 1;
            }
        }
}

void  RandomPattern()
{
    ClearTable();
    srand(time(NULL)); // randomize seed for random numbers
    for (int i = 0; i < TableSize; i++)
        for (int j = 0; j < TableSize - 3; j++)
            if (rand() % 3 == 0)
            {
                Table[i][j] = 1;
            }
            else
            {
                Table[i][j] = 0;
            }
}

void TypeIPattern()
{
    ClearTable();
    for (int i = 0; i < TableSize - 3; i++)
    {
        Table[i][i] = 1;
        Table[i][i + 1] = 1;
    }
}

void TypeIIPattern()
{
    ClearTable();
    for (int i = 0; i < TableSize - 3; i++)
    {
        Table[i][i] = 1;
        Table[i + 3][i] = 1;
    }
}

void TypeIIIPattern()
{
    PatternVector.clear();
    PatternVector = { 
        " ** ", 
        "*   ", 
        "   *",
        " ** "};
    CreatePattern();
}

void TypeIVPattern()
{
    PatternVector.clear();
    PatternVector = {
        "** ",
        "*   ",
        "   *",
        "  **" };
    CreatePattern();
}

void TypeVPattern()
{
    PatternVector.clear();
    PatternVector = {
        "  * ",
        "* * ",
        " * *",
        " *  " };
    CreatePattern();
}

void TypeVIPattern()
{
    PatternVector.clear();
    PatternVector = {
        "  *",
        "* *",
        " **" 
    };
    CreatePattern();
}

int CountNeighbours(int x, int y)
{
    int n = 0;
    if (OLDTable[x][y + 1] == 1) n++;
    if (OLDTable[x + 1][y + 1] == 1) n++;
    if (OLDTable[x + 1][y] == 1) n++;
    if (x > 1)
    {
        if (OLDTable[x - 1][y + 1] == 1) n++;
        if (OLDTable[x - 1][y] == 1) n++;
    }
    if (y > 1)
    {
        if (OLDTable[x][y - 1] == 1) n++;
        if (OLDTable[x + 1][y - 1] == 1) n++;
    }
    if ((x > 1) && (y > 1))
    {
        if (OLDTable[x - 1][y - 1] == 1) n++;
    }
    return n;
}

void UpdateTable()
{
    for (int i = 0; i < TableSize; i++)
        for (int j = 0; j < TableSize; j++)
            OLDTable[i][j] = Table[i][j];
    for (int i = 0; i < TableSize; i++)
        for (int j = 0; j < TableSize; j++)
        {
            if (CountNeighbours(i, j) >= 4) { Table[i][j] = 0; }
            if (CountNeighbours(i, j) == 3) { Table[i][j] = 1; }
            if (CountNeighbours(i, j) <= 1) { Table[i][j] = 0; }
        }
}



void RefreshWindow(HWND hWnd)
{
    RECT rect;
    SetRect(&rect, 0, 0, WindowSize, WindowSize);
    InvalidateRect(hWnd, &rect, FALSE);  // tell windows to re-paint our window (update screen)
    UpdateWindow(hWnd); // call WM_PAINT
}

void DrawCell(HDC hdc, int x, int y)
{
    HBRUSH hBrush;
    RECT rect;
    int r1, r2, r3;
    r1 = rand() % 256; r2 = rand() % 256; r3 = rand() % 256;
    hBrush = CreateSolidBrush(RGB(r1, r2, r3));
    SetRect(&rect, x * CellSize, y * CellSize, x * CellSize + CellSize, y * CellSize + CellSize);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
}

void DrawCellWhite(HDC hdc, int x, int y)
{
    HBRUSH hBrush;
    RECT rect;
    hBrush = CreateSolidBrush(RGB(255, 255, 255));
    SetRect(&rect, x * CellSize, y * CellSize, x * CellSize + CellSize, y * CellSize + CellSize);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
}

void ClearScreen(HDC hdc, HWND hwnd)
{
    HBRUSH hBrush;
    RECT rect;
    hBrush = CreateSolidBrush(RGB(255, 255, 255));
    GetClientRect(hwnd, &rect);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
}


void DrawTable(HDC hdc)
{
    for (int i = 0; i < TableSize; i++)
        for (int j = 0; j < TableSize; j++)
            if (Table[i][j] == 1)
            {
                DrawCell(hdc, i, j);
            }
            else
            {
                DrawCellWhite(hdc, i, j);
            }
}

void UpDateMenuCheck(HMENU hMenu, int &i, int myID)
{
    CheckMenuItem(hMenu, i, MF_UNCHECKED);
    i = myID;
    CheckMenuItem(hMenu, i, MF_CHECKED);
}



// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAMEOFLIFE2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEOFLIFE2));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEOFLIFE2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GAMEOFLIFE2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      100,100, WindowSize, WindowSize, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HMENU hMenu;
    hMenu = GetMenu(hWnd);
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_PATTERN_TYPEI:
            TypeIPattern();
            RefreshWindow(hWnd);
            UpDateMenuCheck(hMenu, iMenuPATTERNSelection, wmId);
            break;
        case ID_PATTERN_TYPEII:
            TypeIIPattern();
            RefreshWindow(hWnd);
            UpDateMenuCheck(hMenu, iMenuPATTERNSelection, wmId);
            break;
        case ID_PATTERN_TYPEIII:
            TypeIIIPattern();
            RefreshWindow(hWnd);
            UpDateMenuCheck(hMenu, iMenuPATTERNSelection, wmId);
            break;
        case ID_PATTERN_TYPEIV:
            TypeIVPattern();
            RefreshWindow(hWnd);
            UpDateMenuCheck(hMenu, iMenuPATTERNSelection, wmId);
            break;
        case ID_PATTERN_TYPEV:
            TypeVPattern();
            RefreshWindow(hWnd);
            UpDateMenuCheck(hMenu, iMenuPATTERNSelection, wmId);
            break;
        case ID_PATTERN_TYPEVI:
            TypeVIPattern();
            RefreshWindow(hWnd);
            UpDateMenuCheck(hMenu, iMenuPATTERNSelection, wmId);
            break;
        case ID_PATTERN_RANDOM:
            RandomPattern();
            RefreshWindow(hWnd);
            UpDateMenuCheck(hMenu, iMenuPATTERNSelection, wmId);
            break;
        case ID_CELLSIZE_TINY:
            CellSize = 5;
            TableSize = WindowSize / CellSize - 2;
            ClearTheScreenFlag = true;
            RefreshWindow(hWnd);
            UpDateMenuCheck(hMenu, iMenuCELLSIZESelection, wmId);
            break;
        case ID_CELLSIZE_SMALL:
            CellSize = 10;
            TableSize = WindowSize / CellSize - 2;
            ClearTheScreenFlag = true;
            RefreshWindow(hWnd);
            UpDateMenuCheck(hMenu, iMenuCELLSIZESelection, wmId);
            break;
        case ID_CELLSIZE_MEDIUM:
            CellSize = 20;
            TableSize = WindowSize / CellSize - 2;
            ClearTheScreenFlag = true;
            RefreshWindow(hWnd);
            UpDateMenuCheck(hMenu, iMenuCELLSIZESelection, wmId);
            break;
        case ID_CELLSIZE_LARGE:
            CellSize = 40;
            TableSize = WindowSize / CellSize - 2;
            ClearTheScreenFlag = true;
            RefreshWindow(hWnd);
            UpDateMenuCheck(hMenu, iMenuCELLSIZESelection, wmId);
            break;
        case ID_SPEED_SLOW40SEC:
            KillTimer(hWnd, TIMER_RAY1);
            msWait = 40000;
            SetTimer(hWnd, TIMER_RAY1, msWait, NULL);
            UpDateMenuCheck(hMenu, iMenuSPEEDSelection, wmId);
            break;
        case ID_SPEED_SLOW4SEC:
            KillTimer(hWnd, TIMER_RAY1);
            msWait = 4000;
            SetTimer(hWnd, TIMER_RAY1, msWait, NULL);
            UpDateMenuCheck(hMenu, iMenuSPEEDSelection, wmId);
            break;
        case ID_SPEED_SLOW2SEC:
            KillTimer(hWnd, TIMER_RAY1);
            msWait = 2000;
            SetTimer(hWnd, TIMER_RAY1, msWait, NULL);
            UpDateMenuCheck(hMenu, iMenuSPEEDSelection, wmId);
            break;
        case ID_SPEED_SLOW1SEC:
            KillTimer(hWnd, TIMER_RAY1);
            msWait = 1000;
            SetTimer(hWnd, TIMER_RAY1, msWait, NULL);
            UpDateMenuCheck(hMenu, iMenuSPEEDSelection, wmId);
            break;
        case ID_SPEED_MEDIUM:
            KillTimer(hWnd, TIMER_RAY1);
            msWait = 500;
            SetTimer(hWnd, TIMER_RAY1, msWait, NULL);
            UpDateMenuCheck(hMenu, iMenuSPEEDSelection, wmId);
            break;
        case ID_SPEED_FAST:
            KillTimer(hWnd, TIMER_RAY1);
            msWait = 50;
            SetTimer(hWnd, TIMER_RAY1, msWait, NULL);
            UpDateMenuCheck(hMenu, iMenuSPEEDSelection, wmId);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_CREATE:
    {
        RandomPattern();
        SetTimer(hWnd, TIMER_RAY1, msWait, NULL); // calls TIMER_RAY1 every 400 milliseconds
        CheckMenuItem(hMenu, iMenuCELLSIZESelection, MF_CHECKED);
        CheckMenuItem(hMenu, iMenuPATTERNSelection, MF_CHECKED);
        CheckMenuItem(hMenu, iMenuSPEEDSelection, MF_CHECKED);
    }
    break;
    case WM_TIMER: // use this to move characters and finally update the screen
    {
        switch (wParam)
        {
        case TIMER_RAY1:
        {
            UpdateTable();
        }
        break; // jump out of switch block
        }
        // finally, update the screen
        RefreshWindow(hWnd); // call WM_PAINT
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (ClearTheScreenFlag == true) 
         { ClearScreen(hdc, hWnd);
           ClearTheScreenFlag = false;
         }
        DrawTable(hdc);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        KillTimer(hWnd, TIMER_RAY1);
        PostQuitMessage(0);
        break;
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
