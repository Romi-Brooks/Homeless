#include <iostream>
#include "utf8.h"

#include "Unpackager.hpp"

namespace engine::tools::unpackage {
	Unpackager::Config::Config() : verbose(false), preserveStructure(true), overwrite(true), outputDir(".") {}

	const char* Unpackager::MemoryFile::GetData() const {
	    return data.data();
	}

	size_t Unpackager::MemoryFile::GetSize() const {
	    return data.size();
	}

	std::string Unpackager::MemoryFile::ToString() const {
	    return {data.begin(), data.end()};
	}

	// ==================== 析构函数实现 ====================
	Unpackager::~Unpackager() {
	    if (package_stream_.is_open()) {
	        package_stream_.close();
	    }
	}

	// ==================== 移动构造和移动赋值实现 ====================
	Unpackager::Unpackager(Unpackager&& other) noexcept
	    : file_table_(std::move(other.file_table_))
	    , file_index_(std::move(other.file_index_))
	    , package_path_(std::move(other.package_path_)) {
	    // 流对象移动复杂，新对象按需重新打开
	}

	Unpackager& Unpackager::operator=(Unpackager&& other) noexcept {
	    if (this != &other) {
	        file_table_ = std::move(other.file_table_);
	        file_index_ = std::move(other.file_index_);
	        package_path_ = std::move(other.package_path_);
	        if (package_stream_.is_open()) {
	            package_stream_.close();
	        }
	    }
	    return *this;
	}

	bool Unpackager::CreateDirectory(const fs::path& dir_path) {
	    try {
	        if (!fs::exists(dir_path)) {
	            return fs::create_directories(dir_path);
	        }
	        return true;
	    } catch (const std::exception& e) {
	    	std::cout << "错误: 创建目录失败 " << dir_path << ": " << e.what() << std::endl;
	        return false;
	    }
	}

	std::string Unpackager::SafePathToString(const fs::path& path) {
	    try {
	        return path.string();
	    } catch (const std::exception& e) {
	    	std::cout << "警告: 路径转换错误: " << e.what() << std::endl;
	        return "unknown_path";
	    }
	}

	uint64_t Unpackager::GetFileSize(const std::string& filename) {
	    try {
	        return static_cast<uint64_t>(fs::file_size(filename));
	    } catch (...) {
	        return 0;
	    }
	}

	Unpackager::Result Unpackager::ReadFileData(const FileEntry& entry, std::vector<char>& buffer) {
	    if (!package_stream_.is_open()) {
	        package_stream_.open(package_path_, std::ios::binary);
	        if (!package_stream_.is_open()) {
	            return Result::ERROR_OPEN_FILE;
	        }
	    }

	    package_stream_.seekg(static_cast<std::streamoff>(entry.offset));
	    if (!package_stream_) {
	        return Result::ERROR_READ_DATA;
	    }

	    buffer.resize(entry.size);
	    package_stream_.read(buffer.data(), static_cast<long long>(entry.size));

	    if (!package_stream_) {
	        return Result::ERROR_READ_DATA;
	    }

	    return Result::SUCCESS;
	}

