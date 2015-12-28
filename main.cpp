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
    TimeCourse(vector<float> & data, Transform& transform)
    : data(data), transform(transform) {}
private:
    Transform& transform;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        VertexArray polyline(sf::LinesStrip, data.size());
        for (int i = 0; i < data.size(); i++)
        {
            Vector2f p = Vector2f(i, data.at(i));
            polyline[i] = transform.transformPoint(p);
        }
        target.draw(polyline, states);
    }
};

Transform getTransform(Vector2f center, Vector2f zoom) {
    Transform transform = Transform::Identity;
    transform = transform.translate(center.x, center.y);
    transform = transform.scale(zoom.x, zoom.y);
    return transform.translate(-center.x, -center.y);
}

int main(int argc, char ** argv)
{
    Vector2f center = Vector2f(500, -1);
    Vector2f zoom = Vector2f(1, 100);
    Transform transform = getTransform(center, zoom);

    if (argc < 2)
    {
        cout << "Usage: viewer [SIGNAL_FILENAME]" << endl;
        cout << "Exiting." << endl;
        exit(-1);
    }

    string filename(argv[1]);
    vector<float> data;
    load_csv(filename, data);
    TimeCourse tc(data, transform);

    RenderWindow window(sf::VideoMode(1000, 200), "Mega Chaos");
    View view;
    view.reset(sf::FloatRect(0, 0, 1000, 200));
    view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseWheelMoved)
            {
                center = Vector2f(event.mouseWheel.x, -1);
                switch (event.mouseWheel.delta)
                {
                case 1:
                    //view.zoom(0.9);
                    zoom.x = zoom.x * 0.9f;
                    transform = getTransform(center, zoom);
                    break;
                case -1:
                    //view.zoom(1.1);
                    zoom.x = zoom.x * 1.1f;
                    transform = getTransform(center, zoom);
                    break;
                }
            }
        }

        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(100, 0)),
            sf::Vertex(sf::Vector2f(100, 200))
        };
        window.clear(sf::Color::Black);
        //window.draw(line, 2, sf::Lines);

        window.setView(view);
        window.draw(tc);
        window.display();
    }
}