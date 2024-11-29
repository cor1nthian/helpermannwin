#include "winverhelper.h"

WinVerCore getWinVersionCore() {
	auto sharedUserData = (unsigned char*)0x7FFE0000;
	char winver[16] = { 0 };
	sprintf(winver, "%d.%d.%d",
		*(unsigned long*)(sharedUserData + 0x26c),  // major version offset
		*(unsigned long*)(sharedUserData + 0x270),  // minor version offset
		*(unsigned long*)(sharedUserData + 0x260)); // build number offset);
	if(startsWith(winver, "5.0")) {
		return WinVerCore::Core50;
	} else if (startsWith(winver, "5.1")) {
		return WinVerCore::Core51;
	} else if (startsWith(winver, "5.2")) {
		return WinVerCore::Core52;
	} else if (startsWith(winver, "6.0")) {
		return WinVerCore::Core60;
	} else if (startsWith(winver, "6.1")) {
		return WinVerCore::Core61;
	} else if (startsWith(winver, "6.2")) {
		return WinVerCore::Core62;
	} else if (startsWith(winver, "6.3")) {
		return WinVerCore::Core63;
	} else if (startsWith(winver, "10.0")) {
		return WinVerCore::Core100;
	} else {
		return WinVerCore::CoreUnknown;
	}
}

WinVerAdvanced getWinVersionAdvanced() {
	OSVERSIONINFOEX verinfo;
	memset(&verinfo, 0, sizeof(OSVERSIONINFOEX));
	verinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	::GetVersionEx((LPOSVERSIONINFO) &verinfo);
	auto sharedUserData = (unsigned char*)0x7FFE0000;
	char winver[16] = { 0 };
	sprintf(winver, "%d.%d.%d",
		*(unsigned long*)(sharedUserData + 0x26c),  // major version offset
		*(unsigned long*)(sharedUserData + 0x270),  // minor version offset
		*(unsigned long*)(sharedUserData + 0x260)); // build number offset);
	if (startsWith(winver, "5.0")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			return WinVerAdvanced::WinSrv2000;
		} else {
			return WinVerAdvanced::Win2000;
		}
	} else if (startsWith(winver, "5.1")) {
		return WinVerAdvanced::WinXP;
	} else if (startsWith(winver, "5.2")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			return WinVerAdvanced::WinSrv2003;
		} else {
			return WinVerAdvanced::WinXP64;
		}
	} else if (startsWith(winver, "6.0")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			return WinVerAdvanced::WinSrv2008;
		} else {
			return WinVerAdvanced::WinVista;
		}
	} else if (startsWith(winver, "6.1")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			return WinVerAdvanced::WinSrv2008R2;
		} else {
			return WinVerAdvanced::Win7;
		}
	} else if (startsWith(winver, "6.2")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			return WinVerAdvanced::WinSrv2012;
		} else {
			return WinVerAdvanced::Win8;
		}
	} else if (startsWith(winver, "6.3")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			return WinVerAdvanced::WinSrv2012R2;
		} else {
			return WinVerAdvanced::Win81;
		}
	} else if (startsWith(winver, "10.0")) {
		unsigned int buildVer = std::stoi(splitStr(std::string(winver), ".").back());
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			if((buildVer >= 14393) && (buildVer < 17763)) {
				return WinVerAdvanced::WinSrv2016;
			} else if ((buildVer >= 17763) && (buildVer < 20348)) {
				return WinVerAdvanced::WinSrv2019;
			} else if (buildVer >= 20348) {
				return WinVerAdvanced::WinSrv2022;
			}
		} else {
			if (buildVer >= 21996) {
				return WinVerAdvanced::Win11;
			} else {
				return WinVerAdvanced::Win10;
			}
		}
	} else {
		return WinVerAdvanced::WinVerUnknown;
	}
	return WinVerAdvanced::WinVerUnknown;
}

std::string getWinVersionStr() {
	auto sharedUserData = (unsigned char*)0x7FFE0000;
	char winver[16] = { 0 };
	sprintf(winver, "%d.%d.%d",
		*(unsigned long*)(sharedUserData + 0x26c),
		*(unsigned long*)(sharedUserData + 0x270),
		*(unsigned long*)(sharedUserData + 0x260));
	return winver;
}

std::wstring getWinVersionWStr() {
	auto sharedUserData = (unsigned char*)0x7FFE0000;
	wchar_t winver[16] = { 0 };
	wsprintf(winver, L"%d.%d.%d",
		*(unsigned long*)(sharedUserData + 0x26c),
		*(unsigned long*)(sharedUserData + 0x270),
		*(unsigned long*)(sharedUserData + 0x260));
	return winver;
}

