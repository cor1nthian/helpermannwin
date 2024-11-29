#ifndef _HTTP_HELPER_H
#define _HTTP_HELPER_H

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
#include <wininet.h>
#include "config.h"
#include "strhelper.h"

class HTTPHelper {
	public:
		HTTPHelper();
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		HTTPHelper(const HTTPHelper &other) = delete;
#else
		HTTPHelper(const HTTPHelper &other) {}
#endif
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		HTTPHelper(HTTPHelper &&other) noexcept = delete;
#endif
		~HTTPHelper();
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		HTTPHelper& operator=(const HTTPHelper &other) = delete;
#else
		HTTPHelper& operator=(const HTTPHelper &other) {}
#endif
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		HTTPHelper& operator=(const HTTPHelper &&other) = delete;
#endif
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		bool operator==(const HTTPHelper &other) const = delete;
#else
		bool operator==(const HTTPHelper &other) {}
#endif
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		bool operator!=(const HTTPHelper &other) const = delete;
#else
		bool operator!=(const HTTPHelper &other) {}
#endif
		bool GetHTTPSResponse(const std::string address, std::string &res, std::string requri = "") const;
		bool GetHTTPSResponse(const std::wstring address, std::string &res, std::wstring requri = L"") const;
	protected:

	private:
		std::string serverAddressFromLink(const std::string link,
			const bool caseInsensitive = true) const;
		std::wstring serverAddressFromLink(const std::wstring link,
			const bool caseInsensitive = true) const;
		std::string uriFromLink(const std::string link, const bool caseInsensitive = true) const;
		std::wstring uriFromLink(const std::wstring link, const bool caseInsensitive = true) const;
};

#endif // _HTTP_HELPER_H