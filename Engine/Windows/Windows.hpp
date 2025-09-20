#ifndef HOMELESS_WINDOWS_HPP
#define HOMELESS_WINDOWS_HPP

#include <string>
#include <SFML/Graphics/RenderWindow.hpp>

namespace atoms_engine_window {
	class Windows {
		private:
			sf::RenderWindow *window_ = nullptr;
			std::string *windowName_ = nullptr;

		public:
			Windows() = default;

			[[nodiscard]] auto getRenderWindow() const -> sf::RenderWindow* {
				return this->window_;
			}

			Windows(Windows& ) = delete;
			Windows(Windows&& ) = delete;
	};
}

#endif //HOMELESS_WINDOWS_HPP