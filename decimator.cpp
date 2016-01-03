#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

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
	for (int i = 0; i < data.size() - 1; i++)
	{
		ofile << data.at(i) << ',';
	}
	ofile << data.at(data.size() - 1);
	ofile.close();
}

class MinMaxTree
{
private:
	vector<vector<float> *> _zoom_levels;
	
	void _next_zoom_level()
	{
		vector<float> * data = _zoom_levels.at(_zoom_levels.size() - 1);
		_zoom_levels.push_back(new vector<float>);
		vector<float> * last_level = _zoom_levels.at(_zoom_levels.size() - 1);
		
		int i = 0;
		for (i = 0; i < data->size() / 4 - 1; i++)
		{
			last_level->push_back(*max_element(&data->at(4*i), &data->at(4*i + 4)));
			last_level->push_back(*min_element(&data->at(4*i), &data->at(4*i + 4)));
		}
		last_level->push_back(*max_element(data->end() - (data->size() - 4*i), data->end()));
		last_level->push_back(*min_element(data->end() - (data->size() - 4*i), data->end()));
		cout << last_level->size() << endl;
	}

public:
	MinMaxTree(vector<float> & data)
	{
		_zoom_levels.push_back(& data);
		cout << data.size() << endl;
		while (_zoom_levels.at(_zoom_levels.size() - 1)->size() > 2000)
		{
			_next_zoom_level();
		}
		cout << _zoom_levels.size() << endl;

		string ofile("offzoom.csv");
		export_csv(ofile, *_zoom_levels.at(_zoom_levels.size() - 4));
		//export_csv(ofile, *_zoom_levels.at());

	}
};

class ZoomableSignal
{
private:
	float _gmax, _gmin;
	MinMaxTree _mmtree;
};

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
    MinMaxTree tree(data);
	return 0;
}