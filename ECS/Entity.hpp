#ifndef HOMELESS_ENTITY_HPP
#define HOMELESS_ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <memory>

#include <map>

enum class MoveSig {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Entity {
	private:
		sf::Texture *texture_ = nullptr;

		std::unique_ptr<sf::CircleShape> shape_;

	public:
		explicit Entity(float radius) {
			CreateCircle(radius);
		}

		Entity(float radius, const sf::Color& color) {
			CreateCircleWithColor(radius,color);
		}

		Entity(float radius, const sf::Color& color, const std::string& path) {
				CreateCircle(radius);
				LoadTexture(path);
		}
		// 移动构造函数
		Entity(Entity&& other) noexcept : shape_(std::move(other.shape_)) {}

		// 移动赋值运算符
		Entity& operator=(Entity&& other) noexcept {
				if (this != &other) {
					shape_ = std::move(other.shape_);
				}
				return *this;
			}
		// 创建圆形纹理的函数
		auto CreateCircleWithColor(float radius, const sf::Color& color) -> sf::Shape* {
			shape_ = std::make_unique<sf::CircleShape>(radius);

			shape_->setFillColor(color);
			return shape_.get(); // 返回原始指针（如果需要）
		}

		// 创建圆形纹理的函数
		auto CreateCircle(float radius) -> sf::Shape* {
			shape_ = std::make_unique<sf::CircleShape>(radius);
			return shape_.get(); // 返回原始指针（如果需要）
		}

		auto LoadTexture(const std::string& path) -> bool {
			if (this->texture_->loadFromFile(path) == false) {
				return false;
			}

			if (!this->shape_) {
				return false;
			}

			this->shape_->setTexture(this->texture_);
			this->shape_->setTextureRect(sf::IntRect());
		}

		// 获取形状的方法
		auto Move(MoveSig Signal) -> void {
			switch (Signal) {
				case MoveSig::LEFT: {
					this->shape_->setPosition({this->shape_->getPosition().x - 1, this->shape_->getPosition().y});
					break;
				}
				case MoveSig::RIGHT: {
					this->shape_->setPosition({this->shape_->getPosition().x + 1, this->shape_->getPosition().y});
					break;
				}
				case MoveSig::UP: {
					this->shape_->setPosition({this->shape_->getPosition().x, this->shape_->getPosition().y - 1});
					break;
				}
				case MoveSig::DOWN: {
					this->shape_->setPosition({this->shape_->getPosition().x, this->shape_->getPosition().y + 1});
					break;
				}
				default: break;
			}
		}

		// 获取形状的方法
		auto GetShape() -> sf::Shape* {
				return shape_.get();
		}

		auto GetRadius() -> float {
				return shape_.get()->getRadius();
		}

		auto Draw(sf::RenderWindow& window)-> void {
				if (shape_) {
					window.draw(*shape_);
				}
		}

		// 禁止拷贝（因为unique_ptr不可拷贝）
		Entity(const Entity&) = delete;
		Entity& operator=(const Entity&) = delete;
};


#endif //HOMELESS_ENTITY_HPP