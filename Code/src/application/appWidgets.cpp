#include "../framework/widget.h"

namespace ui
{
	class Image : public Widget
	{
	public:
		Image(const std::string& imagePath);
		~Image() override = default;

		void Draw(RenderContext& context) const override;
		InputEventState ProcessInput(const InputEvent& event) override;

	private:
		sf::Texture m_texture;
		sf::Sprite m_sprite;
	};

	Image::Image(const std::string& imagePath)
	{
		if (!m_texture.loadFromFile(imagePath))
		{
			throw std::runtime_error("Failed to load image: " + imagePath);
		}
		m_sprite.setTexture(m_texture);
	}

	void Image::Draw(RenderContext& context) const
	{
		context.draw(m_sprite);
	}

	InputEventState Image::ProcessInput(const InputEvent& event)
	{
		// Implement input handling logic if needed
		return InputEventState::Unhandled;
	}
}
