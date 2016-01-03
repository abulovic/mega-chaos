#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

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
        //data.push_back((atof(s_number.c_str()) + 2) * 50);
        data.push_back(atof(s_number.c_str()));
    }
    infile.close();
}

class TimeCourse : public Drawable
{
public:
    vector<float> data;
    TimeCourse(vector<float> & data)
    : data(data)  {
        polyline = VertexArray(sf::LinesStrip, data.size());
        for (int i = 0; i < data.size(); i++)
        {
            Vector2f p = Vector2f((float)i, data.at(i));
            polyline[i].position = p;
            polyline[i].color = Color(253, 151, 31);
        }
    }
private:
    VertexArray polyline;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
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

    float data_min = *min_element(data.begin(),data.end());
    float data_max = *max_element(data.begin(),data.end());
    float data_size = data.size();

    RenderWindow window(sf::VideoMode(1000, 500), "Mega Chaos");
    View upper_view;
    upper_view.reset(sf::FloatRect(0, data_min, data_size, data_max - data_min));
    //upper_view.setSize(800, 200);
    upper_view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));



    float x0p, x1p, x0p_, x1p_, pxm, pxs, l, l_;
    x0p = 0.; x1p = data_size;
    x0p_ = x0p; x1p_ = x1p;
    l = x1p - x0p; l_ = l;

    float y0p, y1p, y0p_, y1p_, pym, pys, h, h_;
    y0p = data_min; 
    y1p = data_max;
    y0p_ = y0p; y1p_ = y1p;
    h = y1p - y0p; h_ = h;
    float z = 0.95f;

    RectangleShape zoomRect;
    Vector2i initialPosition = Vector2i(0, 0);
    zoomRect.setFillColor(Color(76, 165, 0, 100));
    View zoomView;
    zoomView.reset(sf::FloatRect(0, 0, 1000, 500));

    //zoomView.setSize(800, 200);
    zoomView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseWheelMoved)
            {
                if (!Keyboard::isKeyPressed(Keyboard::LControl))
                {
                    if ((event.mouseWheel.delta > 0) && (x1p_ - x0p_) < 0.01)
                    {
                        continue;
                    }
                    pxm = event.mouseWheel.x;
                    pxs = pxm / window.getSize().x * l + x0p_;
                    z = 1 - event.mouseWheel.delta * 0.05;
                    x0p = ((pxs) * (1. - z) + z*x0p_);
                    x1p = z*l_ + x0p;
                    l = x1p - x0p;
                    upper_view.reset(sf::FloatRect(x0p, y0p, l, h));

                    x0p_ = x0p;
                    x1p_ = x1p;
                    l_ = l;

                }
                else
                {
                    if ((event.mouseWheel.delta > 0) && (y1p_ - y0p_) < 0.01)
                    {
                        continue;
                    }
                    pym = event.mouseWheel.y;
                    pys = pym / window.getSize().y * h + y0p_;
                    z = 1 - event.mouseWheel.delta * 0.05;
                    y0p = ((pys) * (1. - z) + z*y0p_);
                    y1p = z*h_ + y0p;
                    h = y1p - y0p;
                    upper_view.reset(sf::FloatRect(x0p, y0p, l, h));

                    y0p_ = y0p;
                    y1p_ = y1p;
                    h_ = h;
                }
            }
            if (event.type == Event::MouseButtonPressed && 
                Keyboard::isKeyPressed(Keyboard::R))
            {
                initialPosition = Mouse::getPosition(window);
                zoomRect.setPosition(initialPosition.x, initialPosition.y);
            }
            if (Keyboard::isKeyPressed(Keyboard::R) && Mouse::isButtonPressed(Mouse::Left))
            {
                zoomRect.setPosition(initialPosition.x, initialPosition.y);

                Vector2i newPos = Mouse::getPosition(window);
                float len = newPos.x - initialPosition.x;
                float wid = newPos.y - initialPosition.y;
                zoomRect.setSize(Vector2f(len, wid));
            }
            if (event.type == Event::MouseButtonReleased && zoomRect.getSize().x > 0)
            {
                float x0 = initialPosition.x;
                float x1 = Mouse::getPosition(window).x;
                x0 = x0 / window.getSize().x * l_ + x0p_;
                x1 = x1 / window.getSize().x * l_ + x0p_;
                float y0 = initialPosition.y;
                float y1 = Mouse::getPosition(window).y;
                y0 = y0 / window.getSize().y * h_ + y0p_;
                y1 = y1 / window.getSize().y * h_ + y0p_;

                upper_view.reset(FloatRect(x0, y0, x1 - x0, y1 - y0));

                x0p_ = x0p = x0;
                x1p_ = x1p = x1;
                y0p_ = y0p = y0;
                y1p_ = y1p = y1;
                l_ = l = x1 - x0;
                h_ = h = y1 - y0;

                zoomRect.setSize(Vector2f(0, 0));
            }
        }
        window.clear(sf::Color(39, 49, 34));
        //window.draw(line, 2, sf::Lines);

        window.setView(upper_view);
        window.draw(tc);

        window.setView(zoomView);
        window.draw(zoomRect);


        window.display();
    }
}