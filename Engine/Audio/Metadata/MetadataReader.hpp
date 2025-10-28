/**
  * @file           : MetadataReader.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/10/12
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_METADATAREADER_HPP
#define HOMELESS_METADATAREADER_HPP

#include <string>

#include "../../Audio/Music.hpp"

namespace engine::audio::metadata {
	class MetadataReader {
		public:
			MetadataReader(const MetadataReader&) = delete;
			MetadataReader& operator=(const MetadataReader&) = delete;

			static MetadataReader& GetInstance() {
				static MetadataReader instance;
				return instance;
			}

			auto GetSongTitle(const Music& music) const -> std::string;

			auto GetSongProducer(const Music& music) const -> std::string;

			auto GetAlbumCover(const Music& music) const -> std::vector<unsigned char>;

		private:
			MetadataReader() = default;

			auto IsMP3(const std::string& filePath) const -> bool;

			auto IsWAV(const std::string& filePath) const -> bool;

			// for wav file, public api use this function
			auto getWAVTitle(const std::string& filePath) const -> std::string;

			auto getWAVProducer(const std::string& filePath) const -> std::string;
	};
}


#endif // HOMELESS_METADATAREADER_HPP
