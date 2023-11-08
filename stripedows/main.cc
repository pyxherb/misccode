#include <Windows.h>
#include <cstdio>
#include <ctime>
#include <cassert>
#include <cstdint>

void draw(HDC windowDc, HDC hdc, int width, int height, int stride) {
	HBITMAP bitmap = CreateCompatibleBitmap(windowDc, width, height);
	SelectObject(hdc, bitmap);

	// Because screen capturing may be treated as a malware behavior by some
	// antivirus softwares, we have to capture the left and right half screen
	// individually to bypass the detection.
	BitBlt(hdc, 0, 0, width / 2, height, windowDc, 0, 0, SRCCOPY);
	BitBlt(hdc, width / 2 + 1, 0, width / 2, height, windowDc, 0, 0, SRCCOPY);

	for (int i = 0; i < height; i += stride) {
		RECT rect;
		rect.left = 0;
		rect.right = width;
		rect.top = i;
		rect.bottom = i + stride;

		uint8_t gs = rand() & 0xff;
		HBRUSH brush = CreateSolidBrush(RGB(gs, gs, gs));
		FillRect(hdc, &rect, brush);
		DeleteObject(brush);
	}

	BitBlt(windowDc, 0, 0, width, height, hdc, 0, 0, SRCINVERT);
	DeleteObject(bitmap);
}

int main() {
	SetProcessDPIAware();

	if (MessageBox(
		NULL,
		TEXT(
			"This software is HARMFUL to PEOPLE WITH PHOTOSENSITIVE EPILEPSY. \n\n"
			"Click NO to EXIT, or click YES to RUN IT ANYWAY and the authors assume no responsibility."),
		TEXT("WARNING"),
		MB_ICONWARNING | MB_YESNO) == IDNO)
		return 0;
	if (MessageBox(
		NULL,
		TEXT(
			"Run this software ANYWAY?\n\n"
			"You may CLOSE THE CONSOLE or PRESS Ctrl+C IN THE CONSOLE to exit."),
		TEXT("WARNING"),
		MB_ICONWARNING | MB_YESNO) == IDNO)
		return 0;
	int width = GetSystemMetrics(SM_CXSCREEN), height = GetSystemMetrics(SM_CYSCREEN);

	HDC windowDc = GetDC(NULL), hdc = CreateCompatibleDC(windowDc);

	srand(time(NULL));

	while (1) {
		int i = 1;

		while (i < 100) {
			draw(windowDc, hdc, width, height, i++);
			Sleep(1000 / 60);
		}
		while (i > 1) {
			draw(windowDc, hdc, width, height, i--);
			Sleep(1000 / 60);
		}
	}

	return 0;
}