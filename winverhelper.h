#ifndef _WINVERHELPER_H
#define _WINVERHELPER_H

/*       In loving memory of @pixo $oft       */
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOKERNEL
#define NOMEMMGR
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NODRAWTEXT
#define NOWINSTYLES
#define NOSHOWWINDOW
#define NOWINMESSAGES
#define NOWH

#include <Windows.h>
#include "config.h"
#include "strhelper.h"

enum class WinVerCore : unsigned char {
	Core50,
	Core51,
	Core52,
	Core60,
	Core61,
	Core62,
	Core63,
	Core100,
	CoreUnknown
};

enum class WinVerAdvanced : unsigned char {
	Win2000,
	WinSrv2000,
	WinXP,
	WinXP64,
	WinSrv2003,
	WinSrv2003R2,
	WinVista,
	WinSrv2008,
	WinSrv2008R2,
	Win7,
	WinSrv2012,
	Win8,
	WinSrv2012R2,
	Win81,
	Win10,
	Win11,
	WinSrv2016,
	WinSrv2019,
	WinSrv2022,
	WinVerUnknown
};

WinVerCore getWinVersionCore();
WinVerAdvanced getWinVersionAdvanced();
std::string getWinVersionStr();
std::wstring getWinVersionWStr();
std::string getWinVersionTextStr();
std::wstring getWinVersionTextWStr();

#endif // _WINVERHELPER_H