#include "compress.h"

// snappy compressor is skipped
bool checkCompressor(const std::wstring compressorName) {
	std::vector<std::wstring> compressors;
	compressors.insert(gc_allowedCompressors.begin() + 1, gc_allowedCompressors.end(),
		gc_allowedCompressors.end());
	if (valInList(compressors, compressorName, false)) {
		return true;
	} else {
		return false;
	}
}

PackageContainer::PackageContainer() {
	m_containerID = 0;
}

PackageContainer::PackageContainer(const unsigned long containerID) {
	m_containerID = containerID;
}

PackageContainer::PackageContainer(const unsigned long containerID, const std::vector<unsigned long> resourceIDs) {
	m_containerID = containerID;
	m_resourceIDs = resourceIDs;
}

PackageContainer::PackageContainer(const PackageContainer &other) {
	if (this != &other) {
		m_containerID = other.m_containerID;
		m_resourceIDs = other.m_resourceIDs;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
PackageContainer::PackageContainer(PackageContainer&& other) noexcept {
	if (this != &other) {
		m_containerID = std::exchange(other.m_containerID, 0);
		m_resourceIDs = std::move(other.m_resourceIDs);
	}
}
#endif

PackageContainer& PackageContainer::operator=(const PackageContainer &other) {
	if (this != &other) {
		m_containerID = other.m_containerID;
		m_resourceIDs = other.m_resourceIDs;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
PackageContainer& PackageContainer::operator=(PackageContainer &&other) noexcept {
	if (this != &other) {
		m_containerID = std::exchange(other.m_containerID, 0);
		m_resourceIDs = std::move(other.m_resourceIDs);
	}
	return *this;
}

#endif
bool PackageContainer::operator==(const PackageContainer &other) {
	if (this != &other) {
		return (m_containerID == other.m_containerID &&
				m_resourceIDs == other.m_resourceIDs);
	} else {
		return true;
	}
}
bool PackageContainer::operator!=(const PackageContainer &other) {
	if (this != &other) {
		return (m_containerID != other.m_containerID ||
				m_resourceIDs != other.m_resourceIDs);
	} else {
		return false;
	}
}

PackageContainer::~PackageContainer() {}

PackageResource::PackageResource() {
	m_compressor = 0;
	m_nameLen = 0;
	m_resourceID = 0;
	m_headerOwnerID = 0;
	m_containerOwnerID = 0;
	m_startOffset = 0;
	m_sizeOriginal = 0;
	m_sizeCompressed = 0;
	m_resourceBuffer = 0;
	PackageHeader* m_owner = 0;

}

PackageResource::PackageResource(const unsigned char compressor, const unsigned short nameLen,
	const unsigned long resourceID, const unsigned long headerOwnerID, const unsigned long containerOwnerID,
	const unsigned long long startOffset, const size_t sizeOriginal, const size_t sizeCompressed,
	const std::wstring resourcePath) {
	m_compressor = compressor;
	m_nameLen = nameLen;
	m_resourceID = resourceID;
	m_headerOwnerID = headerOwnerID;
	m_containerOwnerID = containerOwnerID;
	m_startOffset = startOffset;
	m_sizeOriginal = sizeOriginal;
	m_sizeCompressed = sizeCompressed;
	m_resourcePath = resourcePath;
	m_resourceBuffer = 0;
}

PackageResource::PackageResource(const unsigned char compressor, const unsigned short nameLen,
	const unsigned long resourceID, const unsigned long headerOwnerID, const unsigned long containerOwnerID,
	const unsigned long long startOffset, const size_t sizeOriginal, const size_t sizeCompressed,
	const unsigned char* resourceBuffer, const std::wstring resourcePath) {
	m_compressor = compressor;
	m_nameLen = nameLen;
	m_resourceID = resourceID;
	m_headerOwnerID = headerOwnerID;
	m_containerOwnerID = containerOwnerID;
	m_startOffset = startOffset;
	m_sizeOriginal = sizeOriginal;
	m_sizeCompressed = sizeCompressed;
	m_resourceBuffer = const_cast<unsigned char*>(resourceBuffer);
	m_resourcePath = resourcePath;
}

PackageResource::PackageResource(const PackageResource& other) {
	if (this != &other) {
		m_compressor = other.m_compressor;
		m_nameLen = other.m_nameLen;
		m_resourceID = other.m_resourceID;
		m_headerOwnerID = other.m_headerOwnerID;
		m_containerOwnerID = other.m_containerOwnerID;
		m_startOffset = other.m_startOffset;
		m_sizeOriginal = other.m_sizeOriginal;
		m_sizeCompressed = other.m_sizeCompressed;
		m_resourceBuffer = other.m_resourceBuffer;
		m_resourcePath = other.m_resourcePath;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
PackageResource::PackageResource(PackageResource &&other) {
	if(this != &other) {
		m_compressor = std::exchange(other.m_compressor, 0);
		m_nameLen = std::exchange(other.m_nameLen, 0);
		m_resourceID = std::exchange(other.m_resourceID, 0);
		m_headerOwnerID = std::exchange(other.m_headerOwnerID, 0);
		m_containerOwnerID = std::exchange(other.m_containerOwnerID, 0);
		m_startOffset = std::exchange(other.m_startOffset, 0);
		m_sizeOriginal = std::exchange(other.m_sizeOriginal, 0);
		m_sizeCompressed = std::exchange(other.m_sizeCompressed, 0);
		m_resourceBuffer = std::move(other.m_resourceBuffer);
		m_resourcePath = std::move(other.m_resourcePath);
	}
}
#endif

PackageResource& PackageResource::operator=(const PackageResource &other) {
	if (this != &other) {
		m_compressor = other.m_compressor;
		m_nameLen = other.m_nameLen;
		m_resourceID = other.m_resourceID;
		m_headerOwnerID = other.m_headerOwnerID;
		m_containerOwnerID = other.m_containerOwnerID;
		m_startOffset = other.m_startOffset;
		m_sizeOriginal = other.m_sizeOriginal;
		m_sizeCompressed = other.m_sizeCompressed;
		m_resourceBuffer = other.m_resourceBuffer;
		m_resourcePath = other.m_resourcePath;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
PackageResource& PackageResource::operator=(PackageResource &&other) noexcept {
	if (this != &other) {
		m_compressor = std::exchange(other.m_compressor, 0);
		m_nameLen = std::exchange(other.m_nameLen, 0);
		m_resourceID = std::exchange(other.m_resourceID, 0);
		m_headerOwnerID = std::exchange(other.m_headerOwnerID, 0);
		m_containerOwnerID = std::exchange(other.m_containerOwnerID, 0);
		m_startOffset = std::exchange(other.m_startOffset, 0);
		m_sizeOriginal = std::exchange(other.m_sizeOriginal, 0);
		m_sizeCompressed = std::exchange(other.m_sizeCompressed, 0);
		m_resourceBuffer = std::move(other.m_resourceBuffer);
		m_resourcePath = std::move(other.m_resourcePath);
	}
	return *this;
}
#endif

bool PackageResource::operator==(const PackageResource &other) {
	if (this != &other) {
		return (m_compressor == other.m_compressor &&
				m_nameLen == other.m_nameLen &&
				m_resourceID == other.m_resourceID &&
				m_headerOwnerID == other.m_headerOwnerID &&
				m_containerOwnerID == other.m_containerOwnerID &&
				m_startOffset == other.m_startOffset &&
				m_sizeOriginal == other.m_sizeOriginal &&
				m_sizeCompressed == other.m_sizeCompressed &&
				m_resourceBuffer == other.m_resourceBuffer &&
				m_resourcePath == other.m_resourcePath);
	} else {
		return true;
	}
}

bool PackageResource::operator!=(const PackageResource &other) {
	if (this != &other) {
		return (m_compressor != other.m_compressor ||
				m_nameLen != other.m_nameLen ||
				m_resourceID != other.m_resourceID ||
				m_headerOwnerID != other.m_headerOwnerID ||
				m_containerOwnerID != other.m_containerOwnerID ||
				m_startOffset != other.m_startOffset ||
				m_sizeOriginal != other.m_sizeOriginal ||
				m_sizeCompressed != other.m_sizeCompressed ||
				m_resourceBuffer != other.m_resourceBuffer ||
				m_resourcePath != other.m_resourcePath);
	} else {
		return false;
	}
}

PackageResource::~PackageResource() {
	if (m_resourceBuffer) {
		SAFE_FREE(m_resourceBuffer);
	}
}

unsigned char* PackageResource::GetCompressorName() const {
	std::string compressorName = wstr2str(gc_allowedCompressors[m_compressor]);
	char* compressorNameUnconst = const_cast<char*>(compressorName.c_str());
	return (unsigned char*)compressorNameUnconst;
}

unsigned char PackageResource::GetCompressorIndex() const {
	return m_compressor;
}

unsigned long PackageResource::GetResourceID() const {
	return m_resourceID;
}

unsigned long PackageResource::GetHeaderOwnerID() const {
	return m_headerOwnerID;
}

unsigned long PackageResource::GetContainerOwnerID() const {
	return m_containerOwnerID;
}

unsigned long long PackageResource::GetStartOffset() const {
	return m_startOffset;
}

size_t PackageResource::GetSizeOriginal() const {
	return m_sizeOriginal;
}

size_t PackageResource::GetSizeCompressed() const {
	return m_sizeCompressed;
}

unsigned char* PackageResource::GetResourceBuffer() const {
	return m_resourceBuffer;
}

std::wstring PackageResource::GetResourcePath() const {
	return m_resourcePath;
}

CompressOpResult PackageResource::SetCompressor(const unsigned char compressor) {
	if (compressor <= gc_allowedCompressors.size()) {
		m_compressor = compressor;
		return CompressOpResult::Success;
	} else {
		return CompressOpResult::Fail;
	}
}

CompressOpResult PackageResource::SetCompressor(const std::wstring compressor) {
	if (valInList(gc_allowedCompressors, compressor)) {
		size_t compressIdx = 0;
		std::vector<std::wstring> compressors;
		compressors.insert(gc_allowedCompressors.begin() + 1, gc_allowedCompressors.end(),
			gc_allowedCompressors.end());
		if (!(valInList(compressors, compressor, true, false, &compressIdx))) {
			return CompressOpResult::Fail;
		} else {
			m_compressor = compressIdx;
			return CompressOpResult::Success;
		}
	} else {
		return CompressOpResult::Fail;
	}
}

CompressOpResult PackageResource::SetHeaderOwnerID(const unsigned long headerOwnerID) {
	m_headerOwnerID = headerOwnerID;
	return CompressOpResult::Success;
}

CompressOpResult PackageResource::SetContainerOwnerID(const unsigned long containerOwnerID) {
	m_containerOwnerID = containerOwnerID;
	return CompressOpResult::Success;
}

CompressOpResult PackageResource::SetStartOffset(const unsigned long long startOffset) {
	m_startOffset = startOffset;
	return CompressOpResult::Success;
}

CompressOpResult PackageResource::SetOriginalSize(const size_t sizeOriginal) {
	if (sizeOriginal <= COMPRESS_MAX_RESOURCESZ) {
		m_sizeOriginal = sizeOriginal;
		return CompressOpResult::Success;
	} else {
		return CompressOpResult::Fail;
	}
}

CompressOpResult PackageResource::SetCompressedSize(const size_t sizeCompressedl) {
	if (sizeCompressedl <= COMPRESS_MAX_RESOURCESZ) {
		m_sizeCompressed = sizeCompressedl;
		return CompressOpResult::Success;
	}
	else {
		return CompressOpResult::Fail;
	}
}

CompressOpResult PackageResource::SetResourceSizes(const size_t sizeOriginal, const size_t sizeCompressed) {
	if ((sizeOriginal <= COMPRESS_MAX_RESOURCESZ) && (sizeCompressed <= COMPRESS_MAX_RESOURCESZ)) {
		m_sizeOriginal = sizeOriginal;
		m_sizeCompressed = sizeCompressed;
		return CompressOpResult::Success;
	} else {
		return CompressOpResult::Fail;
	}
}

CompressOpResult PackageResource::SetResourceBuffer(const unsigned char* resourceBuffer) {
	if (resourceBuffer) {
		unsigned char* resourceBufferUnconst = const_cast<unsigned char*>(resourceBuffer);
		if (!IsBadReadPtr(resourceBufferUnconst)) {
			m_resourceBuffer = resourceBufferUnconst;
			return CompressOpResult::Success;
		} else {
			return CompressOpResult::Fail;
		}
	} else {
		return CompressOpResult::Fail;
	}
	
}

CompressOpResult PackageResource::SetResourcePath(const std::wstring resourcePath) {
	if (pathExists(resourcePath)) {
		bool isFld = false;
		if (FSOpResult::Success != isFolder(isFld, resourcePath)) {
			if (isFld) {
				return CompressOpResult::Fail;
			}
		} else {
			return CompressOpResult::Fail;
		}
		m_resourcePath = resourcePath;
		return CompressOpResult::Success;
	} else {
		return CompressOpResult::Fail;
	}
}

CompressOpResult PackageResource::SetResourceData(const unsigned char compressor, const size_t sizeOriginal,
	const size_t sizeCompressed, const unsigned char* resourceBuffer, const std::wstring resourcePath) {
	if (compressor < gc_allowedCompressors.size()) {
		if ((sizeOriginal <= COMPRESS_MAX_RESOURCESZ) && (sizeCompressed <= COMPRESS_MAX_RESOURCESZ)) {
			unsigned char* resourceBufferUnconst = const_cast<unsigned char*>(resourceBuffer);
			if (resourceBuffer) {
				if (!IsBadReadPtr(resourceBufferUnconst)) {
					if (resourcePath.length() && pathExists(resourcePath)) {
						bool isFld = false;
						if (FSOpResult::Success != isFolder(isFld, resourcePath)) {
							if (isFld) {
								return CompressOpResult::Fail;
							}
						} else {
							return CompressOpResult::Fail;
						}
						m_compressor = compressor;
						m_sizeOriginal = sizeOriginal;
						m_sizeCompressed = sizeCompressed;
						m_resourceBuffer = resourceBufferUnconst;
						m_resourcePath = resourcePath;
						return CompressOpResult::Success;
					} else {
						return CompressOpResult::Fail;
					}
				} else {
					return CompressOpResult::Fail;
				}
			} else {
				return CompressOpResult::Fail;
			}
		} else {
			return CompressOpResult::Fail;
		}
	} else {
		return CompressOpResult::Fail;
	}
}

PackageHeader::PackageHeader() {
	m_headerID = 0;
	m_recsSection = 0;
	m_recsResource = 0;
	m_headerSize = 0;
}

PackageHeader::PackageHeader(const unsigned long headerID, const unsigned long recsSection,
	const unsigned long recsResource, const unsigned long headerSize, const std::wstring packagePwd,
	const std::vector<std::wstring> resourceMap, const std::vector<unsigned long> resources) {
	m_headerID = headerID;
	m_recsSection = recsSection;
	m_recsResource = recsResource;
	m_headerSize = headerSize;
	m_packagePwd = packagePwd;
	m_resourceMap = resourceMap;
	m_resourceIDs = resources;
}

PackageHeader::PackageHeader(const PackageHeader &other) {
	if (this != &other) {
		m_headerID = other.m_headerID;
		m_recsSection = other.m_recsSection;
		m_recsResource = other.m_recsResource;
		m_headerSize = other.m_headerSize;
		m_packagePwd = other.m_packagePwd;
		m_resourceIDs = other.m_resourceIDs;
		m_resourceMap = other.m_resourceMap;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
PackageHeader::PackageHeader(PackageHeader &&other) {
	if (this != &other) {
		m_headerID = std::exchange(other.m_headerID, 0);
		m_recsSection = std::exchange(other.m_recsSection, 0);
		m_recsResource = std::exchange(other.m_recsResource, 0);
		m_headerSize = std::exchange(other.m_headerSize, 0);
		m_packagePwd = std::move(other.m_packagePwd);
		m_resourceIDs = std::move(other.m_resourceIDs);
		m_resourceMap = std::move(other.m_resourceMap);
	}
}
#endif

PackageHeader& PackageHeader::operator=(const PackageHeader &other) {
	if (this != &other) {
		m_headerID = other.m_headerID;
		m_recsSection = other.m_recsSection;
		m_recsResource = other.m_recsResource;
		m_headerSize = other.m_headerSize;
		m_packagePwd = other.m_packagePwd;
		m_resourceIDs = other.m_resourceIDs;
		m_resourceMap = other.m_resourceMap;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
PackageHeader& PackageHeader::operator=(PackageHeader &&other) noexcept {
	if (this != &other) {
		m_headerID = std::exchange(other.m_headerID, 0);
		m_recsSection = std::exchange(other.m_recsSection, 0);
		m_recsResource = std::exchange(other.m_recsResource, 0);
		m_headerSize = std::exchange(other.m_headerSize, 0);
		m_packagePwd = std::move(other.m_packagePwd);
		m_resourceIDs = std::move(other.m_resourceIDs);
		m_resourceMap = std::move(other.m_resourceMap);
	}
	return *this;
}
#endif

bool PackageHeader::operator==(const PackageHeader &other) {
	if (this != &other) {
		return (m_headerID == other.m_headerID &&
				m_recsSection == other.m_recsSection &&
				m_recsResource == other.m_recsResource &&
				m_headerSize == other.m_headerSize &&
				m_packagePwd == other.m_packagePwd &&
				m_resourceIDs == other.m_resourceIDs &&
				lower_copy(m_resourceMap) == lower_copy(other.m_resourceMap));
	} else {
		return true;
	}
}

bool PackageHeader::operator!=(const PackageHeader& other) {
	if (this != &other) {
		return (m_headerID != other.m_headerID ||
				m_recsSection != other.m_recsSection ||
				m_recsResource != other.m_recsResource ||
				m_headerSize != other.m_headerSize ||
				m_packagePwd != other.m_packagePwd ||
				m_resourceIDs != other.m_resourceIDs ||
				lower_copy(m_resourceMap) != lower_copy(other.m_resourceMap));
	} else {
		return false;
	}
}

PackageHeader::~PackageHeader() {

}

CompressOpResult PackageHeader::AddResourceSection(const std::wstring sectionPath) {
	return CompressOpResult::Success;
}

PackageHandler::PackageHandler() {}

PackageHandler::PackageHandler(const std::vector<unsigned long> headerIDs) {
	m_headerIDs = headerIDs;
}

PackageHandler::PackageHandler(const PackageHandler &other) {
	if (this != &other) {
		m_headerIDs = other.m_headerIDs;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
PackageHandler::PackageHandler(PackageHandler &&other) noexcept {
	if (this != &other) {
		m_headerIDs = std::move(other.m_headerIDs);
	}
}
#endif

PackageHandler& PackageHandler::operator=(const PackageHandler &other) {
	if (this != &other) {
		m_headerIDs = other.m_headerIDs;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
PackageHandler& PackageHandler::operator=(PackageHandler &&other) noexcept {
	if (this != &other) {
		m_headerIDs = std::move(other.m_headerIDs);
	}
	return *this;
}
#endif

bool PackageHandler::operator==(const PackageHandler &other) {
	if (this != &other) {
		return (m_headerIDs == other.m_headerIDs);
	} else {
		return true;
	}
}

bool PackageHandler::operator!=(const PackageHandler &other) {
	if (this != &other) {
		return (m_headerIDs != other.m_headerIDs);
	} else {
		return false;
	}
}

PackageHandler::~PackageHandler() {}

CompressOpResult PackageHandler::CreatePackage(const std::wstring packFilename, const bool purgeObjectIfExist) {
	if (!packFilename.length()) {
		return CompressOpResult::Fail;
	}
	if (purgeObjectIfExist) {
		if (pathExists(packFilename)) {
			if (FSOpResult::Success != removeObject(packFilename)) {
				return CompressOpResult::Fail;
			}
		}
	}

	return CompressOpResult::Success;
}

CompressOpResult PackageHandler::CreatePackage(const std::wstring packFilename,
	const std::vector<std::wstring> filePaths, const bool purgeObjectIfExist) {
	if (!packFilename.length()) {
		return CompressOpResult::Fail;
	}
	if (!filePaths.size()) {
		return CompressOpResult::Fail;
	}
	if (purgeObjectIfExist) {
		if (pathExists(packFilename)) {
			if (FSOpResult::Success != removeObject(packFilename)) {
				return CompressOpResult::Fail;
			}
		}
	}
	unsigned long long packSz = 0;
	for (size_t i = 0; i < filePaths.size(); ++i) {

	}
	return CompressOpResult::Success;
}

CompressOpResult PackageHandler::ReadPackageHeader(const std::wstring objectPath) {
	return CompressOpResult::Success;
}

CompressOpResult PackageHandler::IsHeadder(bool &isHeader, const std::wstring objectPath) {
	return CompressOpResult::Success;
}

CompressOpResult PackageHandler::IsPackage(bool &isPackage, const std::wstring objectPath) {
	return CompressOpResult::Success;
}

CompressOpResult CompressHandler::CompressBuffer(unsigned char* &compressedBuf, unsigned char* originalBuf,
	const size_t originalBufferSize, size_t &compressedBufferSize,
	const std::wstring compressorName, const unsigned char compressLevel, const unsigned long blockSize,
	const ShuffleOp shuffleOp, const unsigned short typeSize, const unsigned long threadQuantity) const {
	if (!originalBuf || !compressedBuf || 0 >= originalBufferSize) {
		return CompressOpResult::Fail;
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
	int compressOpRes = blosc_set_compressor(wstr2str(compressorNameLow).c_str());
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
	// must return compressed buffer size; zero or less indicates an error
	compressOpRes = blosc_compress(compressLvl, static_cast<int>(shuffleOp), typeSz, originalBufferSize,
		originalBuf, compressedBuf, compressedBufferSize);
	if(0 >= compressOpRes) {
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	compressedBufferSize = compressOpRes;
	blosc_destroy();
	return CompressOpResult::Success;
}

CompressOpResult CompressHandler::CompressBuffer(const std::wstring compressedFilePath,
	const unsigned char* originalBuf, const size_t originalBufferSize,
	size_t &compressedBufferSize, const std::wstring compressorName,
	const unsigned char compressLevel, const unsigned long blockSize, const ShuffleOp shuffleOp,
	const unsigned short typeSize, const unsigned long threadQuantity, const bool purgeObjectIfExist) const {
	if (!originalBuf || 0 >= originalBufferSize) {
		return CompressOpResult::Fail;
	}
	if (purgeObjectIfExist) {
		if (pathExists(compressedFilePath)) {
			if (FSOpResult::Success != removeObject(compressedFilePath)) {
				return CompressOpResult::Fail;
			}
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
	int compressOpRes = blosc_set_compressor(wstr2str(compressorNameLow).c_str());
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
	size_t bufSz = getApproxCompressedBufSz(originalBufferSize);
	unsigned char* compressedBuf = (unsigned char*)malloc(bufSz);
	if (!compressedBuf) {
		return CompressOpResult::Fail;
	}
	memset(compressedBuf, 0, bufSz);
	// must return compressed buffer size; zero or less indicates an error
	compressOpRes = blosc_compress(compressLvl, static_cast<int>(shuffleOp), typeSz, originalBufferSize,
		originalBuf, compressedBuf, compressedBufferSize);
	if (0 >= compressOpRes) {
		SAFE_FREE(compressedBuf);
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	compressedBufferSize = compressOpRes;
	blosc_destroy();
	unsigned char* compressedBufCorrect = (unsigned char*)malloc(compressOpRes);
	if (!compressedBufCorrect) {
		SAFE_FREE(compressedBuf);
		return CompressOpResult::Fail;
	}
	memset(compressedBufCorrect, 0, compressOpRes);
	memcpy(compressedBufCorrect, compressedBuf, compressOpRes);
	SAFE_FREE(compressedBuf);
	if (FSOpResult::Success != buffer2File(compressedBufCorrect, compressedFilePath, compressOpRes)) {
		SAFE_FREE(compressedBufCorrect);
		return CompressOpResult::Fail;
	}
	SAFE_FREE(compressedBufCorrect);
	return CompressOpResult::Success;
}

CompressOpResult CompressHandler::CompressFile(unsigned char* &compressedBuf, size_t &compressedBufferSize,
	const std::wstring originalFilePath, const std::wstring compressorName, const unsigned char compressLevel,
	const unsigned long blockSize, const ShuffleOp shuffleOp, const unsigned short typeSize,
	const unsigned long threadQuantity) const {
	if (!compressedBuf) {
		return CompressOpResult::Fail;
	}
	if (!pathExists(originalFilePath)) {
		return CompressOpResult::Fail;
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
	int compressOpRes = blosc_set_compressor(wstr2str(compressorNameLow).c_str());
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
	unsigned long long originalBufferSize = 0;
	if (FSOpResult::Success == getFileSize(originalBufferSize, originalFilePath)) {
		return CompressOpResult::Fail;
	}
	size_t bufSz = originalBufferSize * sizeof(unsigned char);
	unsigned char* originalBuf = (unsigned char*)malloc(bufSz);
	if (!originalBuf) {
		return CompressOpResult::Fail;
	}
	memset(originalBuf, 0, bufSz);
	if (FSOpResult::Success != file2Buffer(originalBuf, originalFilePath, bufSz)) {
		SAFE_FREE(originalBuf);
		return CompressOpResult::Fail;
	}
	// must return compressed buffer size; zero or less indicates an error
	compressOpRes = blosc_compress(compressLvl, static_cast<int>(shuffleOp), typeSz, originalBufferSize,
		originalBuf, compressedBuf, compressedBufferSize);
	if (0 >= compressOpRes) {
		SAFE_FREE(originalBuf);
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	compressedBufferSize = compressOpRes;
	SAFE_FREE(originalBuf);
	blosc_destroy();
	return CompressOpResult::Success;
}

CompressOpResult CompressHandler::CompressFile(const std::wstring compressedFilePath,
	const std::wstring originalFilePath, const std::wstring compressorName, const unsigned char compressLevel,
	const unsigned long blockSize, const ShuffleOp shuffleOp, const unsigned short typeSize,
	const unsigned long threadQuantity, const bool purgeObjectIfExists) const {
	if (!pathExists(originalFilePath)) {
		return CompressOpResult::Fail;
	}
	if (purgeObjectIfExists) {
		if (pathExists(compressedFilePath)) {
			if(FSOpResult::Success != removeObject(compressedFilePath)) {
				return CompressOpResult::Fail;
			}
		}
	}
	unsigned long long fileSize = 0, compressedBufSz =0;
	if (FSOpResult::Success != getFileSize(fileSize, originalFilePath)) {
		return CompressOpResult::Fail;
	}
	unsigned char* originalBuf = (unsigned char*)malloc(fileSize);
	if (!originalBuf) {
		return CompressOpResult::Fail;
	}
	memset(originalBuf, 0, fileSize);
	if (FSOpResult::Success != file2Buffer(originalBuf, originalFilePath, fileSize)) {
		SAFE_FREE(originalBuf);
		return CompressOpResult::Fail;
	}
	compressedBufSz = getApproxCompressedBufSz(fileSize);
	unsigned char* compressedBuf = (unsigned char*)malloc(compressedBufSz);
	if (!compressedBuf) {
		SAFE_FREE(originalBuf);
		return CompressOpResult::Fail;
	}
	memset(compressedBuf, 0, compressedBufSz);
	const std::wstring compressorNameLow = lower_copy(compressorName);
	if (!checkCompressor(compressorNameLow)) {
		SAFE_FREE(originalBuf);
		SAFE_FREE(compressedBuf);
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
	int compressOpRes = blosc_set_compressor(wstr2str(compressorNameLow).c_str());
	if (COMPRESS_BLOSC_INVALIDVAL == compressOpRes) {
		SAFE_FREE(originalBuf);
		SAFE_FREE(compressedBuf);
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
	unsigned long long originalBufferSize = 0;
	if (FSOpResult::Success == getFileSize(originalBufferSize, originalFilePath)) {
		SAFE_FREE(compressedBuf);
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	// must return compressed buffer size; zero or less indicates an error
	compressOpRes = blosc_compress(compressLvl, static_cast<int>(shuffleOp), typeSz, originalBufferSize,
		originalBuf, compressedBuf, compressedBufSz);
	if (0 >= compressOpRes) {
		SAFE_FREE(originalBuf);
		SAFE_FREE(compressedBuf);
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	compressedBufSz = compressOpRes;
	unsigned char* compressedBufCorrect = (unsigned char*)malloc(compressedBufSz);
	if (!compressedBufCorrect) {
		SAFE_FREE(originalBuf);
		SAFE_FREE(compressedBuf);
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	memset(compressedBufCorrect, 0, compressedBufSz);
	memcpy(compressedBufCorrect, compressedBuf, compressedBufSz);
	SAFE_FREE(compressedBuf);
	if (FSOpResult::Success != buffer2File(compressedBufCorrect, compressedFilePath, compressedBufSz)) {
		SAFE_FREE(compressedBufCorrect);
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	SAFE_FREE(originalBuf);
	SAFE_FREE(compressedBufCorrect);
	blosc_destroy();
	return CompressOpResult::Success;
}

CompressOpResult CompressHandler::DecompressBuffer(unsigned char* &decompressedBuf, unsigned char* originalBuf,
	const size_t originalBufferSize, size_t decompressedBufferSize, const std::wstring compressorName,
	const unsigned long blockSize, const unsigned long threadQuantity) const {
	if (!originalBuf || !decompressedBuf || 0 >= originalBufferSize) {
		return CompressOpResult::Fail;
	}
	const std::wstring compressorNameLow = lower_copy(compressorName);
	if (!checkCompressor(compressorNameLow)) {
		return CompressOpResult::Fail;
	}
	blosc_init();
	int compressOpRes = blosc_set_compressor(wstr2str(compressorNameLow).c_str());
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
	// must return greater than zero value; zero or less indicates an error
	compressOpRes = blosc_decompress(originalBuf, decompressedBuf, decompressedBufferSize);
	if (0 >= compressOpRes) {
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	blosc_destroy();
	return CompressOpResult::Success;
}

CompressOpResult CompressHandler::DecompressBuffer(const unsigned char* originalBuf, const size_t originalBufferSize,
	const std::wstring decompressedFilePath, const std::wstring compressorName, const unsigned long blockSize,
	const unsigned long threadQuantity, const bool purgeObjectIfExist) const  {
	if(originalBuf || 0 >= originalBufferSize) {
		return CompressOpResult::Fail;
	}
	const std::wstring compressorNameLow = lower_copy(compressorName);
	if (!checkCompressor(compressorNameLow)) {
		return CompressOpResult::Fail;
	}
	blosc_init();
	int compressOpRes = blosc_set_compressor(wstr2str(compressorNameLow).c_str());
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
	size_t decompressedBufferSize = getDecompressedBufferSz(originalBuf, originalBufferSize);
	if (!decompressedBufferSize) {
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	unsigned char* decompressedBuf = (unsigned char*)malloc(decompressedBufferSize);
	if (!decompressedBuf) {
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	// must return greater than zero value; zero or less indicates an error
	compressOpRes = blosc_decompress(originalBuf, decompressedBuf, decompressedBufferSize);
	if (0 >= compressOpRes) {
		SAFE_FREE(decompressedBuf);
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	if (purgeObjectIfExist) {
		if (pathExists(decompressedFilePath)) {
			if (FSOpResult::Success != removeObject(decompressedFilePath)) {
				SAFE_FREE(decompressedBuf);
				blosc_destroy();
				return CompressOpResult::Fail;
			}
		}
	}
	if (FSOpResult::Success != buffer2File(decompressedBuf, decompressedFilePath, decompressedBufferSize)) {
		SAFE_FREE(decompressedBuf);
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	SAFE_FREE(decompressedBuf);
	blosc_destroy();
	return CompressOpResult::Success;
}

CompressOpResult CompressHandler::DecompressFile(unsigned char* &decompressedBuf, const size_t decompressedBufferSize,
	const std::wstring originalFilePath, const std::wstring compressorName, const unsigned long blockSize,
	const unsigned long threadQuantity, const bool purgeObjectIfExist) const {
	if (!decompressedBuf) {
		return CompressOpResult::Fail;
	}
	if (!pathExists(originalFilePath)) {
		return CompressOpResult::Fail;
	}
	blosc_init();
	const std::wstring compressorNameLow = lower_copy(compressorName);
	if (!checkCompressor(compressorNameLow)) {
		return CompressOpResult::Fail;
	}
	blosc_init();
	int compressOpRes = blosc_set_compressor(wstr2str(compressorNameLow).c_str());
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
	unsigned long long fileSize = 0;
	if (FSOpResult::Success != getFileSize(fileSize, originalFilePath)) {
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	unsigned char* originalBuf = (unsigned char*)malloc(fileSize);
	if (!originalBuf) {
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	if (FSOpResult::Success != file2Buffer(originalBuf, originalFilePath, fileSize)) {
		SAFE_FREE(originalBuf);
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	// must return greater than zero value; zero or less indicates an error
	compressOpRes = blosc_decompress(originalBuf, decompressedBuf, decompressedBufferSize);
	if (0 >= compressOpRes) {
		SAFE_FREE(originalBuf);
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	SAFE_FREE(originalBuf);
	blosc_destroy();
	return CompressOpResult::Success;
}

CompressOpResult CompressHandler::DecompressFile(const std::wstring decompressedFilePath,
	const std::wstring originalFilePath, const std::wstring compressorName, const unsigned long blockSize,
	const unsigned long threadQuantity, const bool purgeObjectIfExist) const {
	if (!pathExists(originalFilePath)) {
		return CompressOpResult::Fail;
	}
	if(purgeObjectIfExist) {
		if (pathExists(decompressedFilePath)) {
			if (FSOpResult::Success != removeObject(decompressedFilePath)) {
				return CompressOpResult::Fail;
			}
		}
	}
	const std::wstring compressorNameLow = lower_copy(compressorName);
	if (!checkCompressor(compressorNameLow)) {
		return CompressOpResult::Fail;
	}
	blosc_init();
	int compressOpRes = blosc_set_compressor(wstr2str(compressorNameLow).c_str());
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
	unsigned long long fileSize = 0;
	if (FSOpResult::Success != getFileSize(fileSize, originalFilePath)) {
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	unsigned char* compressedBuf = (unsigned char*)malloc(fileSize);
	if (!compressedBuf) {
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	memset(compressedBuf, 0, fileSize);
	if (FSOpResult::Success != file2Buffer(compressedBuf, originalFilePath, fileSize)) {
		SAFE_FREE(compressedBuf);
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	size_t decompressedBufSz = getDecompressedBufferSz(compressedBuf, fileSize);
	if (!decompressedBufSz) {
		SAFE_FREE(compressedBuf);
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	unsigned char* decompressedBuf = (unsigned char*)malloc(decompressedBufSz);
	if (!decompressedBuf) {
		SAFE_FREE(compressedBuf);
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	memset(decompressedBuf, 0, decompressedBufSz);
	// must return greater than zero value; zero or less indicates an error
	compressOpRes = blosc_decompress(compressedBuf, decompressedBuf, decompressedBufSz);
	if (0 >= compressOpRes) {
		SAFE_FREE(compressedBuf);
		SAFE_FREE(decompressedBuf);
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	if (FSOpResult::Success != buffer2File(decompressedBuf, decompressedFilePath, decompressedBufSz)) {
		SAFE_FREE(compressedBuf);
		SAFE_FREE(decompressedBuf);
		blosc_destroy();
		return CompressOpResult::Fail;
	}
	SAFE_FREE(compressedBuf);
	SAFE_FREE(decompressedBuf);
	blosc_destroy();
	return CompressOpResult::Success;
}