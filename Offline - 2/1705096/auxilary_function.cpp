#include<bits/stdc++.h>


using namespace std;



const double PI = 2*acos(0.0);

struct point
{
    double x;
    double y;
    double z;
};

struct Color
{
    double r;
    double g;
    double b;
};

bool cmp(const point &a, const point &b)
{
    return (a.y > b.y || (a.y == b.y && a.x < b.x)); 
}

int orientation(point p1, point p2, point p3)
{
    double val = (p2.y - p1.y) * (p3.x - p2.x)
              - (p2.x - p1.x) * (p3.y - p2.y);

    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

struct Triangle{
    point points[3];
    Color color;
    void print()
    {
        for(int i = 0; i < 3; i++)
        {
            cout << points[i].x << "    " << points[i].y << "   " << points[i].z << endl;
        }
    }
    void srt()
    {
        sort(points, points + 3, cmp);
    }
    vector<pair<point, point>> getIntersectingLines(double scanLine)
    {
        vector<pair<point, point>> lines;
        if(points[0].y >= scanLine && scanLine >= points[1].y)
        {
            lines.push_back(make_pair(points[0], points[1]));
        }
        else if(points[1].y >= scanLine && scanLine >= points[0].y)
        {
            lines.push_back(make_pair(points[1], points[0]));
        }
        if(points[1].y >= scanLine && scanLine >= points[2].y)
        {
            lines.push_back(make_pair(points[1], points[2]));
        }
        else if(points[2].y >= scanLine && scanLine >= points[1].y)
        {
            lines.push_back(make_pair(points[2], points[1]));
        }
        if(points[2].y >= scanLine && scanLine >= points[0].y)
        {
            lines.push_back(make_pair(points[2], points[0]));
        }
        else if(points[0].y >= scanLine && scanLine >= points[2].y)
        {
            lines.push_back(make_pair(points[0], points[2]));
        }
        return lines;
    }
}; 

point normalize(point p)
{
    double length = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
    p.x /= length;
    p.y /= length;
    p.z /= length;
    return p;
}

vector<vector<double>> normalize(vector<vector<double>> mat)
{
    double sum = 0;
    for(int i = 0; i < mat.size(); i++)
    {
        for(int j = 0; j < mat[i].size(); j++)
        {
            sum += mat[i][j]*mat[i][j];
        }
    }
    sum = sqrt(sum);
    for(int i = 0; i < mat.size(); i++)
    {
        for(int j = 0; j < mat[i].size(); j++)
        {
            mat[i][j] /= sum;
        }
    }
    return mat;
}


vector<vector<double>> matrix_multiply(vector<vector<double>> mat1, vector<vector<double>> mat2)
{
    vector<vector<double>> mat3(mat1.size(), vector<double>(mat2[0].size()));
    for(int i = 0; i < mat1.size(); i++)
    {
        for(int j = 0; j < mat2[0].size(); j++)
        {
            for(int k = 0; k < mat2.size(); k++)
            {
                mat3[i][j] += mat1[i][k]*mat2[k][j];
            }
        }
    }
    return mat3;
}

vector<vector<double>> matrix_add(vector<vector<double>> mat1, vector<vector<double>> mat2)
{
    vector<vector<double>> mat3;
    mat3 = mat1;
    for(int i = 0; i < mat1.size(); i++)
    {
        for(int j = 0; j < mat1[i].size(); j++)
        {
            mat3[i][j] += mat2[i][j];
        }
    }
    return mat3;
}

vector<vector<double>> scalar_multiply(double scalar, vector<vector<double>> mat)
{
    for(int i = 0; i < mat.size(); i++)
    {
        for(int j = 0; j < mat[i].size(); j++)
        {
            mat[i][j] *= scalar;
        }
    }
    return mat;
}

void print(vector<vector<double>> vvd)
{
    for(int i = 0; i < vvd.size(); i++)
    {
        for(int j = 0; j < vvd[i].size(); j++)
        {
            cout << vvd[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

vector<vector<double>> R(vector<vector<double>> x, vector<vector<double>> a, double angle)
{
    cout << fixed << setprecision(7);
    angle = (angle*PI)/180.00;
    vector<vector<double>> temp1({
                                    {x[0][0], x[1][0], x[2][0]}
                                });
    vector<vector<double>> temp2({
                                {a[0][0]},
                                {a[1][0]},
                                {a[2][0]}
                                });
    vector<vector<double>> temp3({
                                {0, -a[2][0], a[1][0]},
                                {a[2][0], 0, -a[0][0]},
                                {-a[1][0], a[0][0], 0}
                                });

    //print(x);
    //cout << cos(angle) << endl;
    vector<vector<double>> mat1 = scalar_multiply(cos(angle), x);
    //print(mat1);
    //print(temp1);
    //cout << "&&" << endl;
    //print(temp2);
    //cout << endl;
    vector<vector<double>> mat2 = scalar_multiply(((1 - cos(angle))*matrix_multiply(temp1, temp2)[0][0]), a);
    //print(a);
    //cout << endl;
    //print(mat2);
    //cout << endl;
    vector<vector<double>> mat3 = scalar_multiply(sin(angle), matrix_multiply(temp3, x));
    //print(temp3);
    //cout << endl;
    //print(x);
    //cout << endl;
    //print(mat3);
    return matrix_add(matrix_add(mat1, mat2), mat3);
}

point transformPoint(vector<vector<double>> transformation_matrix, point p)
{
    vector<vector<double>> point_matrix({
                                            {p.x},
                                            {p.y},
                                            {p.z},
                                            {1}
                                        });
    point_matrix = matrix_multiply(transformation_matrix, point_matrix);
    point temp;
    temp.x = point_matrix[0][0]/point_matrix[3][0];
    temp.y = point_matrix[1][0]/point_matrix[3][0];
    temp.z = point_matrix[2][0]/point_matrix[3][0];
    return temp;
}
