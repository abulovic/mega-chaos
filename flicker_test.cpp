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
    : data(data) {}
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        VertexArray polyline(sf::LinesStrip, data.size());
        for (int i = 0; i < data.size(); i++)
        {
            Vector2f p = Vector2f((float)i, data.at(i));
            polyline[i] = p;
        }
        target.draw(polyline, states);
    }
};

Transform getTransform(Vector2f center, Vector2f zoom) {
    Transform transform = Transform::Identity;
    transform = transform.translate(center.x, center.y);
    cout << center.x << " " << center.y << endl;
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

    RenderWindow window(sf::VideoMode(1000, 500), "Mega Chaos");
    View upper_view;
    upper_view.reset(sf::FloatRect(0, -2, 10000, 4));

    //upper_view.setSize(800, 200);
    upper_view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));



    int current_view_level = 0;
    int level_cnt = 3;
    View current_view = upper_view;

    float x0p, x1p, x0p_, x1p_, pxm, pxs, l, l_;
    x0p = 0.; x1p = 10000.;
    x0p_ = x0p; x1p_ = x1p;
    l = x1p - x0p; l_ = l;

    float y0p, y1p, y0p_, y1p_, pym, pys, h, h_;
    y0p = -2.; y1p = 2.;
    y0p_ = y0p; y1p_ = y1p;
    h = y1p - y0p; h_ = h;
    float z = 0.95f;

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
                    std::cout << "X0P_, X1P_: " << x0p_ << ", " << x1p_ << std::endl;
                    std::cout << "Len: " << l_ << "," << l << std::endl;
                    pxm = event.mouseWheel.x;
                    pxs = pxm / window.getSize().x * l + x0p_;
                    std::cout << "PXM, PXS:  " << pxm << "," << pxs << std::endl;
                    z = 1 - event.mouseWheel.delta * 0.05;
                    x0p = ((pxs) * (1. - z) + z*x0p_);
                    x1p = z*l_ + x0p;
                    l = x1p - x0p;
                    std::cout << "X0P, X1P: " << x0p << ", " << x1p << std::endl;
                    std::cout << "Len: " << l_ << std::endl;
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
        }
        window.clear(sf::Color::Black);
        //window.draw(line, 2, sf::Lines);

        window.setView(upper_view);
        window.draw(tc);


        window.display();
    }
}