std::string getWinVersionTextStr() {
	OSVERSIONINFOEX verinfo;
	memset(&verinfo, 0, sizeof(OSVERSIONINFOEX));
	verinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	::GetVersionEx((LPOSVERSIONINFO)&verinfo);
	auto sharedUserData = (unsigned char*)0x7FFE0000;
	char winver[16] = { 0 };
	std::string verTxtPart;
	sprintf(winver, "%d.%d.%d",
		*(unsigned long*)(sharedUserData + 0x26c),  // major version offset
		*(unsigned long*)(sharedUserData + 0x270),  // minor version offset
		*(unsigned long*)(sharedUserData + 0x260)); // build number offset);
	if (startsWith(winver, "5.0")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			verTxtPart = "Windows Server 2000?";
		} else {
			verTxtPart = "Windows 2000?";
		}
	} else if (startsWith(winver, "5.1")) {
		verTxtPart = "Windows XP?";
	} else if (startsWith(winver, "5.2")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			verTxtPart = "Windows Server 2003?";
		} else {
			verTxtPart = "Windows XP 64-bit?";
		}
	} else if (startsWith(winver, "6.0")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			verTxtPart = "Windows Server 2008?";
		} else {
			verTxtPart = "Windows Vista?";
		}
	} else if (startsWith(winver, "6.1")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			verTxtPart = "Windows Server 2003R2?";
		} else {
			verTxtPart = "Windows 7?";
		}
	} else if (startsWith(winver, "6.2")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			verTxtPart = "Windows Server 2012?";
		} else {
			verTxtPart = "Windows 8?";
		}
	} else if (startsWith(winver, "6.3")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			verTxtPart = "Windows Server 2012R2?";
		} else {
			verTxtPart = "Windows Server 8.1?";
		}
	} else if (startsWith(winver, "10.0")) {
		unsigned int buildVer = std::stoi(splitStr(std::string(winver), ".").back());
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			if ((buildVer >= 14393) && (buildVer < 17763)) {
				verTxtPart = "Windows Server 2016?";
			} else if ((buildVer >= 17763) && (buildVer < 20348)) {
				verTxtPart = "Windows Server 2019?";
			} else if (buildVer >= 20348) {
				verTxtPart = "Windows Server 2022?";
			}
		} else {
			if (buildVer >= 21996) {
				verTxtPart = "Windows 11?";
			} else {
				verTxtPart = "Windows 10?";
			}
			verTxtPart = "Windows 10?";
		}
	}
	return verTxtPart + winver;
}

std::wstring getWinVersionTextWStr() {
	OSVERSIONINFOEX verinfo;
	memset(&verinfo, 0, sizeof(OSVERSIONINFOEX));
	verinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	::GetVersionEx((LPOSVERSIONINFO)&verinfo);
	auto sharedUserData = (unsigned char*)0x7FFE0000;
	wchar_t winver[16] = { 0 };
	std::wstring verTxtPart;
	wsprintf(winver, L"%d.%d.%d",
		*(unsigned long*)(sharedUserData + 0x26c),  // major version offset
		*(unsigned long*)(sharedUserData + 0x270),  // minor version offset
		*(unsigned long*)(sharedUserData + 0x260)); // build number offset);
	if (startsWith(winver, L"5.0")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			verTxtPart = L"Windows Server 2000?";
		} else {
			verTxtPart = L"Windows 2000?";
		}
	} else if (startsWith(winver, L"5.1")) {
		verTxtPart = L"Windows XP?";
	} else if (startsWith(winver, L"5.2")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			verTxtPart = L"Windows Server 2003?";
		} else {
			verTxtPart = L"Windows XP 64-bit?";
		}
	} else if (startsWith(winver, L"6.0")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			verTxtPart = L"Windows Server 2008?";
		} else {
			verTxtPart = L"Windows Vista?";
		}
	} else if (startsWith(winver, L"6.1")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			verTxtPart = L"Windows Server 2003R2?";
		} else {
			verTxtPart = L"Windows 7?";
		}
	} else if (startsWith(winver, L"6.2")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			verTxtPart = L"Windows Server 2012?";
		} else {
			verTxtPart = L"Windows 8?";
		}
	} else if (startsWith(winver, L"6.3")) {
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			verTxtPart = L"Windows Server 2012R2?";
		} else {
			verTxtPart = L"Windows Server 8.1?";
		}
	} else if (startsWith(winver, L"10.0")) {
		unsigned int buildVer = std::stoi(wstr2str(splitStr(std::wstring(winver), L".").back()));
		if ((verinfo.wProductType == VER_NT_SERVER) ||
			(verinfo.wProductType == VER_NT_DOMAIN_CONTROLLER)) {
			if ((buildVer >= 14393) && (buildVer < 17763)) {
				verTxtPart = L"Windows Server 2016?";
			} else if ((buildVer >= 17763) && (buildVer < 20348)) {
				verTxtPart = L"Windows Server 2019?";
			} else if (buildVer >= 20348) {
				verTxtPart = L"Windows Server 2022?";
			}
		} else {
			if (buildVer >= 21996) {
				verTxtPart = L"Windows 11?";
			} else {
				verTxtPart = L"Windows 10?";
			}
			verTxtPart = L"Windows 10?";
		}
	}
	return verTxtPart + winver;
}