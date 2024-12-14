#include "FL/Enumerations.H"
#include "FL/Fl.H"
#include "FL/Fl_SVG_Image.H"
#include "FL/fl_draw.H"
//#include "nanosvg/nanosvg.h"
#include <cstdint>
#include <format>
#include <string>

namespace vs{

namespace theme_cute{
    /*
void up_box(int x, int y, int w, int h, Fl_Color c){
        std::string s = std::format("<svg viewBox='0 0 {} {}'>\
  <rect x='1%' y='1%' stroke-width='2' rx='10%' width='96%' height='96%' stroke='rgb({}, {}, {})' fill='none' />\
</svg>",w, h, c&(0xff<<24),c&(0xff<<16),c&(0xff<<8));

    Fl_SVG_Image img (">",s.c_str());
    img.draw(x,y,w,h);
    //fl_draw_box(Fl_Boxtype::FL_FLAT_BOX, x, y,  w, h, c);
    //    let col1 = c.to_rgb();
    //let col = Color::color_average(c, Color::Background, 0.8).to_rgb();
    //draw::draw_rbox(x, y, w, h, 5, true, Color::from_rgb(col.0, col.1, col.2));
}

void diamondup_box(int x, int y, int w, int h, Fl_Color c){
        std::string s = std::format("<svg viewBox='0 0 {} {}'>\
  <rect x='1%' y='1%' stroke-width='2' rx='10%' width='96%' height='96%' stroke='rgb({}, {}, {})' fill='none' />\
</svg>",w, h, c&(0xff<<24),c&(0xff<<16),c&(0xff<<8));

    Fl_SVG_Image img (">>",s.c_str());
    img.draw(x,y,w,h);
    //    let col1 = c.to_rgb();
    //let col = Color::color_average(c, Color::Background, 0.8).to_rgb();
    //draw::draw_rbox(x, y, w, h, 5, true, Color::from_rgb(col.0, col.1, col.2));
    //    //fl_draw_box(Fl_Boxtype::FL_FLAT_BOX, x, y,  w, h, c);

}
*/
/*
#define cmap(i,r,g,b)  [i]={r,g,b}

extern "C"{
constexpr uint8_t colors[][3] = {
    cmap(0, 200, 200, 200),
    cmap(1, 150, 30, 30),
    cmap(2, 0, 180, 0),
    cmap(3, 180, 180, 0),
    cmap(4, 0, 0, 180),
    cmap(5, 180, 0, 180),
    cmap(6, 0, 180, 180),
    cmap(7, 50, 50, 50),
    cmap(8, 100, 100, 100),
    cmap(9, 150, 90, 90),
    cmap(10, 90, 150, 90),
    cmap(11, 150, 150, 90),
    cmap(12, 90, 90, 150),
    cmap(13, 150, 90, 150),
    cmap(14, 90, 150, 150),
    cmap(15, 150, 150, 150),
    cmap(32, 5, 5, 5),
    cmap(33, 10, 10, 10),
    cmap(34, 16, 16, 16),
    cmap(35, 21, 21, 21),
    cmap(36, 26, 26, 26),
    cmap(37, 32, 32, 32),
    cmap(38, 37, 37, 37),
    cmap(39, 42, 42, 42),
    cmap(40, 48, 48, 48),
    cmap(41, 53, 53, 53),
    cmap(42, 58, 58, 58),
    cmap(43, 64, 64, 64),
    cmap(44, 69, 69, 69),
    cmap(45, 74, 74, 74),
    cmap(46, 80, 80, 80),
    cmap(47, 85, 85, 85),
    cmap(48, 90, 90, 90),
    cmap(49, 96, 96, 96),
    cmap(50, 101, 101, 101),
    cmap(51, 106, 106, 106),
    cmap(52, 112, 112, 112),
    cmap(53, 117, 117, 117),
    cmap(54, 122, 122, 122),
    cmap(55, 180, 180, 180),
    cmap(56, 0, 0, 0),
    cmap(59, 30, 140, 30),
    cmap(63, 0, 180, 0),
    cmap(71, 0, 180, 0),
    cmap(88, 180, 0, 0),
    cmap(90, 200, 72, 60),
    cmap(91, 180, 120, 0),
    cmap(95, 180, 180, 0),
    cmap(124, 191, 145, 63),
    cmap(94, 150, 100, 30),
    cmap(254, 60, 66, 66),
    cmap(255, 50, 50, 50),
};
}*/
void apply(){
    //for(int i =0;i<sizeof(colors)/3;i++)Fl::set_color(i,colors[i][0],colors[i][1],colors[i][2]);
    /**/
    /*Fl::set_boxtype(FL_UP_BOX, up_box, 2, 2, 4, 4);
    Fl::set_boxtype(FL_DIAMOND_UP_BOX, diamondup_box, 2, 2, 4, 4);

    Fl::set_boxtype(FL_DOWN_BOX, up_box, 2, 2, 4, 4);
    Fl::set_boxtype(FL_DIAMOND_DOWN_BOX, diamondup_box, 2, 2, 4, 4);

    Fl::set_boxtype(FL_ROUND_DOWN_BOX, diamondup_box, 2, 2, 4, 4);
    Fl::set_boxtype(FL_BORDER_BOX, diamondup_box, 2, 2, 4, 4);*/
}

}

}