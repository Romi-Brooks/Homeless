#ifndef HOMELESS_PACKAGE_HPP
#define HOMELESS_PACKAGE_HPP

#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>


class Package {
	struct FileEntry {
		std::string filename;  // 存储 UTF-8 编码（统一内部格式，方便后续解包）
		size_t offset;
		size_t size;
		std::string type;
	};
};

#endif // HOMELESS_PACKAGE_HPP