	// ==================== 核心功能：加载包文件 ====================
	Unpackager::Result Unpackager::Load(const std::string& packageFile, bool verbose) {
	    package_path_ = packageFile;

	    if (package_stream_.is_open()) {
	        package_stream_.close();
	    }

	    if (!fs::exists(packageFile)) {
	        if (verbose) std::cout << "错误: 包文件不存在: " << packageFile << std::endl;
	        return Result::ERROR_OPEN_FILE;
	    }

	    uint64_t package_size = GetFileSize(packageFile);
	    if (verbose) std::cout << "包文件大小: " << package_size << " 字节" << std::endl;

	    if (package_size < 20) {
	        if (verbose) std::cout << "错误: 文件太小，不是有效的包文件" << std::endl;
	        return Result::ERROR_INVALID_FORMAT;
	    }

	    std::ifstream input(packageFile, std::ios::binary);
	    if (!input.is_open()) {
	        if (verbose) std::cout << "错误: 无法打开包文件" << std::endl;
	        return Result::ERROR_OPEN_FILE;
	    }

	    // 读取魔数
	    char magic[4];
	    input.read(magic, 4);
	    if (!input) {
	        if (verbose) std::cout << "错误: 读取文件头失败" << std::endl;
	        input.close();
	        return Result::ERROR_INVALID_FORMAT;
	    }

	    std::string magic_str(magic, 4);
	    if (verbose) std::cout << "文件魔数: " << magic_str << std::endl;

	    if (magic_str != "HPKG") {
	        if (verbose) std::cout << "错误: 无效的文件魔数，期望 'HPKG'" << std::endl;
	        input.close();
	        return Result::ERROR_INVALID_FORMAT;
	    }

	    // 读取版本号
	    uint16_t version;
	    input.read(reinterpret_cast<char*>(&version), sizeof(version));
	    if (!input) {
	        if (verbose) std::cout << "错误: 读取版本号失败" << std::endl;
	        input.close();
	        return Result::ERROR_INVALID_FORMAT;
	    }

	    if (verbose) std::cout << "文件版本: " << version << std::endl;
	    if (version != VERSION) {
	        if (verbose) std::cout << "错误: 不支持的版本号: " << version << std::endl;
	        input.close();
	        return Result::ERROR_INVALID_FORMAT;
	    }

	    // 读取文件数量
	    uint32_t file_count;
	    input.read(reinterpret_cast<char*>(&file_count), sizeof(file_count));
	    if (!input) {
	        if (verbose) std::cout << "错误: 读取文件数量失败" << std::endl;
	        input.close();
	        return Result::ERROR_INVALID_FORMAT;
	    }

	    if (verbose) std::cout << "文件数量: " << file_count << std::endl;
	    if (file_count > 1000000) {
	        if (verbose) std::cout << "错误: 文件数量异常: " << file_count << std::endl;
	        input.close();
	        return Result::ERROR_CORRUPTED_PACKAGE;
	    }

	    // 读取文件表偏移量
	    input.seekg(-static_cast<std::streamoff>(sizeof(uint64_t)), std::ios::end);
	    if (!input) {
	        if (verbose) std::cout << "错误: 定位到文件表偏移量失败" << std::endl;
	        input.close();
	        return Result::ERROR_INVALID_FORMAT;
	    }

	    uint64_t table_offset;
	    input.read(reinterpret_cast<char*>(&table_offset), sizeof(table_offset));
	    if (!input) {
	        if (verbose) std::cout << "错误: 读取文件表偏移量失败" << std::endl;
	        input.close();
	        return Result::ERROR_INVALID_FORMAT;
	    }

	    if (verbose) std::cout << "文件表偏移量: " << table_offset << std::endl;
	    if (table_offset >= package_size || table_offset < 10) {
	        if (verbose) std::cout << "错误: 无效的文件表偏移量: " << table_offset << std::endl;
	        input.close();
	        return Result::ERROR_CORRUPTED_PACKAGE;
	    }

	    // 定位到文件表
	    input.seekg(static_cast<std::streamoff>(table_offset));
	    if (!input) {
	        if (verbose) std::cout << "错误: 定位到文件表开始位置失败" << std::endl;
	        input.close();
	        return Result::ERROR_READ_FILE_TABLE;
	    }

	    // 读取文件表
	    file_table_.clear();
	    file_index_.clear();
	    for (uint32_t i = 0; i < file_count; ++i) {
	        if (!input) break;

	        FileEntry entry;
	        uint16_t name_length;
	        input.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
	        if (!input || name_length > 4096) {
	            if (verbose) std::cout << "错误: 无效的文件名长度: " << name_length << std::endl;
	            continue;
	        }

	        if (name_length > 0) {
	            std::vector<char> name_buffer(name_length);
	            input.read(name_buffer.data(), name_length);
	            if (input) {
	                entry.filename = std::string(name_buffer.data(), name_length);
	                if (!utf8::is_valid(entry.filename.begin(), entry.filename.end())) {
	                    entry.filename = "file_" + std::to_string(i);
	                }
	            }
	        } else {
	            entry.filename = "unnamed_file_" + std::to_string(i);
	        }

	        // 读取偏移量和大小
	        uint64_t offset, size;
	        input.read(reinterpret_cast<char*>(&offset), sizeof(offset));
	        input.read(reinterpret_cast<char*>(&size), sizeof(size));
	        if (!input) continue;

	        if (offset >= package_size || size > package_size || offset + size > package_size) {
	            if (verbose) {
	                std::cout << "警告: 跳过无效的文件条目: " << entry.filename
	                          << " (offset=" << offset << ", size=" << size << ")" << std::endl;
	            }
	            continue;
	        }

	        entry.offset = offset;
	        entry.size = size;

	        // 读取文件类型
	        uint8_t type_length;
	        input.read(reinterpret_cast<char*>(&type_length), sizeof(type_length));
	        if (!input) continue;

	        if (type_length > 0 && type_length < static_cast<unsigned char>(256)) {
	            std::vector<char> type_buffer(type_length);
	            input.read(type_buffer.data(), type_length);
	            if (input) {
	                entry.type = std::string(type_buffer.data(), type_length);
	                if (!utf8::is_valid(entry.type.begin(), entry.type.end())) {
	                    entry.type = ".dat";
	                }
	            }
	        }

	        file_table_.push_back(entry);
	        file_index_[entry.filename] = file_table_.size() - 1;
	    }

	    input.close();

	    if (file_table_.empty()) {
	        return Result::ERROR_READ_FILE_TABLE;
	    }

	    if (verbose) {
	        std::cout << "成功加载 " << file_table_.size() << " 个文件条目" << std::endl;
	    }

	    return Result::SUCCESS;
	}

