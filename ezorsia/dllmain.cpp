// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "NMCO.h"
#include "ijl15.h"
#include "INIReader.h"
#include "ReplacementFuncs.h"
#include <comutil.h>
#include "BossHP.h"
#include <Resman.h>
#include <CharacterEx.h>
#include "psapi.h"
#include "ChairRelMove.h"

void CreateConsole() {
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout); //CONOUT$
}

std::string GetCurrentProcessName() {
	char buffer[MAX_PATH];
	DWORD size = GetModuleFileNameA(NULL, buffer, MAX_PATH);
	if (size == 0) {
		return "";
	}

	std::string processPath(buffer);
	size_t pos = processPath.find_last_of('\\');
	if (pos != std::string::npos) {
		return processPath.substr(pos + 1);
	}

	return "";
}

HANDLE hThread;

DWORD GetCurrentMemoryUsage()
{
	MEMORYSTATUSEX MS;
	MS.dwLength = sizeof(MS);
	PROCESS_MEMORY_COUNTERS pmc;
	GlobalMemoryStatusEx(&MS);
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	DWORD CurrentMem = pmc.WorkingSetSize / 1048576;
	DWORD TotalMem = MS.ullTotalPhys / 1048576;
	return CurrentMem;
}

void EmptyMemory()
{
	while (true)
	{
		Sleep(1000);
		int memory = GetCurrentMemoryUsage();
		if (memory >= Client::ResManFlushCached) {
			if (Resman::getIWzResMan())
			{
				Resman::getIWzResMan()->raw_FlushCachedObjects(0);
			}
			//autoFlushCacheTime(0);
			//callFlushcache();
			//autoFlushCacheTime(10000);
			std::cout << "flushcache:" << memory << std::endl;
		}
		if (memory >= Client::SetProcessWorkingSetSize)
		{
			std::cout << "EmptyMemory:" << memory << std::endl;
			SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
		}
	}
};

