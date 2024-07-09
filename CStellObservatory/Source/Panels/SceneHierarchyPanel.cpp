#include "SceneHierarchyPanel.h"

#include "CStell/Scene/Components.h"
#include "CStell/Renderer/ShaderType.h"
#include "CStell/Renderer/MaterialSerializer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace CStell
{
	static std::string s_ModelPath = "asset/model/ModelTest.csmesh";

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		m_Context->GetRegistry().view<entt::entity>().each([&](auto entityID)
			{
				Entity entity{ entityID, m_Context.get() };
				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if(entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.85f, 0.1f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.95f, 0.2f, 0.30f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.85f, 0.1f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X"))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.25f, 0.82f, 0.25f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.35f, 0.9f, 0.35f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.25f, 0.82f, 0.25f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y"))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.5f, 0.85f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.65f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.5f, 0.85f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z"))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth
		| ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y / 2.0f;
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight);

			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (opened)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("Add Component");

		if (ImGui::BeginPopup("Add Component"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				m_SelectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Renderer"))
			{
				m_SelectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Model"))
			{
				m_SelectionContext.AddComponent<ModelComponent>(s_ModelPath);
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [&](auto& component)
			{
				DrawVec3Control("Translation", component.Translation);

				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);

				DrawVec3Control("Scale", component.Scale, 1.0f);
			});

		DrawComponent<CameraComponent>("Camera", entity, [&](auto& component)
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				ImGui::Checkbox("Primary", &cameraComponent.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							cameraComponent.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				switch (camera.GetProjectionType())
				{
				case CStell::SceneCamera::ProjectionType::Perspective:
				{
					float fov = camera.GetPerspectiveFOV();
					if (ImGui::DragFloat("Field of view", &fov)) // TODO: Set min and max fov
						camera.SetPerspectiveFOV(fov);

					float perspectiveNear = (float)camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear))
						camera.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = (float)camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar))
						camera.SetPerspectiveFarClip(perspectiveFar);

					break;
				}
				case CStell::SceneCamera::ProjectionType::Orthographic:
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);

					break;
				}
				default:
					break;
				}
			});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [&](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				//ImGui::DragFloat("Tiling", &component.SpriteMaterial.m_TilingFactor, 1.0f, 1.0f, 100.0f);
			});

		DrawComponent<ModelComponent>("Model", entity, [&](auto& component)
			{
				auto& models = entity.GetComponent<ModelComponent>();

				for (auto& mesh : models.ModelInst.GetMeshAsset()->GetMeshes())
				{
					auto& material = mesh.GetMaterial();
					ImGui::PushID(material->GetMaterialPath().c_str());
					bool uniformChanged = false;

					if (ImGui::Button("Recompile Shaders"))
						material->RecompileShaders();

					if (ImGui::Button("Save Material"))
					{
						MaterialSerializer::Serialize(material);
					}

					if (ImGui::Button("Deserialize"))
					{
						MaterialSerializer::Deserialize(material);
					}

					for (auto& uniform : material->GetUniforms())
					{
						std::string uniformName = uniform.first;
						ImGui::PushID(uniformName.c_str());
						switch (uniform.second)
						{
						case ShaderDataType::Int:
							uniformChanged = ImGui::DragInt(uniformName.c_str(), &material->m_IntUniforms[uniformName]);
							break;
						case ShaderDataType::Int2:
							uniformChanged = ImGui::DragInt2(uniformName.c_str(), &material->m_Int2Uniforms[uniformName].x);
							break;
						case ShaderDataType::Int3:
							uniformChanged = ImGui::DragInt3(uniformName.c_str(), &material->m_Int3Uniforms[uniformName].x);
							break;
						case ShaderDataType::Int4:
							uniformChanged = ImGui::DragInt4(uniformName.c_str(), &material->m_Int4Uniforms[uniformName].x);
							break;
						case ShaderDataType::Float:
							uniformChanged = ImGui::DragFloat(uniformName.c_str(), &material->m_FloatUniforms[uniformName]);
							break;
						case ShaderDataType::Float2:
							uniformChanged = ImGui::DragFloat2(uniformName.c_str(), glm::value_ptr(material->m_Float2Uniforms[uniformName]));
							break;
						case ShaderDataType::Float3:
							uniformChanged = ImGui::DragFloat3(uniformName.c_str(), glm::value_ptr(material->m_Float3Uniforms[uniformName]));
							break;
						case ShaderDataType::Float4:
							uniformChanged = ImGui::DragFloat4(uniformName.c_str(), glm::value_ptr(material->m_Float4Uniforms[uniformName]));
							break;
						case ShaderDataType::Mat3:
							break;
						case ShaderDataType::Mat4:
							break;
						default:
							break;
						}
						ImGui::PopID();

						if (uniformChanged)
							material->UpdateShaderUniform("ModelProps");
					}
					ImGui::PopID();
				}
			});
	}
}
