#include <SFML/Graphics.hpp>

int main()
{

    sf::RenderWindow window(sf::VideoMode(200, 200), "Flicker Test v1");
    while (window.isOpen())
    {
    	sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Vertex line[] =
		{
		    sf::Vertex(sf::Vector2f(100, 0)),
		    sf::Vertex(sf::Vector2f(100, 200))
		};
        window.clear(sf::Color::Black);
		window.draw(line, 2, sf::Lines);
        window.display();
    }
}