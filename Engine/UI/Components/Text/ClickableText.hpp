/**
  * @file           : ClickableText.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/10/22
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_CLICKABLETEXT_HPP
#define HOMELESS_CLICKABLETEXT_HPP
#include <string>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>

namespace engine::ui::components {
	class ClickableText {
		public:
			ClickableText(std::string text);
	};
}
#endif // HOMELESS_CLICKABLETEXT_HPP
