// DLL Main, the master brain

/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details. */

// Thanks: http://www.uc-forum.com/forum/visual-basic/38132-writeprocessmemory-alternative.html

#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <io.h>

//Offsets
BYTE* b_inGame = (BYTE*)0x0044DD98; //Are you in-game?

//Slot 2 only atm
int* i_Cash = (int*)0x004503F8;
int* i_Colony = (int*)0x00482FB8;
BYTE* b_Speed = (BYTE*)0x0044DD90; //Speed, but I had some trouble editing it...
int* i_Speed = (int*)0x00482FA0; //Real speed.
int* i_Charge = (int*)0x00482FAC;
BYTE* b_Charge = (BYTE*)0x0045042C;
BYTE* b_Shields = (BYTE*)0x00450410;
int* i_Energy = (int*)0x00482FA4;
BYTE* b_Energy = (BYTE*)0x00450428;
int* i_Cargo = (int*)0x004503E0;
BYTE* b_Cargo = (BYTE*)0x00450424;
//BYTE* b_NukeAmmo = (BYTE*)0x00450408;
//int* i_NukeEnable = (int*)0x0049B7D8; //3 is to activate nukes.
BYTE* b_Deaths = (BYTE*)0x004505AC;
BYTE* b_Dead = (BYTE*)0x00482F8C; //Are you dead?

//Offset variables
int ingame = 0; //Initially we aren't in-game
int charge_e = 0; //Charge, shield, etc. Enabled?
int shield_e = 0;
int energy_e = 0;
int cargo_e = 0;
int lastDeath = 0; //Last death count

FILE * BOYNEXTDOOR; //Config

int customconfig = 0;
int extradosh = 0;

//Code variables
short keys[256];
HANDLE hInput, hOutput;
int i;
int checkdelay = 500;
int inGameCheck = 0;
int firsttime = 1;
int iSlot;
int iSpeed;

