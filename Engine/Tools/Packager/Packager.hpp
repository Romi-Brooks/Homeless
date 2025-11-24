#ifndef PACKAGER_HPP
#define PACKAGER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;
namespace engine::tools::package {
	class Packager {
		public:
			struct Config {
				bool compress;
				bool verbose;
				bool preserveStructure;
				bool overwrite;

				Config();
			};

			struct FileEntry {
				std::string filename;
				uint64_t offset;
				uint64_t size;
				std::string type;
				std::string original_path;
			};

			enum class Result {
				SUCCESS,
				ERROR_OPEN_OUTPUT,
				ERROR_READ_FILE,
				ERROR_INVALID_PATH,
				ERROR_EMPTY_PACKAGE,
				ERROR_WRITE_FAILED,
				ERROR_ENCODING
			};

		private:
			std::vector<FileEntry> file_table_;
			std::unordered_map<std::string, size_t> file_index_;

			static constexpr char MAGIC[4] = {'H', 'P', 'K', 'G'};
			static constexpr uint16_t VERSION = 1;

			std::string NormalizePath(const std::string& path);
			std::string GenerateInternalFilename(const fs::path& filePath, const Config& config);
			std::string ToUTF8(const std::string& str);
			std::string SafePathToString(const fs::path& path);
			std::string SafeRelativePath(const fs::path& path);

			bool CollectFiles(const std::vector<std::string>& resourcePaths, std::vector<fs::path>& allFiles, const Config& config);
			bool IsValidUTF8(const std::string& str);


		public:
			Packager() = default;
			~Packager() = default;

			Packager(const Packager&) = delete;
			Packager& operator=(const Packager&) = delete;

			Result Pack(const std::vector<std::string>& resourcePaths, const std::string& outputFile, const Config& config = Config());
			std::vector<std::string> GetPackedFiles() const;
			void PrintPackageInfo() const;
		};
}


#endif // PACKAGER_HPP