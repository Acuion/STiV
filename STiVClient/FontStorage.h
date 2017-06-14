#pragma once
#include <SFML/Graphics/Font.hpp>

class FontStorage
{
public:
    static void initFonts()
    {
        mFonts["consolas"].loadFromFile("Content\\consola.ttf");
    }

    static const sf::Font& getFont(const std::string& name)
    {
        return mFonts[name];
    }
private:
    static std::map<std::string, sf::Font> FontStorage::mFonts;
};
