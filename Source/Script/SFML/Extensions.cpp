#include "Extensions.hpp"
#include "Extensions.inl"

#include <Util/ResourceManager.hpp>
#include <SFML/Graphics/Texture.hpp>

std::unordered_map<void*, Texture_t*> Script::SFML::shapes;

bool Script::SFML::Extensions::RegisteredExtensions()
{
    const int TotalExtensions = 17;

    int ret = 0;
    ret += Extensions::CircleShape;
    ret += Extensions::Color;
    ret += Extensions::ConvexShape;
    ret += Extensions::Font;
    ret += Extensions::Joystick;
    ret += Extensions::Keyboard;
    ret += Extensions::Mouse;
    ret += Extensions::Music;
    ret += Extensions::Packet;
    ret += Extensions::RectangleShape;
    ret += Extensions::RenderTarget;
    ret += Extensions::Shader;
    ret += Extensions::Sound;
    ret += Extensions::Sprite;
    ret += Extensions::Text;
    ret += Extensions::Texture;
    ret += Extensions::View;

    return ret == TotalExtensions;
}