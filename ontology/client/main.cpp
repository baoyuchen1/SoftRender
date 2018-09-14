//#include "/ontology/ontology/engine/render/base.h"
//#include"/ontology/ontology/engine/render/draw.h"
//#include"/ontology/ontology/engine/render/base.h"
//#include"/ontology/ontology/engine/render/camera.h"
#include"../engine/base.h"
#include"../engine/render/draw.h"
#include"../engine/render/camera.h"
#include"../engine/resource/initx3d.h"
 int screen_w, screen_h, screen_exit = 0;
int screen_mx = 0, screen_my = 0, screen_mb = 0;
int screen_keys[512];	// 当前键盘按下状态
static HWND screen_handle;		// 主窗口 HWND
static HDC screen_dc;		// 配套的 HDC
static HBITMAP screen_hb;		// DIB
static HBITMAP screen_ob;		// 老的 BITMAP
unsigned char *screen_fb;		// frame buffer
long screen_pitch = 0;
int screen_init(int w, int h, const TCHAR *title);
int screen_close(void);								// 
void screen_dispatch(void);							// handle message
void screen_update(void);							//show FrameBuffer

// win32 event handler
static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);
static LRESULT screen_events(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE: screen_exit = 1; break;
	case WM_KEYDOWN: screen_keys[wParam & 511] = 1; break;
	case WM_KEYUP: screen_keys[wParam & 511] = 0; break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#endif



int screen_init(int w, int h, const TCHAR *title) {
	WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)screen_events, 0, 0, 0,
		NULL, NULL, NULL, NULL, _T("SCREEN3.1415926") };
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,
		w * h * 4, 0, 0, 0, 0 } };
	RECT rect = { 0, 0, w, h };
	int wx, wy, sx, sy;
	LPVOID ptr;
	HDC hDC;

	screen_close();

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wc)) return -1;

	screen_handle = CreateWindow(_T("SCREEN3.1415926"), title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	if (screen_handle == NULL) return -2;

	screen_exit = 0;
	hDC = GetDC(screen_handle);
	screen_dc = CreateCompatibleDC(hDC);
	ReleaseDC(screen_handle, hDC);

	screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	if (screen_hb == NULL) return -3;

	screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);
	screen_fb = (unsigned char*)ptr;
	screen_w = w;
	screen_h = h;
	screen_pitch = w * 4;

	AdjustWindowRect(&rect, GetWindowLong(screen_handle, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(screen_handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(screen_handle);

	ShowWindow(screen_handle, SW_NORMAL);
	screen_dispatch();

	memset(screen_keys, 0, sizeof(int) * 512);
	memset(screen_fb, 0, w * h * 4);

	return 0;
}

int screen_close(void)
{
	if (screen_dc) {
		if (screen_ob) {
			SelectObject(screen_dc, screen_ob);
			screen_ob = NULL;
		}
		DeleteDC(screen_dc);
		screen_dc = NULL;
	}
	if (screen_hb) {
		DeleteObject(screen_hb);
		screen_hb = NULL;
	}
	if (screen_handle) {
		CloseWindow(screen_handle);
		screen_handle = NULL;
	}
	return 0;
}

void screen_dispatch(void)
{
	MSG msg;
	while (1) {
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

void screen_update(void)
{
	HDC hDC = GetDC(screen_handle);
	BitBlt(hDC, 0, 0, screen_w, screen_h, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(screen_handle, hDC);
	screen_dispatch();
}

void init_object(object::object_ptr obj) {

}

void init_rendlist(object::renderList_type* rend_list) {
	vector4* a = new vector4(100, 200, 0, 0);
	vector4* b = new vector4(300, 200, 0, 0);
	vector4* c = new vector4(200, 300, 0, 0);
	object::polyrendlist_ptr h = new object::polyrendlist(*a, *b, *c);
	h->color.a = 100;
	h->color.b = 100;
	h->color.g = 100;
	h->color.r = 100;
	h->state = 1;
	rend_list->poly_ptrs.push_back(h);
}



int main(void)
{
	device* draw_instance = new device();
	resource::x3d* x3d_p = new resource::x3d();
//	int states[] = { RENDER_STATE_TEXTURE, RENDER_STATE_COLOR, RENDER_STATE_WIREFRAME };
	TCHAR *title = _T("niconiconi");
	
	if (screen_init(800, 600, title))
		return -1;
	draw_instance->draw_init(800, 600, screen_fb);
	object::renderList_type* pp = new object::renderList_type();
	x3d_p->init_renderlist("s.x3d", (*pp));
	COLOR t;
	t.r = 500;
	t.g = 122;
	t.b = 50;
	while (true) {
		screen_dispatch();
		for (int  x = 0; x < 500; x++)
		{
			for (int y = 0; y < 500; y++)
			{
				draw_instance->draw_pixel(x, y, t);
			}
		}
		screen_update();
		Sleep(100);
	}
	return 0;
}
