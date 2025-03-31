#include <iostream>
#include <SFML/Graphics.hpp>
#include "Box2DHelper.h"
#include "SFMLRenderer.h"
#include <vector>

class Bala : public Box2DHelper {
public:
    sf::CircleShape bala;
    b2Body* bb;
    Bala(b2World& mundo, const sf::Vector2f& cannonPos);
    void update();
};

Bala::Bala(b2World& mundo, const sf::Vector2f& cannonPos) {
    bala.setRadius(3);
    bala.setOrigin(3, 3);
    bala.setFillColor(sf::Color(20, 20, 20));

    // Crear el cuerpo din�mico de la bala en la posici�n del ca��n
    bb = Box2DHelper::CreateCircularDynamicBody(&mundo, 3, 0.2, 0.8, 0.3);
    bb->SetTransform(b2Vec2(cannonPos.x, cannonPos.y), 0); // Establecer posici�n en la del ca��n
}

void Bala::update() {
    b2Vec2 position = bb->GetPosition();
    bala.setPosition(position.x, position.y);
}

int main()
{
    float fps = 60;
    float frameTime = 1.0f / fps;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Unidad 2 - Ejercicio 6");

    SFMLRenderer renderer(&window);
    b2World phyWorld(b2Vec2(0.0f, 90.8f));

    window.setFramerateLimit(fps);

    phyWorld.SetDebugDraw(&renderer);

    std::vector<Bala*> balas;
    // Zoom para achicar dimensiones
    sf::View camera;
    camera.setSize(100, 100);
    camera.setCenter(50, 50);
    window.setView(camera);

    // Piso
    sf::RectangleShape groundShape(sf::Vector2f(500, 5));
    groundShape.setFillColor(sf::Color::Black);
    groundShape.setOrigin(250, 2.5);
    groundShape.setPosition(50, 80);

    // F�sicas del Piso
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(&phyWorld, 500, 5);
    groundBody->SetTransform(b2Vec2(50.0f, 80.0f), 0);
    groundBody->GetFixtureList()->SetFriction(0.5);

    //"Ca�on"
    sf::RectangleShape canon(sf::Vector2f(10, 10));
    canon.setOrigin(5, 5);
    canon.setPosition(0, 50);  // Posicionamos el ca��n en el centro de la pantalla
    canon.setFillColor(sf::Color::Black);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                switch (event.key.code)
                {
                case sf::Mouse::Button::Left:
                    // Obtener la posici�n del rat�n
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);

                    // Crear una nueva bala en la posici�n del ca��n
                    Bala* b = new Bala(phyWorld, canon.getPosition());

                    // Calcular la direcci�n desde el ca��n hacia el mouse
                    b2Vec2 direction = b2Vec2(worldMousePos.x - b->bb->GetPosition().x,
                        worldMousePos.y - b->bb->GetPosition().y);
                    direction.Normalize();

                    // Aplicar impulso en la direcci�n hacia el rat�n
                    b->bb->ApplyLinearImpulseToCenter(b2Vec2(direction.x * 3000.0f, direction.y * 3000.0f), true);

                    balas.push_back(b);

                    break;
                }
                break;
            }
        }

        // Actualizaci�n de cada bala
        for (size_t i = 0; i < balas.size(); ++i) {
            Bala* balaPtr = balas[i];
            Bala& bala = *balaPtr;
            bala.update();
        }

        // Actualizaci�n de motor de f�sicas
        phyWorld.Step(frameTime, 8, 8);
        phyWorld.ClearForces();
        phyWorld.DebugDraw();

        // Renderizado con SFML
        window.clear(sf::Color::Yellow);
        window.draw(groundShape);
        for (size_t i = 0; i < balas.size(); ++i) {
            Bala* balaPtr = balas[i];
            Bala& bala = *balaPtr;
            window.draw(bala.bala);
        }
        window.draw(canon);
        window.display();
    }

    return 0;
}
