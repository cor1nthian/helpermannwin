#include "crypthelper.h"
#include "fshelper.h"

CryptOpResult calcBufferHash(std::wstring& hash, const unsigned char* buffer, const size_t bufferSize,
	const HashType hashType, const bool hashUCase) {
	::HCRYPTPROV hProv = 0;
	if (::CryptAcquireContext(&hProv, 0, 0, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
		::HCRYPTPROV hHash = 0;
		if (::CryptCreateHash(hProv, (::ALG_ID)hashType, 0, 0, &hHash)) {
			if (::CryptHashData(hHash, buffer, bufferSize, 0)) {
				unsigned long cbHashSize = 0, dwCount = sizeof(DWORD);
				if (::CryptGetHashParam(hHash, HP_HASHSIZE, (unsigned char*)&cbHashSize, &dwCount, 0)) {
					std::vector<unsigned char> buffer(cbHashSize);
					if (::CryptGetHashParam(hHash, HP_HASHVAL, reinterpret_cast<unsigned char*>(&buffer[0]), &cbHashSize, 0)) {
						std::ostringstream oss;
						for (std::vector<unsigned char>::const_iterator iter = buffer.begin(); iter != buffer.end();
							++iter) {
							oss.fill('0');
							oss.width(2);
							oss << std::hex << static_cast<const int>(*iter);
						}
						::CryptDestroyHash(hHash);
						::CryptReleaseContext(hProv, 0);
						if (hashUCase) {
							hash = str2wstr(upper_copy(oss.str()));
						} else {
							hash = str2wstr(oss.str());
						}
						::CryptDestroyHash(hHash);
						::CryptReleaseContext(hProv, 0);
						return CryptOpResult::Success;
					}
					::CryptDestroyHash(hHash);
					::CryptReleaseContext(hProv, 0);
				}
				::CryptDestroyHash(hHash);
				::CryptReleaseContext(hProv, 0);
			}
			::CryptDestroyHash(hHash);
			::CryptReleaseContext(hProv, 0);
		}
		::CryptReleaseContext(hProv, 0);
	}
	return CryptOpResult::Fail;
}

CryptOpResult calcFileHash(std::wstring& hash, const std::wstring filePath, const HashType hashType,
	const bool hashUCase) {
	::HCRYPTPROV hProv = 0;
	if (::CryptAcquireContext(&hProv, 0, 0, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
		::HCRYPTPROV hHash = 0;
		if (::CryptCreateHash(hProv, (::ALG_ID)hashType, 0, 0, &hHash)) {
			unsigned long long fsize = 0;
			if (FSOpResult::Success != getFileSize(fsize, filePath)) {
				return CryptOpResult::Fail;
			}
			unsigned char* fileContent = (unsigned char*)malloc(fsize + 1);
			if (!fileContent) {
				return CryptOpResult::Fail;
			}
			memset(fileContent, 0, (fsize + 1) * sizeof(unsigned char));
			if (FSOpResult::Success != file2Buffer(fileContent, filePath, fsize)) {
				return CryptOpResult::Fail;
			}
			if (::CryptHashData(hHash, static_cast<const unsigned char*>(fileContent), fsize, 0)) {
				unsigned long cbHashSize = 0, dwCount = sizeof(DWORD);
				if (::CryptGetHashParam(hHash, HP_HASHSIZE, (unsigned char*)&cbHashSize, &dwCount, 0)) {
					std::vector<unsigned char> buffer(cbHashSize);
					if (::CryptGetHashParam(hHash, HP_HASHVAL, reinterpret_cast<unsigned char*>(&buffer[0]), &cbHashSize, 0)) {
						std::ostringstream oss;
						for (std::vector<unsigned char>::const_iterator iter = buffer.begin(); iter != buffer.end();
							++iter) {
							oss.fill('0');
							oss.width(2);
							oss << std::hex << static_cast<const int>(*iter);
						}
						::CryptDestroyHash(hHash);
						::CryptReleaseContext(hProv, 0);
						SAFE_FREE(fileContent);
						if (hashUCase) {
							hash = str2wstr(upper_copy(oss.str()));
						} else {
							hash = str2wstr(oss.str());
						}
						::CryptDestroyHash(hHash);
						::CryptReleaseContext(hProv, 0);
						return CryptOpResult::Success;
					}
					SAFE_FREE(fileContent);
					::CryptDestroyHash(hHash);
					::CryptReleaseContext(hProv, 0);
				}
				SAFE_FREE(fileContent);
				::CryptDestroyHash(hHash);
				::CryptReleaseContext(hProv, 0);
			}
			SAFE_FREE(fileContent);
			::CryptDestroyHash(hHash);
			::CryptReleaseContext(hProv, 0);
		}
		::CryptReleaseContext(hProv, 0);
	}
	return CryptOpResult::Fail;
}