	// ==================== 内存解包功能实现 ====================
	Unpackager::Result Unpackager::ExtractFileToMemory(const std::string& filename, MemoryFile& memoryFile) {
		const auto it = file_index_.find(filename);
	    if (it == file_index_.end()) {
	        return Result::ERROR_FILES_NOT_FOUND;
	    }

	    const FileEntry& entry = file_table_[it->second];
	    std::vector<char> buffer;
		const Result result = ReadFileData(entry, buffer);
	    if (result != Result::SUCCESS) {
	        return result;
	    }

	    memoryFile.filename = entry.filename;
	    memoryFile.type = entry.type;
	    memoryFile.data = std::move(buffer);

	    return Result::SUCCESS;
	}

	std::unique_ptr<Unpackager::MemoryFile> Unpackager::ExtractFileToMemory(const std::string& filename) {
	    auto memoryFile = std::make_unique<MemoryFile>();
		const Result result = ExtractFileToMemory(filename, *memoryFile);
	    if (result == Result::SUCCESS) {
	        return memoryFile;
	    }
	    return nullptr;
	}

	Unpackager::Result Unpackager::ExtractFilesToMemory(const std::vector<std::string>& filenames,
	                                                   std::vector<MemoryFile>& memoryFiles) {
	    memoryFiles.clear();

	    for (const auto& filename : filenames) {
	        MemoryFile memoryFile;
			const Result result = ExtractFileToMemory(filename, memoryFile);
	        if (result == Result::SUCCESS) {
	            memoryFiles.push_back(std::move(memoryFile));
	        } else {
	        	std::cout << "警告: 无法提取文件到内存: " << filename << std::endl;
	        }
	    }

	    return memoryFiles.empty() ? Result::ERROR_FILES_NOT_FOUND : Result::SUCCESS;
	}

	Unpackager::Result Unpackager::ExtractAllToMemory(std::vector<MemoryFile>& memoryFiles) {
	    memoryFiles.clear();

	    for (const auto& entry : file_table_) {
	        MemoryFile memoryFile;
			const Result result = ExtractFileToMemory(entry.filename, memoryFile);
	        if (result == Result::SUCCESS) {
	            memoryFiles.push_back(std::move(memoryFile));
	        } else {
	        	std::cout << "警告: 无法提取文件到内存: " << entry.filename << std::endl;
	        }
	    }

	    return memoryFiles.empty() ? Result::ERROR_READ_DATA : Result::SUCCESS;
	}

	Unpackager::Result Unpackager::GetFileData(const std::string& filename, const char** data, size_t* size) {
		const auto it = file_index_.find(filename);
	    if (it == file_index_.end()) {
	        return Result::ERROR_FILES_NOT_FOUND;
	    }

	    const FileEntry& entry = file_table_[it->second];
	    std::vector<char> buffer;
		const Result result = ReadFileData(entry, buffer);
	    if (result != Result::SUCCESS) {
	        return result;
	    }

	    *data = buffer.data();
	    *size = buffer.size();

	    return Result::SUCCESS;
	}

	// ==================== 磁盘解包功能实现 ====================
	Unpackager::Result Unpackager::UnpackAll(const Config& config) {
	    if (file_table_.empty()) {
	        return Result::ERROR_READ_FILE_TABLE;
	    }

		auto overall_result = Result::SUCCESS;
	    int success_count = 0;

	    for (const auto& entry : file_table_) {
			const Result file_result = ExtractFile(entry.filename, config);
	        if (file_result == Result::SUCCESS) {
	            success_count++;
	        } else if (overall_result == Result::SUCCESS) {
	            overall_result = file_result;
	        }
	    }

	    if (config.verbose) {
	        std::cout << "解包完成: " << success_count << "/" << file_table_.size()
	                  << " 个文件解压到 " << config.outputDir << std::endl;
	    }

	    return overall_result;
	}

