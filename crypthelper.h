#ifndef _CRYPTHELPER_H
#define _CRYPTHELPER_H

#include <memory>
#include "cryptcommon.h"
#include "strhelper.h"

CryptOpResult calcBufferHash(std::wstring &hash, const unsigned char* buffer, const size_t bufferSize,
	const HashType hashType, const bool hashUCase = true);
CryptOpResult calcFileHash(std::wstring &hash, const std::wstring filePath, const HashType hashType,
	const bool hashUCase = true);

#endif