void Injected() {
	while (!Client::canInjected) {
		Sleep(10);
	}
	std::cout << "GetModuleFileName hook created" << std::endl;
	Hook_StringPool__GetString(true); //hook stringpool modification //ty !! popcorn //ty darter
	Hook_StringPool__GetStringW(true);
	std::string processName = GetCurrentProcessName();
	std::cout << "Current process name: " << processName << std::endl;
	Client::CRCBypass();
	Resman::Hook_InitializeResMan();
	Resman::Hook_InitInlinkOutlink();
	Hook_SetActiveChairRelMove(true);
	CharacterEx::InitExpOverride(Client::longEXP);
	CharacterEx::InitLevelOverride(Client::shortLevel);
	CharacterEx::InitDamageSkinOverride(Client::DamageSkin > 0 || Client::RemoteDamageSkin);
	CharacterEx::InitHypontizeFix(Client::s5221009);
	Client::UpdateGameStartup();
	Client::UpdateResolution();
	//Client::LongQuickSlot();
	Client::FixMouseWheel();
	Client::JumpCap();
	Client::NoPassword();
	BossHP::Hook();
	Client::MoreHook();
	Client::Skill();
	//Hook_CItemInfo__GetItemName(Client::showItemID);
	Hook_CItemInfo__GetItemDesc(Client::showItemID);
	ijl15::CreateHook(); //NMCO::CreateHook();
	hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)EmptyMemory, NULL, 0, 0);
	std::cout << "NMCO hook initialized" << std::endl;
	Client::injected = true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	{
		INIReader reader("config.ini");
		if (reader.ParseError() == 0) {
			Client::m_nGameWidth = reader.GetInteger("general", "width", 1280);
			Client::m_nGameHeight = reader.GetInteger("general", "height", 720);
			Client::MsgAmount = reader.GetInteger("general", "MsgAmount", 26);
			Client::CustomLoginFrame = reader.GetBoolean("general", "CustomLoginFrame", true);
			Client::WindowedMode = reader.GetBoolean("general", "WindowedMode", true);
			Client::RemoveLogos = reader.GetBoolean("general", "RemoveLogos", true);
			Client::RemoveLoginNxIdDialog = reader.GetBoolean("general", "RemoveLoginNxIdDialog", true);
			Memory::UseVirtuProtect = reader.GetBoolean("general", "UseVirtuProtect", true);
			Client::ResManFlushCached = reader.GetInteger("general", "ResManFlushCached", 512);
			Client::SetProcessWorkingSetSize = reader.GetInteger("general", "SetProcessWorkingSetSize", 1024);
			Client::setDamageCap = reader.GetReal("optional", "setDamageCap", 199999);
			Client::setMAtkCap = reader.GetReal("optional", "setMAtkCap", 1999);
			Client::setAccCap = reader.GetReal("optional", "setAccCap", 999);
			Client::setAvdCap = reader.GetReal("optional", "setAvdCap", 999);
			Client::setAtkOutCap = reader.GetReal("optional", "setAtkOutCap", 199999);
			Client::longEXP = reader.GetBoolean("optional", "longEXP", false);
			Client::shortLevel = reader.GetBoolean("optional", "shortLevel", false);
			Client::useTubi = reader.GetBoolean("optional", "useTubi", false);
			Client::bigLoginFrame = reader.GetBoolean("general", "bigLoginFrame", false);
			Client::speedMovementCap = reader.GetInteger("optional", "speedMovementCap", 140);
			Client::jumpCap = reader.GetInteger("optional", "jumpCap", 123);
			Client::debug = reader.GetBoolean("debug", "debug", false);
			Client::noPassword = reader.GetBoolean("debug", "noPassword", false);
			ownLoginFrame = reader.GetBoolean("optional", "ownLoginFrame", false);
			ownCashShopFrame = reader.GetBoolean("optional", "ownCashShopFrame", false);
			EzorsiaV2WzIncluded = reader.GetBoolean("general", "EzorsiaV2WzIncluded", true);
			Client::ServerIP_AddressFromINI = reader.Get("general", "ServerIP_Address", "127.0.0.1");
			Client::ServerIP_Address_hook = reader.GetBoolean("general", "ServerIP_Address_hook", true);
			Client::climbSpeedAuto = reader.GetBoolean("optional", "climbSpeedAuto", false);
			Client::climbSpeed = reader.GetFloat("optional", "climbSpeed", 1.0);
			Client::serverIP_Port = reader.GetInteger("general", "serverIP_Port", 8484);
			Client::talkRepeat = reader.GetBoolean("optional", "talkRepeat", false);
			Client::talkTime = reader.GetInteger("optional", "talkTime", 2000);
			Client::meleePunching = reader.GetBoolean("optional", "meleePunching", true);
			Client::holdAttack = reader.GetBoolean("optional", "holdAttack", false);
			Client::spLimit = reader.GetBoolean("optional", "spLimit", true);
			Client::tamingMobUnlock = reader.GetBoolean("optional", "tamingMobUnlock", false);
			Client::longSlots = reader.GetBoolean("ui", "LongSlots", false);
			Client::longSlotsKey = reader.GetInteger("ui", "longSlotsKey", 1);
			Client::showItemID = reader.GetBoolean("ui", "showItemID", false);
			Client::showWeaponSpeed = reader.GetBoolean("ui", "showWeaponSpeed", true);
			Client::minimizeMaptitleColor = reader.GetBoolean("ui", "minimizeMaptitleColor", false);
			Client::StatDetailBackgrndWidthEx = reader.GetInteger("ui", "StatDetailBackgrndWidthEx", 23);
			Client::DamageSkin = reader.GetInteger("ui", "DamageSkin", 0);
			Client::RemoteDamageSkin = reader.GetBoolean("ui", "RemoteDamageSkin", false);
			Client::tamingMob198Effect = reader.GetBoolean("ui", "tamingMob198", false);
			Client::s4221001 = reader.GetBoolean("skill", "s4221001", false);
			Client::s4221007 = reader.GetBoolean("skill", "s4221007", false);
			Client::s14101004 = reader.GetBoolean("skill", "s14101004", true);
			Client::s5221009 = reader.GetBoolean("skill", "s5221009", false);
		}
		if (Client::debug)
			CreateConsole();	//console for devs, use this to log stuff if you want
		Hook_CreateMutexA(true); //multiclient //ty darter, angel, and alias!
		Hook_gethostbyname(true);
		CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(&Injected), NULL, 0, NULL);
		break;
	}
	default: break;
	case DLL_PROCESS_DETACH:
		if (hThread)
			CloseHandle(hThread);
		ExitProcess(0);
	}
	return TRUE;
}