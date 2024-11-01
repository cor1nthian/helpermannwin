#ifndef _HWGETTER_H
#define _HWGETTER_H

#include "config.h"
#include "strhelper.h"
#include "winerrhelper.h"

#pragma comment(lib, "setupapi.lib")

DEFINE_GUID(GUID_DEVINTERFACE_USB_DISK, 0x53f56307L, 0xb6bf, 0x11d0, 0x94, 0xf2,0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);

const std::wstring const gc_hwEmptyval		= L"(null)";
const std::wstring const gc_hwFriendlyName	= L"Friendly name:";
const std::wstring const gc_hwDevInstanceID	= L"DeviceInstanceId:";
const std::wstring const gc_hwClass			= L"Class:";
const std::wstring const gc_hwClassGUID		= L"Class GUID:";

std::vector<std::wstring> HW_GetHardDrives();
std::vector<std::wstring> HW_EnumAllDevices();

#endif