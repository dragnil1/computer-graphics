#include "auxilary_function.cpp"
#include "bitmap_image.hpp"

point eye, look, up;
double fovY, aspectRatio, near, far;
ifstream indata;
ofstream outdata;
int total_triangles = 0;

void read_data()
{
    indata.open("scene.txt");
    outdata.open("stage1.txt");
    outdata << fixed << setprecision(7);
    if (!indata)
    {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }

    indata >> eye.x >> eye.y >> eye.z;
    indata >> look.x >> look.y >> look.z;
    indata >> up.x >> up.y >> up.z;
    indata >> fovY >> aspectRatio >> near >> far;
    // indata.close();
}

void modeling_transformation()
{
    read_data();
    stack<vector<vector<double>>> stk;
    vector<vector<double>> identity({{1.0, 0.0, 0.0, 0.0},
                                     {0.0, 1.0, 0.0, 0.0},
                                     {0.0, 0.0, 1.0, 0.0},
                                     {0.0, 0.0, 0.0, 1.0}});
    stk.push(identity);
    string command;
    vector<int> pushed_element;
    while (true)
    {
        indata >> command;

        if (command == "triangle")
        {
            point p[3], transformed_p[3];
            for (int i = 0; i < 3; i++)
            {
                indata >> p[i].x >> p[i].y >> p[i].z;
                transformed_p[i] = transformPoint(stk.top(), p[i]);
                outdata << transformed_p[i].x << " " << transformed_p[i].y << " " << transformed_p[i].z << endl;
            }
            outdata << endl;
            total_triangles++;
        }
        else if (command == "translate")
        {
            double tx, ty, tz;
            indata >> tx >> ty >> tz;
            vector<vector<double>> translation({{1, 0, 0, tx},
                                                {0, 1, 0, ty},
                                                {0, 0, 1, tz},
                                                {0, 0, 0, 1}});
            stk.push(matrix_multiply(stk.top(), translation));
            pushed_element.back()++;
        }
        else if (command == "scale")
        {
            double sx, sy, sz;
            indata >> sx >> sy >> sz;
            vector<vector<double>> scale({{sx, 0, 0, 0},
                                          {0, sy, 0, 0},
                                          {0, 0, sz, 0},
                                          {0, 0, 0, 1}});
            stk.push(matrix_multiply(stk.top(), scale));
            pushed_element.back()++;
        }
        else if (command == "rotate")
        {
            double angle;
            point a;
            indata >> angle >> a.x >> a.y >> a.z;
            a = normalize(a);
            vector<vector<double>> mat1({{0},
                                         {0},
                                         {0}});
            vector<vector<double>> mat2({{a.x},
                                         {a.y},
                                         {a.z}});
            mat1[0][0] = 1;
            vector<vector<double>> c1 = R(mat1, mat2, angle);
            mat1[0][0] = 0;
            mat1[1][0] = 1;
            vector<vector<double>> c2 = R(mat1, mat2, angle);
            mat1[1][0] = 0;
            mat1[2][0] = 1;
            vector<vector<double>> c3 = R(mat1, mat2, angle);
            vector<vector<double>> rotation({{c1[0][0], c2[0][0], c3[0][0], 0},
                                             {c1[1][0], c2[1][0], c3[1][0], 0},
                                             {c1[2][0], c2[2][0], c3[2][0], 0},
                                             {0, 0, 0, 1}});
            stk.push(matrix_multiply(stk.top(), rotation));
            pushed_element.back()++;
        }
        else if (command == "push")
        {
            pushed_element.push_back(0);
        }
        else if (command == "pop")
        {
            while (pushed_element.back() > 0)
            {
                stk.pop();
                pushed_element.back()--;
            }
            pushed_element.pop_back();
        }
        else
        {
            break;
        }
    }
    indata.close();
    outdata.close();
}

