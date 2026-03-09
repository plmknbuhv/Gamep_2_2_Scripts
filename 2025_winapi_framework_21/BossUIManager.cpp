#include "pch.h"
#include "BossUIManager.h"

#include "Health.h"
#include "Overlap.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "TweenManager.h"

BossUIManager::BossUIManager()
{
	m_boss = nullptr;
	m_bossHealth = nullptr;
	m_BackgroundTex = nullptr;
	m_BarTex = nullptr;
	m_HitedTex = nullptr;
	waitTimer = 0;
	hitObjs.clear();
	bossDead = false;

	
    
}
void BossUIManager::SetBoss(Boss* bs)
{
	this->m_boss = bs;
	m_bossHealth = bs->GetHealthCompo();
	currentHealth = m_bossHealth->GetCurrentHealth();
    m_BackgroundTex = GET_SINGLE(ResourceManager)->GetTexture(L"BossHPBackgroundUI");
    m_BarTex = GET_SINGLE(ResourceManager)->GetTexture(L"BossHPBarUI");
    m_HitedTex = GET_SINGLE(ResourceManager)->GetTexture(L"BossHPHitedUI");

	maxHealth = m_bossHealth->GetMaxHealth();
	currentHealth = maxHealth;
	pos = GetPos();

	uI_WitdhSize = GetSize().x;
	uI_hightSize = GetSize().y;
	startXPos = (pos.x - uI_WitdhSize / 2);
	startYPos = (pos.y - uI_hightSize / 2);
}


BossUIManager::~BossUIManager()
{
}

void BossUIManager::Update()
{
	if (!bossDead)
	{
		if (m_bossHealth->GetCurrentHealth() != currentHealth)
		{
			// 채력 변화가 있음.
			damagedPer = (currentHealth - m_bossHealth->GetCurrentHealth()) / maxHealth;
			currentHealth = m_bossHealth->GetCurrentHealth();
			if (currentHealth <= 0)
			{
				bossDead = true;
				Persent_text = L"00.0%";
				
				GET_SINGLE(TweenManager)->MoveTo(this,{pos.x,pos.y- 10},1,EaseType::Linear,[this]()
				{
					SetDead();
				});
				return;
			}
			hited = true;
			waitTimer += 0.1f;
			wstring text = std::to_wstring((currentHealth / m_bossHealth->GetMaxHealth()) * 100 );
			Persent_text =  text.substr(0,text.find('.') + 2 + 1)+ L"%";

			HealthHitObj* obj = new HealthHitObj;
			int sizeX =  (uI_WitdhSize - uI_barPadingSize * 2)* damagedPer;
			int sizeY = uI_hightSize - uI_barPadingSize * 2;
			int hitstartXpos = startXPos+ uI_barPadingSize +  (uI_WitdhSize - uI_barPadingSize * 2) * (currentHealth/maxHealth);
			int hitstartYPos = startYPos+ uI_barPadingSize;
			obj->SetPosScale({hitstartXpos,hitstartYPos},{ sizeX ,sizeY});
			hitObjs.push_back(obj);
		}
		if (hited)
		{
			if (waitTimer <= 0 )
			{
				hited = false;
			}else
			{
				waitTimer -= fDT;
			}
		}
	}else
	{
		
	}
    
	for (HealthHitObj* hit_obj : hitObjs)
	{

		if (!hit_obj->GetDestroy())
			hit_obj->Update();
		if (hit_obj->GetDestroy())
		{
			hitObjs.erase(std::remove(hitObjs.begin(), hitObjs.end(), hit_obj), hitObjs.end());
			SAFE_DELETE(hit_obj)
		}
	}
	
    
}

void BossUIManager::Render(HDC _hdc)
{
	if (m_BackgroundTex == nullptr) return;
	LONG width = m_BackgroundTex->GetWidth();
	LONG height = m_BackgroundTex->GetHeight();

	StretchBlt(_hdc
		, startXPos
		, startYPos
		, uI_WitdhSize
		, uI_hightSize
		, m_BackgroundTex->GetTextureDC()
		, 0, 0, width,height,SRCCOPY);

	if (hited)
	{
		StretchBlt(_hdc
		, startXPos+ uI_barPadingSize
		, startYPos+ uI_barPadingSize
		, (uI_WitdhSize - uI_barPadingSize * 2) * (currentHealth/maxHealth)
		, uI_hightSize - uI_barPadingSize * 2
		, m_HitedTex->GetTextureDC()
		, 0, 0, width,height,SRCCOPY);
	}else
	{
		StretchBlt(_hdc
		, startXPos+ uI_barPadingSize
		, startYPos+ uI_barPadingSize
		, (uI_WitdhSize - uI_barPadingSize * 2) * (currentHealth/maxHealth)
		, uI_hightSize - uI_barPadingSize * 2
		, m_BarTex->GetTextureDC()
		, 0, 0, width,height,SRCCOPY);
	}
	

	
	
	Object::Render(_hdc);

	for (HealthHitObj* hit_obj : hitObjs)
	{
		if (!hit_obj->GetDestroy())
			hit_obj->Render(_hdc);
	}
	/*if (currentHealth / maxHealth > 0.5)
	{
		GDISelector green(_hdc, PenType::GREEN);
	}else
	{
		GDISelector red(_hdc, PenType::RED);
	}*/
	::TextOut(_hdc,pos.x,pos.y,Persent_text.c_str(),Persent_text.length());	
    
}

