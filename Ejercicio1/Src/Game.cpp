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
	while(wnd->isOpen())
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

	// Dibujar bloque dinámico
	b2Vec2 pos = controlBody->GetPosition();

	sf::RectangleShape controlShape(sf::Vector2f(50, 10)); // Mismas dimensiones que en Box2D
	controlShape.setFillColor(sf::Color::Red);
	controlShape.setOrigin(25, 5); // Centro del rectángulo
	controlShape.setPosition(pos.x, pos.y);
	wnd->draw(controlShape);

	
}

void Game::DoEvents()
{
	Event evt;
	while(wnd->pollEvent(evt))
	{
		switch(evt.type)
		{
			case Event::Closed:
				wnd->close();
				break;
			
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



	// Creamos un piso
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 95.0f), 0.0f);

	// Creamos un bloque que cae
	controlBody= Box2DHelper::CreateRectangularDynamicBody(phyWorld, 50, 10, 1.0, 0.3,0.1);
	controlBody->SetTransform(b2Vec2(50.0f, 30.0f), 0.0f);
}

Game::~Game(void)
{ }