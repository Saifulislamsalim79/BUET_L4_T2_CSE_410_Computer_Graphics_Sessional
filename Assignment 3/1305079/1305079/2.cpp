#include<bits/stdc++.h>
#include "bitmap_image.hpp"
using namespace std;

class Point{
public:
    double x, y, z;
};

struct Color{
    int r, g, b;
};

class Triangle{
public:
    Point points[3];
    Color color;
    bool in_out = false;
};

class Edge{
public:
    double x_at_y_min, y_max, del_x;
    Triangle* parent;
};
class Table{
    public:
    vector<Edge*> list;
};



int screen_width, screen_height;
double x_left_limit, x_right_limit, y_bottom_limit, y_top_limit, z_rear_limit, z_front_limit;
double x_limit, y_limit, z_limit;
double dx, dy;
double topy, leftx;

vector <Triangle*> triangles;
Table **table;

bool comparetor(Edge* e, Edge* e1)
{
    return e->x_at_y_min < e1->x_at_y_min;
}

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

    table=new Table*[screen_height];
    for(int i=0;i<screen_height;i++)
    {
        table[i]=new Table;
    }

    config.close();

    //reading stage3.txt
    ifstream stage3("stage3.txt");



    while(!stage3.eof())
    {
        Triangle *tr = new Triangle;
        stage3 >> tr->points[0].x >> tr->points[0].y >> tr->points[0].z ;
        stage3 >> tr->points[1].x >> tr->points[1].y >> tr->points[1].z ;
        stage3 >> tr->points[2].x >> tr->points[2].y >> tr->points[2].z ;

        //srand (time(0));
        tr->color.r = rand() % 256;
        tr->color.g = rand() % 256;
        tr->color.b = rand() % 256;

        triangles.push_back(tr);
        for(int i=0; i<3 ; i++)
        {
            Point p0 = tr->points[i];
            Point p1 = tr->points[(i+1)%3];

            double miny, minx, maxy, maxx;

            if(p0.y<p1.y)
            {
                miny = p0.y;
                minx = p0.x;
                maxy = p1.y;
                maxx = p1.x;
            }
            else
            {
                miny = p1.y;
                minx = p1.x;
                maxy = p0.y;
                maxx = p0.x;
            }
            double delta_x = (maxx - minx)*dy/(maxy-miny);

            if(miny>y_top_limit || maxy < y_bottom_limit || miny==maxy) continue;

            if(miny<y_bottom_limit)
            {
                minx = delta_x*(y_bottom_limit-miny)/dy;
                miny = y_bottom_limit;
            }
            if(maxy>y_top_limit)
            {
                maxy = y_top_limit;

            }

            Edge* edge = new Edge;
            edge->x_at_y_min = minx;
            edge->y_max = maxy;
            edge->del_x = delta_x;
            edge->parent = tr;

            int idx = (topy-miny)/dy;
            table[idx]->list.push_back(edge);

            sort(table[idx]->list.begin(), table[idx]->list.end(), comparetor);
        }
    }
    cout<<"READ"<<endl;
    stage3.close();
}

vector<double> plane_equation(Triangle* tr)
{
    vector<double> coeff;

    Point ab = {tr->points[1].x - tr->points[0].x, tr->points[1].y - tr->points[0].y, tr->points[1].z - tr->points[0].z};
    Point bc = {tr->points[2].x - tr->points[1].x, tr->points[2].y - tr->points[1].y, tr->points[2].z - tr->points[1].z};

    Point normal = {ab.y*bc.z - bc.y*ab.z, ab.z*bc.x - bc.z*ab.x, ab.x*bc.y - ab.y*bc.x};

    double D = - normal.x*tr->points[2].x - normal.y*tr->points[2].y - normal.z*tr->points[2].z;

    coeff.push_back(normal.x);
    coeff.push_back(normal.y);
    coeff.push_back(normal.z);
    coeff.push_back(D);

    return coeff;
}

double get_z_value(double x, double y, vector<double> plane)
{
    return ((plane[0]*x + plane[1]*y + plane[3])/(-plane[2]));
}




void apply_procedure()
{
    bitmap_image image(screen_width, screen_height);

    for(double y=y_bottom_limit; y<y_top_limit; y+=dy)
    {

        int r = (topy-y)/dy;
        if(table[r]->list.size()>0)
        {
            Table* AET = table[r];
            int i;

            for(i=0; i<AET->list.size()-1; i++)
            {
                AET->list[i]->parent->in_out =!AET->list[i]->parent->in_out;
                for(double x=AET->list[i]->x_at_y_min ;x<AET->list[i+1]->x_at_y_min ;x+=dx  )
                {

                    if(x<x_left_limit || x>x_right_limit) continue;
                    double z = z_rear_limit;
                    for(int j=0; j< triangles.size(); j++)
                    {
                        if(triangles[j]->in_out)
                        {
                            vector<double> coeff = plane_equation(triangles[j]);

                            double tempz = get_z_value(x, y, coeff);

                            if(tempz < z)
                            {
                                z = tempz;
                                int c = (x - leftx)/dx;

                                image.set_pixel(c,r, triangles[j]->color.r, triangles[j]->color.g, triangles[j]->color.b);
                            }
                        }

                    }

                }
            }
            AET->list[i]->parent->in_out =!AET->list[i]->parent->in_out;// = true;
            for(i=0; i<AET->list.size(); i++)
            {
                AET->list[i]->x_at_y_min += AET->list[i]->del_x;
                if(y+dy <= AET->list[i]->y_max && r != 0)
                {
                    table[r-1]->list.push_back(AET->list[i]);
                }
            }
            if(r!=0)sort(table[r-1]->list.begin(), table[r-1]->list.end(), comparetor);
        }
    }
    image.save_image("test2.bmp");
}



void free_memory()
{
    for(int i=0; i<screen_height; i++)
    {
        delete table;
    }
    delete table;
    triangles.clear();
}


int main()
{
    read_data();
    apply_procedure();
    free_memory();
}
