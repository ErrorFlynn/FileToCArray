#include "ftca_gui.hpp"
#include <Windows.h>

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PSTR, _In_ int)
{
	ftca_form fm;
	fm.show();
	nana::exec();
}