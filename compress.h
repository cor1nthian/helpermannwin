#ifndef _COMPRESS_H
#define _COMPRESS_H

#include "config.h"
#include <blosc.h>
#include "syshelper.h"
#include "fshelper.h"
#include "crypt.h"

const wchar_t* const gc_noCompression = L"nocompression";
const wchar_t* const gc_headerStart = L"BHDR\1";
const wchar_t* const gc_headerEnd = L"\1";
const wchar_t* const gc_packStart = L"BFP\2";
const wchar_t* const gc_packEnd = L"\2";

// snappy compressor is skipped
const std::vector<std::wstring> const gc_allowedCompressors{ gc_noCompression, L"blosclz", L"lz4", L"lz4hc", L"zlib",
	L"zstd"};

enum class CompressOpResult : unsigned char {
	Success,
	Fail
};

enum class ShuffleOp : unsigned char {
	NoShuffle = BLOSC_NOSHUFFLE,
	Shuffle = BLOSC_SHUFFLE,
	BitShuffle = BLOSC_BITSHUFFLE
};

hminline size_t getApproxCompressedBufSz(const size_t originalBufferSize) {
	return (BLOSC_MAX_OVERHEAD + originalBufferSize);
}

hminline size_t getDecompressedBufferSz(const unsigned char* compressedBuf, const size_t bufferSize) {
	if (!compressedBuf || !bufferSize) {
		return 0;
	}
	size_t bufSz = 0;
	if (COMPRESS_BLOSC_INVALIDVAL == blosc_cbuffer_validate(compressedBuf, bufferSize, &bufSz)) {
		return 0;
	} else {
		return bufSz;
	}
}

bool checkCompressor(const std::wstring compressorName);

/*
              PACK STRUCTURE
_________________________________________
|                                       |
|           HEADER START SIGN           |
|_______________________________________|
|                                       |
|             HEADER SIZE               |
|_______________________________________|
|                                       |
               HEADER ID                |
|_______________________________________|
|                                       |
|           HEADER RECORDS NUM          |
|_______________________________________|
|                                       |
|            PACK RECORDS NUM           |
|_______________________________________|
|                                       |
|            RESOURCE 1 ID              |
|_______________________________________|
|                                       |
|         RESOURCE 1 OWNER ID           |
|_______________________________________|
|                                       |
|       RESOURCE 1 COMPRESSOR IDX       |
|_______________________________________|
|                                       |
|      RESOURCE N START DATA OFFSET     |
|_______________________________________|
|                                       |
|       RESOURCE 1 ORIGINAL SIZE        |
|_______________________________________|
|                                       |
|      RESOURCE 1 COMPRESSED SIZE       |
|_______________________________________|
|                                       |
|       RESOURCE 1 INTERNAL PATH        |
|_______________________________________|
|                                       |
|                  ...                  |
|_______________________________________|
|                                       |
|             RESOURCE N ID             |
|_______________________________________|
|                                       |
|          RESOURCE N OWNER ID          |
|_______________________________________|
|                                       |
|       RESOURCE N COMPRESSOR IDX       |
|_______________________________________|
|                                       |
|      RESOURCE N START DATA OFFSET     |
|_______________________________________|
|                                       |
|       RESOURCE N ORIGINAL SIZE        |
|_______________________________________|
|                                       |
|      RESOURCE N COMPRESSED SIZE       |
|_______________________________________|
|                                       |
|        RESOURCE N INTERNAL PATH       |
|_______________________________________|
|                                       |
|            HEADER END SIGN            |
|_______________________________________|
|                                       |
|       PACKAGE DATA START SIGN         |
|_______________________________________|
|                                       |
|           RESOURCE 1 DATA             |
|_______________________________________|
|                                       |
|                  ...                  |
|_______________________________________|
|                                       |
|          RESOURCE N DATA              |
|_______________________________________|
|                                       |
|        PACKAGE DATA END SIGN          |
|_______________________________________|
*/

class PackageResource;
class PackageHeader;
class PackageHandler;
class CompressHandler;

