#pragma once
#include "Player.h"

class Object;
struct SpawnObject
{
	Object* obj;
	Layer type;
};
class Scene
{
public:
	Scene();
	virtual ~Scene(); // 가상 소멸자
public:
	virtual void Init() abstract;
	virtual void Update();
	virtual void LateUpdate();
	virtual void FixedUpdate(float _fixedDT);
	
	virtual void Render(HDC _hdc);
	virtual void Release();
public:
	void AddObject(Object* _obj, Layer _type)
	{
		m_vecObj[(UINT)_type].push_back(_obj);
	}
	const vector<Object*>& GetLayerObjects(Layer _type) const
	{
		return m_vecObj[(UINT)_type];
	}
	// 자신을 기준으로 가장 가까운 레이어의 오브젝트를 구하는 메서드(없으면 nullptr을 반환)(_distance는 조건 거리(0이거나 넣지 않으면 씬 전체))
	template<typename T>
	T* GetNearbyObject(Entity* _self, Layer _layer, float _distance = 0.f)
	{
		static_assert(std::is_base_of<Object, T>::value, "Object로부터 상속받아야 함");
		T* returnObjValue = nullptr;
		vector<Object*> objects = GetLayerObjects(_layer);
		for (auto obj : objects)
		{
			T* objectcast = dynamic_cast<T*>(obj);
			if (objectcast == nullptr)
				continue;

			if (objectcast->GetIsDead())
				continue;

			float distance = Vector2::Distance(objectcast->GetPos(), _self->GetPos());
			if (distance <= _distance || _distance <= 0.f) 
			{
				if (returnObjValue == nullptr || Vector2::Distance(returnObjValue->GetPos(), _self->GetPos()) > distance)
				{
					returnObjValue = objectcast;
				}
			}
		}
		return returnObjValue;
	}
	template<typename T>
	T* Spawn(Layer _type, Vec2 _pos, Vec2 _size)
	{
		static_assert(std::is_base_of<Object, T>::value, "Object로부터 상속받아야 함");
		T* obj = new T;
		obj->SetPos(_pos);
		obj->SetSize(_size);
		AddObject(obj, _type);
		return obj;
	}
	void RequestDestroy(Object* obj);
	void RequestSpawn(Object* obj, Layer _type);
	void FlushEvent();
	void SetNextScene(std::wstring _name)
	{
		m_nextSceneName = _name;
	}
	void SetExitGame()
	{
		m_exitScene = true;
	}
private:
	void RemoveObject(Object* _obj);
private:
	vector<Object*> m_vecObj[(UINT)Layer::END];
	vector<Object*> m_killObject;
	vector<SpawnObject> m_spawnObject;
	std::wstring m_nextSceneName;
	bool m_exitScene = false;
};

