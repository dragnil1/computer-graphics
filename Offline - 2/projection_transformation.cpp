#include "auxilary_function.cpp"

double fovY, aspectRatio, near, far;

vector<vector<double>> projection_transformation()
{
    double fovX = fovY * aspectRatio;
    double t = near * tan(fovY/2);
    double r = near * tan(fovX/2);
    vector<vector<double>> p({
        {near/r, 0, 0, 0},
        {0, near/t, 0, 0},
        {0, 0, -(far + near)/(far - near), -(2*far*near)/(far - near)},
        {0, 0, -1, 0}
    });
    return p;
}