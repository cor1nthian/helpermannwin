#ifndef _CRYPT_H
#define _CRYPT_H

// Microsoft Information Protection (MIP) SDK version release history
// https://learn.microsoft.com/en-us/information-protection/develop/version-release-history
// Full MIP SDK 1.15.107
// https://mega.nz/file/qRNlDRYA#7VHFmM9jVZMt9MrOlp6J1IoSAu1jNdAgYUxkjbxoMyo
// Windows MIP SDK 1.15.107
// https://mega.nz/file/vRFikTDC#ovJ0oOh18YdEy2X5usHgGlV8wfO4IUElg-yOmjq9oVI
// Windows MIP SDK Mini 1.15.107
// https://mega.nz/file/bds1lJ5B#neCmVG6qx7-MzB7I9zTiYsPovL0mqkCYjpgwAU1a2l4

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
#include <Wincrypt.h>
#include <memory>
#include "fshelper.h"

FSOpResult calcBufferHash(std::wstring& hash, const unsigned char* buffer, const size_t bufferSize,
	const HashType hashType, const bool hashUCase = true);
FSOpResult calcFileHash(std::wstring& hash, const std::wstring filePath, const HashType hashType,
	const bool hashUCase = true);

#endif
