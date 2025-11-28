#ifndef UNPACKAGER_HPP
#define UNPACKAGER_HPP

// Standard Library
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <memory>

namespace engine::tools::unpackage {
	class Unpackager final {
		public:
		    struct Config {
		        bool verbose;
		        bool preserveStructure;
		        bool overwrite;
		        std::string outputDir;

		        Config();
		    };

		    struct FileEntry {
		        std::string filename;
		        uint64_t offset;
		        uint64_t size;
		        std::string type;
		    };

		    struct MemoryFile {
		        std::string filename;
		        std::string type;
		        std::vector<char> data;

		        [[nodiscard]] const char* GetData() const;
		        [[nodiscard]] size_t GetSize() const;
		        [[nodiscard]] std::string ToString() const;
		    };

		    // 操作结果
		    enum class Result {
		        SUCCESS,
		        ERROR_OPEN_FILE,
		        ERROR_INVALID_FORMAT,
		        ERROR_READ_FILE_TABLE,
		        ERROR_EXTRACT_FILE,
		        ERROR_CREATE_DIRECTORY,
		        ERROR_ENCODING,
		        ERROR_CORRUPTED_PACKAGE,
		        ERROR_FILES_NOT_FOUND,
		        ERROR_READ_DATA
		    };

		    Unpackager() = default;
		    ~Unpackager();

		    Unpackager(const Unpackager&) = delete;
		    Unpackager& operator=(const Unpackager&) = delete;
		    Unpackager(Unpackager&& other) noexcept;
		    Unpackager& operator=(Unpackager&& other) noexcept;

		    Result Load(const std::string& packageFile, bool verbose = true);

		    Result ExtractFileToMemory(const std::string& filename, MemoryFile& memoryFile);
		    Result ExtractFilesToMemory(const std::vector<std::string>& filenames, std::vector<MemoryFile>& memoryFiles);
		    Result ExtractAllToMemory(std::vector<MemoryFile>& memoryFiles);
		    Result GetFileData(const std::string& filename, const char** data, size_t* size);
			std::unique_ptr<MemoryFile> ExtractFileToMemory(const std::string& filename);

		    Result UnpackAll(const Config& config = Config());
		    Result ExtractFile(const std::string& filename, const Config& config = Config());

		    std::vector<std::string> GetFileList() const;
		    bool Contains(const std::string& filename) const;
		    const FileEntry* GetFileInfo(const std::string& filename) const;
		    void PrintPackageInfo() const;

		private:
		    std::vector<FileEntry> file_table_;
		    std::unordered_map<std::string, size_t> file_index_;
		    std::string package_path_;
		    std::ifstream package_stream_;

		    // 魔数和版本号（
		    static constexpr char MAGIC[4] = {'H', 'P', 'K', 'G'};
		    static constexpr uint16_t VERSION = 1;

		    // 工具函数
		    bool CreateDirectory(const std::filesystem::path& dir_path);
		    std::string SafePathToString(const std::filesystem::path& path);
		    uint64_t GetFileSize(const std::string& filename);
		    Result ReadFileData(const FileEntry& entry, std::vector<char>& buffer);
		};
}

#endif // UNPACKAGER_HPP