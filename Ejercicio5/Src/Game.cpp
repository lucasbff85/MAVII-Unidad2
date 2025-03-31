#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

Game::Game(int ancho, int alto, std::string titulo)
{
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom();
    InitPhysics();
}

void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor);
        DoEvents();
        CheckCollisions();
        UpdatePhysics();
        DrawGame();
        wnd->display();
    }
}

void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8);
    phyWorld->ClearForces();
    // Comentado si no tienes DebugDraw configurado
    // phyWorld->DebugDraw();
}

void Game::DrawGame()
{
    // Dibujamos el suelo
    sf::RectangleShape groundShape(sf::Vector2f(500, 5));
    groundShape.setFillColor(sf::Color::Red);
    groundShape.setOrigin(250, 2.5);
    groundShape.setPosition(50, 80);
    float grados = 17;
    groundShape.rotate(grados);
    wnd->draw(groundShape);

    // Dibujar pared izquierda (verde)
    sf::RectangleShape leftWallShape(sf::Vector2f(5, 100));
    leftWallShape.setFillColor(sf::Color::Green);
    leftWallShape.setPosition(0, 0);
    wnd->draw(leftWallShape);

    // Dibujar pared derecha (verde)
    sf::RectangleShape rightWallShape(sf::Vector2f(5, 100));
    rightWallShape.setFillColor(sf::Color::Green);
    rightWallShape.setPosition(95, 0); // 100 - grosor de la pared
    wnd->draw(rightWallShape);

    // Dibujar la caja dinámica
    sf::RectangleShape controlShape(sf::Vector2f(20, 20));
    controlShape.setFillColor(sf::Color::Red);
    controlShape.setOrigin(10, 10); // Centro del rectángulo
    controlShape.setPosition(sf::Vector2f(boxBody->GetPosition().x, boxBody->GetPosition().y));
    controlShape.setRotation(boxBody->GetAngle() * 180 / b2_pi);
    wnd->draw(controlShape);
}

void Game::DoEvents()
{
    Event evt;
    while (wnd->pollEvent(evt))
    {
        if (evt.type == Event::Closed)
        {
            wnd->close();
        }
        else if (evt.type == Event::KeyPressed)
        {
            if (evt.key.code == Keyboard::Left)
            {
                boxBody->ApplyLinearImpulse(b2Vec2(-20.0f, 0.0f), boxBody->GetWorldCenter(), true);
            }
            else if (evt.key.code == Keyboard::Right)
            {
                boxBody->ApplyLinearImpulse(b2Vec2(20.0f, 0.0f), boxBody->GetWorldCenter(), true);
            }
        }
    }
}

void Game::CheckCollisions()
{
    // Implementación pendiente
}

void Game::SetZoom()
{
    View camara;
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara); // Asignamos la cámara
}

void Game::InitPhysics()
{
    float grados = 17;
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Pared izquierda
    b2Body* leftWall = Box2DHelper::CreateRectangularStaticBody(phyWorld, 5, 100);
    leftWall->SetTransform(b2Vec2(2.5f, 50.0f), 0.0f);

    // Pared derecha
    b2Body* rightWall = Box2DHelper::CreateRectangularStaticBody(phyWorld, 5, 100);
    rightWall->SetTransform(b2Vec2(97.5f, 50.0f), 0.0f);

    // Piso
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 500, 5);
    groundBody->SetTransform(b2Vec2(50.0f, 80.0f), grados * b2_pi / 180);
    groundBody->GetFixtureList()->SetFriction(2.2f);

    // Caja dinámica
    boxBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 20, 20, 0.5f, 0.5f, 0.3f);
    boxBody->SetTransform(b2Vec2(50.0f, 85.0f), 0.0f);
}

Game::~Game(void)
{
    delete wnd;
    delete phyWorld;
}
