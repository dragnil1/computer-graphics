#include<bits/stdc++.h>
#include "bitmap_image.hpp"

using namespace std;

class Color
{
    public:
    int r,g,b;
    Color(int r,int g,int b)
    {
        this->r=r;
        this->g=g;
        this->b=b;
    }
};


bitmap_image image1, image2, image3;
int W, H;
int N;
int x_start, y_start, x_end, y_end;
Color color(1, 1, 1);
vector<vector<Color>> frameBuffer1, frameBuffer2, frameBuffer3;


void midPointLine(int x0, int y0, int x1, int y1, Color color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int d = 2*dy - dx;
    int incrE = 2*dy;
    int incrNE = 2*(dy - dx);
    int x = x0;
    int y = y0;
    frameBuffer1[x][y] = color;
    while(x < x1)
    {
        if(d <= 0)
        {
            d += incrE;
            x++;
        }
        else
        {
            d += incrNE;
            x++;
            y++;
        }
        frameBuffer1[x][y] = color;
    }
}

void colorLowerPixel(int x, int y, int x0, int y0, int x1, int y1, Color color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int dx_c = dx*y0 - dy*x0;
    int d = 8*(dy*x - dx*y + dx_c) - 3*(dy + dx);
    int cnt = 0;
    for(int i = 0; i < 4; i++, d += 2*dy)
    {
        for(int j = 0, d1 = d; j < 4; j++, d1 += 2*dx)
        {
            if(d1 <= 0)
            {
                cnt++;
            }
        }
    }
    //frameBuffer2[x][y] = Color(180 - (int)(color.r*cnt*1.00)/16,180 - (int)(color.g*cnt*1.00)/16,180 - (int)(color.b*cnt*1.00)/16);
    //frameBuffer2[x][y] = Color( color.r + (color.r*cnt*1.00)/16, color.g + (color.g*cnt*1.00)/16, color.b + (color.b*cnt*1.00)/16);
    frameBuffer2[x][y] = Color(180*(1 - cnt/16.00),180*(1 - cnt/16.00),180*(1 - cnt/16.00));
}

void colorUpperPixel(int x, int y, int x0, int y0, int x1, int y1, Color color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int dx_c = dx*y0 - dy*x0;
    int d = 8*(dy*x - dx*y + dx_c) - 3*(dy + dx);
    int cnt = 0;
    for(int i = 0; i < 4; i++, d += 2*dy)
    {
        for(int j = 0, d1 = d; j < 4; j++, d1 += 2*dx)
        {
            if(d1 >= 0)
            {
                cnt++;
            }
        }
    }
    //frameBuffer2[x][y] = Color(180 - (int)(color.r*cnt*1.00)/16,180 - (int)(color.g*cnt*1.00)/16,180 - (int)(color.b*cnt*1.00)/16);
    //frameBuffer2[x][y] = Color( color.r + (color.r*cnt*1.00)/16, color.g + (color.g*cnt*1.00)/16, color.b + (color.b*cnt*1.00)/16);
    frameBuffer2[x][y] = Color(180*(1 - cnt/16.00),180*(1 - cnt/16.00),180*(1 - cnt/16.00));
}


void unWeightedAreaSampling(int x0, int y0, int x1, int y1, Color color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int d = 2*dy - dx;
    int incrE = 2*dy;
    int incrNE = 2*(dy - dx);
    int x = x0;
    int y = y0;
    frameBuffer2[x][y] = color;
    colorLowerPixel(x, y + 1, x0, y0 + 1, x1, y1 + 1, color);
    colorUpperPixel(x, y - 1, x0, y0 - 1, x1, y1 - 1, color);
    while(x < x1)
    {

        if(d <= 0)
        {
            d += incrE;
            x++;
            colorLowerPixel(x, y + 1, x0, y0 + 1, x1, y1 + 1, color);
        }
        else
        {
            d += incrNE;
            x++;
            y++;
            colorUpperPixel(x, y - 1, x0, y0 - 1, x1, y1 - 1, color);
            
        }
        frameBuffer2[x][y] = color;
        //colorLowerPixel(x, y + 1, x0, y0 + 1, x1, y1 + 1, color);
        //colorUpperPixel(x, y - 1, x0, y0 - 1, x1, y1 - 1, color);
    }
}





