#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"

int main()
{
    const unsigned int SCREEN_WIDTH = 1280;
    const unsigned int SCREEN_HEIGHT = 720;

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Window Title");            // Create a window with specified size.
    window.setFramerateLimit(60);                                                                   // Limit the framerate to be below 60, no guarantee it will be 60
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;
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
        /* UI Components */
        ImGui::End();

        // ---------------- Rendering ---------------------------- //
        window.clear(sf::Color(18, 33, 43)); // Color background
        ImGui::SFML::Render(window);                                            // Render UI
        window.display();                                                       // Swaps buffers
    }
    ImGui::SFML::Shutdown();
    return 0;
}
