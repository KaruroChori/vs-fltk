#pragma once

#include <cstdint>
#include <cstdlib>
#include <optional>
#include <ui.hpp>

namespace vs{

struct field_prefix_t{
  size_t tag;
  void (*free)(void* ptr);
  void *base[0];   //Just to offer a base with the right offset
};

struct field_t;

struct field_enum_t{
  std::optional<int>(*deserialize)(const char* src) = nullptr;
  const char* (*serialize)(size_t src)= nullptr;
};

enum struct field_ret_t{
  WRONG_TYPE=-2,
  NULL_SRC=-1,
  OK=0,
  BAD=0
};

struct field_enums_t{
    field_enum_t enums[];

    inline field_enum_t operator[](int i ){
      if(i<__LAST && i>__FIRST)return enums[i];
      /*Unrecognized field model*/
      exit(1);
    };

    enum types{
        __FIRST,
        ALIGN_POSITION, ALIGN_IMAGE, ALIGN_WRAP, ALIGN_CLIP, ALIGN_INSIDE, 
        FONT, 
        BOXTYPE, 
        FLEX_LAYOUT,
        __LAST
    };
};

struct field_model_t{
  ///Setup field object based on data from string src
  field_ret_t(*deserialize)(field_t* obj_dst, const char* src, const ui_base* env) = nullptr;  
  //Create a new string with all serialized information from field obj_src
  field_ret_t(*serialize)(const field_t* obj_src, const char** dst, const ui_base* env) = nullptr;   
};

struct field_models_t{
    field_model_t models[];

    inline field_model_t operator[](int i ){
      if(i<__LAST && i>__FIRST)return models[i];
      /*Unrecognized field model*/
      exit(1);
    };

    enum types{
        __FIRST, FLAG, ENUM, RAW, PATH, CSTRING, STRING_VIEW, COLOR,
        ISCALAR_1, ISCALAR_2, ISCALAR_3, ISCALAR_4,
        FSCALAR_1, FSCALAR_2, FSCALAR_3, FSCALAR_4,
        __LAST
    };
};

struct field_t{
  //Allocators for field_t so that they can be overridden if so desired.
  static inline constexpr void(*lfree)(void*)=free;
  static inline constexpr void*(*lalloc)(size_t)=malloc;

  field_models_t::types type : sizeof(field_models_t::types)*8-10;
  uint32_t valid: 1;
  uint32_t need_cleanup: 1;
  uint32_t subtype: 8;

  union storage_t{
    bool              FLAG;
    size_t            ENUM;
    void*             RAW;
    const char *      CSTRING;
    struct{
      const char* ptr;
      size_t size;
    }                 STRING_VIEW;
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
  field_t(field_models_t::types type, bool weak = false, uint32_t subtype=0);

  /**
   * @brief 
   * 
   * @param src the source field to use to copy data from
   * @param weak if data should be copied weakly.
   * @return int 0 if all fine, else error codes
   */
  int store_from_field(const field_t *src, bool weak = false);

  /**
   * @brief 
   * 
   * @param src the source field to use to copy data from
   * @param weak if data should be copied weakly.
   * @return int 0 if all fine, else error codes
   */
  int store_from_data(storage_t data, bool weak = false);

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

field_models_t extern field_models;
field_enums_t extern field_enums;

}
