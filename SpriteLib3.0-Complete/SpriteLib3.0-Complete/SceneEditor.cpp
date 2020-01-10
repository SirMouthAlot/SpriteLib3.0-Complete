#include "SceneEditor.h"

void SceneEditor::ResetEditor()
{
	m_currentEntityName = "None Selected";
	m_currentEntityNum = 0;
	m_selected = false;
}

void SceneEditor::CreateTab(entt::registry* reg, Scene* scene)
{
	bool temp = false;
	if (ImGui::BeginTabItem(m_selectable.GetName().c_str(), m_selectable.GetSelected()))
	{
		temp = true;

		ImGui::EndTabItem();
	}

	if (temp)
		CreateEditor(reg, scene);
	else
		ResetEditor();
}

void SceneEditor::CreateEditor(entt::registry * reg, Scene* scene)
{
	if (m_selectable.GetSelected())
	{
		auto view = reg->view<EntityIdentifier>();

		//Combo (drop down menu)
		if (ImGui::BeginCombo("", m_currentEntityName.c_str()))
		{
			//Loop through all entities
			for (auto entity : view)
			{
				//Specify name + entity number
				//Assuming it has a name
				auto& tempID = view.get(entity);
				std::string tempName = (tempID.GetName() + " : " + std::to_string(entity));

				//highlight the selected entity within the menu
				if (entity == m_currentEntityNum)
					tempID.SetSelected(true);
				else
					tempID.SetSelected(false);

				//The selectable within the combo
				if (ImGui::Selectable(tempName.c_str(), tempID.GetSelected()))
				{
					//Set the current entity name and number
					m_currentEntityName = tempName;
					m_currentEntityNum = entity;

					//We make sure it's known that something has been selected at least once
					m_selected = true;
				}
			}
			//End combo menu
			ImGui::EndCombo();
		}

		//Has something been selected at least once?
		if (m_selected)
		{
			//Creates the treenode for entity information
			if (ImGui::TreeNode("Entity Information"))
			{
				//Reference to identifier for the current selected entity
				auto& tempID = reg->get<EntityIdentifier>(m_currentEntityNum);

				//Prints out entity number
				ImGui::Text(("Entity Number: " + std::to_string(m_currentEntityNum)).c_str());

				//What components are attached to this entity?
				if (tempID.GetIdentifier() & EntityIdentifier::CameraBit())
				{
					if (ImGui::TreeNode("Camera Component"))
					{
						auto& tempCam = reg->get<Camera>(m_currentEntityNum);
						std::string projType = tempCam.GetProjectionType() == Orthographic ? "Orthographic" : "Perspective";
						float position[] = { tempCam.GetPosition().x, tempCam.GetPosition().y, tempCam.GetPosition().z };
						float aspect = tempCam.GetWindowSize().x / tempCam.GetWindowSize().y;
						float ortho[] = { tempCam.GetOrthoSize().x, tempCam.GetOrthoSize().y, tempCam.GetOrthoSize().z, tempCam.GetOrthoSize().w };
						float nearFar[] = { tempCam.GetNear(), tempCam.GetFar() };
						float fovY[] = { tempCam.GetFOV().y };

						ImGui::Text(EntityIdentifier::MainCamera() == m_currentEntityNum ? "Main Camera" : "Side Camera");
						ImGui::Text(("Projection Type: " + projType).c_str());

						switch (tempCam.GetProjectionType())
						{
						case Orthographic:
							//Allows for panning, and zooming
							if (ImGui::DragFloat2("Position", &position[0], 1.f, -1000.f, 1000.f))
							{
								tempCam.SetPosition(vec3(position[0], position[1], position[2]));
							}

							if (ImGui::DragFloat4("Ortho size", ortho, 0.5f, -1000.f, 1000.f))
							{
								tempCam.Orthographic(aspect, ortho[0], ortho[1], ortho[2], ortho[3], nearFar[0], nearFar[1]);
							}
							break;
						case Perspective:
							//Allow for movement (don't do rotation because it's orthographic)
							if (ImGui::DragFloat3("Position", position, 0.1f, -1000.f, 1000.f))
							{
								tempCam.SetPosition(vec3(position[0], position[1], position[2]));
							}
							//Adjust fov
							if (ImGui::DragFloat("Fov Y", fovY, 0.1f, 1.f, 100.f))
							{
								tempCam.Perspective(fovY[0], aspect, nearFar[0], nearFar[1]);
							}
							//Adjust near far plane
							if (ImGui::DragFloat2("Near/Far", nearFar, 0.5f, -1000.f, 1000.f))
							{
								if (tempCam.GetNear() >= tempCam.GetFar())
									tempCam.SetNear(tempCam.GetFar() - 10.f);
								tempCam.Perspective(fovY[0], aspect, nearFar[0], nearFar[1]);
							}

							break;
						}

						ImGui::TreePop();
					}
				}
				if (tempID.GetIdentifier() & EntityIdentifier::TransformBit())
				{
					if (ImGui::TreeNode("Transform Component"))
					{
						auto& tempTran = reg->get<Transform>(m_currentEntityNum);
						float position[] = { tempTran.GetPosition().x, tempTran.GetPosition().y, tempTran.GetPosition().z };
						float scale[] = { tempTran.GetScale().x, tempTran.GetScale().y, tempTran.GetScale().z };

						if (ImGui::DragFloat3("Position", position, 0.1f, -200.f, 200.f))
							tempTran.SetPosition(vec3(position[0], position[1], position[2]));
						if (ImGui::DragFloat3("Scale", scale, 0.1f, 1.f, 1000.f))
							tempTran.SetScale(vec3(scale[0], scale[1], scale[2]));

						ImGui::TreePop();
					}
				}
				if (tempID.GetIdentifier() & EntityIdentifier::SpriteBit())
				{
					if (ImGui::TreeNode("Sprite Component"))
					{
						auto& tempSpr = reg->get<Sprite>(m_currentEntityNum);
						float size[] = { float(tempSpr.GetWidth()), float(tempSpr.GetHeight()) };

						ImGui::Text(("File Name: " + tempSpr.GetFileName()).c_str());

						if (ImGui::DragFloat2("Sprite Size", size, 1.0f, 1.f, 1000.f))
						{
							tempSpr.SetWidth(int(size[0]));
							tempSpr.SetHeight(int(size[1]));
						}

						ImGui::Text(("Bottom left ( " + std::to_string(tempSpr.GetBottomLeft().x) + ", " + std::to_string(tempSpr.GetBottomLeft().y)
							+ " )").c_str());
						ImGui::Text(("Top Right (" + std::to_string(tempSpr.GetTopRight().x) + ", " + std::to_string(tempSpr.GetTopRight().y)
							+ " )").c_str());


						ImGui::TreePop();
					}
				}
				if (tempID.GetIdentifier() & EntityIdentifier::AnimationBit())
				{
					if (ImGui::TreeNode("Animation Controller Component"))
					{
						auto& tempCont = reg->get<AnimationController>(m_currentEntityNum);

						ImGui::Text(("File Name: " + tempCont.GetFileName()).c_str());
						ImGui::Text(("Number of Animations: " + std::to_string(tempCont.GetAnimations().size())).c_str());
						ImGui::Text(("Active Animation: Animation" + std::to_string(tempCont.GetActiveAnim())).c_str());

						for (unsigned i = 0; i < tempCont.GetAnimations().size(); i++)
						{
							if (ImGui::TreeNode(("Animation" + std::to_string(i)).c_str()))
							{
								auto& tempAnim = tempCont.GetAnimation(i);
								std::string repeat = tempAnim.GetRepeating() ? "True" : "False";

								ImGui::Text(("Number of Frames: " + std::to_string(tempAnim.GetFrames().size())).c_str());
								ImGui::Text(("Repeating: " + repeat).c_str());
								ImGui::Text(("Seconds Per Frame: " + std::to_string(tempAnim.GetSecPerFrame())).c_str());

								for (unsigned j = 0; j < tempAnim.GetFrames().size(); j++)
								{
									if (ImGui::TreeNode(("Frame" + std::to_string(j)).c_str()))
									{
										auto tempFrames = tempAnim.GetFrames();

										ImGui::Text(("Bottom Left: ( " + std::to_string(tempFrames[j].m_bottomLeft.x) + ", "
											+ std::to_string(tempFrames[j].m_bottomLeft.y) + " )").c_str());
										ImGui::Text(("Top Right: ( " + std::to_string(tempFrames[j].m_topRight.x) + ", "
											+ std::to_string(tempFrames[j].m_topRight.y) + " )").c_str());

										ImGui::TreePop();
									}
								}

								ImGui::TreePop();
							}
						}

						ImGui::TreePop();
					}
				}				
				ImGui::TreePop();
			}
		}

		if (ImGui::TreeNode("Spawn New Entities"))
		{
			if (ImGui::TreeNode("Enemy Entities"))
			{
				
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Environment Entities"))
			{
				
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		if (ImGui::Button("Save Scene", ImVec2(100.f, 20.f)))
		{
			scene->SaveScene();
		}
	}
}
