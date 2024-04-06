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

#include <box2d/box2d.h>

#define MAPSIZE_X 64
#define MAPSIZE_Y 64

char map[MAPSIZE_X*MAPSIZE_Y+1];





int32 velocityIterations = 6;
int32 positionIterations = 2;

float timeStep = 1.0f / 60.0f;

float randomFloat()
{
    float value = 1.0f - ((float)(rand()) / (float)(RAND_MAX) ) * 2.0;
    //printf("%f\n",value);
    return value;
}

b2Vec2 gravity(0.0f, 10.0f);
b2World world(gravity);


void makeMap(){
    for (long unsigned int i = 0 ; i < sizeof(map) ; i++) {
        int random = std::rand() % 2;
        std::cout << i << " value ";

        map[i] = random;
        std::cout << random << std::endl;
    }
}

std::vector<b2Body *> boxList;

void loadBoxes() {

    // Box shape
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(0.30f, 0.30f);

    // Box fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.4f;
    

    
    for (int i=0;i<200;i++) {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        float x = randomFloat();
        float y = randomFloat() * 4.0f - 8.0;
        bodyDef.position.Set(x, y);
        bodyDef.angularVelocity = randomFloat();
        bodyDef.angularDamping = 0.1f;
        b2Body* body = world.CreateBody(&bodyDef);
        body->CreateFixture(&fixtureDef);
        boxList.push_back(body);

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
        std::cout << "Failed to load sprite texture." << std::endl;
        exit(0);
    }


    sf::Texture textureBackground;
    //if (!texture.loadFromFile("colored_transparent_packed.png", sf::IntRect(16*28, 16*2, 16, 16)) )
    if (!textureBackground.loadFromFile("graphics/ArtDecoGirl.png"))
    {
        std::cout << "Failed to load background texture." << std::endl;
        exit(0);
    }


    // box2d ground body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 15.0f);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    b2Body* groundBody;
    groundBody = world.CreateBody(&groundBodyDef);
    groundBody->CreateFixture(&groundBox, 0.0f);


    loadBoxes();

    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML testing.");
    window.setMouseCursorVisible(false);

    sf::RectangleShape box(sf::Vector2f(texture.getSize()));
    //sf::CircleShape shape(300.f);
    //shape.setFillColor(sf::Color::Green);
    box.setTexture(&texture);
    sf::Vector2f boxLoc(0.0f,100.0f);
    box.setPosition(boxLoc);


    sf::RectangleShape background(sf::Vector2f(textureBackground.getSize()));
    //sf::CircleShape shape(300.f);
    //shape.setFillColor(sf::Color::Green);
    background.setTexture(&textureBackground);
    sf::Vector2f backgroundLoc(0.0f,-800.0f);
    background.setPosition(backgroundLoc);


    sf::ContextSettings settings = window.getSettings();
    std::cout << "GLSL version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;


    sf::Sprite fysicsSprite;
    fysicsSprite.setTexture(texture);
    fysicsSprite.setTextureRect(sf::IntRect(16*23, 16*3, 16, 16));
    fysicsSprite.setOrigin(sf::Vector2f(8.0f,8.0f));
    fysicsSprite.scale(sf::Vector2f(2.0f,2.0f));

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

    void box2DDynBody();

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
            {
            for(b2Body* body: boxList) {
                float x = randomFloat() * 10.0f;
                float y = (-1.0 + randomFloat()) * 40.0f;
                b2Vec2 force = b2Vec2(x, y);
                body->ApplyForceToCenter(force, true);
                }
            }


        playerShipSprite.setPosition(sf::Vector2f(playerXpos,580.0f));

        window.clear();
        window.draw(background,&shader);

/*
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

*/

        // Draw all tiles on the screen
//        window.draw(box,&shader);

        for(sf::Vector2f position : ammoList) {
            ammoSprite.setPosition(position);
            window.draw(ammoSprite);
        }

        world.Step(timeStep, velocityIterations, positionIterations);

        for(b2Body* body: boxList) {
            b2Vec2 position = body->GetPosition();
            float angle = body->GetAngle();
            fysicsSprite.setPosition(sf::Vector2f(400+position.x*50.0,300+position.y*50.0));
            fysicsSprite.setRotation(angle);
            window.draw(fysicsSprite);
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


/*
// Shader checkerboard

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;

    // Time varying pixel color
    // vec3 col = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));
    
    vec2 size = iResolution.xy / 40.0;
    
    float total = floor(uv.x * size.x) + floor(uv.y * size.y);
    bool isEven = mod(total,2.0) == 0.0;

    vec4 colBlack = vec4(0.0,0.0,0.0,1.0);
    vec4 colWhite = vec4(1.0,1.0,1.0,1.0);

    // Output to screen
    fragColor = (isEven) ? colBlack : colWhite;
}
*/