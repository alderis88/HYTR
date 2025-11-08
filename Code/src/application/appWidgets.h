
// Disable multiple Include
#ifndef APP_WIDGETS_H
#define APP_WIDGETS_H

#include "../framework/widget.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "../core/redirections.h"

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
}

#endif // APP_WIDGETS_H
