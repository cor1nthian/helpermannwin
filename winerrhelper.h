#ifndef _WINERR_HELP_H
#define _WINERR_HELP_H

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

void errorMsgBox(const std::wstring errormsg, const std::wstring msgboxCaption = L"ERROR",
	const unsigned int msgboxIcon = MB_ICONERROR);
void setLastError(const unsigned long code);
unsigned long getLastErrorCode();
std::wstring getErrorDetails(const unsigned long errcode, const std::wstring funcName = L"",
	const std::wstring additionalInfo = L"");

#endif // _WINERR_HELP_H