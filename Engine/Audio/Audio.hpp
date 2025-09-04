#ifndef HOMELESS_AUDIO_HPP
#define HOMELESS_AUDIO_HPP

#include <SFML/Audio/Music.hpp>

namespace engine {
	class Audio {	// Make sure that Audio controller is a singleton mode
		protected:
			std::vector<sf::Music> audios_ {};  // Music list

		private:
			Audio() = default;
			~Audio() = default;

		public:
			Audio(const Audio&) = delete;

			static Audio& GetAudio() {
				static Audio audio;
				return audio;
			}

			auto LoadFile(const std::string& file) -> void {
				audios_.emplace_back(file);
			}

			auto Play(const size_t index) -> void {
				audios_[index].play();
			}

			auto GetFileName(const size_t index) -> std::string {
				return audios_[index].
			}


	};
}


#endif //HOMELESS_AUDIO_HPP