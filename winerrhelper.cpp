#include "winerrhelper.h"

void errorMsgBox(const std::wstring errormsg, const std::wstring msgboxCaption, const unsigned int msgboxIcon) {
	::MessageBox(0, errormsg.c_str(), msgboxCaption.c_str(), msgboxIcon);
}

void setLastError(const unsigned long code) {
	::SetLastError(code);
}

unsigned long getLastErrorCode() {
	return ::GetLastError();
}

std::wstring getErrorDetails(const unsigned long errcode, const std::wstring funcName,
	const std::wstring additionalInfo) {
	unsigned long errCode = errcode;
	if (!errCode) {
		errCode = ::GetLastError();
	}
	wchar_t descBuf[256] = { 0 };
	wchar_t msgbuf[1024] = { 0 };
	if (funcName.length()) {
		wcscpy(msgbuf, L"Error calling function ");
		wcscat(msgbuf, funcName.c_str());
	} else {
		wcscpy(msgbuf, L"Call failed");
	}
	wcscat(msgbuf, L"\n\nError code: ");
	wcscat(msgbuf, std::to_wstring(errCode).c_str());
	wcscat(msgbuf, L"\n\nDetails:\n");
	::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		descBuf, 256, NULL);
	wcscat(msgbuf, descBuf);
	if (additionalInfo.length()) {
		wcscat(msgbuf, L"\nAdditional info:\n");
		wcscat(msgbuf, additionalInfo.c_str());
	}
	return msgbuf;
}