void view_transformation()
{
    vector<vector<double>> mat1({{look.x},
                                 {look.y},
                                 {look.z}});
    vector<vector<double>> mat2({{-eye.x},
                                 {-eye.y},
                                 {-eye.z}});
    
    vector<vector<double>> mat3 = matrix_add(mat1, mat2); // l
    
    mat3 = normalize(mat3);
    
   
    vector<vector<double>> mat4({{0, -mat3[2][0], mat3[1][0]},
                                 {mat3[2][0], 0, -mat3[0][0]},
                                 {-mat3[1][0], mat3[0][0], 0}});
    vector<vector<double>> mat5({{up.x},
                                 {up.y},
                                 {up.z}});
    vector<vector<double>> mat6 = matrix_multiply(mat4, mat5); // r
   
    mat6 = normalize(mat6);
    
    
    vector<vector<double>> mat7({{0, -mat6[2][0], mat6[1][0]},
                                 {mat6[2][0], 0, -mat6[0][0]},
                                 {-mat6[1][0], mat6[0][0], 0}});
                                 
    vector<vector<double>> mat8 = matrix_multiply(mat7, mat3); // u
    
    vector<vector<double>> T({{1, 0, 0, -eye.x},
                              {0, 1, 0, -eye.y},
                              {0, 0, 1, -eye.z},
                              {0, 0, 0, 1}});
    
    vector<vector<double>> R({{mat6[0][0], mat6[1][0], mat6[2][0], 0},
                              {mat8[0][0], mat8[1][0], mat8[2][0], 0},
                              {-mat3[0][0], -mat3[1][0], -mat3[2][0], 0},
                              {0, 0, 0, 1}});
    auto V = matrix_multiply(R, T);

    indata.open("stage1.txt");
    outdata.open("stage2.txt");
    outdata << fixed << setprecision(7);
    point p;
    for(int i = 0; i < total_triangles; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            indata >> p.x >> p.y >> p.z;
            vector<vector<double>> temp({{p.x},
                                         {p.y},
                                         {p.z},
                                         {1}});
            temp = matrix_multiply(V, temp);
            outdata << temp[0][0]/temp[3][0] << " " << temp[1][0]/temp[3][0] << " " << temp[2][0]/temp[3][0] << endl;
        }
        outdata << endl;
    }
    indata.close();
    outdata.close();
}

void projection_transformation()
{
    double fovX = fovY * aspectRatio;
    double t = near * tan((PI*fovY)/360);
    double r = near * tan((PI*fovX)/360);
    vector<vector<double>> p({
        {near/r, 0, 0, 0},
        {0, near/t, 0, 0},
        {0, 0, -(far + near)/(far - near), -(2*far*near)/(far - near)},
        {0, 0, -1, 0}
    });
    print(p);
    indata.open("stage2.txt");
    outdata.open("stage3.txt");
    outdata << fixed << setprecision(7);
    point p1;
    for(int i = 0; i < total_triangles; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            indata >> p1.x >> p1.y >> p1.z;
            vector<vector<double>> temp({{p1.x},
                                         {p1.y},
                                         {p1.z},
                                         {1}});
            temp = matrix_multiply(p, temp);
            outdata << temp[0][0]/temp[3][0] << " " << temp[1][0]/temp[3][0] << " " << temp[2][0]/temp[3][0]  << endl;
        }
        outdata << endl;
    }
    indata.close();
    outdata.close();
}

double screen_width, screen_height, left_x, right_x,
    top_y, bottom_y, near_z, far_z;

vector<Triangle> triangles;

double dx, dy, Top_Y, Left_X, Bottom_Y, Right_X;
vector<vector<double>> z_buffer;
vector<vector<Color>> f_buffer;
bitmap_image image;

