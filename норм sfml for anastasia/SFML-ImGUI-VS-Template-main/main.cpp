#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
/*#include <iostream>
using std::cout;*/

int main()
{
    const unsigned int SCREEN_WIDTH = 400;
    const unsigned int SCREEN_HEIGHT = 300;

    const auto kInvisible = ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBackground;


    int v[3] = {}, for_print[3];
    std::string input_str;
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Window Title", sf::Style::Close);            // Create a window with specified size.
    window.setFramerateLimit(60);                                                                   // Limit the framerate to be below 60, no guarantee it will be 60
    ImGui::SFML::Init(window); //связь sfml и imgui
    sf::Clock deltaClock;
    int cur;
    const char* elems[] = { "One", "Two", "Three" };
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("UI");
        //ImGui::SetWindowPos({50, 50});
        //ImGui::SetWindowSize({250, 150});
        ImGui::SetWindowFontScale(1.5);
        ImGui::SetCursorPos({0, 0});
        auto& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Button] = ImColor(0,255,0);
        ImGui::InputInt3("Input x, y, z", v);
        ImGui::InputText("Input", &input_str);
        ImGui::LabelText("Vector", input_str.c_str());
        //ImGui::ListBox("lst", &cur, elems);
        if (ImGui::Button("Push Me soft")) {
            for_print[0] = v[0];
            for_print[1] = v[1];
            for_print[2] = v[2];
            //cout << input_str << '\n';

        }
        ImGui::BeginChild("8767878");
        ImGui::EndChild();
        ImGui::End();

        ImGui::Begin("Menu");
        ImGui::SetWindowPos({300, 50});
        ImGui::End();

        // ---------------- Rendering ---------------------------- //
        window.clear(sf::Color(18, 33, 43)); // Color background
        ImGui::SFML::Render(window);                                            // Render UI
        window.display();                                                       // Swaps buffers
    }
    ImGui::SFML::Shutdown();
    return 0;
}
