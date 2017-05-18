// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#pragma once

#include <SFML\Graphics.hpp>
#include <map>
#include "MultitargetShader.h"

class PostProcessingManager
{
public:
    enum class MultitargetShaderType
    {
        None,
        Explosion
    };
    static void init();
    static void addTargetToShaderProcessing(MultitargetShaderType shader, const sf::Vector2f& where, float time);
    static void postprocess(sf::RenderTexture& rt);
private:
    PostProcessingManager();
    static std::map<MultitargetShaderType, MultitargetShader> mShaderFromType;
};

