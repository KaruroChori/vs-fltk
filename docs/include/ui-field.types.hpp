#pragma once

#include <ui.hpp>

namespace vs{
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
