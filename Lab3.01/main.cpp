#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

const double step = 0.1;
const double x_s = -8;
const double y_s = -5;

const double x_e = 8;
const double y_e = 5;

struct Pixel {
    unsigned char R;
    unsigned char G;
    unsigned char B;
};

struct Point {
    long double x;
    long double y;
    long double v;
};

struct Dot {
    long double x;
    long double y;
    long double x_proj;
    long double y_proj;
};

long double distance(long double X1, long double Y1, long double X2, long double Y2)
{
    return sqrt(pow(X1 - X2, 2) + pow(Y1 - Y2, 2));
}

pair<int, int> getX(const vector<Point>& buff, long double x, long double y)
{
    double min_up = 1e100, min_down = 1e100;
    int up = 0, down = 0;
    for (int i = 0; i < buff.size(); i++)
    {
        if ((distance(buff[i].x, buff[i].y, x, y) < min_up) && (buff[i].x > x))
        {
            min_up = distance(buff[i].x, buff[i].y, x, y);
            up = i;
        }
        if ((distance(buff[i].x, buff[i].y, x, y) < min_down) && (buff[i].x < x))
        {
            min_down = distance(buff[i].x, buff[i].y, x, y);
            down = i;
        }
    }
    return make_pair(up, down);
}

pair<int, int> getY(const vector<Point>& buff, long double x, long  double y)
{
    double min_up = 1e100, min_down = 1e100;
    int up = 0, down = 0;
    for (int i = 0; i < buff.size(); i++)
    {
        if ((distance(buff[i].x, buff[i].y, x, y) < min_up) && (buff[i].y > y))
        {
            min_up = distance(buff[i].x, buff[i].y, x, y);
            up = i;
        }
        if ((distance(buff[i].x, buff[i].y, x, y) < min_down) && (buff[i].y < y))
        {
            min_down = distance(buff[i].x, buff[i].y, x, y);
            down = i;
        }
    }
    return make_pair(up, down);
}



int main()
{
    ifstream input;
    vector<Point> buff;
    vector<Dot> dots;
    Point temp_point;
    Pixel* image;
    string input_name, output_name;
    cin >> input_name >> output_name;
    input.open(input_name, std::ios_base::in);
    string temp;
    while (!input.eof())
    {
        input >> temp_point.x;
        input >> temp_point.y;
        input >> temp;
        temp_point.v = atof(temp.c_str());
        buff.push_back(temp_point);
    }
    cout << "complete";
    for (double y = y_s; y <= y_e; y += step)
    {
        for (double x = x_s; x <= x_e; x += step)
        {
            pair<int, int> x_closest = getX(buff, x, y);
            pair<int, int> y_closest = getY(buff, x, y);

            dots.push_back({ x, y,
            (buff[x_closest.first].v - buff[x_closest.second].v) / (buff[x_closest.first].x - buff[x_closest.second].x),
            (buff[y_closest.first].v - buff[y_closest.second].v) / (buff[y_closest.first].y - buff[y_closest.second].y) });
        }
    }
    double maxField = 25;
    image = new Pixel[dots.size()];
    for (int i = 0; i < dots.size(); i++)
    {
        image[i].R = 0;
        image[i].G = 0;
        image[i].B = 0;
    }
    for (int i = 0; i < dots.size(); i++)
    {
        double buffer;
        buffer = (distance(0, 0, abs(dots[i].x_proj), abs(dots[i].y_proj)) / maxField);
        if (buffer > 1)
            buffer = 1;
        if (buffer < 0.25)
        {
            image[i].B = buffer * 4 * 200;
        }
        else
        {
            image[i].R = ((buffer - 0.25) * 255) / 0.75;
            image[i].B = 255 - image[i].R;
        }
    }
    int width = (x_e - x_s) / step + 1, height = (y_e - y_s) / step + 1;
    ofstream output;
    output.open(output_name, ios_base::out | ios_base::binary);
    output << "P6" << '\n';
    output << width << ' ' << height << '\n' << "255" << '\n';
    char* image_char = (char*)image;
    output.write(image_char, width * height * 3);
    output.close();
    return 0;
}
