#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include "Graphics/Shaders/MultitargetShader.h"

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

