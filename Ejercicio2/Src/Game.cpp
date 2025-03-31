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
        CheckCollitions();
        UpdatePhysics();
        DrawGame();
        wnd->display();
    }
}

void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8);
    phyWorld->ClearForces();
    phyWorld->DebugDraw();
}

void Game::DrawGame()
{
    float wallThickness = 3.0f;
    float worldSize = 100.0f;

    sf::RectangleShape walls[4];
    for (int i = 0; i < 4; i++) {
        walls[i].setFillColor(sf::Color::White);
    }

    // Pared izquierda
    walls[0].setSize(sf::Vector2f(wallThickness, worldSize));
    walls[0].setPosition(0, 0);

    // Pared derecha
    walls[1].setSize(sf::Vector2f(wallThickness, worldSize));
    walls[1].setPosition(worldSize - wallThickness, 0);

    // Techo
    walls[2].setSize(sf::Vector2f(worldSize, wallThickness));
    walls[2].setPosition(0, 0);

    // Piso
    walls[3].setSize(sf::Vector2f(worldSize, wallThickness));
    walls[3].setPosition(0, worldSize - wallThickness);

    for (auto& wall : walls) wnd->draw(wall);

    // Dibujar pelota
    b2Vec2 pos = ballBody->GetPosition();
    sf::CircleShape ballShape(3);
    ballShape.setFillColor(sf::Color::Red);
    ballShape.setOrigin(3, 3);
    ballShape.setPosition(pos.x, pos.y);
    wnd->draw(ballShape);
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
    }
}

void Game::CheckCollitions()
{
    // Veremos más adelante
}

void Game::SetZoom()
{
    View camara;
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara);
}

void Game::InitPhysics()
{
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));
    float wallThickness = 3.0f;
    float worldSize = 100.0f;

    // Pared izquierda
    b2Body* leftWall = Box2DHelper::CreateRectangularStaticBody(phyWorld, wallThickness, worldSize);
    leftWall->SetTransform(b2Vec2(wallThickness / 2, worldSize / 2), 0.0f);

    // Pared derecha
    b2Body* rightWall = Box2DHelper::CreateRectangularStaticBody(phyWorld, wallThickness, worldSize);
    rightWall->SetTransform(b2Vec2(worldSize - wallThickness / 2, worldSize / 2), 0.0f);

    // Techo
    b2Body* topWall = Box2DHelper::CreateRectangularStaticBody(phyWorld, worldSize, wallThickness);
    topWall->SetTransform(b2Vec2(worldSize / 2, wallThickness / 2), 0.0f);

    // Piso
    b2Body* bottomWall = Box2DHelper::CreateRectangularStaticBody(phyWorld, worldSize, wallThickness);
    bottomWall->SetTransform(b2Vec2(worldSize / 2, worldSize - wallThickness / 2), 0.0f);

    // Crear la pelota
    ballBody = Box2DHelper::CreateCircularDynamicBody(phyWorld, 3, 1.0, 0.3, 1.0);
    ballBody->SetTransform(b2Vec2(50.0f, 50.0f), 0.5f);
    ballBody->SetLinearVelocity(b2Vec2(35.0f, -35.0f)); // Velocidad inicial en X e Y

}

Game::~Game()
{
}
