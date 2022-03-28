#include "Animation/MorphAnimationManager.h"
#include "Gameplay/GameObject.h"
#include "Utils/ImGuiHelper.h"
#include "Utils/JsonGlmHelpers.h"
#include "Gameplay/Scene.h"
#include "GLFW/glfw3.h"

MorphAnimationManager::MorphAnim::MorphAnim(const std::vector<Gameplay::MeshResource::Sptr>& meshes, float pace) {
	index = 0;
	secondsBetween = pace;
	done = false;

	for (int i = 0; i < meshes.size(); i++) {
		morphFrames.push_back(meshes[i]);
	}
}

MorphAnimationManager::MorphAnimationManager():IComponent() {
	currentlyAnimating = true,
	futureContinuity = true,
	hasDeclaredOrder = false,
	toBePaused = false,
	morphOff = false;
}
MorphAnimationManager::~MorphAnimationManager() = default;

void MorphAnimationManager::AddAnim(const std::vector<Gameplay::MeshResource::Sptr>& meshes, float pace) {
	MorphAnim anim(meshes, pace);
	animations.push_back(anim);
}

void MorphAnimationManager::SetCurrentAnim(int newCur) {
	if (newCur < animations.size()) {
		if (animations[currentAnimIndex].index) { animations[currentAnimIndex].index = 0; }

		currentAnimIndex = newCur;
	}
}

void MorphAnimationManager::SetAnOrder(std::vector<int> anOrder) {
	if (order.size() == anOrder.size()) { order.clear(); }

	for (int i = 0; i < anOrder.size(); i++) {
		order.push_back(anOrder[i]);
	}

	hasDeclaredOrder = true;
	orderIndex = -1;

}

void MorphAnimationManager::SetContinuity(bool c) {
	isContinuous = c;
	futureContinuity = c;
}

void MorphAnimationManager::SetIdleIndex(int i) {
	curIdleIndex = i;
}



void MorphAnimationManager::Update(float deltaTime) {
	//ManageInput();

	if (currentlyAnimating) {

		if (animations[currentAnimIndex].done) {

			//if (toBePaused) {
			//	toBePaused = false;
			//	currentlyAnimating = false;
			//}
			//else {
				if (hasDeclaredOrder) {
					orderIndex = (orderIndex == -1) ? 0 : orderIndex + 1;
					if (orderIndex == order.size()) {
						if (isContinuous) {
							orderIndex = 0;

							animations[currentAnimIndex].done = false;
							animations[currentAnimIndex].index = 0;

							SetCurrentAnim(order[orderIndex]);
							InterpolateMeshes(deltaTime);
						}
						else {
							orderIndex = 0;
							hasDeclaredOrder = false;
							animations[currentAnimIndex].done = false;
							animations[currentAnimIndex].index = 0;
							currentAnimIndex = curIdleIndex;
							InterpolateMeshes(deltaTime);
						}
					}
					else {
						animations[currentAnimIndex].done = false;
						animations[currentAnimIndex].index = 0;
						SetCurrentAnim(order[orderIndex]);
						InterpolateMeshes(deltaTime);
					}
				}
				else {
					animations[currentAnimIndex].done = false;
					animations[currentAnimIndex].index = curIdleIndex;
					InterpolateMeshes(deltaTime);
				}
			//}

		}
		else {
			InterpolateMeshes(deltaTime);
		}

	}

}

void MorphAnimationManager::InterpolateMeshes(float deltaTime) {
	float t;
	int index1;

	m_timer += deltaTime;

	if (m_timer > animations[currentAnimIndex].secondsBetween) {
		m_timer = 0.f;

		animations[currentAnimIndex].index++;

		if (animations[currentAnimIndex].index >= (animations[currentAnimIndex].morphFrames.size() - 1)) {

			if (hasDeclaredOrder && !isContinuous) {
				animations[currentAnimIndex].done = true;
			}
			else if (animations[currentAnimIndex].index > (animations[currentAnimIndex].morphFrames.size() - 1)) {
				animations[currentAnimIndex].index = 0;
			}


		}


	}

	if (!animations[currentAnimIndex].done) {
		t = m_timer / animations[currentAnimIndex].secondsBetween;
		index1 = ((animations[currentAnimIndex].index) >= (animations[currentAnimIndex].morphFrames.size()-1)) ? 0 : (animations[currentAnimIndex].index + 1);

		GetGameObject()->Get<MorphRenderComponent>()->UpdateVBOs(animations[currentAnimIndex].morphFrames[animations[currentAnimIndex].index], animations[currentAnimIndex].morphFrames[index1], t);
	}
}





