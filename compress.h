#ifndef _COMPRESS_H
#define _COMPRESS_H

#include "config.h"
#include <blosc.h>
#include "syshelper.h"
#include "fshelper.h"

const wchar_t* const gc_noCompression = L"nocompression";
const wchar_t* const gc_packStart = L"BPACK\1";
const wchar_t* const gc_packEnd = L"\1";
const wchar_t* const gc_headerStart = L"BHDR\2";
const wchar_t* const gc_headerEnd = L"\2";
const wchar_t* const gc_resourceStart = L"BFP\3";
const wchar_t* const  gc_resourceEnd = L"\3";

// snappy compressor is skipped
const std::vector<std::wstring> const gc_allowedCompressors{ gc_noCompression, L"blosclz", L"lz4", L"lz4hc", L"zlib",
	L"zstd"};

enum class CompressOpResult : unsigned char {
	Success,
	Fail
};

enum class CompressPwdHashType : unsigned long {
	NoPwd,
	MD5Hash = CALG_MD5,
	SHA1Hash = CALG_SHA1,
	SHA256Hash = CALG_SHA_256
};

enum class CompressHeaderInclude : unsigned char {
	Included,
	Excluded
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

hminline const wchar_t* getCpmpressorName(const unsigned char index) {
	if (gc_allowedCompressors.size() > index) {
		return gc_allowedCompressors[index].c_str();
	} else {
		return L"";
	}
}

unsigned char getCompressorIndex(const std::wstring compressorName);
bool checkCompressor(const std::wstring compressorName);

/*
              PACK STRUCTURE
_________________________________________
|  HEADER MIGHT NOT BE PRESENT IN PACK  |
|_______________________________________|
|          PACKAGE START SIGN           |
|_______________________________________|
|                                       |
|           HEADER START SIGN           |
|_______________________________________|
|                                       |
|         HEADER PWD HASH TYPE          |
|_______________________________________|
|                                       |
|           HEADER PWD HASH             |
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
|          CONTAINER RECORDS NUM        |
|_______________________________________|
|                                       |
|            CONTAINER 1 ID             |
|_______________________________________|
|                                       |
|        CONTAINER 1 STRING ID          |
|_______________________________________|
|                                       |
|           CONTAINER 1 PATH            |
|_______________________________________|
|                                       |
|                  ...                  |
|_______________________________________|
|                                       |
|            CONTAINER N ID             |
|_______________________________________|
|                                       |
|        CONTAINER N STRING ID          |
|_______________________________________|
|                                       |
|           CONTAINER N PATH            |
|_______________________________________|
|                                       |
|             HEADER SIZE               |
|_______________________________________|
|                                       |
|           HEADER END SIGN             |
|_______________________________________|
|                                       |
|       PACKAGE DATA START SIGN         |
|_______________________________________|
|                                       |
|         RESOURCE 1 STRING ID          |
|_______________________________________|
|                                       |
|      RESOURCE 1 HEADER OWNER ID       |
|_______________________________________|
|                                       |
| RESOURCE 1 CONTAINER OWNER STRING ID  |
|_______________________________________|
|                                       |
|       RESOURCE 1 COMPRESSOR IDX       |
|_______________________________________|
|                                       |
|      RESOURCE 1 START DATA OFFSET     |
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
|           RESOURCE 1 DATA             |
|_______________________________________|
|                                       |
|                  ...                  |
|_______________________________________|
|                                       |
|         RESOURCE N STRING ID          |
|_______________________________________|
|                                       |
|      RESOURCE N HEADER OWNER ID       |
|_______________________________________|
|                                       |
| RESOURCE N CONTAINER OWNER STRING ID  |
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
|          RESOURCE N DATA              |
|_______________________________________|
|                                       |
|        PACKAGE DATA END SIGN          |
|_______________________________________|
|                                       |
|          PACKAGE END SIGN             |
|_______________________________________|
*/

class PackageContainer;
class PackageResource;
class PackageHeader;
class PackageHandler;
class CompressHandler;

class PackageContainer {
	public:
		PackageContainer();
		PackageContainer(const unsigned long containerID, const std::wstring conrainerStrID,
			const std::wstring containerPath);
		PackageContainer(const unsigned long containerID, const std::wstring conrainerStrID,
			const std::wstring containerPath, const std::vector<unsigned long> resourceIDs);
		PackageContainer(const PackageContainer &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		PackageContainer(PackageContainer &&other) noexcept;
#endif
		PackageContainer& operator=(const PackageContainer &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		PackageContainer& operator=(PackageContainer &&other) noexcept;
#endif
		bool operator==(const PackageContainer &other);
		bool operator!=(const PackageContainer& other);
		~PackageContainer();
		unsigned long GetContainerID() const;
		std::wstring GetContainerStrID() const;
		std::wstring GetContainerPath() const;
		CompressOpResult SetContainerID(unsigned long containerID);
		CompressOpResult SetContainerStrID(const std::wstring containerStrID);
		CompressOpResult SetContainerPath(const std::wstring containerPath);
		friend class PackageResource;
		friend class PackageHeader;
		friend class PackageHandler;
		friend class CompressHandler;
	protected:
	private:
		/*       FUNCTIONS       */
		CompressOpResult generateContainerStrID(std::wstring& containerStrID);
		/*       VARIABLES       */
		unsigned long m_containerID;
		std::wstring m_containerPath;
		std::wstring m_containerStrID;
		std::vector<unsigned long> m_containerIDs;
		std::vector<std::wstring> m_resources;
};

class PackageResource {
	public:
		PackageResource();
		PackageResource(const unsigned char compressor, const unsigned short nameLen, const unsigned long resourceID,
			const std::wstring resourceStrID, const std::wstring resourceName, const unsigned long headerOwnerID,
			const unsigned long containerOwnerID, const unsigned long long startOffset, const size_t sizeOriginal,
			const size_t sizeCompressed, const std::wstring resourcePath);
		PackageResource(const unsigned char compressor, const unsigned short nameLen, const unsigned long resourceID, 
			const std::wstring resourceStrID, const std::wstring resourceName, const unsigned long headerOwnerID,
			const unsigned long containerOwnerID, const unsigned long long startOffset, const size_t sizeOriginal,
			const size_t sizeCompressed, const unsigned char* resourceBuffer, const std::wstring resourcePath);
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
		unsigned long GetHeaderOwnerID() const;
		unsigned long GetContainerOwnerID() const;
		unsigned long long GetStartOffset() const;
		size_t GetSizeOriginal() const;
		size_t GetSizeCompressed() const;
		unsigned char* GetResourceBuffer() const;
		std::wstring GetResourcePath() const;
		std::wstring GetResourceStrID() const;
		std::wstring GetResourceMame() const;
		CompressOpResult SetCompressor(const unsigned char compressor);
		CompressOpResult SetCompressor(const std::wstring compressor);
		CompressOpResult SetHeaderOwnerID(const unsigned long ownerID);
		CompressOpResult SetContainerOwnerID(const unsigned long ownerID);
		CompressOpResult SetStartOffset(const unsigned long long startOffset);
		CompressOpResult SetOriginalSize(const size_t sizeOriginal);
		CompressOpResult SetCompressedSize(const size_t sizeCompressedl);
		CompressOpResult SetResourceSizes(const size_t sizeOriginal, const size_t sizeCompressed);
		CompressOpResult SetResourceBuffer(const unsigned char* resourceBuffer);
		CompressOpResult SetResourceStrID(const std::wstring resourceStrID);
		CompressOpResult SetResourceNmae(const std::wstring resourceName);
		CompressOpResult SetResourcePath(const std::wstring resourcePath);
		CompressOpResult SetResourceData(const unsigned char compressor, const size_t sizeOriginal,
			const size_t sizeCompressed, const unsigned char* resourceBuffer, const std::wstring resourcePath);
		friend class PackageContainer;
		friend class PackageHeader;
		friend class PackageHandler;
		friend class CompressHandler;
	protected:
	private:
		/*       FUNCTIONS       */
		CompressOpResult generateResourceStrID(std::wstring &resourceStrID);
		/*       VARIABLES       */
		unsigned char m_compressor; // index
		unsigned short m_nameLen;
		unsigned long m_resourceID;
		unsigned long m_containerOwnerID;
		unsigned long m_headerOwnerID;
		unsigned long long m_startOffset;
		size_t m_sizeOriginal;
		size_t m_sizeCompressed;
		unsigned char* m_resourceBuffer;
		std::wstring m_resourceName;
		std::wstring m_resourceStrID;
		std::wstring m_resourcePath;
};

class PackageHeader {
	public:
		PackageHeader();
		PackageHeader(const unsigned long headerID, const unsigned long headerSize, const std::wstring packagePwd,
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
		friend class PackageContainer;
		friend class PackageResource;
		friend class PackageHandler;
		friend class CompressHandler;
	protected:
	private:
		/*       FUNCTIONS       */
		/*       VARIABLES       */
		unsigned long m_headerID;
		unsigned long m_headerSize;
		std::wstring m_headerPwd;
		std::vector<unsigned long> m_headerIDs;
		std::vector<std::wstring> m_containerIDs;
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
		CompressOpResult CreatePackage(const std::wstring packFilename,
			const CompressHeaderInclude headerInclude = CompressHeaderInclude::Included,
			const CompressPwdHashType pwdHashType = CompressPwdHashType::MD5Hash,
			const std::wstring packPwd = L"123", const bool purgeObjectIfExists = true);
		CompressOpResult CreatePackage(const std::wstring packFilename, const std::vector<std::wstring> filePaths,
			const bool purgeObjectIfExists = true);
		CompressOpResult ReadPackageHeader(const std::wstring objectPath);
		CompressOpResult IsHeadder(bool &isHeader, const std::wstring objectPath);
		CompressOpResult IsPackage(bool &isPackage, const std::wstring objectPath);
		friend class PackageContainer;
		friend class PackageResource;
		friend class PackageHeader;
		friend class CompressHandler;
	protected:
	private:
		/*       FUNCTIONS       */
		/*       VARIABLES       */
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
		friend class PackageContainer;
		friend class PackageResource;
		friend class PackageHeader;
		friend class PackageHandler;
		friend bool checkCompressor(const std::wstring compressorName);
	protected:
	private:
};

#endif