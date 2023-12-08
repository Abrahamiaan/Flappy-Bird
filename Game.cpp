#include "Game.h"
#include "Pipe.h"
#include <iostream>


Game::Game(sf::RenderWindow& window) :
	window(window),
	is_enter_pressed(false),
	run_game(true),
	start_monitoring(false),
	pipe_counter(71),
	pipe_spawn_time(70),
	score(0)
{
	window.setFramerateLimit(60);
	bg_texture.loadFromFile("images\\bg.png");
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setScale(SCALE_FACTOR, SCALE_FACTOR);
	bg_sprite.setPosition(0,-250);

	ground_texture.loadFromFile("images\\ground.png");
	ground_sprite1.setTexture(ground_texture);
	ground_sprite2.setTexture(ground_texture);

	ground_sprite1.setScale(SCALE_FACTOR, SCALE_FACTOR);
	ground_sprite2.setScale(SCALE_FACTOR, SCALE_FACTOR);

	ground_sprite1.setPosition(0, 578);
	ground_sprite2.setPosition(ground_sprite1.getGlobalBounds().width, 578);

	font.loadFromFile("./Images/arial.ttf");
	restart_text.setFont(font);
	restart_text.setCharacterSize(48);
	restart_text.setFillColor(sf::Color::Black);
	restart_text.setPosition(150, 650);
	restart_text.setString("Restart Game!!");

	score_text.setFont(font);
	score_text.setCharacterSize(24);
	score_text.setFillColor(sf::Color::Black);
	score_text.setPosition(15, 15);
	score_text.setString("Score: 0");

	icon.loadFromFile("C:\\Users\\Home\\Desktop\\Works(Cpp)\\Flappy Bird\\images\\FBicon.jpg");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	Pipe::loadTextures();

}

void Game::gameProcess(sf::Time& dt)
{
	if (is_enter_pressed) {
		
		moveGround(dt);

		if (pipe_counter > pipe_spawn_time) {
			pipes.push_back(Pipe(dist(rd)));
			pipe_counter = 0;
		}
		pipe_counter++;

		for (int i = 0; i < pipes.size(); i++) {
			
			pipes[i].update(dt);

			if (pipes[i].getRightBound() < 0) {
				pipes.erase(pipes.begin() + i);
			}
		}
		checkDeath();
		checkScore();
	}
	bird.update(dt);
}

void Game::checkDeath()
{
	if (pipes.size() > 0) {
		if (pipes[0].sprite_down.getGlobalBounds().intersects(bird.bird_sprite.getGlobalBounds()) ||
			pipes[0].sprite_up.getGlobalBounds().intersects(bird.bird_sprite.getGlobalBounds()) ||
			bird.bird_sprite.getGlobalBounds().top >= 540) {

			is_enter_pressed = false;
			run_game = false;
		}
	}
}

void Game::startGameLoop()
{

	sf::Clock clock;
	// Game loop
	while (window.isOpen()) {

		sf::Time dt = clock.restart();
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed)
				window.close();

			if (ev.type == sf::Event::KeyPressed && run_game)
			{
				if (ev.key.code == sf::Keyboard::Enter && !is_enter_pressed)
				{
					is_enter_pressed = true;
					bird.setShouldFly(true);
				}
				if (ev.key.code == sf::Keyboard::Space && is_enter_pressed)
				{
					bird.flapBird(dt);
				}
			}
			if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left && !run_game)
			{

				if (restart_text.getGlobalBounds().contains(ev.mouseButton.x, ev.mouseButton.y))
				{
					restartGame();
				}

			}
		}
		
		gameProcess(dt);

		draw();
		window.display();

	}
}

void Game::checkScore()
{
	if (pipes.size() > 0)
	{
		if (!start_monitoring)
		{
			if (bird.bird_sprite.getGlobalBounds().left > pipes[0].sprite_down.getGlobalBounds().left &&
				bird.getRightBound() < pipes[0].getRightBound())
			{
				start_monitoring = true;
			}
		}
		else
		{
			if (bird.bird_sprite.getGlobalBounds().left > pipes[0].getRightBound())
			{
				score++;
				score_text.setString("Score: " + toString(score));
				start_monitoring = false;
			}
		}
	}
}

void Game::draw()
{
	window.draw(bg_sprite);
	for (Pipe& pipe : pipes) {
		window.draw(pipe.sprite_up);
		window.draw(pipe.sprite_down);
	}
	window.draw(ground_sprite1);
	window.draw(ground_sprite2);
	window.draw(bird.bird_sprite);
	window.draw(score_text);
	if (!run_game)
	{
		window.draw(restart_text);
	}

		
}

void Game::moveGround(sf::Time& dt)
{
	ground_sprite1.move(-move_speed * dt.asSeconds(), 0.f);
	ground_sprite2.move(-move_speed * dt.asSeconds(), 0.f);

	if(ground_sprite1.getGlobalBounds().left + ground_sprite1.getGlobalBounds().width < 0){
		ground_sprite1.setPosition(ground_sprite2.getGlobalBounds().left + ground_sprite2.getGlobalBounds().width, 578);
	}
	if (ground_sprite2.getGlobalBounds().left + ground_sprite2.getGlobalBounds().width < 0) {
		ground_sprite2.setPosition(ground_sprite1.getGlobalBounds().left + ground_sprite1.getGlobalBounds().width, 578);
	}
}

void Game::restartGame()
{
	bird.resetBirdPosition();
	bird.setShouldFly(false);
	run_game = true;
	is_enter_pressed = false;
	pipe_counter = 71;
	pipes.clear();
	score = 0;
	score_text.setString("Score: 0");
}

std::string Game::toString(int num)
{
	std::stringstream ss;
	ss << num;
	return ss.str();
}