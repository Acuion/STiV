#include "Graphics/Shaders/MultitargetShader.h"
#include "Misc/Timer.h"
#include <SFML/OpenGL.hpp>

MultitargetShader::MultitargetShader(const std::string & pathToShader)
{
    mShader = new sf::Shader();
    mShader->loadFromFile(pathToShader, sf::Shader::Type::Fragment);
 }

MultitargetShader::MultitargetShader(MultitargetShader && ms)
{
    mTargetPos = std::move(ms.mTargetPos);
    mTargetTime = std::move(ms.mTargetTime);
    mShader = ms.mShader;
    ms.mShader = nullptr;
}

MultitargetShader::~MultitargetShader()
{
    delete mShader;
}

void MultitargetShader::addTarget(sf::Vector2f where, int startTime)
{
    mTargetPos.push_back(where);
    mTargetTime.push_back(Timer::getElapsedTime() - startTime);
}

void MultitargetShader::apply(sf::RenderTexture & rt)
{
    mShader->setUniform("currentTexture", sf::Shader::CurrentTexture);
    mShader->setUniform("textureSize", (sf::Vector2f)rt.getSize());

    for (auto& x : mTargetPos)
    {
        auto& gv = rt.getView();
        int dx = gv.getCenter().x - gv.getSize().x / 2;
        int dy = gv.getCenter().y - gv.getSize().y / 2;
        x.x -= dx;
        x.y -= dy;
    }

    for (int i = 0; i < mTargetPos.size(); i += 100)
    {
        mShader->setUniform("targetsCount", (int)mTargetPos.size() - i);
        mShader->setUniformArray("targetPos", &mTargetPos[i], mTargetPos.size() - i);
        mShader->setUniformArray("targetTime", &mTargetTime[i], mTargetTime.size() - i);
        rt.draw(sf::Sprite(rt.getTexture()), sf::RenderStates(mShader));
        rt.display();
    }
    mTargetPos.clear();
    mTargetTime.clear();
}
