#include "Animation/MorphRenderComponent.h"
#include "Gameplay/Components/RenderComponent.h"
#include "Gameplay/Components/ComponentManager.h"
#include "Gameplay/GameObject.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Application/Application.h"
#include "Application/Layers/RenderLayer.h"

MorphRenderComponent::MorphRenderComponent() : IComponent() {
	/*_mesh0 = GetGameObject()->Get<RenderComponent>()->GetMesh();
	_mesh1 = _mesh0;

	UpdateVBOs(_mesh0, _mesh1, 0.f);*/
}

MorphRenderComponent::MorphRenderComponent(const Gameplay::MeshResource::Sptr& baseMesh): IComponent()
{
	_mesh0 = baseMesh;
	_mesh1 = baseMesh;
	_t = 0;


	//UpdateVBOs(_mesh0, _mesh1, 0.f);

}

void MorphRenderComponent::UpdateVBOs(const Gameplay::MeshResource::Sptr& mesh0, const Gameplay::MeshResource::Sptr& mesh1, float t) {
	_t = t;

	//Change your mesh1 bozo
	VertexArrayObject::Sptr vao = VertexArrayObject::Create();

	//Grab The Buffer Bindings attached to target mesh
	VertexBuffer::Sptr pBuff = mesh1->Mesh->GetBufferBinding(AttribUsage::Position)->GetBuffer();
	VertexBuffer::Sptr nBuff = mesh1->Mesh->GetBufferBinding(AttribUsage::Normal)->GetBuffer();
	VertexBuffer::Sptr cBuff = mesh1->Mesh->GetBufferBinding(AttribUsage::Color)->GetBuffer();

	//find the vertex declaration helper 
	const VertexArrayObject::VertexDeclaration& v_dec = (mesh1->Mesh->GetVDecl());
	
	//Assign each attribute to a layout for our shader using v_decl helper and Attribute Hints
	BufferAttribute posBuff(6, 3, AttributeType::Float, v_dec.at(0).Stride, v_dec.at(0).Offset, AttribUsage::Position);
	BufferAttribute norBuff(7, 3, AttributeType::Float, v_dec.at(2).Stride, v_dec.at(2).Offset, AttribUsage::Normal);
	BufferAttribute colBuff(8, 4, AttributeType::Float, v_dec.at(1).Stride, v_dec.at(1).Offset, AttribUsage::Color);
	
	//Package the Data to send to our Vertex Buffer
	const std::vector<BufferAttribute> posAttributes{posBuff};
	const std::vector<BufferAttribute> norAttributes{norBuff};
	const std::vector<BufferAttribute> colAttributes{colBuff};

	//Send the data
	vao->AddVertexBuffer(pBuff, posAttributes);
	vao->AddVertexBuffer(nBuff, norAttributes);
	vao->AddVertexBuffer(cBuff, colAttributes);

	//Swap the VAO to our new mesh
	//if (_t >=1.0)
	GetGameObject()->Get<RenderComponent>()->SetVAO(vao);
}

nlohmann::json MorphRenderComponent::ToJson() const {
	nlohmann::json result;
	result["_mesh0"] = _mesh0 ? _mesh0->GetGUID().str() : "null";
	result["_mesh1"] = _mesh1 ? _mesh1->GetGUID().str() : "null";
	result["_t"] = _t;
	return result;
}

MorphRenderComponent::Sptr MorphRenderComponent::FromJson(const nlohmann::json& data) {
	MorphRenderComponent::Sptr result = std::make_shared<MorphRenderComponent>();
	result->_mesh0 = ResourceManager::Get<Gameplay::MeshResource>(Guid(data["_mesh0"].get<std::string>()));
	result->_mesh1 = ResourceManager::Get<Gameplay::MeshResource>(Guid(data["_mesh1"].get<std::string>()));
	result->_t = data["_t"];

	return result;
}

void MorphRenderComponent::RenderImGui() {
	
}
