#include <iostream>
#include "httphelper.h"

const size_t replySize = 1024;

HTTPHelper::HTTPHelper() {}

HTTPHelper::~HTTPHelper() {}

bool HTTPHelper::GetHTTPSResponse(const std::string address, std::string &res, std::string requri) const {
	std::string srvaddr = serverAddressFromLink(address);
	std::string uri = "";
	if ("" == requri) {
		uri = uriFromLink(address);
	} else {
		uri = requri;
	}
	HINTERNET hInternet = ::InternetOpen(L"", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet != NULL) {
		// InternetOpen ok
		HINTERNET hConnect = ::InternetConnectA(hInternet, srvaddr.c_str(), INTERNET_DEFAULT_HTTPS_PORT,
			NULL, NULL, INTERNET_SERVICE_HTTP, NULL, 1);
		if (hConnect != NULL) {
			// InternetConnect ok
			HINTERNET hRequest = ::HttpOpenRequestA(hConnect, "GET", uri.c_str(), NULL, NULL, 0,
				INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_SECURE, 1);
			if (hRequest != NULL) {
				// HttpOpenRequest ok
				bool isSent = ::HttpSendRequestA(hRequest, NULL, 0, NULL, 0);
				if (isSent) {
					// HttpSendRequest ok
					char szData[replySize] = { 0 };
					for (;;) {
						// reading data
						DWORD dwByteRead = 0;
						bool isRead = ::InternetReadFile(hRequest, szData, sizeof(szData) - 1, &dwByteRead);
						// break cycle if error or end
						if (!isRead || dwByteRead == 0)
							break;

						// saving result
						szData[dwByteRead] = 0;
						res = szData;
					}
					::InternetCloseHandle(hRequest);
					::InternetCloseHandle(hConnect);
					::InternetCloseHandle(hInternet);
					res = removeFromBothSides_copy(res, "\n");
					return true;
				} else {
					// HttpSendRequest failed
					::InternetCloseHandle(hRequest);
					::InternetCloseHandle(hConnect);
					::InternetCloseHandle(hInternet);
					return false;
				}
			} else {
				// HttpOpenRequest failed
				::InternetCloseHandle(hConnect);
				::InternetCloseHandle(hInternet);
				return false;
			}
		} else {
			// InternetConnect failed
			::InternetCloseHandle(hInternet);
			return false;
		}
	} else {
		// InternetOpen failed
		return false;
	}
}

bool HTTPHelper::GetHTTPSResponse(const std::wstring address,
	std::string &res, std::wstring requri) const {
	std::wstring srvaddr = serverAddressFromLink(address);
	std::wstring uri = L"";
	if (L"" == requri) {
		uri = uriFromLink(address);
	} else {
		uri = requri;
	}
	HINTERNET hInternet = ::InternetOpen(L"", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet != NULL) {
		// InternetOpen ok
		HINTERNET hConnect = ::InternetConnect(hInternet, srvaddr.c_str(), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, 1);
		if (hConnect != NULL) {
			// InternetConnect ok
			HINTERNET hRequest = ::HttpOpenRequest(hConnect, L"GET", (LPCWSTR)uri.c_str(), NULL, NULL, 0, INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_SECURE, 1);
			if (hRequest != NULL) {
				// HttpOpenRequest ok
				bool isSent = ::HttpSendRequest(hRequest, NULL, 0, NULL, 0);
				if (isSent) {
					// HttpSendRequest ok
					char szData[replySize] = { 0 };
					for (;;) {
						// reading data
						DWORD dwByteRead = 0;
						bool isRead = ::InternetReadFile(hRequest, szData, sizeof(szData) - 1, &dwByteRead);
						// break cycle if error or end
						if (!isRead || dwByteRead == 0)
							break;

						// saving result
						szData[dwByteRead] = 0;
						res = szData;
					}
					::InternetCloseHandle(hRequest);
					::InternetCloseHandle(hConnect);
					::InternetCloseHandle(hInternet);
					res = removeFromBothSides_copy(res, "\n");
					return true;
				} else {
					// HttpSendRequest failed
					::InternetCloseHandle(hRequest);
					::InternetCloseHandle(hConnect);
					::InternetCloseHandle(hInternet);
					return false;
				}
			} else {
				// HttpOpenRequest failed
				::InternetCloseHandle(hConnect);
				::InternetCloseHandle(hInternet);
				return false;
			}
		} else {
			// InternetConnect failed
			::InternetCloseHandle(hInternet);
			return false;
		}
	} else {
		// InternetOpen failed
		return false;
	}
}

std::string HTTPHelper::serverAddressFromLink(const std::string link,
	const bool caseInsensitive) const {
	std::string line = link;
	if (startsWith(line, "http://", caseInsensitive)) {
		line = removeFromStart_copy(line, "http://", caseInsensitive);
	}
	if (startsWith(line, "https://", caseInsensitive)) {
		line = removeFromStart_copy(line, "https://", caseInsensitive);
	}
	std::vector<std::string> lineSpl = splitStr(line, "/");
	return lineSpl.front();
}

std::wstring HTTPHelper::serverAddressFromLink(const std::wstring link,
	const bool caseInsensitive) const {
	std::wstring line = link;
	if (startsWith(line, L"http://", caseInsensitive)) {
		line = removeFromStart_copy(line, L"http://", caseInsensitive);
	}
	if (startsWith(line, L"https://", caseInsensitive)) {
		line = removeFromStart_copy(line, L"https://", caseInsensitive);
	}
	std::vector<std::wstring> lineSpl = splitStr(line, L"/");
	return lineSpl.front();
}

std::string HTTPHelper::uriFromLink(const std::string link, const bool caseInsensitive) const {
	std::string line = link;
	std::string ret = "";
	if (startsWith(line, "http://", caseInsensitive)) {
		line = removeFromStart_copy(line, "http://", caseInsensitive);
	}
	if (startsWith(line, "https://", caseInsensitive)) {
		line = removeFromStart_copy(line, "https://", caseInsensitive);
	}
	std::vector<std::string> lineSpl = splitStr(line, "/");
	std::string tstr = "";
	if (lineSpl.size() > 1) {
		for (size_t i = 1; i < lineSpl.size(); ++i) {
			ret += lineSpl[i] + "/";
		}
		ret = "/" + removeFromBothSides_copy(ret, "/");
	} else {
		ret = line;
	}
	return ret;
}

std::wstring HTTPHelper::uriFromLink(const std::wstring link, const bool caseInsensitive) const {
	std::wstring line = link;
	std::wstring ret = L"";
	if (startsWith(line, L"http://", caseInsensitive)) {
		line = removeFromStart_copy(line, L"http://", caseInsensitive);
	}
	if (startsWith(line, L"https://", caseInsensitive)) {
		line = removeFromStart_copy(line, L"https://", caseInsensitive);
	}
	std::vector<std::wstring> lineSpl = splitStr(line, L"/");
	if (lineSpl.size() > 1) {
		for (size_t i = 1; i < lineSpl.size(); ++i) {
			ret += lineSpl[i] + L"/";
		}
		ret = L"/" + removeFromBothSides_copy(ret, L"/");
	} else {
		ret = line;
	}
	return ret;
}