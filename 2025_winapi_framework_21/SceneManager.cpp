#include "pch.h"
#include "SceneManager.h"
#include "Object.h"
#include "Collider.h"
#include "EndingScene.h"
#include "GameScene.h"
#include "PlayerDeathScene.h"
#include "TitleScene.h"

void SceneManager::PhysicsSyncColliders()
{
	for (UINT i = 0; i < (UINT)Layer::END; ++i)
	{
		const auto& objects = m_curScene->GetLayerObjects((Layer)i);
		for (Object* obj : objects)
		{
			if (!obj)
				continue;

			if (auto* col = obj->GetComponent<Collider>())
				col->LateUpdate(); // sync  
		}
	}
}

void SceneManager::FixedUpdate(float _fixedDT)
{
	if (m_curScene == nullptr)
		return;
	m_curScene->FixedUpdate(_fixedDT);
	PhysicsSyncColliders();
}

void SceneManager::Init()
{
    m_curScene = nullptr;
    // 씬 등록
    RegisterScene(L"GameScene", std::make_shared<GameScene>());
    RegisterScene(L"TitleScene", std::make_shared<TitleScene>());
    RegisterScene(L"PlayerDeathScene", std::make_shared<PlayerDeathScene>());
    RegisterScene(L"EndingScene", std::make_shared<EndingScene>());
    
    // 첫 씬 로드
    LoadScene(L"TitleScene");
}
void SceneManager::Update()
{
	if (m_curScene == nullptr)
		return;
	m_curScene->Update();
	m_curScene->LateUpdate();
}


void SceneManager::Render(HDC _hdc)
{
	if (m_curScene == nullptr)
		return;
	m_curScene->Render(_hdc);
}



void SceneManager::RegisterScene(const wstring& _name, std::shared_ptr<Scene> _scene)
{
	if (_name.empty() || _scene == nullptr)
		return;
	m_mapScenes.insert(m_mapScenes.end(), {_name, _scene});
}

void SceneManager::LoadScene(const wstring& _name)
{
	// 씬이 이미 로드되어있다는것
	if (m_curScene != nullptr)
	{
		m_curScene->Release();
		m_curScene = nullptr;
	}

	auto iter = m_mapScenes.find(_name);
	if (iter != m_mapScenes.end())
	{
		//???
		m_curScene = iter->second;
		m_curScene->Init();
	}
}
