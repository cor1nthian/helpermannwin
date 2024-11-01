#include "compress.h"

CompressOpResult CompressHandler::CompressBuffer(unsigned char* &compressedBuf,
	unsigned char* originalBuf, const size_t originalBufferSize,
	size_t &compressedBufferSize, const bool reallocCompressedBuffer,
	const std::wstring compressorName, const unsigned char compressLevel,
	const unsigned long blockSize, const ShuffleOp shuffleOp,
	const const unsigned short typeSize, const unsigned long threadQuantity) const {
	if (!originalBuf || 0 >= originalBufferSize) {
		return CompressOpResult::Fail;
	}
	if (!reallocCompressedBuffer) {
		if (!compressedBuf || compressedBufferSize < originalBufferSize + BLOSC_MAX_OVERHEAD) {
			return CompressOpResult::Fail;
		}
	}
	const std::wstring compressorNameLow = lower_copy(compressorName);
	if (!checkCompressor(compressorNameLow)) {
		return CompressOpResult::Fail;
	}
	blosc_init();
	unsigned char compressLvl = compressLevel;
	if (COMPRESS_BLOSC_COMPRESSLVLMIN > compressLvl) {
		compressLvl = COMPRESS_BLOSC_COMPRESSLVLMIN;
	}
	if (COMPRESS_BLOSC_COMPRESSLVLMAX < compressLvl) {
		compressLvl = COMPRESS_BLOSC_COMPRESSLVLMAX;
	}
	short compressOpRes = blosc_set_compressor(wstr2str(compressorNameLow).c_str());
	if (COMPRESS_BLOSC_INVALIDVAL == compressOpRes) {
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	unsigned int blockSz = blockSize;
	if (COMPRESS_BLOSC_MAXBLOCKSZ < blockSz) {
		blockSz = COMPRESS_BLOSC_MAXBLOCKSZ;
	}
	blosc_set_blocksize(blockSz);
	unsigned short threadQuan = threadQuantity;
	if (COMPRESS_BLOSC_THREADSMIN > threadQuan) {
		threadQuan = COMPRESS_BLOSC_THREADSMIN;
	}
	if (COMPRESS_BLOSC_THREADSMAX < threadQuan) {
		threadQuan = COMPRESS_BLOSC_THREADSMAX;
	}
	blosc_set_nthreads(threadQuan);
	unsigned short typeSz = typeSize;
	if (COMPRESS_BLOSC_TYPESZMIN > typeSz) {
		typeSz = COMPRESS_BLOSC_TYPESZMIN;
	}
	if (COMPRESS_BLOSC_TYPESZMAX < typeSz) {
		typeSz = COMPRESS_BLOSC_TYPESZMAX;
	}
	if (reallocCompressedBuffer) {
		if (!compressedBuf || compressedBufferSize < originalBufferSize + BLOSC_MAX_OVERHEAD) {
			if (compressedBuf) {
				SAFE_FREE(compressedBuf);
			}
			size_t bufSz = (originalBufferSize + BLOSC_MAX_OVERHEAD) *
				sizeof(unsigned char);
			compressedBuf = (unsigned char*)malloc(bufSz);
			if (!compressedBuf) {
				blosc_destroy();
				return CompressOpResult::Fail;
			}
			memset(&compressedBuf, 0, bufSz);
			compressedBufferSize = originalBufferSize + BLOSC_MAX_OVERHEAD;
		}
	}
	// must return compressed buffer size; zero or less indicate an error
	compressOpRes = blosc_compress(compressLvl, static_cast<int>(shuffleOp), typeSz,
		originalBufferSize, originalBuf, compressedBuf, compressedBufferSize);
	if(0 >= compressOpRes) {
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	compressedBufferSize = compressOpRes;
	blosc_destroy();
	return CompressOpResult::Success;
}

CompressOpResult CompressHandler::CompressBuffer(const std::wstring compressedFilePath,
	unsigned char* originalBuf, const bool purgeFileIfExists) const {
	blosc_init();
	blosc_destroy();
	return CompressOpResult::Success;
}

CompressOpResult CompressHandler::CompressFile(unsigned char* &compressedBuf,
	const std::wstring originalFilePath) const {
	blosc_init();
	blosc_destroy();
	return CompressOpResult::Success;
}

CompressOpResult CompressHandler::CompressFile(const std::wstring compressedFilePath,
	const std::wstring originalFilePath) const {
	blosc_init();
	blosc_destroy();
	return CompressOpResult::Success;
}

CompressOpResult CompressHandler::DecompressBuffer(unsigned char* &decompressedBuf,
	unsigned char* originalBuf) const {
	blosc_init();
	blosc_destroy();
	return CompressOpResult::Success;
}

CompressOpResult CompressHandler::DecompressBuffer(const std::wstring decompressedFilePath,
	unsigned char* originalBuf) const {
	blosc_init();
	blosc_destroy();
	return CompressOpResult::Success;
}

CompressOpResult CompressHandler::DecompressFile(unsigned char* &decompressedBuf,
	const std::wstring originalFilePath) const {
	blosc_init();
	blosc_destroy();
	return CompressOpResult::Success;
}

CompressOpResult CompressHandler::DecompressFile(const std::wstring decompressedFilePath,
	const std::wstring originalFilePath) const {
	blosc_init();
	blosc_destroy();
	return CompressOpResult::Success;
}

bool CompressHandler::checkCompressor(const std::wstring compressorName) const {
	std::vector<std::wstring> allowedCompressors { L"blosclz", L"lz4", L"lz4hc", L"zlib", L"ztsd" };
	if (valInList(allowedCompressors, compressorName, false)) {
		return true;
	} else {
		return false;
	}
}