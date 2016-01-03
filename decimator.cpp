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
        data.push_back(atof(s_number.c_str()));
    }
    infile.close();
}

void export_csv(string & filename, vector<float> & data)
{
	ofstream ofile(filename.c_str());
	for (uint32_t i = 0; i < data.size() - 1; i++)
	{
		ofile << data.at(i) << ',';
	}
	ofile << data.at(data.size() - 1);
	ofile.close();
}

bool y_val_comp(Vector2f v1, Vector2f v2)
{
	return v1.y  < v2.y;
}

class MinMaxDecimator
{
private:
	void _next_zoom_level()
	{
		vector<Vector2f> * data = zoom_levels.at(zoom_levels.size() - 1);
		zoom_levels.push_back(new vector<Vector2f>);
		vector<Vector2f> * last_level = zoom_levels.at(zoom_levels.size() - 1);
		Vector2f new_max, new_min;
		
		uint32_t i = 0;
		for (i = 0; i < data->size() / 4 - 1; i++)
		{
			new_max = *max_element(&data->at(4*i), &data->at(4*i + 4), y_val_comp);
			new_min = *min_element(&data->at(4*i), &data->at(4*i + 4), y_val_comp);
			last_level->push_back(new_max);
			last_level->push_back(new_min);
		}
		new_max = *max_element(data->end() - (data->size() - 4*i), data->end(), y_val_comp);
		new_min = *min_element(data->end() - (data->size() - 4*i), data->end(), y_val_comp);
		last_level->push_back(new_max);
		last_level->push_back(new_min);
		cout << last_level->size() << endl;
	}

public:
	vector<vector<Vector2f> *> zoom_levels;

	MinMaxDecimator(vector<float> & data)
	{
		zoom_levels.push_back(new vector<Vector2f>);
		vector<Vector2f> * _1st_level = zoom_levels.at(0);
		for (uint32_t i = 0; i < data.size(); i++)
		{
			_1st_level->push_back(Vector2f(i, data.at(i)));
		}
		while (zoom_levels.at(zoom_levels.size() - 1)->size() > 2000)
		{
			_next_zoom_level();
		}
		cout << "ZOOM LEVEL CNT: " << zoom_levels.size() << endl;

		string ofile("offzoom.csv");
		//export_csv(ofile, *zoom_levels.at(zoom_levels.size() - 4));
		//export_csv(ofile, *zoom_levels.at());
	}

	uint32_t get_level_cnt()
	{
		return zoom_levels.size();
	}

	float get_global_min()
	{
		uint32_t level = get_level_cnt() - 1;
		return min_element(zoom_levels.at(level)->begin(), zoom_levels.at(level)->end(), y_val_comp)->y;
	}

	float get_global_max()
	{
		uint32_t level = get_level_cnt() - 1;
		return max_element(zoom_levels.at(level)->begin(), zoom_levels.at(level)->end(), y_val_comp)->y;
	}

	void level_to_csv(uint32_t level, string & fname)
	{
		vector<Vector2f> * vec = zoom_levels.at(level);
		vector<float> values;
		for (uint32_t i = 0; i < vec->size(); i++)
		{
			values.push_back(vec->at(i).y);
		}
		export_csv(fname, values);
	}

	vector<Vector2f> * get_level_data(uint32_t level)
	{
		return zoom_levels.at(level);
	}
};


class PlotSignal : public Drawable
{
private:
	vector<VertexArray *> zoom_levels;
	float _gmax, _gmin;
	MinMaxDecimator _mm_decimator;
	
	Color _color;
	string _label;
	uint32_t screen_width = 1000;

	uint32_t visible_siglen;

public:
	PlotSignal(vector<float> & data):
	_mm_decimator(data) 
	{
		_gmin = _mm_decimator.get_global_min();
		_gmax = _mm_decimator.get_global_max();
		VertexArray * vtemp;
		vector<Vector2f> * level_data;
		for(uint32_t i = 0; i < _mm_decimator.get_level_cnt(); i++)
		{
			level_data = _mm_decimator.get_level_data(i);
			zoom_levels.push_back(new VertexArray(LinesStrip, level_data->size()));
			vtemp = zoom_levels.at(zoom_levels.size() - 1);
			cout << "SIZE OF VTEMP: " << vtemp->getVertexCount() << endl;
			for (uint32_t j = 0; j < level_data->size(); j++)
			{
				(*vtemp)[j].position = level_data->at(j);
				(*vtemp)[j].color = Color(253, 151, 31);
			}
		}
		visible_siglen = data.size();
		cout << "constructor" << endl;
	}

	void set_visible_siglen(uint32_t siglen)
	{
		visible_siglen = siglen;
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
    	float visible_perc = (float)visible_siglen / (float)zoom_levels.at(0)->getVertexCount();
    	uint32_t i;
    	for (i = 0; i < zoom_levels.size(); i++)
    	{
    		//cout << "visible count: "<< zoom_levels.at(i)->getVertexCount() << endl;
    		if(zoom_levels.at(i)->getVertexCount() * visible_perc < screen_width * 4){
    			//cout << "BREAK" << endl;
    			break;
    		}
    	}
    	//cout << "FINAL I: "<< i << endl;
    	//uint32_t zlevel = i-1 ? i > 0: i;
    	uint32_t zlevel;
    	if (i == 0)
    	{
    		zlevel = 0;
    	}
    	else {
    		zlevel = i - 1;
    	}
    	//cout << "Zlevel: " << zlevel << endl;
    	target.draw(*zoom_levels.at(zlevel), states);
    }
};

/*
int main(int argc, char const *argv[])
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
    MinMaxDecimator decimator(data);
	return 0;
}
*/