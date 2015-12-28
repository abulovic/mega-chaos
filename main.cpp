#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

void load_csv(string & filename, vector<float> & data)
{
    ifstream infile(filename.c_str());
    string s_number, line;
    getline(infile, line);
    stringstream linestream(line);
    while(getline(linestream, s_number, ','))
    {
        data.push_back(atof(s_number.c_str()));
    }
    infile.close();
}

class TimeCourse : public Drawable
{
public:
    vector<float> data;
    TimeCourse(vector<float> & data)
    : data(data) {}
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        VertexArray polyline(sf::LinesStrip, data.size());
        for (int i = 0; i < data.size(); i++)
        {
            polyline[i] = Vector2f(i, (data.at(i) + 1) * 100);
        }
        target.draw(polyline, states);
    }
};

int main(int argc, char ** argv)
{

    if (argc < 2)
    {
        cout << "Usage: viewer [SIGNAL_FILENAME]" << endl;
        cout << "Exiting." << endl;
        exit(-1);
    }

    string filename(argv[1]);
    vector<float> data;
    load_csv(filename, data);
    TimeCourse tc(data);

    sf::RenderWindow window(sf::VideoMode(1000, 200), "Flicker Test v1");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseWheelMoved)
            {
                if (event.mouseWheel.delta == 1)
                    cout << "forward" << endl;
                else
                    cout << "backward" << endl;
            }
        }

        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(100, 0)),
            sf::Vertex(sf::Vector2f(100, 200))
        };
        window.clear(sf::Color::Black);
        //window.draw(line, 2, sf::Lines);
        window.draw(tc);
        window.display();
    }
}