	Unpackager::Result Unpackager::ExtractFile(const std::string& filename, const Config& config) {
	    auto it = file_index_.find(filename);
	    if (it == file_index_.end()) {
	        if (config.verbose) {
	            std::cout << "错误: 文件未找到 - " << filename << std::endl;
	        }
	        return Result::ERROR_EXTRACT_FILE;
	    }

	    const FileEntry& entry = file_table_[it->second];

	    // 构建输出路径
	    fs::path output_path;
	    try {
	        output_path = config.outputDir;
	        if (config.preserveStructure) {
	            output_path /= filename;
	        } else {
	            fs::path file_path(filename);
	            output_path /= file_path.filename();
	        }
	    } catch (const std::exception& e) {
	        if (config.verbose) {
	            std::cout << "错误: 构建输出路径失败: " << e.what() << std::endl;
	        }
	        return Result::ERROR_ENCODING;
	    }

	    // 创建目录
	    fs::path output_dir = output_path.parent_path();
	    if (!CreateDirectory(output_dir)) {
	        return Result::ERROR_CREATE_DIRECTORY;
	    }

	    // 检查文件是否已存在
	    try {
	        if (fs::exists(output_path) && !config.overwrite) {
	            if (config.verbose) {
	                std::cout << "跳过: 文件已存在 - " << SafePathToString(output_path) << std::endl;
	            }
	            return Result::SUCCESS;
	        }
	    } catch (const std::exception& e) {
	        if (config.verbose) {
	            std::cout << "警告: 检查文件存在性失败: " << e.what() << std::endl;
	        }
	    }

	    // 读取文件数据
	    std::vector<char> buffer;
	    Result read_result = ReadFileData(entry, buffer);
	    if (read_result != Result::SUCCESS) {
	        return read_result;
	    }

	    // 写入文件
	    std::ofstream output;
	    try {
	        output.open(output_path, std::ios::binary);
	    } catch (const std::exception& e) {
	        if (config.verbose) {
	            std::cout << "错误: 无法创建输出文件 " << SafePathToString(output_path)
	                      << ": " << e.what() << std::endl;
	        }
	        return Result::ERROR_EXTRACT_FILE;
	    }

	    if (!output.is_open()) {
	        return Result::ERROR_EXTRACT_FILE;
	    }

	    output.write(buffer.data(), static_cast<long long>(entry.size));
	    output.close();

	    if (config.verbose) {
	        std::cout << "解压: " << SafePathToString(output_path) << " (" << entry.size << " 字节)" << std::endl;
	    }

	    return Result::SUCCESS;
	}

	// ==================== 查询功能实现 ====================
	std::vector<std::string> Unpackager::GetFileList() const {
	    std::vector<std::string> files;
	    for (const auto& entry : file_table_) {
	        files.push_back(entry.filename);
	    }
	    return files;
	}

	bool Unpackager::Contains(const std::string& filename) const {
	    return file_index_.contains(filename);
	}

	const Unpackager::FileEntry* Unpackager::GetFileInfo(const std::string& filename) const {
		const auto it = file_index_.find(filename);
	    if (it != file_index_.end()) {
	        return &file_table_[it->second];
	    }
	    return nullptr;
	}

	void Unpackager::PrintPackageInfo() const {
		std::cout << "包文件: " << package_path_ << std::endl;
		std::cout << "包含 " << file_table_.size() << " 个文件:" << std::endl;

		uint64_t total_size = 0;
		for (const auto& entry : file_table_) {
			std::string display_name;
			for (const char c : entry.filename) {
				if (c >= 32 && c <= 126) {
					display_name += c;
				} else if (display_name.length() < 50) {
					display_name += '?';
				}
			}

			if (display_name.length() > 50) {
				display_name = display_name.substr(0, 47) + "...";
			}

			std::cout << "  " << display_name << " [" << entry.type << "] - "
					  << entry.size << " 字节" << std::endl;
			total_size += entry.size;
		}

		std::cout << "总大小: " << total_size << " 字节" << std::endl;
		}
}
