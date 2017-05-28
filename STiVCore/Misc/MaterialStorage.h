#pragma once

#include <Box2D/Box2D.h>
#include <map>

class MaterialStorage
{
	static std::map<std::string, b2FixtureDef> mMaterials;
public:
	static void initMaterials()
	{
		b2FixtureDef mat;
		mat.density = 1;
		mat.friction = 0.3;
		mMaterials["default"] = mat;
	}

	static b2FixtureDef* getMaterial(const std::string& name)
	{
		return new b2FixtureDef(mMaterials[name]);
	}
};

