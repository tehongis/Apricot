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
#include <SFML/Audio.hpp>

#define MAPSIZE_X 64
#define MAPSIZE_Y 64

char map[MAPSIZE_X*MAPSIZE_Y+1];

void makeMap(){
    for (int i = 0 ; i < sizeof(map) ; i++) {
        int random = std::rand() % 2;
        std::cout << i << " value ";

        map[i] = random;
        std::cout << random << std::endl;
    }
}

std::vector<sf::Vector2f> ammoList;
void addAmmo(float xPos){
    ammoList.push_back(sf::Vector2f(xPos,550.0f));
}

int main(){

    std::cout << MAPSIZE_X << " times" << MAPSIZE_Y << " should be " << MAPSIZE_X*MAPSIZE_Y << std::endl;
    std::cout << "Making map." << std::endl;
    makeMap();
    std::cout << "Map done." << std::endl;

    sf::Music music;

    std::cout << "Loading music." << std::endl;

    if (!music.openFromFile("music/fsm-team-escp-quasarise.ogg")) {
        std::cout << "Unable to load music." << std::endl;
        exit(0);
    }

    sf::SoundBuffer shotBuffer;
    if (!shotBuffer.loadFromFile("music/ping.ogg"))
        return -1;

    sf::Sound shotSound;
    shotSound.setBuffer(shotBuffer);

    if (!sf::Shader::isAvailable())
    {
        std::cout << "No shader available." << std::endl;
        exit(0);
    }

    sf::Shader shader;
    if (!shader.loadFromFile("shaders/vertex_shader.vert", "shaders/fragment_shader.frag"))
    {
        std::cout << "Failed to load shaders." << std::endl;
        exit(0);
    }

    sf::Texture texture;
    //if (!texture.loadFromFile("colored_transparent_packed.png", sf::IntRect(16*28, 16*2, 16, 16)) )
    if (!texture.loadFromFile("graphics/colored_transparent_packed.png"))
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

    sf::Sprite playerShipSprite;
    //playerShipSprite.setTexture(texture);
    playerShipSprite.setTexture(texture);
    playerShipSprite.setTextureRect(sf::IntRect(16*47, 16*2, 16, 16));
    playerShipSprite.setOrigin(sf::Vector2f(8.0f,8.0f));
    playerShipSprite.scale(sf::Vector2f(2.0f,2.0f));
    //playerShipSprite.rotate(-1.0f);

    sf::Sprite ammoSprite;
    //playerShipSprite.setTexture(texture);
    ammoSprite.setTexture(texture);
    ammoSprite.setTextureRect(sf::IntRect(16*22, 16*4, 16, 16));
    ammoSprite.setOrigin(sf::Vector2f(8.0f,8.0f));
    ammoSprite.scale(sf::Vector2f(1.0f,1.0f));
    //playerShipSprite.rotate(-1.0f);


    music.play();

    int shotSkip = 0;

    float playerXpos = 400.0f;

    while (window.isOpen())
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

        shader.setUniform("u_time", clock.getElapsedTime().asSeconds());
        shader.setUniform("u_resolution", sf::Glsl::Vec2{ window.getSize() });
        shader.setUniform("u_mouse", sf::Glsl::Vec2{ mousePos });

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                if (playerXpos > 0) {
                    playerXpos = playerXpos - 5.0f;
                    }
            }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                if (playerXpos < 800) {
                    playerXpos = playerXpos + 5.0f;
                    }
            }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                if (shotSkip == 0) {
                    addAmmo(playerXpos);
                    shotSound.play();
                    shotSkip=5;
                    }
            }
        if (shotSkip > 0) {
            shotSkip--;
        }

        playerShipSprite.setPosition(sf::Vector2f(playerXpos,580.0f));

        window.clear();

        sf::Sprite mapTileSprite;

        for (int y = 0; y < MAPSIZE_Y; y++) {
            for (int x = 0; x < MAPSIZE_X; x++) {
                char tilevalue = map[y*MAPSIZE_X+x];
                if (tilevalue > 0) {
                    mapTileSprite.setTexture(texture);
                    mapTileSprite.setTextureRect(sf::IntRect(16 * (tilevalue % 48), 16* (int)tilevalue/48, 16, 16));
                    mapTileSprite.setOrigin(sf::Vector2f(8.0f,8.0f));
                    mapTileSprite.scale(sf::Vector2f(2.0f,2.0f));
                    mapTileSprite.setPosition(x*16,y*16);
                    window.draw(mapTileSprite);
                    }
                }
            }


        window.draw(box,&shader);
        for(sf::Vector2f position : ammoList) {
            ammoSprite.setPosition(position);
            window.draw(ammoSprite);
        }

        //playerShipSprite.setRotation();
        window.draw(playerShipSprite);

        window.display();

        std::vector<sf::Vector2f> tempammoList;
        for(sf::Vector2f position : ammoList) {
            if (position.y > 0) {
                position.y = position.y - 10;
                tempammoList.push_back(sf::Vector2f(position.x,position.y));
            }
        }
        ammoList = tempammoList;

        window.setFramerateLimit(60);
    }
 
    return 0;
}




/*

#include <box2d/box2d.h>
#include <Box2D/Box2D.h>
#include <Box2D/Box2D.h>

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
