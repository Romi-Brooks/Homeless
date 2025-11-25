/**
  * @file           : MetadataReader.cpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/10/12
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#include "MetadataReader.hpp"

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>
#include <taglib/mpegfile.h>
#include <taglib/wavfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/infotag.h>  // For WAV RIFF INFO tag

#include <utf8.h>

#include "../../FileSystem/Encoder/Encoder.hpp"
#include "../../Log/LogSystem.hpp"


namespace engine::audio::metadata {
	auto MetadataReader::GetSongTitle(const Music& music) const -> std::string {
	}

	auto MetadataReader::GetSongProducer(const Music& music) const -> std::string {}

	auto MetadataReader::GetAlbumCover(const Music& music) const -> std::vector<unsigned char> {}

	auto MetadataReader::IsMP3(const std::string& filePath) const -> bool {
	}

	auto MetadataReader::IsWAV(const std::string& filePath) const -> bool {}

	auto MetadataReader::getWAVTitle(const std::string& filePath) const -> std::string {}

	auto MetadataReader::getWAVProducer(const std::string& filePath) const -> std::string {}
}