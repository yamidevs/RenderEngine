#include "userinterfaces/WorldControllerUI.h"

#include "imgui/imgui.h"
#include "WorldConfig.h"
#include "Time.h"

Engine::Window::WorldControllerUI::WorldControllerUI(GLFWwindow * surface)
	:Engine::Window::UserInterface(surface)
{
	
}

void Engine::Window::WorldControllerUI::drawGraphics()
{
	ImVec2 pos(1024.0f - 300.0f, 0.0f);
	ImGui::SetNextWindowPos(pos);
	ImVec2 size(300.0f, 800.0f);
	ImGui::SetNextWindowSize(size);
	ImGui::StyleColorsDark();
	
	if (ImGui::Begin("World Controller##app"))
	{
		int fps = int(floor(1.0f / Time::deltaTime));
		std::string fpsStr = "FPS: " + std::to_string(fps);
		ImGui::Text(fpsStr.c_str());

		ImGui::Spacing(); ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("Drawing settings");
		ImGui::PushItemWidth(150.0f);
		ImGui::Combo("Drawing Method##app", reinterpret_cast< int32_t* >(&Engine::Settings::drawingMethod), "Shaded\0Wireframe\0", 2);
		ImGui::Spacing();
		ImGui::Combo("Travel Method##app", reinterpret_cast<int32_t*>(&Engine::Settings::travelMethod), "Manual\0Bezier\0Straight", 3);

		ImGui::Spacing(); ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("Light settings");
		ImGui::ColorPicker3("Light color", &Engine::Settings::lightColor[0], ImGuiColorEditFlags_PickerHueWheel);
		ImGui::Spacing();
		ImGui::SliderFloat3("Light Direction", &Engine::Settings::lightDirection[0], -1.f, 1.f);

		ImGui::Spacing(); ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("Terrain settings");
		ImGui::SliderFloat("Amplitude##app", &Engine::Settings::terrainAmplitude, 0.0f, 10.0f);
		ImGui::SliderFloat("Frecuency##app",&Engine::Settings::terrainFrecuency, 0.0f, 10.0f);
		ImGui::SliderFloat("Scale##app", &Engine::Settings::terrainScale, 0.0f, 50.0f);
		ImGui::InputInt("Octaves##app", reinterpret_cast< int32_t*>(&Engine::Settings::terrainOctaves));


		ImGui::Spacing(); ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("Water settings");
		ImGui::ColorPicker3("Water Color", &Engine::Settings::waterColor[0], ImGuiColorEditFlags_PickerHueWheel);
		ImGui::Spacing();
		ImGui::SliderFloat("Water speed", &Engine::Settings::waterSpeed, 0.0f, 1.0f);
		ImGui::Spacing();
		ImGui::SliderFloat("Water height", &Engine::Settings::waterHeight, 0.0f, 1.0f);

		ImGui::End();
	}
}