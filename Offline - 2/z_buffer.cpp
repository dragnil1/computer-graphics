#include "auxilary_function.cpp"
#include "bitmap_image.hpp"

using namespace std;

double screen_width, screen_height, left_x, right_x,
    top_y, bottom_y, near_z, far_z;

vector<Triangle> triangles;

double dx, dy, Top_Y, Left_X, Bottom_Y, Right_X;
vector<vector<double>> z_buffer;
bitmap_image image;

void read_data()
{
    ifstream indata;
    indata.open("config.txt");
    if (!indata)
    {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }

    indata >> screen_width >> screen_height;
    indata >> left_x;
    indata >> bottom_y;
    indata >> near_z >> far_z;

    right_x = -left_x;
    top_y = -bottom_y;

    // cout << screen_width << " " << screen_height << endl;
    // cout << left_x << " " << right_x << " " << top_y << " " << bottom_y << endl;
    // cout << near_z << " " << far_z << endl;
    indata.close();

    indata.open("stage3.txt");
    Triangle triangle;
    while (!indata.eof())
    {
        for (int i = 0; i < 3; i++)
        {
            indata >> triangle.points[i].x >> triangle.points[i].y >> triangle.points[i].z;
        }
        triangle.print();
        cout << endl;
        triangles.push_back(triangle);
    }
    indata.close();
}

void initialize_buffer()
{
    dx = (right_x - left_x) / screen_width;
    dy = (top_y - bottom_y) / screen_height;
    Top_Y = top_y - dy / 2;
    Bottom_Y = bottom_y + dy / 2;
    Left_X = left_x + dx / 2;
    Right_X = right_x - dx / 2;
    z_buffer.assign(screen_width, vector<double>(screen_height, far_z));
    image.setwidth_height(screen_width, screen_height, true);
    for (int i = 0; i < screen_width; i++)
    {
        for (int j = 0; j < screen_height; j++)
        {
            image.set_pixel(i, j, 0, 0, 0);
        }
    }
    /// image.save_image("test.bmp");
}

int clip_top_y(Triangle triangle)
{
    double max_y = max(triangle.points[0].y, max(triangle.points[1].y, triangle.points[2].y));

    /*if (max_y > Top_Y)
    {
        return 0;
    }
    for (int i = 0; i < screen_height; i++)
    {
        double y = Top_Y - i * dy;
        if (abs(y - max_y) <= dy)
        {
            return i;
        }
    }*/
    max_y = min(max_y, top_y);
    return round((top_y - max_y) / dy);
}

int clip_bottom_y(Triangle triangle)
{
    double min_y = min(triangle.points[0].y, min(triangle.points[1].y, triangle.points[2].y));

    /*if (min_y < Bottom_Y)
    {
        return screen_height - 1;
    }
    for (int i = screen_height - 1; i >= 0; i--)
    {
        double y = Top_Y - i * dy;
        if (y >= min_y)
        {
            return i;
        }
    }*/
    min_y = max(min_y, bottom_y);
    return round((top_y - min_y) / dy);
}

int clip_left_x(Triangle triangle, int scan_row, double &xa)
{
    double min_x = right_x + 10;
    double scan_y = Top_Y - scan_row * dy;
    for (int i = 0; i < 3; i++)
    {
        for (int j = i + 1; j < 3; j++)
        {
            if (abs(triangle.points[i].y - triangle.points[j].y) < 1e-6)
            {
                continue;
            }
            double x = ((scan_y - triangle.points[i].y) * (triangle.points[i].x - triangle.points[j].x)) /
                           (triangle.points[i].y - triangle.points[j].y) +
                       triangle.points[i].x;
            double x_max = max(triangle.points[i].x, triangle.points[j].x);
            double x_min = min(triangle.points[i].x, triangle.points[j].x);
            if (x_min >= x && x_max <= x)
            {
                if (x < min_x)
                {
                    min_x = x;
                }
            }
        }
    }
    xa = min_x;
    if (min_x < Left_X)
    {
        return 0;
    }
    for (int i = 0; i < screen_width; i++)
    {
        double x = Left_X + i * dx;
        if (abs(x - min_x) <= dx)
        {
            return i;
        }
    }
}

int clip_right_x(Triangle triangle, int scan_row, double &xb)
{
    double max_x = left_x - 10;
    double scan_y = Top_Y - scan_row * dy;
    for (int i = 0; i < 3; i++)
    {
        for (int j = i + 1; j < 3; j++)
        {
            if (abs(triangle.points[i].y - triangle.points[j].y) < 1e-6)
            {
                continue;
            }
            double x = ((scan_y - triangle.points[i].y) * (triangle.points[i].x - triangle.points[j].x)) /
                           (triangle.points[i].y - triangle.points[j].y) +
                       triangle.points[i].x;
            //cout << x << " ### " << scan_y << endl;
            double x_max = max(triangle.points[i].x, triangle.points[j].x);
            double x_min = min(triangle.points[i].x, triangle.points[j].x);
            if (x_min >= x && x_max <= x)
            {
                if (x > max_x)
                {
                    max_x = x;
                }
            }
        }
    }
    //cout << endl;
    xb = max_x;
    if (max_x > Right_X)
    {
        return screen_width - 1;
    }
    for (int i = screen_width - 1; i >= 0; i--)
    {
        double x = Left_X + i * dx;
        if (abs(x - max_x) <= dx)
        {
            return i;
        }
    }
}

void procedure()
{
    for (int k = 0; k < triangles.size(); k++)
    {
        auto triangle = triangles[k];
        triangle.srt();
        cout << "###########" << endl;
        triangle.print();
        if (k % 3 == 0)
        {
            triangle.color.r = 180;
        }
        else if (k % 3 == 1)
        {
            triangle.color.b = 100;
        }
        else
        {
            triangle.color.g = 50;
        }
        triangle.color.r += k * 10;
        triangle.color.g += k * 10;
        triangle.color.b += k * 10;
        ///cout << Top_Y - clip_top_y(triangle) * dy << " " << Top_Y - clip_bottom_y(triangle) * dy << endl;
        for (int i = clip_top_y(triangle); i <= clip_bottom_y(triangle); i++)
        {
            /// cout << i << " ";
            double xa, xb;
            for (int j = clip_left_x(triangle, i, xa); j <= clip_right_x(triangle, i, xb); j++)
            {
                if(xa > right_x || xb < left_x)
                {
                    continue;
                }
                double z1 = triangle.points[0].z;
                double z2 = triangle.points[1].z;
                double z3 = triangle.points[2].z;
                double y1 = triangle.points[0].y;
                double y2 = triangle.points[1].y;
                double y3 = triangle.points[2].y;
                double ys = Top_Y - i * dy;
                double xp = Left_X + j * dx;
                double za = z1 - (z1 - z2) * (y1 - ys) / (y1 - y2);
                double zb = z1 - (z1 - z3) * (y1 - ys) / (y1 - y3);
                double zp = zb - (zb - za) * (xb - xp) / (xb - xa);
                cout << xb << " " << xa << " " << Top_Y - i*dy <<  endl;
                if (z_buffer[j][i] - zp > 1e-9)
                {
                    if (zp - near_z >= 1e-9)
                    {
                        z_buffer[j][i] = zp;
                    }
                    image.set_pixel(j, i, triangle.color.r, triangle.color.g, triangle.color.b);
                }
            }
            // cout << endl;
        }
    }
    image.save_image("test.bmp");
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    read_data();
    initialize_buffer();
    procedure();
    return 0;
}