void read_data1()
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
    for(int j = 0; j < total_triangles; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            indata >> triangle.points[i].x >> triangle.points[i].y >> triangle.points[i].z;
        }
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
    z_buffer.assign(screen_height, vector<double>(screen_width, far_z));
    Color color = {0, 0, 0};
    f_buffer.assign(screen_height, vector<Color>(screen_width, color));
    image.setwidth_height(screen_width, screen_height, true);
    for (int i = 0; i < screen_width; i++)
    {
        for (int j = 0; j < screen_height; j++)
        {
            image.set_pixel(i, j, 0, 0, 0);
        }
    }
}

int clip_top_y(Triangle triangle)
{
    double max_y = max(triangle.points[0].y, max(triangle.points[1].y, triangle.points[2].y));
    max_y = min(max_y, top_y);
    return round((top_y - max_y) / dy);
}

int clip_bottom_y(Triangle triangle)
{
    double min_y = min(triangle.points[0].y, min(triangle.points[1].y, triangle.points[2].y));
    min_y = max(min_y, bottom_y);
    return round((top_y - min_y) / dy);
}

int clip_left_x(double &xa)
{
    xa = max(xa, left_x);
    return round((right_x + xa) / dx);
}

int clip_right_x(double &xb)
{
    xb = min(xb, right_x);
    return round((right_x + xb) / dx);
}

void procedure()
{
    for (int k = 0; k < triangles.size(); k++)
    {
        auto triangle = triangles[k];
        triangle.color.b = rand();
        triangle.color.g = rand();
        triangle.color.r = rand();
        ///cout << clip_top_y(triangle) << " " << clip_bottom_y(triangle) << endl;
        for (int i = clip_top_y(triangle); i < clip_bottom_y(triangle); i++)
        {
            double ys = Top_Y - i * dy;
            auto lines = triangle.getIntersectingLines(ys);
            if(lines.size() != 2)
            {
                continue;
            }
            auto line1 = lines[0];
            auto line2 = lines[1];
            double za = line1.first.z - ((line1.first.z - line1.second.z) * (line1.first.y - ys)) / (line1.first.y - line1.second.y);
            double zb = line2.first.z - ((line2.first.z - line2.second.z) * (line2.first.y - ys)) / (line2.first.y - line2.second.y);
            double xa = line1.first.x - ((line1.first.x - line1.second.x) * (line1.first.y - ys)) / (line1.first.y - line1.second.y);
            double xb = line2.first.x - ((line2.first.x - line2.second.x) * (line2.first.y - ys)) / (line2.first.y - line2.second.y);
            

            if(xa > xb)
            {
                swap(xa, xb);
                swap(za, zb);
            }

            double m = (zb - za)/(xb - xa);
            double incr = dx*m;
            double zp = za;
            
            for (int j = clip_left_x(xa); j < clip_right_x(xb); j++)
            {
                if (zp < z_buffer[i][j])
                {
                    if (zp > near_z)
                    {
                        z_buffer[i][j] = zp;
                        f_buffer[i][j] = triangle.color;
                    }
                }
                zp += incr;
            }
        }
    }
    for(int i = 0; i < screen_height; i++)
    {
        for(int j = 0; j < screen_width; j++)
        {
            image.set_pixel(j, i, f_buffer[i][j].r, f_buffer[i][j].g, f_buffer[i][j].b);
        }
    }
}

void save()
{
    image.save_image("output.bmp");
    outdata.open("z_buffer.txt");
    outdata << fixed << setprecision(6);
    for(int i = 0; i < screen_height; i++)
    {
        for(int j = 0; j < screen_width; j++)
        {
            if(z_buffer[i][j] < far_z)
                outdata << z_buffer[i][j] << "        ";
        }
        outdata << endl;
    }
    outdata.close();
}

void free_memory()
{
    z_buffer.clear();
    f_buffer.clear();
}

void zBuffer()
{
    read_data1();
    initialize_buffer();
    procedure();
    save();
    free_memory();
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    modeling_transformation();
    view_transformation();
    projection_transformation();
    zBuffer();
}