void MorphAnimationManager::RenderImGui() {
	currentlyAnimating |= LABEL_LEFT(ImGui::Checkbox, "Is_Animating:", &currentlyAnimating);
	int selectedItem = 0;
	LABEL_LEFT(ImGui::Text, "Current Animation: %i" , currentAnimIndex);

		//std::string prefix = "loadedanim_" + std::to_string(i);
		//result[prefix + "_index"] = animations[i].index;
		//result[prefix + "_interval"] = animations[i].secondsBetween;
		//result[prefix + "_is_done"] = animations[i].done;
		//result[prefix + "_frame_count"] = animations[i].morphFrames.size();
		//std::string pref2 = prefix + "_frame_";
		//for (int j = 0; j < animations[i].morphFrames.size(); j++) {
		//	result[pref2 + std::to_string(j)] = animations[i].morphFrames[j]->GetGUID().str();
		//}
	
	//_isProjectionDirty |= LABEL_LEFT(ImGui::DragFloat, "Far Plane ", &_farPlane, 0.1f, _nearPlane);
	//
	//_isProjectionDirty |= ImGui::Checkbox("Is Ortho", &_isOrtho);
	//
	//if (_isOrtho) {
	//	_isProjectionDirty |= LABEL_LEFT(ImGui::DragFloat, "Ortho Scale", &_orthoVerticalScale, 0.01f, 0.01f);
	//}
	//else {
	//	float fov_deg = glm::degrees(_fovRadians);
	//	if (LABEL_LEFT(ImGui::SliderFloat, "FOV (deg) ", &fov_deg, 0.1f, 180.0f)) {
	//		_fovRadians = glm::radians(fov_deg);
	//		_isProjectionDirty |= true;
	//	}
	//}
}

nlohmann::json MorphAnimationManager::ToJson() const {
	nlohmann::json result;

	result["is_currently_animating"] = currentlyAnimating;
	result["current_anim_index"] = currentAnimIndex;
	result["current_idle_index"] = curIdleIndex;
	result["order_index"] = orderIndex;
	result["animation_queue_size"] = order.size();
	for (int i = 0; i < order.size(); i++) {
		result["animation_queue_index_" + std::to_string(i)] = order[i];
	}

	result["current_animation_timer"] = m_timer;

	result["anim_is_continuous"] = isContinuous;
	result["has_future_continuity"] = futureContinuity;
	result["has_declared_order"] = hasDeclaredOrder;
	result["anim_to_be_paused"] = toBePaused;
	result["animation_disabled"] = morphOff;

	result["loaded_animation_count"] = animations.size();
	for (int i = 0; i < animations.size(); i++) {
		std::string prefix = "loadedanim_" + std::to_string(i);
		result[prefix + "_index"] = animations[i].index;
		result[prefix + "_interval"] = animations[i].secondsBetween;
		result[prefix + "_is_done"] = animations[i].done;
		result[prefix + "_frame_count"] = animations[i].morphFrames.size();
		std::string pref2 = prefix + "_frame_";
		for (int j = 0; j < animations[i].morphFrames.size(); j++) {
			result[pref2 + std::to_string(j)] = animations[i].morphFrames[j]->GetGUID().str();
		}
	}

	return result;
}

MorphAnimationManager::

MorphAnimationManager::Sptr MorphAnimationManager::FromJson(const nlohmann::json& blob) {
	MorphAnimationManager::Sptr result = std::make_shared<MorphAnimationManager>();
	result->currentlyAnimating = blob["is_currently_animating"];
	result->currentAnimIndex = blob["current_anim_index"];
	result->curIdleIndex = blob["current_idle_index"];
	result->orderIndex = blob["order_index"];
	int orderSize = blob["animation_queue_size"];

	for (int i = 0; i < orderSize; i++) {
		result->order.push_back(blob["animation_queue_index_" + std::to_string(i)]);
	}

	result->m_timer = blob["current_animation_timer"];
	result->isContinuous = blob["anim_is_continuous"];
	result->futureContinuity = blob["has_future_continuity"];
	result->hasDeclaredOrder = blob["has_declared_order"];
	result->toBePaused = blob["anim_to_be_paused"];
	result->morphOff = blob["animation_disabled"];
	int numOfAnims = blob["loaded_animation_count"];

	for (int i = 0; i < numOfAnims; i++) {
		std::string prefix = "loadedanim_" + std::to_string(i);
		std::vector<Gameplay::MeshResource::Sptr> ms;
		int index = blob[prefix + "_index"];
		float interval = blob[prefix + "_interval"];
		bool complete = blob[prefix + "_is_done"];
		int frameCount = blob[prefix + "_frame_count"];
		std::string pref2 = prefix + "_frame_";
		for (int j = 0; j < frameCount; j++) {
			ms.push_back(ResourceManager::Get<Gameplay::MeshResource>(Guid(blob[pref2 + std::to_string(j)].get<std::string>())));
		}
		result->AddAnim(ms, interval);
		result->animations.back().index = index;
		result->animations.back().done = complete;
	}

	return result;
}