#ifndef HOMELESS_ENTITY_HPP
#define HOMELESS_ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <memory>

enum class Movement {
	Entity_MoveUp,
	Entity_MoveDown,
	Entity_MoveLeft,
	Entity_MoveRight
};

class Entity {
	protected:

		std::unique_ptr<sf::Texture> texture_;

		std::unique_ptr<sf::CircleShape> shape_;

		unsigned int hp_;
		unsigned int attack_;

		bool isSelected_ = false;

	public:
		Entity(const unsigned int hp, const unsigned int attack) : hp_(hp), attack_(attack) {};

		virtual ~Entity() = default;

		Entity(Entity&& other) noexcept : shape_(std::move(other.shape_)), hp_(other.hp_), attack_(other.attack_) {}

		// 移动赋值运算符
		Entity& operator=(Entity&& other) noexcept {
				if (this != &other) {
					shape_ = std::move(other.shape_);
				}
				return *this;
			}

		auto CreateCircleWithColor(float radius, const sf::Color& color) -> sf::Shape* {
				shape_ = std::make_unique<sf::CircleShape>(radius);
				shape_->setFillColor(color);
				return shape_.get();
			}

		auto CreateCircle(float radius) -> sf::Shape* {
				shape_ = std::make_unique<sf::CircleShape>(radius);
				return shape_.get();
			}

		auto LoadTexture(const std::string& path) const -> bool {
			if (this->texture_->loadFromFile(path) == false) {
				return false;
			}

			if (!this->shape_) {
				return false;
			}

			this->shape_->setTexture(this->texture_.get());
			this->shape_->setTextureRect(sf::IntRect());
			return true;
		}

		virtual auto Move(const Movement Signal) const -> void {
				// should give to subclass
		}

		auto Attack(Entity& target) const -> bool {
			if (!this->IsAlive()) {
				return false;
			}
			target.Damage(this->attack_);
			return true;
		}

		auto Damage(unsigned int damage) -> bool {
			if (!this->IsAlive()) {
				return false;
			}
			this->hp_= this->hp_ - damage;
			return true;
		}


		auto SetSelected(bool selected) -> void { isSelected_ = selected; }

		virtual auto setPosition(float x, float y) -> bool {
				shape_->setPosition({x, y});
				return true;
			}


		auto IsAlive() const ->bool { return this->hp_ > 0; }

		auto IsSelected() const -> bool { return isSelected_; }


		auto GetShape() const -> sf::Shape* {
			return shape_.get();
		}

		auto GetRadius() const -> float {
			return shape_.get()->getRadius();
		}

		auto GetHP() const -> unsigned int {
			return this->hp_;
		}

		auto GetAttack() const -> unsigned int {
			return this->attack_;
		}

		auto GetPosition() const -> sf::Vector2f {
			return	this->shape_.get()->getPosition();
		}

		virtual auto Draw(sf::RenderWindow& window) const -> void {
				if (shape_) {
					window.draw(*shape_);
				}
		}


		// Can not copy a unique ptr
		Entity(const Entity&) = delete;
		Entity& operator=(const Entity&) = delete;
};


#endif //HOMELESS_ENTITY_HPP