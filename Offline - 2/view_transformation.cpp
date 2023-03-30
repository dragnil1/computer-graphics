#include "auxilary_function.cpp"

point eye, look, up;

vector<vector<double>> view_transformation()
{
    vector<vector<double>> mat1({
        {look.x},
        {look.y},
        {look.z}
    });
    vector<vector<double>> mat2({
        {-eye.x},
        {-eye.y},
        {-eye.z}
    });
    vector<vector<double>> mat3 = matrix_add(mat1, mat2);//l
    mat3 = normalize(mat3);
    vector<vector<double>> mat4({
        {0, -mat3[2][0], mat3[1][0]},
        {mat3[2][0], 0, -mat3[0][0]},
        {-mat3[1][0], mat3[0][0], 0}
    });
    vector<vector<double>> mat5({
        {up.x},
        {up.y},
        {up.z}
    });
    vector<vector<double>> mat6 = matrix_multiply(mat4, mat5);//r
    mat6 = normalize(mat6);
    vector<vector<double>> mat7({
        {0, -mat6[2][0], mat6[1][0]},
        {mat6[2][0], 0, -mat6[0][0]},
        {-mat6[1][0], mat6[0][0], 0}
    });
    vector<vector<double>> mat8 = matrix_multiply(mat7, mat3);//u
    vector<vector<double>> T({
        {1, 0, 0, eye.x},
        {0, 1, 0, eye.y},
        {0, 0, 1, eye.z},
        {0, 0, 0, 1}
    });

    vector<vector<double>> R({
        {mat6[0][0], mat6[1][0], mat6[2][0], 0},
        {mat8[0][0], mat8[1][0], mat8[2][0], 0},
        {-mat3[0][0], -mat3[1][0], -mat3[2][0], 0},
        {0, 0, 0, 1}
    });
    return matrix_multiply(R, T);
}