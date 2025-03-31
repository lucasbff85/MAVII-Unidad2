#include <iostream>
#include "Box2DHelper.h"
#include "SFMLRenderer.h"

int main()
{
    float fps = 60;
    float frameTime = 1.0f / fps;

    sf::RenderWindow* window = new sf::RenderWindow(VideoMode(800, 600), "Unidad 2 - Ejercicio 5");

    SFMLRenderer* renderer = new SFMLRenderer(window);
    b2World* phyWorld = new b2World(b2Vec2(0.0f, 20.0f));

    window->setFramerateLimit(fps);

    phyWorld->SetDebugDraw(renderer);

    //Zoom para achicar dimensiones
    sf::View camera;
    camera.setSize(100, 100);
    camera.setCenter(50, 50);
    window->setView(camera);

    //Representacion Piso
    sf::RectangleShape groundShape(sf::Vector2f(500, 5));
    groundShape.setFillColor(sf::Color::Blue);
    groundShape.setOrigin(250, 2.5);
    groundShape.setPosition(50, 80);
    float grados = 17;
    groundShape.rotate(grados);

    //Fisicas del Piso
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 500, 5);
    groundBody->SetTransform(b2Vec2(50.0f, 80.0f), grados * b2_pi / 180);
    groundBody->GetFixtureList()->SetFriction(2.2f);

    //Bloque
    b2Body* block = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 10, 20, 0.2f, 0.1f, 0.3f);
    block->SetTransform(b2Vec2(5, 10), 0.0f);

    sf::RectangleShape blockShape(sf::Vector2f(10, 20));
    blockShape.setOrigin(5, 10);

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::KeyPressed:
                // Aplicar un impulso lineal al bloque según la tecla presionada
                switch (event.key.code)
                {
                case sf::Keyboard::Left:
                    block->ApplyLinearImpulse(b2Vec2(-5.0f, 0), block->GetWorldCenter(), true);
                    break;
                case sf::Keyboard::Right:
                    block->ApplyLinearImpulse(b2Vec2(5.0f, 0), block->GetWorldCenter(), true);
                    break;
                default:
                    break;
                }
                break;
            }
        }

        //Caida del Bloque
        blockShape.setPosition(sf::Vector2f(block->GetPosition().x, block->GetPosition().y));
        blockShape.setRotation(block->GetAngle() * 180 / b2_pi);

        //Actualizacion de Motor de Fisicas
        phyWorld->Step(frameTime, 8, 8);
        phyWorld->ClearForces();
        phyWorld->DebugDraw();


        //SFML Renderer
        window->clear();
        window->draw(groundShape);
        window->draw(blockShape);
        window->display();
    }


    return 0;
}