#pragma once

#include <cstdint>
#include <ui.hpp>

namespace vs{

struct field_prefix_t{
  size_t tag;
  void (*free)(void* ptr);
  void *base[0];   //Just to offer a base with the right offset
};


struct field_model_t{
  int(*deserialize)(void* obj_dst, const char* src);  //Setup obj based on data from src
  int(*serialize)(const void* obj_src, const char** dst);   //Create a new string with the serialized information of obj inside
};

struct field_models_t{
    field_model_t models[];

    inline field_model_t operator[](int i ){if(i<__LAST && i>__FIRST)return models[i];};

    enum types : uint{
        __FIRST, FLAG, ENUM, RAW, PATH, CSTRING, STRING_VIEW, COLOR,
        ISCALAR_1, ISCALAR_2, ISCALAR_3, ISCALAR_4,
        FSCALAR_1, FSCALAR_2, FSCALAR_3, FSCALAR_4,
        __LAST
    };
};

struct field_t{
  field_models_t::types type : sizeof(field_models_t::types)*8-9;
  uint32_t need_cleanup: 1;
  uint32_t subtype: 8;

  union{
    bool              FLAG;
    size_t            ENUM;
    void*             RAW;
    const char *      CSTRING;
    std::string_view  STRING_VIEW;  //TODO: replace with isomorphic structure which can have ownership.
    uint8_t           COLOR[4];
    uint32_t          ISCALAR_1[1];
    uint32_t          ISCALAR_2[2];
    uint32_t          ISCALAR_3[3];
    uint32_t          ISCALAR_4[4];
    float             FSCALAR_1[1];
    float             FSCALAR_2[2];
    float             FSCALAR_3[3];
    float             FSCALAR_4[4];
  }storage;

  /**
   * @brief Construct a new field object
   * 
   * @param type 
   * @param need_cleanup 
   * @param subtype 
   */
  field_t(field_models_t::types type, bool need_cleanup = false, uint32_t subtype=0);

  /**
   * @brief Destroy the field object
   * If set for cleanup and of compatible type, raw, cstring or stringview content will be freed.
   */
  ~field_t();

};


namespace field_types{
  constexpr const char * fl_align_pos_s(Fl_Align v);
  Fl_Align fl_align_pos_i(const char* v);

  constexpr const char * fl_align_image_s(Fl_Align v);
  Fl_Align fl_align_image_i(const char* v);

  constexpr const char * fl_align_wrap_s(Fl_Align v);
  Fl_Align fl_align_wrap_i(const char* v);

  constexpr const char * fl_align_clip_s(Fl_Align v);
  Fl_Align fl_align_clip_i(const char* v);

  constexpr const char * fl_align_inside_s(Fl_Align v);
  Fl_Align fl_align_inside_i(const char* v);

  constexpr static const char *fl_font_s(Fl_Font v);
  Fl_Align fl_font_i(const char *v);

  constexpr static const char *fl_boxtype_s(Fl_Font v);
  Fl_Boxtype fl_boxtype_i(const char *v);

  constexpr static const char *fl_flex_layout_s(int v);
  int fl_flex_layout_i(const char *v);

  // Helpers to compute expressions into values that Fl_Widgets can use.

  bool h_px(unsigned int T, size_t *dst, const char *expr,
                   const ui_base *env);
  bool h_colour(uint32_t *dst, const char *expr,
                       const ui_base *env);

  bool h_flag(bool *dst, const char *expr,
                       const ui_base *env);
}
}
