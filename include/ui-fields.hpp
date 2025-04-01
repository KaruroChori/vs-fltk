#pragma once

/**
 * @file ui-fields.hpp
 * @author karurochari
 * @brief 
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <cstdint>
#include <cstdlib>
#include <optional>
#include <ui.hpp>

namespace vs{

struct field_t;

struct field_enum_t{
  std::optional<int>(*deserialize)(const char* src) = nullptr;
  const char* (*serialize)(size_t src)= nullptr;
};

enum struct field_ret_t{
  UNKNOWN_MODEL=-4,
  INVALID_SRC=-3,
  WRONG_TYPE=-2,
  NULL_ACCESS=-1,
  OK=0,
  BAD=1
};

struct field_enums_t{
    enum types{
      __FIRST,
      ALIGN_POSITION, ALIGN_IMAGE, ALIGN_WRAP, ALIGN_CLIP, ALIGN_INSIDE, 
      FONT, 
      BOXTYPE, 
      FLEX_LAYOUT,
      __LAST
    };

    field_enum_t enums[__LAST];

    inline field_enum_t operator[](int i ){
      if(i<__LAST && i>__FIRST)return enums[i];
      /*Unrecognized field model*/
      throw "Not implemented";
    };
};

struct field_model_t{
  ///Setup field object based on data from string src
  field_ret_t(*deserialize)(field_t* obj_dst, const char* src, const ui_base* env) = nullptr;  
  //Create a new string with all serialized information from field obj_src
  field_ret_t(*serialize)(const field_t* obj_src, const char** dst, const ui_base* env) = nullptr;   
};

struct field_models_t{
    enum types{
      __FIRST, 
      FLAG, 
      ENUM, 
      RAW, 
      //PATH, //TODO: this was defined but never later used. I need to check in specs if this had any meaning or if it was just a mistake left in there
      CSTRING, 
      STRING_VIEW, 
      COLOR,
      ISCALAR_1, ISCALAR_2, ISCALAR_3, ISCALAR_4,
      FSCALAR_1, FSCALAR_2, FSCALAR_3, FSCALAR_4,
      __LAST
    };

    field_model_t models[__LAST];

    field_ret_t deserialize(types model, field_t* _dst, const char* src, const ui_base* env);
    field_ret_t serialize(types model, const field_t* _src, const char**  dst, const ui_base* env);
};

struct field_t{
  //Allocators for field_t so that they can be overridden if so desired.
  static inline constexpr void(*lfree)(void*)=free;
  static inline constexpr void*(*lalloc)(size_t)=malloc;

  field_models_t::types type : sizeof(field_models_t::types)*8-11;
  ///If true the content of this field is still considered valid, else accessing it is not right.
  uint32_t valid: 1;
  ///If true, the embedded script must handle the cleanup of this field when out of scope. If false ownership is externally handled by the engine.
  uint32_t need_cleanup: 1;
  ///Not implemented for not. Later, if set for raw or cstring it will allow elements up to 16 bytes to be stored directly in `storage`
  uint32_t is_inline: 1;

  //Some types (ENUM & RAW) allows for subtype information to be added.
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
   * @brief Destroy the field object
   * If set for cleanup and of compatible type, raw, cstring or stringview content will be freed.
   */
  ~field_t();

  /**
   * @brief Reset content of the field. To be used for deletions or before setting.
   * 
   */
  void reset();

  /**
   * @brief From a weak to a strong field. Copy data from parent.
   * 
   */
  void copy();

  field_ret_t from_string(const char* str, const ui_base *env=nullptr);
  field_ret_t to_string(const char** str, const ui_base *env=nullptr);

  //void from_string(std::string_view str);
  //void to_string(std::string_view& str);
};

/**
 * @brief Legacy namespace to be replaced by definitions in field_models/field_enums
 * 
 */
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
