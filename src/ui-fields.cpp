#include <cassert>
#include <ui-fields.hpp>

namespace vs{

  field_t::field_t(field_models_t::types type, bool weak, uint32_t subtype){
    if(weak==true && (type==field_models_t::CSTRING || type==field_models_t::RAW ||type==field_models_t::STRING_VIEW) )need_cleanup=true;
    this->subtype=subtype;
    this->type=type;
    memset(&storage,0,sizeof(storage)); //Maybe I should do something different for floats?
  }

  field_t::~field_t(){
    if(need_cleanup){
      assert( type==field_models_t::CSTRING || type==field_models_t::RAW ||type==field_models_t::STRING_VIEW);
      if(this->storage.RAW!=nullptr)lfree(this->storage.RAW);
      need_cleanup=false;
      this->storage.RAW = nullptr;
    }
    valid=false;
  }

  int field_t::store_from_field(const field_t *src, bool weak){
    //TODO:
    return 1;
  }

  int field_t::store_from_data(storage_t data, bool weak){
    //TODO:
    return 1;
  }


namespace field_types{

  constexpr const char * fl_align_pos_s(Fl_Align v){
    if(v==FL_ALIGN_CENTER){return "center";}
    if(v==FL_ALIGN_TOP){return "top";}
    if(v==FL_ALIGN_BOTTOM){return "bottom";}
    if(v==FL_ALIGN_LEFT){return "left";}
    if(v==FL_ALIGN_RIGHT){return "right";}
    if(v==FL_ALIGN_TOP_LEFT){return "top-left";}
    if(v==FL_ALIGN_TOP_RIGHT){return "top-right";}
    if(v==FL_ALIGN_BOTTOM_LEFT){return "bottom-left";}
    if(v==FL_ALIGN_BOTTOM_RIGHT){return "bottom-right";}
    if(v==FL_ALIGN_LEFT_TOP){return "left-top";}
    if(v==FL_ALIGN_RIGHT_TOP){return "right-top";}
    if(v==FL_ALIGN_LEFT_BOTTOM){return "left-bottom";}
    if(v==FL_ALIGN_RIGHT_BOTTOM){return "right-bottom";}
    return nullptr;
  }

  Fl_Align fl_align_pos_i(const char* v){
    if(strcmp(v,"center")==0){return FL_ALIGN_CENTER;}
    if(strcmp(v,"top")==0){return FL_ALIGN_TOP;}
    if(strcmp(v,"bottom")==0){return FL_ALIGN_BOTTOM;}
    if(strcmp(v,"left")==0){return FL_ALIGN_LEFT;}
    if(strcmp(v,"right")==0){return FL_ALIGN_RIGHT;}
    if(strcmp(v,"top-left")==0){return FL_ALIGN_TOP_LEFT;}
    if(strcmp(v,"top-right")==0){return FL_ALIGN_TOP_RIGHT;}
    if(strcmp(v,"bottom-left")==0){return FL_ALIGN_BOTTOM_LEFT;}
    if(strcmp(v,"bottom-right")==0){return FL_ALIGN_BOTTOM_RIGHT;}
    if(strcmp(v,"left-top")==0){return FL_ALIGN_LEFT_TOP;}
    if(strcmp(v,"right-top")==0){return FL_ALIGN_RIGHT_TOP;}
    if(strcmp(v,"left-bottom")==0){return FL_ALIGN_LEFT_BOTTOM;}
    if(strcmp(v,"right-bottom")==0){return FL_ALIGN_RIGHT_BOTTOM;}
    return -1;
  }

  constexpr const char * fl_align_image_s(Fl_Align v){
    if(v==FL_ALIGN_TEXT_OVER_IMAGE){return "txt-over-img";}
    if(v==FL_ALIGN_IMAGE_OVER_TEXT){return "img-over-txt";}
    if(v==FL_ALIGN_IMAGE_NEXT_TO_TEXT){return "img-next-txt";}
    if(v==FL_ALIGN_TEXT_NEXT_TO_IMAGE){return "txt-next-img";}
    if(v==FL_ALIGN_IMAGE_BACKDROP){return "img-back";}
    return nullptr;
  }
  Fl_Align fl_align_image_i(const char* v){
    if(strcmp(v,"txt-over-img")==0){return FL_ALIGN_TEXT_OVER_IMAGE;}
    if(strcmp(v,"img-over-txt")==0){return FL_ALIGN_IMAGE_OVER_TEXT;}
    if(strcmp(v,"img-next-txt")==0){return FL_ALIGN_IMAGE_NEXT_TO_TEXT;}
    if(strcmp(v,"txt-next-img")==0){return FL_ALIGN_TEXT_NEXT_TO_IMAGE;}
    if(strcmp(v,"img-back")==0){return FL_ALIGN_IMAGE_BACKDROP;}
    return -1;
  }


  constexpr const char * fl_align_clip_s(Fl_Align v){
    if(v==FL_ALIGN_CLIP){return "clip";}
    if(v==0){return "no-clip";}
    return nullptr;
  }