class PackageResource {
	public:
		PackageResource();
		PackageResource(const unsigned char compressor, const unsigned short nameLen, const unsigned long resourceID, const unsigned long ownerID,
			const unsigned long long startOffset, const size_t sizeOriginal, const size_t sizeCompressed,
			const std::wstring resourcePath);
		PackageResource(const unsigned char compressor, const unsigned short nameLen, const unsigned long resourceID, const unsigned long ownerID,
			const unsigned long long startOffset, const size_t sizeOriginal, const size_t sizeCompressed,
			const unsigned char* resourceBuffer, const std::wstring resourcePath);
		PackageResource(const PackageResource &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		PackageResource(PackageResource &&other);
#endif
		PackageResource& operator=(const PackageResource &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		PackageResource& operator=(PackageResource &&other) noexcept;
#endif
		bool operator==(const PackageResource &other);
		bool operator!=(const PackageResource &other);
		~PackageResource();
		unsigned char* GetCompressorName() const;
		unsigned char GetCompressorIndex() const;
		unsigned long GetResourceID() const;
		unsigned long GetOwnerID() const;
		unsigned long long GetStartOffset() const;
		size_t GetSizeOriginal() const;
		size_t GetSizeCompressed() const;
		unsigned char* GetResourceBuffer() const;
		std::wstring GetResourcePath() const;
		CompressOpResult SetCompressor(const unsigned char compressor);
		CompressOpResult SetCompressor(const std::wstring compressor);
		CompressOpResult SetOwnerID(const unsigned long ownerID);
		CompressOpResult SetStartOffset(const unsigned long long startOffset);
		CompressOpResult SetOriginalSize(const size_t sizeOriginal);
		CompressOpResult SetCompressedSize(const size_t sizeCompressedl);
		CompressOpResult SetResourceSizes(const size_t sizeOriginal, const size_t sizeCompressed);
		CompressOpResult SetResourceBuffer(const unsigned char* resourceBuffer);
		CompressOpResult SetResourcePath(const std::wstring resourcePath);
		CompressOpResult SetResourceData(const unsigned char compressor, const size_t sizeOriginal,
			const size_t sizeCompressed, const unsigned char* resourceBuffer, const std::wstring resourcePath);
		friend class PackageHeader;
		friend class PackageHandler;
		friend class CompressHandler;
	protected:
	private:
		unsigned char m_compressor;
		unsigned short m_nameLen;
		unsigned long m_resourceID;
		unsigned long m_ownerID;
		unsigned long long m_startOffset;
		size_t m_sizeOriginal;
		size_t m_sizeCompressed;
		unsigned char* m_resourceBuffer;
		std::wstring m_resourcePath;
};

class PackageHeader {
	public:
		PackageHeader();
		PackageHeader(const unsigned long headerID, const unsigned long recsSection,
			const unsigned long recsResource, const unsigned long headerSize, const std::wstring packagePwd,
			const std::vector<std::wstring> resourceMap, const std::vector<unsigned long> resources);
		PackageHeader(const PackageHeader &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		PackageHeader(PackageHeader &&other);
#endif
		PackageHeader& operator=(const PackageHeader &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		PackageHeader& operator=(PackageHeader &&other) noexcept;
#endif
		bool operator==(const PackageHeader &other);
		bool operator!=(const PackageHeader &other);
		~PackageHeader();
		CompressOpResult SetPackagePwd(const std::wstring pwd);
		CompressOpResult AddResourceSection(const std::wstring sectionPath);
		friend class PackageResource;
		friend class PackageHandler;
		friend class CompressHandler;
	protected:
	private:
		unsigned long m_headerID;
		unsigned long m_recsSection;
		unsigned long m_recsResource;
		unsigned long m_headerSize;
		std::wstring m_packagePwd;
		std::vector<std::wstring> m_resourceMap;
		std::vector<unsigned long> m_resourceIDs;
};

class PackageHandler {
	public:
		PackageHandler();
		PackageHandler(const std::vector<unsigned long> headerIDs);
		PackageHandler(const PackageHandler &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		PackageHandler(PackageHandler &&other) noexcept;
#endif
		PackageHandler& operator=(const PackageHandler &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		PackageHandler& operator=(PackageHandler &&other) noexcept;
#endif
		bool operator==(const PackageHandler &other);
		bool operator!=(const PackageHandler &other);
		~PackageHandler();
		CompressOpResult CreatePackage(const std::wstring packFilename, const bool purgeObjectIfExist = true);
		CompressOpResult CreatePackage(const std::wstring packFilename, const std::vector<std::wstring> filePaths,
			const bool purgeObjectIfExist = true);
		CompressOpResult ReadPackageHeader(const std::wstring objectPath);
		CompressOpResult IsHeadder(bool &isHeader, const std::wstring objectPath);
		CompressOpResult IsPackage(bool &isPackage, const std::wstring objectPath);
		friend class PackageResource;
		friend class PackageHeader;
		friend class CompressHandler;
	protected:
	private:
		std::vector<unsigned long> m_headerIDs;
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
			const std::wstring compressorName = L"blosclz", const unsigned char compressLevel = 5,
			const unsigned long blockSize = 0, const ShuffleOp shuffleOp = ShuffleOp::NoShuffle,
			const unsigned short typeSize = 128, const unsigned long threadQuantity = 2) const;
		CompressOpResult CompressBuffer(const std::wstring compressedFilePath, const unsigned char* originalBuf,
			const size_t originalBufferSize, size_t &compressedBufferSize,
			const std::wstring compressorName = L"blosclz", const unsigned char compressLevel = 5,
			const unsigned long blockSize = 0, const ShuffleOp shuffleOp = ShuffleOp::NoShuffle,
			const unsigned short typeSize = 128, const unsigned long threadQuantity = 2,
			const bool purgeObjectIfExist = true) const;
		CompressOpResult CompressFile(unsigned char* &compressedBuf, size_t &compressedBufferSize,
			const std::wstring originalFilePath, const std::wstring compressorName = L"blosclz",
			const unsigned char compressLevel = 5, const unsigned long blockSize = 0,
			const ShuffleOp shuffleOp = ShuffleOp::NoShuffle, const unsigned short typeSize = 128,
			const unsigned long threadQuantity = 2) const;
		CompressOpResult CompressFile(const std::wstring compressedFilePath,
			const std::wstring originalFilePath, const std::wstring compressorName = L"blosclz",
			const unsigned char compressLevel = 5, const unsigned long blockSize = 0,
			const ShuffleOp shuffleOp = ShuffleOp::NoShuffle, const unsigned short typeSize = 128,
			const unsigned long threadQuantity = 2, const bool purgeObjectIfExist = true) const;
		CompressOpResult DecompressBuffer(unsigned char* &decompressedBuf, unsigned char* originalBuf,
			const size_t originalBufferSize, size_t decompressedBufferSize,
			const std::wstring compressorName = L"blosclz", const unsigned long blockSize = 0, 
			const unsigned long threadQuantity = 2) const;
		CompressOpResult DecompressBuffer(const unsigned char* originalBuf, const size_t originalBufferSize,
			const std::wstring decompressedFilePath, const std::wstring compressorName = L"blosclz",
			const unsigned long blockSize = 0, const unsigned long threadQuantity = 2,
			const bool purgeObjectIfExist = true) const;
		CompressOpResult DecompressFile(unsigned char* &decompressedBuf, const size_t decompressedBufferSize,
			const std::wstring originalFilePath, const std::wstring compressorName = L"blosclz",
			const unsigned long blockSize = 0, const unsigned long threadQuantity = 2,
			const bool purgeObjectIfExist = true) const;
		CompressOpResult DecompressFile(const std::wstring decompressedFilePath,
			const std::wstring originalFilePath, const std::wstring compressorName = L"blosclz",
			const unsigned long blockSize = 0, const unsigned long threadQuantity = 2,
			const bool purgeObjectIfExist = true) const;
		friend class PackageResource;
		friend class PackageHeader;
		friend class PackageHandler;
		friend bool checkCompressor(const std::wstring compressorName);
	protected:
	private:
};

#endif