void GreenText() //When everything is a-ok
{
	SetConsoleTextAttribute(hOutput, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

void RedText() //When something fucks up
{
	SetConsoleTextAttribute(hOutput, FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void BlueText() //When someone activates a cheat
{
	SetConsoleTextAttribute(hOutput, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

void WhiteText()
{
	SetConsoleTextAttribute(hOutput, 8);
}

void GetRichFast() //Add 500000 Cash
{
	if (!extradosh)
		*i_Cash = *i_Cash + 500000;
	else if (extradosh)
		*i_Cash = *i_Cash + 5000000;
	BlueText(); //Cheater!
	fprintf(stdout, "Money Added. -- Current Dosh: %d\n", *i_Cash);
	WhiteText();
}

void AddSomeGuys() //Add 5000 People
{
	*i_Colony = *i_Colony + 5000;
	BlueText(); //Cheater!
	fprintf(stdout, "People Added. -- Current People: %d\n", *i_Colony);
	WhiteText();
}

void FirstTime()
{
	fprintf(stdout, "What Slot are you?\n(Wait until you are in-screen, SP is always 2)\n");
	scanf("%d", &iSlot);
	if (iSlot == NULL || iSlot == 0 || iSlot > 8)
	{
		fprintf(stdout, "Invalid, defaulting to 2.\n");
		iSlot = 2;
	}
}

void DikklanLoop()
{
	if (firsttime) {
		fprintf(stdout, "Warpath97 - Mix Server Hack Console Loaded!\n========================================\n\n");
		fprintf(stdout, "====  NUMPAD0 - Show the key bindings  ====\n");
		WhiteText(); //Everything went ok!
		FirstTime();
		if (!customconfig)
		{
			BOYNEXTDOOR = fopen("config.cfg", "r+");
			if (BOYNEXTDOOR == NULL)
			{
				RedText();
				fprintf(stdout, "config.cfg failed to load. Either it's missing or lost.\nDefault config loaded.\n");
				WhiteText();
				fputs("NOP", BOYNEXTDOOR);
				fclose(BOYNEXTDOOR);
				customconfig = 1;
			}
			else
			{
				fprintf(stdout, "config.cfg loaded.\n");
				char Buffer[3];
				fgets(Buffer, 3, BOYNEXTDOOR);
				if (strstr(Buffer, "YES"))
				{
					fprintf(stdout, "CONFIG: The player does want extra cash per hit!\n");
					extradosh = 1;
				}
				else if (strstr(Buffer, "NOP"))
				{
					fprintf(stdout, "CONFIG: The player does not want extra cash per hit!\n");
				}
				fclose(BOYNEXTDOOR);
				customconfig = 1;
			}
		}
		firsttime = 0;
	}

	for (;;){
		if (GetAsyncKeyState(VK_NUMPAD0))
		{
			fprintf(stdout, "\n====  K E Y   B I N D I N G S  ====\n");
			if (!extradosh)
				fprintf(stdout, "NUMPAD 1 - Add 500000 to Cash!\n");
			else if (extradosh)
				fprintf(stdout, "NUMPAD 1 - Add 5000000 to Cash!\n");
			fprintf(stdout, "NUMPAD 2 - Add 5000 people onto your ship\n");
			fprintf(stdout, "NUMPAD 3 - Infinite Charge\n");
			fprintf(stdout, "NUMPAD 4 - Infinite Shields\n");
			fprintf(stdout, "NUMPAD 5 - Infinite Energy\n");
			fprintf(stdout, "NUMPAD 6 - Infinite Cargo -- Go Green!\n");
			fprintf(stdout, "NUMPAD 7 - Sonic Mode -- Custom Speed.\n\n");
			Sleep(300);
		}
		else if (GetAsyncKeyState(VK_NUMPAD1)) {
			fprintf(stdout, "Player hit Dosh Key! -- Current Dosh: %d\n", *i_Cash);
			int msgboxID = MessageBoxA(0, "Do you want to add 500000 to your cash?", "Get-Rich-Scam Confirmation", MB_YESNO | MB_ICONWARNING);

			switch (msgboxID)
			{
			case IDNO:
				fprintf(stdout, "Player decided not to use the Cash cheat.\n");
				break;
			case IDYES:
				GetRichFast();
				break;
			}
			Sleep(18);
		}
		else if (GetAsyncKeyState(VK_NUMPAD2)) {
			fprintf(stdout, "Player hit Colony Key! -- Current People: %d\n", *i_Colony);
			int msgboxID = MessageBoxA(0, "Do you want to add 5000 people to your ship?", "People Cloning Machine Confirmation", MB_YESNO | MB_ICONWARNING);

			switch (msgboxID)
			{
			case IDNO:
				fprintf(stdout, "Player decided not to add people to his ship.\n");
				break;
			case IDYES:
				AddSomeGuys();
				break;
			}
			Sleep(18);
		}
		else if (GetAsyncKeyState(VK_NUMPAD3)) {
			fprintf(stdout, "Player hit Infinite Charge Key!\n");
			if (!charge_e)
			{
				int msgboxID = MessageBoxA(0, "Do you want to shoot forever?", "Forever Shooting Confirmation", MB_YESNO | MB_ICONWARNING);

				switch (msgboxID)
				{
				case IDNO:
					fprintf(stdout, "Player decided not to shoot forever.\n");
					break;
				case IDYES:
					BlueText(); //Cheater!
					fprintf(stdout, "Infinite charge enabled.\n");
					charge_e = 1;
					WhiteText();
					break;
				}
				Sleep(18);
			}
			else if (charge_e)
			{
				int msgboxID = MessageBoxA(0, "Do you want to go back to shooting normally?", "Reverted Shooting Confirmation", MB_YESNO | MB_ICONWARNING);

				switch (msgboxID)
				{
				case IDNO:
					fprintf(stdout, "Player decided not to shoot normally again.\n");
					break;
				case IDYES:
					fprintf(stdout, "Infinite charge disabled.\n");
					charge_e = 0;
					break;
				}
				Sleep(18);
			}
		}
		else if (GetAsyncKeyState(VK_NUMPAD4)) {
			fprintf(stdout, "Player hit Infinite shield Key!\n");
			if (!shield_e)
			{
				int msgboxID = MessageBoxA(0, "Do you want to put up an AT-Field?", "AT Field Confirmation", MB_YESNO | MB_ICONWARNING);

				switch (msgboxID)
				{
				case IDNO:
					fprintf(stdout, "Player decided not put up an AT-Field.\n");
					break;
				case IDYES:
					BlueText(); //Cheater!
					fprintf(stdout, "AT Field enabled.\n");
					shield_e = 1;
					WhiteText();
					break;
				}
				Sleep(18);
			}
			else if (shield_e)
			{
				int msgboxID = MessageBoxA(0, "Do you want to disabled the AT-Field?", "AT Field-Disable Confirmation", MB_YESNO | MB_ICONWARNING);

				switch (msgboxID)
				{
				case IDNO:
					fprintf(stdout, "Player decided not to disable his AT Field.\n");
					break;
				case IDYES:
					fprintf(stdout, "Infinite shield disabled.\n");
					shield_e = 0;
					break;
				}
				Sleep(18);
			}
		}
		else if (GetAsyncKeyState(VK_NUMPAD5)) {
			fprintf(stdout, "Player hit Infinite Energy Key!\n");
			if (!energy_e)
			{
				int msgboxID = MessageBoxA(0, "Do you want to be up forever?", "IM AWAKE FOREVER Confirmation", MB_YESNO | MB_ICONWARNING);

				switch (msgboxID)
				{
				case IDNO:
					fprintf(stdout, "Player decided not have all the energy.\n");
					break;
				case IDYES:
					BlueText(); //Cheater!
					fprintf(stdout, "Infinite energy enabled.\n");
					energy_e = 1;
					WhiteText();
					break;
				}
				Sleep(18);
			}
			else if (energy_e)
			{
				int msgboxID = MessageBoxA(0, "Do you want to go back to mortality?", "Mortal Confirmation", MB_YESNO | MB_ICONWARNING);

				switch (msgboxID)
				{
				case IDNO:
					fprintf(stdout, "Player decided not to become mortal.\n");
					break;
				case IDYES:
					fprintf(stdout, "Infinite energy disabled.\n");
					energy_e = 0;
					break;
				}
				Sleep(18);
			}
		}
		else if (GetAsyncKeyState(VK_NUMPAD6)) {
			fprintf(stdout, "Player hit Infinite Cargo Key!\n");
			if (!cargo_e)
			{
				int msgboxID = MessageBoxA(0, "Do you want to Pack cargo?", "Cargo Confirmation", MB_YESNO | MB_ICONWARNING);

				switch (msgboxID)
				{
				case IDNO:
					fprintf(stdout, "Player decided not go green.\n");
					break;
				case IDYES:
					GreenText();
					fprintf(stdout, "Infinite cargo enabled, go green!.\n");
					cargo_e = 1;
					WhiteText();
					break;
				}
				Sleep(18);
			}
			else if (cargo_e)
			{
				int msgboxID = MessageBoxA(0, "Do you want to ruin the environment?", "Greenhouse Gases Confirmation", MB_YESNO | MB_ICONWARNING);

				switch (msgboxID)
				{
				case IDNO:
					fprintf(stdout, "Player decided not to ruin the environment.\n");
					break;
				case IDYES:
					fprintf(stdout, "Infinite cargo disabled.\n");
					cargo_e = 0;
					break;
				}
				Sleep(18);
			}
		}
		else if (GetAsyncKeyState(VK_NUMPAD7)) {
			fprintf(stdout, "Player hit Speed Button!\n");
			fprintf(stdout, "What speed do you want? (1-1000) (Recommended: 100~)\n");
			scanf("%d", &iSpeed);
			if (iSpeed == NULL || iSpeed > 1000)
			{
				fprintf(stdout, "Invalid, defaulting to 3.\n");
				iSpeed = 3;
			}
			*i_Speed = iSpeed;
		}


		if (lastDeath < *b_Deaths)
		{
			RedText();
			fprintf(stdout, "Player died.\n");
			lastDeath = *b_Deaths;
			WhiteText();
		}

		if (charge_e)
		{
			*b_Charge = 5;
			*i_Charge = 5999;
		}

		if (energy_e)
		{
			*b_Energy = 5;
			*i_Energy = 5999;
		}

		if (shield_e)
		{
			*b_Shields = 5;
		}

		if (cargo_e)
		{
			*b_Cargo = 5;
			*i_Cargo = 5999;
		}

		inGameCheck++;
		if (inGameCheck > 100)
		{
			if (*b_inGame && !ingame)
			{
				GreenText();
				fprintf(stdout, "\nYou are now playing in a game!\nSlot: %d\n", iSlot);
				WhiteText();
				ingame = 1;
			}
			else if (!*b_inGame && ingame)
			{
				RedText();
				fprintf(stdout, "\nGame Finished.\nDisabling Infinite Cheats\n");
				WhiteText();
				energy_e = 0;
				shield_e = 0;
				charge_e = 0;
				cargo_e = 0;
				lastDeath = 0;
				ingame = 0;
			}
			inGameCheck = 0;
		}
		Sleep(30); //100% CPU killer
	}
}

DWORD WINAPI HeloBrasil(LPVOID lpParam)
{
	Sleep(1200);
	MessageBoxA(0, "We goona huge hack!\nNow Great ready for the MADNESS\nThanks Bros, Dethklan 2012", "DLL Injected Successfully!", 0);
	AllocConsole();
	hInput = GetStdHandle(STD_INPUT_HANDLE);
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	int iConHandle = _open_osfhandle((int)hOutput, 0);
	*stdout = *_fdopen(iConHandle, "w");

	iConHandle = _open_osfhandle((int)hInput, 0);
	*stdin = *_fdopen(iConHandle, "r");

	DikklanLoop();
	return TRUE;

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD HelloThere, LPVOID lpVoid)
{
	if (HelloThere == DLL_PROCESS_ATTACH) {
		CreateThread(NULL, 0, &HeloBrasil, NULL, 0, NULL);
	}
	return TRUE;
}