double Filter(double distance)
{
    if(distance >= 0 && distance < 1.00/16)
    {
        return .780;
    }
    else if(distance >= 1.00/16 && distance < 2.00/16)
    {
        return .775;
    }
    else if(distance >= 2.00/16 && distance < 3.00/16)
    {
        return .760;
    }
    else if(distance >= 3.00/16 && distance < 4.00/16)
    {
        return .736;
    }
    else if(distance >= 4.00/16 && distance < 5.00/16)
    {
        return .703;
    }
    else if(distance >= 5.00/16 && distance < 6.00/16)
    {
        return .662;
    }
    else if(distance >= 6.00/16 && distance < 7.00/16)
    {
        return .613;
    }
    else if(distance >= 7.00/16 && distance < 8.00/16)
    {
        return .558;
    }
    else if(distance >= 8.00/16 && distance < 9.00/16)
    {
        return .500;
    }
    else if(distance >= 9.00/16 && distance < 10.00/16)
    {
        return .441;
    }
    else if(distance >= 10.00/16 && distance < 11.00/16)
    {
        return .383;
    }
    else if(distance >= 11.00/16 && distance < 12.00/16)
    {
        return .328;
    }
    else if(distance >= 12.00/16 && distance < 13.00/16)
    {
        return .276;
    }
    else if(distance >= 13.00/16 && distance < 14.00/16)
    {
        return .228;
    }
    else if(distance >= 14.00/16 && distance < 15.00/16)
    {
        return .184;
    }
    else if(distance >= 15.00/16 && distance < 16.00/16)
    {
        return .145;
    }
    else if(distance >= 16.00/16 && distance < 17.00/16)
    {
        return .110;
    }
    else if(distance >= 17.00/16 && distance < 18.00/16)
    {
        return .080;
    }
    else if(distance >= 18.00/16 && distance < 19.00/16)
    {
        return .056;
    }
    else if(distance >= 19.00/16 && distance < 20.00/16)
    {
        return .036;
    }
    else if(distance >= 20.00/16 && distance < 21.00/16)
    {
        return .021;
    }
    else if(distance >= 21.00/16 && distance < 22.00/16)
    {
        return .010;
    }
    else if(distance >= 22.00/16 && distance < 23.00/16)
    {
        return .004;
    }
    else if(distance >= 23.00/16 && distance < 24.00/16)
    {
        return .001;
    }
    else
    {
        return .000;
    }

}

void intensifyPixel(int x, int y, double distance)
{
    double intensity = Filter(abs(distance));
    frameBuffer3[x][y] = Color(180*(.78 - intensity), 180*(.78 - intensity), 180*(.78 - intensity));
}

void weightedAreaSampling(int x0, int y0, int x1, int y1, Color color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int d = 2*dy - dx;
    int incrE = 2*dy;
    int incrNE = 2*(dy - dx);
    int two_v_dx = 0;
    double invDenom = 1.0 / (2.0*sqrt(dx*dx + dy*dy));
    double two_dx_invDenom = 2.0*dx*invDenom;
    int x = x0;
    int y = y0;
    intensifyPixel(x, y, 0);
    intensifyPixel(x, y + 1, two_dx_invDenom);
    intensifyPixel(x, y - 1, two_dx_invDenom);
    while(x < x1)
    {

        if(d <= 0)
        {
            two_v_dx = d + dx;
            d += incrE;
            x++;
        }
        else
        {
            two_v_dx = d-dx; 
            d += incrNE;
            x++;
            y++;
        }
        intensifyPixel(x, y, two_v_dx*invDenom);
        intensifyPixel(x, y + 1, two_dx_invDenom - two_v_dx*invDenom);
        intensifyPixel(x, y - 1, two_dx_invDenom + two_v_dx*invDenom);
    }
}

void loadData()
{
    ifstream indata;
    indata.open("input.txt");
    if (!indata)
    {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    indata >> W >> H;
    indata >> N;
    frameBuffer1.assign(W, vector<Color>(H, Color(255, 255, 255)));
    frameBuffer2.assign(W, vector<Color>(H, Color(255, 255, 255)));
    frameBuffer3.assign(W, vector<Color>(H, Color(255, 255, 255)));
    for(int i = 0; i < N; i++)
    {
        indata >> x_start >> y_start >> x_end >> y_end;
        ///indata >> color.r >> color.g >> color.b;
        color.r = color.g = color.b = 0;
        midPointLine(x_start, y_start, x_end, y_end, color);
        unWeightedAreaSampling(x_start, y_start, x_end, y_end, color);
        weightedAreaSampling(x_start, y_start, x_end, y_end, color);
    }
    indata.close();
}



void draw(bitmap_image &image, vector<vector<Color>> &frameBuffer, string name)
{
    image.setwidth_height(W, H, true);
    for(int i = 0; i < W; i++)
    {
        for(int j = 0; j < H; j++)
        {
            image.set_pixel(i, j, frameBuffer[i][j + 2*(H - j) - H].r, frameBuffer[i][j + 2*(H - j) - H].g, frameBuffer[i][j + 2*(H - j) - H].b);
        }
    }
    image.save_image(name);
}



int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    loadData();
    draw(image1, frameBuffer1, "1_R.bmp");
    draw(image2, frameBuffer2, "2_RUA.bmp");
    draw(image3, frameBuffer3, "3_RWA.bmp");
    return 0;
}