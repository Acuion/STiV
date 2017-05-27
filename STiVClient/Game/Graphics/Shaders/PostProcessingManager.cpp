#include "PostProcessingManager.h"

std::map<PostProcessingManager::MultitargetShaderType, MultitargetShader> PostProcessingManager::mShaderFromType;

void PostProcessingManager::init()
{
    mShaderFromType.emplace(std::make_pair(MultitargetShaderType::Explosion, MultitargetShader("Content\\Shaders\\explosion.frag")));
}

void PostProcessingManager::addTargetToShaderProcessing(MultitargetShaderType shader, const sf::Vector2f& where, float time)
{
    if (shader == MultitargetShaderType::None)
        return;
    mShaderFromType.at(shader).addTarget(where, time);
}

void PostProcessingManager::postprocess(sf::RenderTexture & rt)
{
    for (auto& x : mShaderFromType)
        x.second.apply(rt);
}
