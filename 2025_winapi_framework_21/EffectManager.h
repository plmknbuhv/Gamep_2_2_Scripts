#pragma once
#include "SceneManager.h"

class Effect;
class EffectManager
{
	DECLARE_SINGLE(EffectManager);
public:
	template<typename T>
	void SpawnEffect(Vec2 _pos)
	{
		static_assert(std::is_base_of<Effect, T>::value, "Effect is not base of T");

		Effect* effect = GET_SINGLE(SceneManager)->GetCurScene()
			->Spawn<T>(Layer::EFFECT, _pos, {50, 50});

		effect->PlayEffect(_pos);
	}
};

