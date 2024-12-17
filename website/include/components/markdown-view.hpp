#pragma once

#include "FL/Enumerations.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Help_View.H"

#include "FL/Fl_Flex.H"
#include "FL/Fl_Pack.H"
#include "FL/Fl_Scroll.H"
#include "FL/Fl_Text_Display.H"
#include "FL/Fl_Widget.H"
#include "FL/Fl_Window.H"
#include "ui.hpp"
#include <FL/Enumerations.H>
#include <FL/Fl_Box.H>


namespace vs{

class Fl_Markdown : public Fl_Window{
    /*void draw() override{
        Fl_Box box(0,0,100,100,"ciao");
    }*/


    public:
        template<typename... Args>
        Fl_Markdown(Args ...w):Fl_Window(w...){
            Fl_Scroll* scroll = new Fl_Scroll(0, 0, this->w(), this->h());
            scroll->color(FL_RED);

            Fl_Pack* flex = new Fl_Pack(0, 0, 280, 0);
            flex->color(FL_GREEN);

            Fl_Text_Display* text = new Fl_Text_Display(0,0,flex->w(),100);
            auto textbuf= new Fl_Text_Buffer();
            textbuf->text("Hello world crudel");
            text->buffer(textbuf);
            text->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);


            Fl_Button* box = new Fl_Button(0,0,0,0,"ciao mo\nndoao! oaooaooao ooaooao ooaooao ooaooao ooaooao ooaooao ");
            int ww, hh;
            box->labelcolor(FL_GREEN);
            box->align(FL_ALIGN_RIGHT|FL_ALIGN_TOP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
            box->measure_label(ww, hh);
            
            box->size(ww,hh);
            box->measure_label(ww, hh);
            box->size(ww,hh);

            Fl_Box* box2 = new Fl_Box(0,0,100,100,"mondo");
            for(int i =0;i<20;i++){
                Fl_Box* box3 = new Fl_Box(0,0,100,100,"mondo crudeleeeee");
                box3->align(FL_ALIGN_RIGHT);

            }
            flex->end();
            scroll->end();
            resizable(scroll);
        }


};


}