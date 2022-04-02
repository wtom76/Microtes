// Microtes.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include <future>
#include <atomic>
#include <cassert>
#include "Microtes.h"
#include "configuration.h"
#include "graphics.h"
#include "data/field.h"
#include "data/microte_impl/a001/a001_microte_impl.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE	g_instance{nullptr};				// current instance
HWND		g_main_wnd{nullptr};				// main wnd

HBITMAP		g_toolbar_bitmap{nullptr};			// DeletObject
HWND		g_toolbar_wnd{nullptr};
HIMAGELIST	g_image_list{nullptr};				// call ImageList_Destroy

HWND		g_field_wnd{nullptr};

WCHAR		szTitle[MAX_LOADSTRING];			// The title bar text
WCHAR		szWindowClass[MAX_LOADSTRING];		// the main window class name
WCHAR		field_window_class[]{L"field_window_class"};

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HWND create_toolbar(HWND parent_wnd);

LRESULT CALLBACK field_window_proc(HWND, UINT, WPARAM, LPARAM);
ATOM register_field_window_class(HINSTANCE instance);
HWND create_field_window(HWND parent_wnd);

//---------------------------------------------------------------------------------------------------------
mct::data::field	g_field;
std::future<void>	g_fut;
std::atomic<bool>	g_running{false};
std::atomic<bool>	g_inited{false};


//---------------------------------------------------------------------------------------------------------
void init()
{
	assert(!g_inited);
	srand(static_cast<unsigned int>(time(nullptr)));
	mct::data::microte_impl::a001::microte_impl::populate(g_field);
	g_inited = true;
}
//---------------------------------------------------------------------------------------------------------
void run()
{
	if (!g_inited)
	{
		init();
	}
	while (g_running)
	{
		g_field.cycle();
		InvalidateRect(g_field_wnd, nullptr, FALSE);
	}
}
//---------------------------------------------------------------------------------------------------------
void start()
{
	if (g_fut.valid())
	{
		return;
	}
	g_running = true;
	g_fut = std::async(std::launch::async, [](){ run(); });
}
//---------------------------------------------------------------------------------------------------------
void stop()
{
	g_running = false;
	if (g_fut.valid())
	{
		g_fut.get();
	}
}
//---------------------------------------------------------------------------------------------------------
void step()
{
	if (g_fut.valid())
	{
		return;
	}
	if (!g_inited)
	{
		init();
	}
	g_field.cycle();
	InvalidateRect(g_field_wnd, nullptr, FALSE);
}
//---------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPWSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//mct::graphics graphics_instance_;

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MICROTES, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	register_field_window_class(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL accel_table{LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MICROTES))};
	MSG msg{};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, accel_table, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return static_cast<int>(msg.wParam);
}
//---------------------------------------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MICROTES));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	//wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.hbrBackground  = (HBRUSH)(CreateSolidBrush(RGB(0, 0, 0))); // TODO: share brush, cleanup
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MICROTES);
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
//---------------------------------------------------------------------------------------------------------
ATOM register_field_window_class(HINSTANCE instance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style          = 0;
	wcex.lpfnWndProc    = field_window_proc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = instance;
	wcex.hIcon          = nullptr;
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(CreateSolidBrush(RGB(0, 0, 0))); // TODO: share brush, cleanup
	wcex.lpszMenuName   = nullptr;
	wcex.lpszClassName  = field_window_class;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
//---------------------------------------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_instance = hInstance; // Store instance handle in our global variable

   g_main_wnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
	  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!g_main_wnd)
   {
	  return FALSE;
   }

   g_toolbar_wnd = create_toolbar(g_main_wnd);
   g_field_wnd = create_field_window(g_main_wnd);

   ShowWindow(g_main_wnd, nCmdShow);
   UpdateWindow(g_main_wnd);

   return TRUE;
}
//---------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case ID_START:
				start();
				break;
			case ID_STOP:
				stop();
				break;
			case ID_STEP:
				step();
				break;
			case IDM_ABOUT:
				DialogBox(g_instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				stop();
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	//case WM_PAINT:
	//	{
	//		PAINTSTRUCT ps;
	//		HDC hdc{BeginPaint(hWnd, &ps)};
	//		g_field.draw(hdc);
	//		EndPaint(hWnd, &ps);
	//	}
	//	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------------
LRESULT CALLBACK field_window_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc{BeginPaint(hWnd, &ps)};
			g_field.draw(hdc);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------------------------------
HWND create_toolbar(HWND parent_wnd)
{
	constexpr int image_list_id{0};
	constexpr int num_buttons{3};
	constexpr int bitmap_size{16};
	
	constexpr DWORD button_styles{BTNS_AUTOSIZE};

	HWND toolbar_wnd{CreateWindowEx(0, TOOLBARCLASSNAME, nullptr,
									WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0,
									parent_wnd, nullptr, g_instance, nullptr)};

	if (!toolbar_wnd)
	{
		return nullptr;
	}

	g_image_list = ImageList_Create(bitmap_size, bitmap_size,	// Dimensions of individual bitmaps.
									ILC_COLOR4 | ILC_MASK,		// Ensures transparent background.
									num_buttons, 0);

	g_toolbar_bitmap = LoadBitmap(g_instance, MAKEINTRESOURCE(IDB_TOOLBAR));
	assert(g_toolbar_bitmap);

	ImageList_Add(g_image_list, g_toolbar_bitmap, nullptr);

	SendMessage(toolbar_wnd, TB_SETIMAGELIST, (WPARAM)image_list_id, (LPARAM)g_image_list);

	TBBUTTON buttons[num_buttons] = 
	{
		{ MAKELONG(0, image_list_id), ID_START,	TBSTATE_ENABLED, button_styles, {0}, 0, (INT_PTR)L"Start" },
		{ MAKELONG(1, image_list_id), ID_STEP,	TBSTATE_ENABLED, button_styles, {0}, 0, (INT_PTR)L"Step"  },
		{ MAKELONG(2, image_list_id), ID_STOP,	TBSTATE_ENABLED, button_styles, {0}, 0, (INT_PTR)L"Stop"  }
	};

	SendMessage(toolbar_wnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(toolbar_wnd, TB_ADDBUTTONS, (WPARAM)num_buttons, (LPARAM)&buttons);

	// Resize the toolbar, and then show it.
	SendMessage(toolbar_wnd, TB_AUTOSIZE, 0, 0); 
	ShowWindow(toolbar_wnd, TRUE);
	
	return toolbar_wnd;
}
//---------------------------------------------------------------------------------------------------------
HWND create_field_window(HWND parent_wnd)
{
	HWND wnd{CreateWindowExW(0, field_window_class, nullptr, WS_CHILD,
		0, 56,
		static_cast<int>(mct::config().field_size().x() * mct::config().green_ball_size().x()),
		static_cast<int>(mct::config().field_size().y() * mct::config().green_ball_size().y()),
		parent_wnd, nullptr, g_instance, nullptr)};
	ShowWindow(wnd, TRUE);
	return wnd;
}
