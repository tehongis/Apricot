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


#include <box2d/box2d.h>
//#include <Box2D/Box2D.h>
//#include <Box2D/Box2D.h>


int main(){

/*
b2Body* b2World::CreateBody(const b2BodyDef* def)
b2Joint* b2World::CreateJoint(const b2JointDef* def)


b2Fixture* b2Body::CreateFixture(const b2FixtureDef* def)
void b2Body::DestroyFixture(b2Fixture* fixture)
or
b2Fixture* b2Body::CreateFixture(const b2Shape* shape, float density)


//Creating a World:
b2Vec2 gravity(0.0f, -10.0f);
b2World world(gravity);

// fixed body:
b2BodyDef groundBodyDef;
groundBodyDef.position.Set(0.0f, -10.0f);

b2Body* groundBody = world.CreateBody(&groundBodyDef);

b2PolygonShape groundBox;
groundBox.SetAsBox(50.0f, 10.0f);

groundBody->CreateFixture(&groundBox, 0.0f);


// dynamic body:
b2BodyDef bodyDef;
bodyDef.type = b2_dynamicBody;
bodyDef.position.Set(0.0f, 4.0f);
b2Body* body = world.CreateBody(&bodyDef);

b2PolygonShape dynamicBox;
dynamicBox.SetAsBox(1.0f, 1.0f);

b2FixtureDef fixtureDef;
fixtureDef.shape = &dynamicBox;
fixtureDef.density = 1.0f;
fixtureDef.friction = 0.3f;

body->CreateFixture(&fixtureDef);


//update:
float timeStep = 1.0f / 60.0f;
int32 velocityIterations = 6;
int32 positionIterations = 2;

for (int32 i = 0; i < 60; ++i)
{
    world.Step(timeStep, velocityIterations, positionIterations);
    b2Vec2 position = body->GetPosition();
    float angle = body->GetAngle();
    printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
}

// cleanup:
void b2World::DestroyBody(b2Body* body)
void b2World::DestroyJoint(b2Joint* joint)
*/

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
    //if (!texture.loadFromFile("colored_transparent_packed.png", sf::IntRect(16*28, 16*2, 16, 16)) )
    if (!texture.loadFromFile("colored_transparent_packed.png"))
    {
        std::cout << "Failed to load texture." << std::endl;
        exit(0);
    }


    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML testing.");
    window.setMouseCursorVisible(false);

    sf::RectangleShape box(sf::Vector2f(texture.getSize()));
    //sf::CircleShape shape(300.f);
    //shape.setFillColor(sf::Color::Green);
    box.setTexture(&texture);
    sf::Vector2f boxLoc(0.0f,100.0f);
    box.setPosition(boxLoc);

    sf::ContextSettings settings = window.getSettings();
    std::cout << "GLSL version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;

    sf::Sprite sprite;
    //sprite.setTexture(texture);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(16*47, 16*2, 16, 16));
    sprite.scale(sf::Vector2f(3.0f,3.0f));
    sprite.setOrigin(sf::Vector2f(8.0f,8.0f));

    while (window.isOpen())
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                sprite.rotate(-1.0f);
            }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                sprite.rotate(1.0f);
            }

        sprite.setPosition(sf::Vector2f(64.0f,64.0f));

        window.clear();
        //window.draw(box,&shader);
        //sprite.setRotation();
        window.draw(sprite);

    shader.setUniform("u_time", clock.getElapsedTime().asSeconds());
    shader.setUniform("u_resolution", sf::Glsl::Vec2{ window.getSize() });
    shader.setUniform("u_mouse", sf::Glsl::Vec2{ mousePos });

        window.display();
        window.setFramerateLimit(60);
    }
 
    return 0;
}

