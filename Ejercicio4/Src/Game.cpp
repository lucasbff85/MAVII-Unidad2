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
	// Dibujamos el suelo
	sf::RectangleShape groundShape(sf::Vector2f(100, 10));
	groundShape.setFillColor(sf::Color::Blue);
	groundShape.setPosition(0, 90);
	wnd->draw(groundShape);


	// Dibujar pared izquierda (color verde)
	sf::RectangleShape leftWallShape(sf::Vector2f(5, 100));
	leftWallShape.setFillColor(sf::Color::Green);
	leftWallShape.setPosition(0, 0);
	wnd->draw(leftWallShape);

	// Dibujar pared derecha (color verde)
	sf::RectangleShape rightWallShape(sf::Vector2f(5, 100));
	rightWallShape.setFillColor(sf::Color::Green);
	rightWallShape.setPosition(95, 0); // 100 - 5 de grosor
	wnd->draw(rightWallShape);


	// Dibujar caja
	b2Vec2 pos = boxBody->GetPosition();

	sf::RectangleShape controlShape(sf::Vector2f(20, 20)); // Mismas dimensiones que en Box2D
	controlShape.setFillColor(sf::Color::Red);
	controlShape.setOrigin(10, 10); // Centro del rectángulo
	controlShape.setPosition(pos.x, pos.y);
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
			// Aumentamos significativamente la fuerza aplicada
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

void Game::CheckCollitions()
{
	// Veremos mas adelante
}

// Definimos el area del mundo que veremos en nuestro juego
void Game::SetZoom()
{
	View camara;
	// Posicion del view
	camara.setSize(100.0f, 100.0f);
	camara.setCenter(50.0f, 50.0f);
	wnd->setView(camara); //asignamos la camara
}

void Game::InitPhysics()
{
	// Inicializamos el mundo con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	// Pared izquierda (5 unidades de ancho, 100 de alto)
	b2Body* leftWall = Box2DHelper::CreateRectangularStaticBody(phyWorld, 5, 100);
	leftWall->SetTransform(b2Vec2(2.5f, 50.0f), 0.0f); // Centrado en X=2.5 (5/2)

	// Pared derecha (5 unidades de ancho, 100 de alto)
	b2Body* rightWall = Box2DHelper::CreateRectangularStaticBody(phyWorld, 5, 100);
	rightWall->SetTransform(b2Vec2(97.5f, 50.0f), 0.0f); // Centrado en X=97.5 (100-2.5)

	// Creamos un piso
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 95.0f), 0.0f);

	// Crear el cuerpo dinámico (caja)
	boxBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 20, 20, 0.5, 0.5, 0.3); 
	boxBody->SetTransform(b2Vec2(50.0f, 85.0f), 0.0f); 
}

Game::~Game(void)
{
}