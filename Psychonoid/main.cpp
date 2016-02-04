
#include "ArcanoidApp.h"
#if defined(WINDOWS) || defined(WIN32) || defined(_WIN32)
#include<windows.h>
#endif

// WinMain
#if defined(WINDOWS) || defined(WIN32) || defined(_WIN32)
int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR str, int show) {
#else
int main(int argc, const char *argv) {
#endif

	CArcanoidApp *Arcanoid = new CArcanoidApp();
	Arcanoid->Run();

	return 0;

}
