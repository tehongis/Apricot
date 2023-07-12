#include <iostream>
#include <vector>
#include <string>

#define _USE_MATH_DEFINES
#include <cmath>

#include <SFML/Main.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

int main(){

    if (!sf::Shader::isAvailable())
    {
        std::cout << "No shader available." << std::endl;
        exit(0);
    }

    sf::Shader shader;
    if (!shader.loadFromFile("vertex_shader.vert", "fragment_shader.frag"))
    {
        std::cout << "Failed to load shaders." << std::endl;
        exit(0);
    }

    sf::Texture texture;
    if (!texture.loadFromFile("image.png"))
    {
        std::cout << "Failed to load texture." << std::endl;
        exit(0);
    }

    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML testing.");
    sf::CircleShape shape(300.f);

    sf::ContextSettings settings = window.getSettings();
    std::cout << "GLSL version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;

/*
    float (GLSL type float)
    2 floats, sf::Vector2f (GLSL type vec2)
    3 floats, sf::Vector3f (GLSL type vec3)
    4 floats (GLSL type vec4)
    sf::Color (GLSL type vec4)
    sf::Transform (GLSL type mat4)
    sf::Texture (GLSL type sampler2D)
*/

    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

    shader.setUniform("u_resolution", sf::Glsl::Vec2{ window.getSize() });
    shader.setUniform("u_mouse", sf::Glsl::Vec2{ mousePos });
    

    //shape.setFillColor(sf::Color::Green);
    shape.setTexture(&texture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape,&shader);

    shader.setUniform("u_time", clock.getElapsedTime().asSeconds());

        window.display();
    }
 
    return 0;
}

