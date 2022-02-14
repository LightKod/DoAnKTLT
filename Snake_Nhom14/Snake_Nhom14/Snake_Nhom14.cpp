// Snake_Nhom14.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include<string>
#include <conio.h>
#include <mmsystem.h>
#include "Graphic.h"
#include "Gameplay.h"
#include "GUI.h"
#include "Sound.h"
#include "SaveLoad.h"
#pragma comment(lib, "Winmm.lib")

using namespace std;

int main()
{
	SetUp();
	OpenSound();
	PlayMusic();
	Load();
	//GUI();
	// RunGamePlay();
	// GoToXYPixel(10, 10);
	CloseSound();
	cout << "Hello snake\n";
	// system("pause");
}
