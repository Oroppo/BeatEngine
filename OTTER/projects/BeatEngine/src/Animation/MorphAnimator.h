#pragma once
#include "Gameplay/Physics/TriggerVolume.h"
#include "Gameplay/Components/RenderComponent.h"
#include "Gameplay/Components/ComponentManager.h"
#include "Utils/GlmBulletConversions.h"
#include "Gameplay/Components/IComponent.h"
#include <GLFW/glfw3.h>
#include "Gameplay/Physics/RigidBody.h"
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/ImGuiHelper.h"

#include <memory>

/// <summary>
/// Provides an example behaviour that uses some of the trigger interface to change the material
/// of the game object the component is attached to when entering or leaving a trigger
/// </summary>
using namespace Gameplay;
class Morphanimator : public Gameplay::IComponent {

public:
	template <typename T>
	T Lerp(const T& p0, const T& p1, float t);
	typedef std::shared_ptr<Morphanimator> Sptr;
	Morphanimator();
	virtual ~Morphanimator();

	//MorphAnimator(MorphAnimator&&) = default;
	//MorphAnimator& operator=(MorphAnimator&&) = default;

	virtual void Update(float deltaTime) override;
	virtual void Awake() override;
	void SetInitial();

	//need these cause sages framework requires them in
	virtual void OnTriggerVolumeEntered(const std::shared_ptr<Gameplay::Physics::RigidBody>& body) override;
	virtual void OnTriggerVolumeLeaving(const std::shared_ptr<Gameplay::Physics::RigidBody>& body) override;
	virtual void RenderImGui() override;

	void SetFrameTime(float t);
	void SetFrames(std::vector<MeshResource::Sptr>& f);

	virtual nlohmann::json ToJson() const override;
	static Morphanimator::Sptr FromJson(const nlohmann::json& blob);
	MAKE_TYPENAME(Morphanimator);

protected:

	class AnimData
	{
	public:

		//TODO: You'll need to define a way to store and manage full
		//animation clips for the exercise.
		//make a vector
		std::vector<MeshResource::Sptr> frames;
		//const Mesh* frame0;
		//const Mesh* frame1;

		//The time inbetween frames.
		float frameTime;

		AnimData();
		~AnimData() = default;
	};
	std::unique_ptr<AnimData> m_data;
	float m_timer;
	bool m_forwards;
	size_t m_segmentIndex;

};