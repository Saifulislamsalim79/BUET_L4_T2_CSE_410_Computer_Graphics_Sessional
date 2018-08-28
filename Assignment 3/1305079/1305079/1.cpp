#include<bits/stdc++.h>
#include "bitmap_image.hpp"
using namespace std;

struct Point{
    double x, y, z;
};

struct Color{
    int r, g, b;
};

struct Triangle{
    Point points[3];
    Color color;
};

double screen_width, screen_height;
double x_left_limit, x_right_limit, y_bottom_limit, y_top_limit, z_rear_limit, z_front_limit;
double x_limit, y_limit, z_limit;
double dx, dy;
double topy, leftx;

vector <Triangle> triangles;
double** z_buffer;
Color** frame_buffer;


void read_data()
{
    //reading config.txt
    ifstream config("config.txt");

    config >> screen_width >> screen_height;
    config >> x_left_limit >> y_bottom_limit >> z_front_limit >> z_rear_limit;

    x_right_limit = - x_left_limit;
    y_top_limit = - y_bottom_limit;

    x_limit = x_right_limit*2;
    y_limit = y_top_limit*2;
    z_limit = z_rear_limit - z_front_limit;

    dx = x_limit/screen_width;
    dy = y_limit/screen_height;

    topy = y_top_limit - dy/2;
    leftx = x_left_limit + dx/2;

    //cout<<"config.txt\n";
    cout<<screen_width<<" "<<screen_height<<"\n"<<x_left_limit<<" "<<x_right_limit<<"\n"<<y_bottom_limit<<" "<<y_top_limit<<"\n"<<z_front_limit<<" "<<z_rear_limit;

    //cout << "\nLimits : " << x_limit << " " << y_limit << " " << z_limit << "\n";
    config.close();

    //reading stage3.txt
    //cout<<"\n\nstage3.txt\n";
    ifstream stage3("stage3.txt");

    Triangle tr;

    while(!stage3.eof())
    {
        stage3 >> tr.points[0].x >> tr.points[0].y >> tr.points[0].z ;
        stage3 >> tr.points[1].x >> tr.points[1].y >> tr.points[1].z ;
        stage3 >> tr.points[2].x >> tr.points[2].y >> tr.points[2].z ;

        //srand (time(0));
        tr.color.r = rand() % 256;
        tr.color.g = rand() % 256;
        tr.color.b = rand() % 256;

        triangles.push_back(tr);

        //cout << tr.points[0].x << " " << tr.points[0].y << " " << tr.points[0].z << "\n" ;
        //cout << tr.points[1].x << " " << tr.points[1].y << " " << tr.points[1].z << "\n";
        //cout << tr.points[2].x << " " << tr.points[2].y << " " << tr.points[2].z << "\n";

        //cout << tr.color.r << " " << tr.color.g << " " << tr.color.b << "\n\n" ;
    }
    stage3.close();
}

void initialize_z_buffer_and_frame_buffer()
{
    vector<double> temp;
    printf("0\n");

    z_buffer = new double*[(int)screen_height];

    for(int i=0; i<(int)screen_height; i++)
    {
//        for(int j=0; j<screen_width; j++)
//        {
//            temp.push_back(z_rear_limit);
//        }
//        z_buffer.push_back(temp);
        z_buffer[i] = new double[(int)screen_width];
        for(int j = 0; j < (int)screen_width; j++)
        {
            //printf("%d %d\n", i, j);
            z_buffer[i][j] = z_rear_limit;
        }
    }

    printf("1\n");

    vector<Color> temp_color;
    Color clr = {0,0,0};

    frame_buffer = new Color*[(int)screen_height];
    for(int i=0; i<screen_height; i++)
    {
//        for(int j=0; j<screen_width; j++)
//        {
//            temp_color.push_back(clr);
//        }
//        frame_buffer.push_back(temp_color);
        frame_buffer[i] = new Color[(int)screen_width];
    }

    printf("2\n");
}

double max_val(double a, double b, double c)
{
    if(a>=b && a>=c) return a;
    else if(b>=a && b>=c) return b;
    else return c;
}

double min_val(double a, double b, double c)
{
    if(a<=b && a<=c) return a;
    else if(b<=a && b<=c) return b;
    else return c;
}

double max_x(Triangle tr)
{
    return max_val(tr.points[0].x, tr.points[1].x, tr.points[2].x);
}

double min_x(Triangle tr)
{
    return min_val(tr.points[0].x, tr.points[1].x, tr.points[2].x);
}

double max_y(Triangle tr)
{
    return max_val(tr.points[0].y, tr.points[1].y, tr.points[2].y);
}

double min_y(Triangle tr)
{
    return min_val(tr.points[0].y, tr.points[1].y, tr.points[2].y);
}

