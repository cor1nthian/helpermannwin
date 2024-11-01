#ifndef _COMPRESS_H
#define _COMPRESS_H

#include <blosc.h>
#include "config.h"
#include "strhelper.h"
#include "fshelper.h"

enum class CompressOpResult : unsigned char {
	Success,
	Fail
};

enum class ShuffleOp : unsigned char {
	NoShuffle = BLOSC_NOSHUFFLE,
	Shuffle = BLOSC_SHUFFLE,
	BitShuffle = BLOSC_BITSHUFFLE
};

class CompressHandler {
	public:
		CompressHandler() {}
		CompressHandler(const CompressHandler &other) = delete;
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		CompressHandler(CompressHandler &&other) noexcept = delete;
#endif
		CompressHandler& operator=(const CompressHandler &other) = delete;
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		CompressHandler& operator=(const CompressHandler &&other) noexcept = delete;
#endif
		bool operator==(const CompressHandler &other) = delete;
		bool operator!=(const CompressHandler &other) = delete;
		~CompressHandler() {}
		CompressOpResult CompressBuffer(unsigned char* &compressedBuf, unsigned char* originalBuf,
			const size_t originalBufferSize, size_t &compressedBufferSize,
			const bool reallocCompressedBuffer = true, const std::wstring compressorName = L"blosclz",
			const unsigned char compressLevel = 5, const unsigned long blockSize = 0,
			const ShuffleOp shuffleOp = ShuffleOp::NoShuffle, const const unsigned short typeSize = 128,
			const unsigned long threadQuantity = 2) const;
		CompressOpResult CompressBuffer(const std::wstring compressedFilePath, unsigned char* originalBuf,
			const bool purgeFileIfExists = true) const;
		CompressOpResult CompressFile(unsigned char* &compressedBuf, const std::wstring originalFilePath) const;
		CompressOpResult CompressFile(const std::wstring compressedFilePath,
			const std::wstring originalFilePath) const;
		CompressOpResult DecompressBuffer(unsigned char* &decompressedBuf, unsigned char* originalBuf) const;
		CompressOpResult DecompressBuffer(const std::wstring decompressedFilePath,
			unsigned char* originalBuf) const;
		CompressOpResult DecompressFile(unsigned char* &decompressedBuf, const std::wstring originalFilePath) const;
		CompressOpResult DecompressFile(const std::wstring decompressedFilePath,
			const std::wstring originalFilePath) const;
	protected:
	private:
		bool checkCompressor(const std::wstring compressorName) const;
};

#endif