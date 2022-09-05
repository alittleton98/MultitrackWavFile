// ffmx_pack.cpp : Defines the entry point for the application.


#include "ffmx_pack.h"

using namespace std;

int main()
{
	cout << "Welcome to the Full Fidelity Mix Packager (ffmx) " << endl;
	system("pause");
	

	CreateMTWVNative((BYTE)1, 1, 1, (DWORD)48000, 32, "C:\\untitled.mtwv");
	return 0;
} 
