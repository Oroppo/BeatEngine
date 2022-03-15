#include "Animation/MorphAnimator.h"
#include "Gameplay/Components/ComponentManager.h"
#include "Utils/GlmBulletConversions.h"
#include "Gameplay/Components/IComponent.h"
#include <GLFW/glfw3.h>
#include "Gameplay/Physics/RigidBody.h"
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/ImGuiHelper.h"
#include "Animation/MorphMeshRenderer.h"


Morphanimator::AnimData::AnimData()
{
	frames.push_back(nullptr);
	frames.push_back(nullptr);
	frameTime = 1.0f;

}

Morphanimator::Morphanimator() :
	IComponent()
{ }
Morphanimator::~Morphanimator() = default;
void Morphanimator::Awake()
{

}

void Morphanimator::SetInitial()
{
	m_data = std::make_unique<AnimData>();
	m_timer = 0.0f;
	m_forwards = true;
	m_segmentIndex = 0;
}

void Morphanimator::OnTriggerVolumeEntered(const std::shared_ptr<Gameplay::Physics::RigidBody>&body)
{
}

void Morphanimator::OnTriggerVolumeLeaving(const std::shared_ptr<Gameplay::Physics::RigidBody>&body)
{
}

void Morphanimator::RenderImGui()
{
}

void Morphanimator::Update(float deltaTime)
{

	if (m_data->frameTime > 0)
	{
		m_timer += deltaTime;
		while (m_timer > m_data->frameTime)
		{
			m_timer -= m_data->frameTime;

			m_segmentIndex += 1;


		}
		if (m_segmentIndex >= m_data->frames.size())
			m_segmentIndex = 0;
		float t = m_timer / m_data->frameTime;
		
		if (m_data->frames.size() < 2)
		{
			return;
		}
		size_t p0_index, p1_index;

		p1_index = m_segmentIndex;
		p0_index = (p1_index == 0) ? m_data->frames.size() - 1 : p1_index - 1;

		float garb;
		Lerp(p0_index, p1_index, t);
		//little jank to call from mesh renderer's update but it's a  tad more efficient than having 2 updates running
		GetGameObject()->Get<MorphMeshRenderer>()->UpdateData(m_data->frames[p0_index], m_data->frames[p1_index], glm::modf(t, garb));

	}
	else
	{
		return;
	}
}


void Morphanimator::SetFrameTime(float t)
{
	m_data->frameTime = t;
}

void Morphanimator::SetFrames(std::vector<MeshResource::Sptr>&f)
{
	m_data->frames.clear();
	m_data->frames.reserve(f.size());

	for (int i = 0; i < f.size(); i++)
	{
		//m_data->frames.push_back(f[i]->Mesh);
		m_data->frames.push_back(f[i]);
	}
}

nlohmann::json Morphanimator::ToJson() const {

	return {

	};
	return {};
}
template <typename T>
T Morphanimator::Lerp(const T& p0, const T& p1, float t)
{
	return (1.0f - t) * p0 + t * p1;
}
Morphanimator::Sptr Morphanimator::FromJson(const nlohmann::json & blob) {
	Morphanimator::Sptr result = std::make_shared<Morphanimator>();

	return result;
}