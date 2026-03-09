#include "pch.h"
#include "Scene.h"
#include "Object.h"
#include "CollisionManager.h"
#include "Rigidbody.h"
#include "SceneManager.h"

void Scene::FixedUpdate(float _fixedDT)
{
	for (UINT i = 0; i < (UINT)Layer::END; ++i)
	{
		auto& vec = m_vecObj[i];
		for (auto* obj : vec)
		{
			if (auto rb = obj->GetComponent<Rigidbody>())
			{
				if (rb != nullptr)
					rb->FixedUpdate(_fixedDT);
			}
		}
	}
}
Scene::Scene()
{
	m_nextSceneName = L"";
}

Scene::~Scene()
{
	Release();
}

void Scene::Update()
{
	// ?? OBJ UPDATE
	for (UINT i = 0; i < (UINT)Layer::END; ++i)
	{
		auto& vec = m_vecObj[i];
		for (auto* obj : vec)
		{
			if (obj != nullptr)
			{
				if (!obj->GetIsDead())
					obj->Update();
			}
		}
	}
}
void Scene::LateUpdate()
{
	for (UINT i = 0; i < (UINT)Layer::END; ++i)
	{
		auto& vec = m_vecObj[i];
		for (auto* obj : vec)
		{
			if (obj != nullptr)
			{
				if (!obj->GetIsDead())
					obj->LateUpdate();
			}
		}
	}
}

void Scene::Render(HDC _hdc)
{
	for (UINT i = 0; i < (UINT)Layer::END; ++i)
	{
		auto& vec = m_vecObj[i];
		for (auto* obj : vec)
		{
			if (obj != nullptr)
			{
				if (!obj->GetIsDead())
					obj->Render(_hdc);
			}
		}
	}
}

void Scene::Release()
{
	FlushEvent();
	for (UINT i = 0; i < (UINT)Layer::END; ++i)
	{
		auto& vec = m_vecObj[i];
		for (auto* obj : vec)
			SAFE_DELETE(obj);
		vec.clear();
	}
	GET_SINGLE(CollisionManager)->CheckReset();
}

void Scene::RequestDestroy(Object* obj)
{
	if (obj == nullptr)
		return;

	// 죽지 않으면 넣어라
	if (!obj->GetIsDead())
	{
		obj->SetDead();
		// 실제 delete는 FlushObject에서
		m_killObject.push_back(obj);
	}
}
void Scene::FlushEvent()
{
	// 삭제
	for (Object* d : m_killObject)
	{
		if (d == nullptr)
			continue;

		RemoveObject(d);
		SAFE_DELETE(d);
	}
	m_killObject.clear();

	// 생성

	// 씬 변경
	if (!m_nextSceneName.empty())
	{
		// 씬 매니저에게 요청
		wstring temp = m_nextSceneName;
		m_nextSceneName = L"";
		GET_SINGLE(SceneManager)->LoadScene(temp);
	}
	if(m_exitScene)
	{
		m_exitScene = false;
		::PostQuitMessage(0);
	}
}

void Scene::RequestSpawn(Object* obj, Layer _type)
{

}


void Scene::RemoveObject(Object* _obj)
{
	for (UINT i = 0; i < (UINT)Layer::END; ++i)
	{
		auto& v = m_vecObj[i];
		v.erase(std::remove(v.begin(), v.end(), _obj), v.end());
	}

}