  Fl_Align fl_align_clip_i(const char* v){
    if(strcmp(v,"clip")==0){return FL_ALIGN_CLIP;}
    if(strcmp(v,"no-clip")==0){return 0;}
    if(strcmp(v,"true")==0){return FL_ALIGN_CLIP;}
    if(strcmp(v,"false")==0){return 0;}
    return -1;
  }


  constexpr const char * fl_align_wrap_s(Fl_Align v){
    if(v==FL_ALIGN_CLIP){return "clip";}
    if(v==FL_ALIGN_WRAP){return "wrap";}
    if(v==FL_ALIGN_NOWRAP){return "no-wrap";}
    return nullptr;
  }

  Fl_Align fl_align_wrap_i(const char* v){
    if(strcmp(v,"clip")==0){return FL_ALIGN_CLIP;}
    if(strcmp(v,"wrap")==0){return FL_ALIGN_WRAP;}
    if(strcmp(v,"no-wrap")==0){return FL_ALIGN_NOWRAP;}
    if(strcmp(v,"true")==0){return FL_ALIGN_WRAP;}
    if(strcmp(v,"false")==0){return FL_ALIGN_NOWRAP;}
    return -1;
  }

  constexpr const char * fl_align_inside_s(Fl_Align v){
    if(v==FL_ALIGN_INSIDE){return "inside";}
    if(v==0){return "outside";}
    return nullptr;
  }

  Fl_Align fl_align_inside_i(const char* v){
    if(strcmp(v,"inside")==0){return FL_ALIGN_INSIDE;}
    if(strcmp(v,"outside")==0){return 0;}
    if(strcmp(v,"true")==0){return FL_ALIGN_INSIDE;}
    if(strcmp(v,"false")==0){return 0;}
    return -1;
  }

  constexpr const char* fl_font_s(Fl_Font v){
    if(v==FL_HELVETICA){return "helvetica";}
    if(v==FL_COURIER){return "courier";}
    if(v==FL_TIMES){return "times";}
    if(v==FL_SYMBOL){return "symbol";}
    if(v==FL_SCREEN){return "mono";}
    if(v==FL_SCREEN_BOLD){return "mono-bold";}
    if(v==FL_ZAPF_DINGBATS){return "zapf-dingbats";}
    if(v==FL_FREE_FONT){return "free";}
    //Only for helvetica, courier and times
    if(v==FL_BOLD){return "bold";}
    if(v==FL_ITALIC){return "italic";}
    return nullptr;
  }

  Fl_Align fl_font_i(const char* v){
    if(strcmp(v,"helvetica")==0){return FL_HELVETICA;}
    if(strcmp(v,"courier")==0){return FL_COURIER;}
    if(strcmp(v,"times")==0){return FL_TIMES;}
    if(strcmp(v,"symbol")==0){return FL_SYMBOL;}
    if(strcmp(v,"mono")==0){return FL_SCREEN;}
    if(strcmp(v,"mono-bold")==0){return FL_SCREEN_BOLD;}
    if(strcmp(v,"zapf-dingbats")==0){return FL_ZAPF_DINGBATS;}
    if(strcmp(v,"free")==0){return FL_FREE_FONT;}

    if(strcmp(v,"bold")==0){return FL_BOLD;}
    if(strcmp(v,"italic")==0){return FL_ITALIC;}
    return -1;
  }



  //TODO:
  constexpr const char* fl_boxtype_s(Fl_Font v){return nullptr;}
  Fl_Boxtype fl_boxtype_i(const char* v){return FL_NO_BOX;}

  constexpr const char* fl_flex_layout_s(int v){
    if(v==0)return "vertical";
    else if(v==1)return "horizontal";
    return nullptr;
  }
  int fl_flex_layout_i(const char* v){
    if(strcmp(v,"vertical")==0){return 0;}
    if(strcmp(v,"horizontal")==0){return 1;}
    return -1;
  }


  //TODO: I need to check if number conversions failed or not too.
  bool h_px(unsigned int T, size_t* dst, const char* expr, const ui_base* env){
    if (T==1){
      dst[0]=std::stoi(expr);
      return true;
    }

    auto box = split(expr,',');
    if(box.size()!=T)return false;

    int c=0;
    for(const auto& i : box) dst[c++]=std::stoi(i);
    return true;
  }

  bool h_colour(uint32_t* dst, const char* expr,  const ui_base* env){
    if(expr[0]=='#'){
      dst[0] = std::stoi(expr+1,nullptr,16)<<8;
    }
    else{
      //TODO: named colours not supported yet. But indexed colors via index are.
      dst[0] = std::stoi(expr) & 0xff;
    }
    return true;
  }

  bool h_flag(bool* dst, const char* expr,  const ui_base* env){
    if(strcmp(expr, "false")==0){dst[0]=false;}
    else if(strcmp(expr,"true")==0){dst[0]=true;}
    return false;
  }

}

}
