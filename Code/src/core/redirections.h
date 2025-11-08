#pragma once

using RenderContext = sf::RenderWindow;
using InputEvent		= sf::Event;
using Color					= sf::Color;
using Vector2f			= sf::Vector2f;

namespace Json = rapidjson;

enum class InputEventState
{
	Handled,
	Unhandled
};
