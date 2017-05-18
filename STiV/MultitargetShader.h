// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#pragma once

#include "Timer.h"
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>

class MultitargetShader
{
public:
    MultitargetShader(const std::string& pathToShader);
    MultitargetShader(MultitargetShader&& ms);
    ~MultitargetShader();
    void addTarget(sf::Vector2f where, int startTime);
    void apply(sf::RenderTexture& rt);
private:
    std::vector<sf::Vector2f> mTargetPos;
    std::vector<float> mTargetTime;
    sf::Shader* mShader = nullptr;
};

