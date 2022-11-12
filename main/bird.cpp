#include "bird.h"

#include <cmath>

const double g = 0.1;
const double v_tap_up = -1;
const double v_tap_left = -1;
const double a_tap_left = 0.1;
const double v_tap_right = 1;
const double a_tap_right = -0.1;

Bird::Bird(double x, double y, double vx, double vy, double ax, double ay)
    : x(x), y(y), vx(vx), vy(vy), ax(ax), ay(ay) {}

std::pair<int, int> Bird::get_pos() const {
    return std::make_pair((int)std::round(x), (int)std::round(y));
}
std::pair<double, double> Bird::get_v() const { return std::make_pair(vx, vy); }
std::pair<double, double> Bird::get_a() const { return std::make_pair(ax, ay); }
void Bird::set_pos(int _x, int _y) {
    x = _x;
    y = _y;
}

void Bird::set_v(double _vx, double _vy) {
    vx = _vx;
    vy = _vy;
}

void Bird::set_a(double _ax, double _ay) {
    ax = _ax;
    ay = _ay;
}

void Bird::draw(Display& display) const {
    // TODO: draw the bird on the display
    // hint: you should check if the bird is inside the display
    /* Your code here */
    if(x>0&&x<display.get_cols()-1&&y>0&&y<display.get_rows()-1)
    {
        display.put_string((int)x,(int)y,"@");
    }
    
}

bool Bird::is_inside(int x, int y) const {
    if (x == this->x &&y == this->y) {
        return true;
    }
    return false;
}

void Bird::tick(int key) {
    // TODO: update the bird's properties according to key input
    /* Your code here */
    if (key == ' ')
    {


        vy=v_tap_up;
        ay=g;    
    } 
    else if (key == 'z') 
    {
        vx=v_tap_left;
        ax=a_tap_left;
    } 
    else if (key == 'x') 
    {
        vx=v_tap_right;
        ax=a_tap_right;
    }
    else
    {
        if ((vx + ax) * ax >= 0) {
                ax=0;
                vx=0;
            } 
    }
    ay=g;
    x += vx + ax/2;
    y += vy + ay/2;
    vx += ax;
    vy += ay;
    
}

bool Bird::detect_collide(Object& obj) {
    // TODO: detect collision with the specified object
    /* Your code here */
    if(obj.is_inside(x,y))
    {
        this->valid=false;
        return true;
    }
    else
        return false;
}