double top_Scanline(Triangle tr)
{
    double res = max_y(tr);
    if(res < y_top_limit) return res;
    else return y_top_limit;
}

double bottom_Scanline(Triangle tr)
{
    double res = min_y(tr);
    if(res > y_bottom_limit)  return res;
    else return y_bottom_limit;
}

vector<double> left_and_right_intersecting_points(Triangle tr, double py)
{
    vector<double> left_right_x, px;
    int j=0;
    for(int i=0; i<=2; i++)
    {
        Point p0 = tr.points[i];
        Point p1 = tr.points[(i+1)%3];

        if(p0.y == p1.y) continue;

        double t = (py-p0.y)/(p1.y-p0.y);
        if(t<0 || t>1)continue;
        if(j<2)
        {
            px.push_back(p0.x + t*(p1.x - p0.x));
            j++;
        }
    }
    sort(px.begin(), px.end());

    return px;
}

vector<double> plane_equation(Triangle tr)
{
    vector<double> coeff;

    Point ab = {tr.points[1].x - tr.points[0].x, tr.points[1].y - tr.points[0].y, tr.points[1].z - tr.points[0].z};
    Point bc = {tr.points[2].x - tr.points[1].x, tr.points[2].y - tr.points[1].y, tr.points[2].z - tr.points[1].z};

    Point normal = {ab.y*bc.z - bc.y*ab.z, ab.z*bc.x - bc.z*ab.x, ab.x*bc.y - ab.y*bc.x};

    double D = - normal.x*tr.points[2].x - normal.y*tr.points[2].y - normal.z*tr.points[2].z;

    coeff.push_back(normal.x);
    coeff.push_back(normal.y);
    coeff.push_back(normal.z);
    coeff.push_back(D);

    //cout << "\n\n\nA: " << normal.x <<  "\nB: " << normal.y <<  "\nC: " << normal.z << "\nD: " << D;

    return coeff;
}

double get_z_value(double x, double y, vector<double> plane)
{
    return ((plane[0]*x + plane[1]*y + plane[3])/(-plane[2]));
}

int x_to_column(double x)
{
    return round((x - leftx)/dx);
}

int y_to_row(double y)
{
    return round((topy - y)/dy);
}

double column_to_x(int c)
{
    return leftx + c*dx;
}

double row_to_y(int r)
{
    return topy - r*dy;
}



void apply_procedure()
{
    for(int i=0; i<triangles.size(); i++)
    {
        double top_scanline = top_Scanline(triangles[i]);
        double bottom_scanline = bottom_Scanline(triangles[i]);
        double left_x, right_x;
        vector<double> plane = plane_equation(triangles[i]);

        //cout << "Triangle:"<<i<<"------------------\n";

        int r = y_to_row(bottom_scanline);
        int end_r = y_to_row(top_scanline);

        for(; r >= end_r; r--)
        {
            double j = row_to_y(r);
            vector<double> temp = left_and_right_intersecting_points(triangles[i], j);
            if(temp.size()<2) continue;
            left_x = temp[0];
            right_x = temp[1];

            if(left_x < x_left_limit) left_x = x_left_limit;
            if(right_x > x_right_limit) right_x = x_right_limit;

            int c = x_to_column(left_x);
            int end_c = x_to_column(right_x);

            for(; c <= end_c; c++)
            {
                double k = column_to_x(c);

                double temp_z = get_z_value(k, j, plane);



                //int r = y_to_row(j);
                //int c = x_to_column(k);
                if(r < 0 || r >= screen_height || c < 0 || c >= screen_width)
                {
                    continue;
                }

                //printf("%d %d\n", r, c);

                if(temp_z < z_buffer[r][c] && temp_z >= z_front_limit)
                {
                    z_buffer[r][c] = temp_z;
                    frame_buffer[r][c] = triangles[i].color;
                }
            }
        }
    }
}

void save()
{
    bitmap_image image(screen_width, screen_height);
    freopen("z_buffer.txt", "w", stdout);

    for(int i=0;i<screen_width;i++){
        for(int j=0;j<screen_height;j++){
            if(z_buffer[i][j] < z_rear_limit)
            {
                printf("%lf\t", z_buffer[i][j]);
            }
            image.set_pixel(j,i,frame_buffer[i][j].r, frame_buffer[i][j].g, frame_buffer[i][j].b);
        }
        printf("\n");
    }

    image.save_image("test.bmp");
}

void free_memory()
{
    triangles.clear();
    for(int i=0; i<screen_height; i++)
    {
        delete z_buffer[i];
        delete frame_buffer[i];
    }
    delete z_buffer;
    delete frame_buffer;
}


int main()
{
    read_data();
    initialize_z_buffer_and_frame_buffer();
    apply_procedure();
    save();
    free_memory();
}
