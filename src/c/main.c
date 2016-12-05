#include <pebble.h>
#include "pebble-rtltr/rtltr.h"

//#define CLIPPING 1
//#define CLIPPING PBL_IF_ROUND_ELSE(1, 0)

//#define GTEXT_OVERFLOW_MODE GTextOverflowModeFill
//#define GTEXT_OVERFLOW_MODE GTextOverflowModeTrailingEllipsis
#define GTEXT_OVERFLOW_MODE GTextOverflowModeWordWrap

#define PERSIST_KEY_FONT 10

#define FOREACH_FONT(FONT) \
        FONT(SIMPLE) \
        FONT(HANDWRITING) \
        FONT(TORA_ASHKENAZ) \
        FONT(MACCABI) \
        FONT(RASHI) \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum font {
  FOREACH_FONT(GENERATE_ENUM)
  UNKNOWN
};

const char * const FONT_ID[] = {
  FOREACH_FONT(GENERATE_STRING)
  NULL
};

static enum font str2font(const char * const str) {
  enum font f;
  for (f = 0; FONT_ID[f] != NULL && strcmp(FONT_ID[f], str); f++) {
  }
  return f;
}

#define VERTICAL_OFFSET (-6)
static char
  m01[] = "הקדו",
  m02[] = "יתשו\nתוקד",
  m03[] = "שולשו\nתוקד",
  m04[] = "עבראו\nתוקד",
  m05[] = "השימחו",
  m06[] = "ששו\nתוקד",
  m07[] = "עבשו\nתוקד",
  m08[] = "הנומשו\nתוקד",
  m09[] = "עשתו\nתוקד",
  m10[] = "הרשעו",
  m11[] = "תחאו\nהרשע\nתוקד",
  m12[] = "םייתשו\nהרשע\nתוקד",
  m13[] = "שולשו\nהרשע\nתוקד",
  m14[] = "עבראו\nהרשע\nתוקד",
  m15[] = "עברו",
  m16[] = "ששו\nהרשע\nתוקד",
  m17[] = "עבשו\nהרשע\nתוקד",
  m18[] = "הנומשו\nהרשע\nתוקד",
  m19[] = "עשתו\nהרשע\nתוקד",
  m20[] = "םירשעו",
  m21[] = "םירשע\nתחאו",
  m22[] = "םירשע\nםייתשו",
  m23[] = "םירשע\nשולשו",
  m24[] = "םירשע\nעבראו",
  m25[] = "םירשע\nשמחו",
  m26[] = "םירשע\nששו",
  m27[] = "םירשע\nעבשו",
  m28[] = "םירשע\nהנומשו",
  m29[] = "םירשע\nעשתו",
  m30[] = "יצחו",
  m31[] = "םישולש\nתחאו",
  m32[] = "םישולש\nםייתשו",
  m33[] = "םישולש\nשולשו",
  m34[] = "םישולש\nעבראו",
  m35[] = "םישולש\nשמחו",
  m36[] = "םישולש\nששו",
  m37[] = "םישולש\nעבשו",
  m38[] = "םישולש\nהנומשו",
  m39[] = "םישולש\nעשתו",
  m40[] = "םירשע",
  m41[] = "םיעברא\nתחאו",
  m42[] = "םיעברא\nםייתשו",
  m43[] = "םיעברא\nשולשו",
  m44[] = "םיעברא\nעבראו",
  m45[] = "עבר",
  m46[] = "םיעברא\nששו",
  m47[] = "םיעברא\nעבשו",
  m48[] = "םיעברא\nהנומשו",
  m49[] = "םיעברא\nעשתו",
  m50[] = "הרשע",
  m51[] = "םישימח\nתחאו",
  m52[] = "םישימח\nםייתשו",
  m53[] = "םישימח\nשולשו",
  m54[] = "םישימח\nעבראו",
  m55[] = "השימח",
  m56[] = "עברא\nתוקד",
  m57[] = "שולש\nתוקד",
  m58[] = "יתש\nתוקד",
  m59[] = "הקד"
  ;
/*
[אבדהוחילםמנעצקרשת]
  גז
  ט
  ךכ
  ן
  ס
  ףפ
  ץ
*/
#define MAX_MINUTES 59
static char * MINUTES[MAX_MINUTES] = {
      m01,m02,m03,m04,m05,m06,m07,m08,m09,
  m10,m11,m12,m13,m14,m15,m16,m17,m18,m19,
  m20,m21,m22,m23,m24,m25,m26,m27,m28,m29,
  m30,m31,m32,m33,m34,m35,m36,m37,m38,m39,
  m40,m41,m42,m43,m44,m45,m46,m47,m48,m49,
  m50,m51,m52,m53,m54,m55,m56,m57,m58,m59
};

static char
  h12[] = "םייתש\nהרשע",
  h01[] = "תחא",
  h02[] = "םייתש",
  h03[] = "שולש",
  h04[] = "עברא",
  h05[] = "שמח",
  h06[] = "שש",
  h07[] = "עבש",
  h08[] = "הנומש",
  h09[] = "עשת",
  h10[] = "רשע",
  h11[] = "תחא\nהרשע"
  ;
#define MAX_HOURS 12
static char * HOURS[MAX_HOURS] = {
  h12,h01,h02,h03,h04,h05,h06,h07,h08,h09,h10,h11
};

static char
  h12_[] = "םייתשל\nהרשע",
  h01_[] = "תחאל",
  h02_[] = "םייתשל",
  h03_[] = "שולשל",
  h04_[] = "עבראל",
  h05_[] = "שמחל",
  h06_[] = "ששל",
  h07_[] = "עבשל",
  h08_[] = "הנומשל",
  h09_[] = "עשתל",
  h10_[] = "רשעל",
  h11_[] = "תחאל\nהרשע"
  ;
#define MAX_HOURS_WITH_TO 12
static char * HOURS_WITH_TO[MAX_HOURS_WITH_TO] = {
  h12_,h01_,h02_,h03_,h04_,h05_,h06_,h07_,h08_,h09_,h10_,h11_
};  

typedef struct Font {
  char* name;
  uint32_t size_small;
  uint32_t size_medium;
  uint32_t size_large;
#if defined(CLIPPING) && CLIPPING == 1
  int clip_right;
#endif
} Font;

#define MAX_FONTS 5
static const struct Font fonts[MAX_FONTS] = {
  { // Simple CLM Bold
    "Simple"
#ifndef PBL_PLATFORM_EMERY
    ,RESOURCE_ID_FONT_KEY_SIMPLE_28
#endif
    ,RESOURCE_ID_FONT_KEY_SIMPLE_36
    ,RESOURCE_ID_FONT_KEY_SIMPLE_42
#ifdef PBL_PLATFORM_EMERY
    ,RESOURCE_ID_FONT_KEY_SIMPLE_48
#endif
#if defined(CLIPPING) && CLIPPING == 1
    ,0
#endif
  },
  { // Handwriting
    "Handwriting"
#ifndef PBL_PLATFORM_EMERY
    ,RESOURCE_ID_FONT_KEY_KTAV_28
#endif
    ,RESOURCE_ID_FONT_KEY_KTAV_36
    ,RESOURCE_ID_FONT_KEY_KTAV_42
#ifdef PBL_PLATFORM_EMERY
    ,RESOURCE_ID_FONT_KEY_KTAV_48
#endif
#if defined(CLIPPING) && CLIPPING == 1
    ,23
#endif
  },
  { // Tora Ashkenaz
    "Tora"
#ifndef PBL_PLATFORM_EMERY
    ,RESOURCE_ID_FONT_KEY_ASHKENAZ_28
#endif
    ,RESOURCE_ID_FONT_KEY_ASHKENAZ_36
    ,RESOURCE_ID_FONT_KEY_ASHKENAZ_42
#ifdef PBL_PLATFORM_EMERY
    ,RESOURCE_ID_FONT_KEY_ASHKENAZ_48
#endif
#if defined(CLIPPING) && CLIPPING == 1
    ,0
#endif
  },
  { // Maccabi (modern)
    "Maccabi"
#ifndef PBL_PLATFORM_EMERY
    ,RESOURCE_ID_FONT_KEY_MACABI_28
#endif
    ,RESOURCE_ID_FONT_KEY_MACABI_36
    ,RESOURCE_ID_FONT_KEY_MACABI_42
#ifdef PBL_PLATFORM_EMERY
    ,RESOURCE_ID_FONT_KEY_MACABI_48
#endif
#if defined(CLIPPING) && CLIPPING == 1
    ,0
#endif
  },
  { // Rashi
    "Rashi"
#ifndef PBL_PLATFORM_EMERY
    ,RESOURCE_ID_FONT_KEY_RASHI_28
#endif
    ,RESOURCE_ID_FONT_KEY_RASHI_36
    ,RESOURCE_ID_FONT_KEY_RASHI_42
#ifdef PBL_PLATFORM_EMERY
    ,RESOURCE_ID_FONT_KEY_RASHI_48
#endif
#if defined(CLIPPING) && CLIPPING == 1
    ,0
#endif
  }
};

static enum font font = HANDWRITING;

static GFont s_time_font_large = NULL;
static GFont s_time_font_medium = NULL;
static GFont s_time_font_small = NULL;

static Window *s_main_window;
static GRect window_bounds, no_bounds;
static TextLayer *s_time_text_layer;
static Layer *s_time_layer;

void get_heb_desc_from_time(struct tm *t, char* timeText) {
    int hour = t->tm_hour;
    int minutes = t->tm_min;
    int next_hour = hour + 1;
    
    if (hour > 11) {
        hour -= 12;
    }
    
    if (hour == 11){
        next_hour = 0;
    } else {
        next_hour = hour + 1;
    }

    if (minutes == 0){
        strcpy(timeText, HOURS[hour]);
        return;
    }

    if (minutes == 40 || minutes == 45 || minutes == 50 || minutes >= 55){
        strcat(timeText, MINUTES[minutes - 1]);
        strcat(timeText, "\n");
        strcat(timeText, HOURS_WITH_TO[next_hour]);
        return;
    }

    strcat(timeText, HOURS[hour]);
    strcat(timeText, "\n");
    strcat(timeText, MINUTES[minutes - 1]);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[100] = {0}; // 53 should be more than enough
  strcpy(buffer, "");
  get_heb_desc_from_time(tick_time, buffer);
//   strcpy(buffer, "שש");
//   strcpy(buffer, "עבר\nםייתשל");
//   strcpy(buffer, "עבר\nהנומשל");
//   strcpy(buffer, "רשע\nםירשעו");
//   strcpy(buffer, "תחא\nהרשע\nםישולש\nהנומשו");
//   strcpy(buffer, "םירשע\nםייתשל\nהרשע");
//   strcpy(buffer, "תחא\nהרשע\nםיעברא\nשולשו");
//   strcpy(buffer, "םייתש\nהרשע\nעשתו\nהרשע\nתוקד");

  GFont font = s_time_font_large;
  GSize content_size = graphics_text_layout_get_content_size(buffer, font, no_bounds, GTEXT_OVERFLOW_MODE, GTextAlignmentCenter);
//  APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "update_time42 window:%d,%d content:%d,%d, frame.y: %d", window_bounds.size.w, window_bounds.size.h, content_size.w,content_size.h, window_bounds.origin.y + (window_bounds.size.h - content_size.h) / 2 + VERTICAL_OFFSET);
  if (content_size.w >= window_bounds.size.w || content_size.h >= window_bounds.size.h) {
    font = s_time_font_medium;
    content_size = graphics_text_layout_get_content_size(buffer, font, no_bounds, GTEXT_OVERFLOW_MODE, GTextAlignmentCenter);
//  APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "update_time36 window:%d,%d content:%d,%d, frame.y: %d", window_bounds.size.w, window_bounds.size.h, content_size.w,content_size.h, window_bounds.origin.y + (window_bounds.size.h - content_size.h) / 2 + VERTICAL_OFFSET);
    if (content_size.w >= window_bounds.size.w || content_size.h >= window_bounds.size.h) {
      font = s_time_font_small;
      content_size = graphics_text_layout_get_content_size(buffer, font, no_bounds, GTEXT_OVERFLOW_MODE, GTextAlignmentCenter);
//  APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "update_time28 window:%d,%d content:%d,%d, frame.y: %d", window_bounds.size.w, window_bounds.size.h, content_size.w,content_size.h, window_bounds.origin.y + (window_bounds.size.h - content_size.h) / 2 + VERTICAL_OFFSET);
    }
  }
  text_layer_set_font(s_time_text_layer, font);

  // Vertical alignment
//   GSize content_size = text_layer_get_content_size(s_time_text_layer);
  APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "update_time str");
  int16_t y = window_bounds.origin.y + (window_bounds.size.h - content_size.h) / 2 + VERTICAL_OFFSET;
  char *fs = (font == s_time_font_large ? "large" : (font == s_time_font_medium ? "medium" : "small"));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "update_time window:%d,%d content:%d,%d, frame.y:%d, len:%d, font_size:%s",
    window_bounds.size.w, window_bounds.size.h, content_size.w,content_size.h, y, strlen(buffer), fs);
  APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "update_time str:%s", buffer);
  layer_set_frame(s_time_layer, GRect(window_bounds.origin.x, y, window_bounds.size.w, window_bounds.size.h - y));

  // Display this time on the TextLayer
  text_layer_set_text(s_time_text_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void unload_fonts() {
  // Unload GFont
  if (s_time_font_large != NULL) {
    fonts_unload_custom_font(s_time_font_large);
    s_time_font_large = NULL;
  }
  if (s_time_font_medium != NULL) {
    fonts_unload_custom_font(s_time_font_medium);
    s_time_font_medium = NULL;
  }
  if (s_time_font_small != NULL) {
    fonts_unload_custom_font(s_time_font_small);
    s_time_font_small = NULL;
  }
}

static void load_fonts() {
  GFont fallback_font = NULL;
  unload_fonts();

  // Create GFont
  if (fonts[font].size_small) {
    fallback_font = s_time_font_small = fonts_load_custom_font(resource_get_handle(fonts[font].size_small));
  }
  if (fonts[font].size_medium) {
    fallback_font = s_time_font_medium = fonts_load_custom_font(resource_get_handle(fonts[font].size_medium));
  } else {
    s_time_font_medium = fallback_font;
  }
  if (fonts[font].size_large) {
    s_time_font_large = fonts_load_custom_font(resource_get_handle(fonts[font].size_large));
  } else {
    s_time_font_large = fallback_font;
  }
}

#if defined(CLIPPING) && CLIPPING == 1
static void bg_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
}
#endif

static void main_window_load(Window *window) {
  window_set_background_color(window, GColorBlack);
  Layer *window_layer = window_get_root_layer(window);
  window_bounds = layer_get_bounds(window_layer);
  no_bounds = GRect(0, 0, window_bounds.size.w * 2, window_bounds.size.h * 2);

  // Create time TextLayer
  s_time_text_layer = text_layer_create(PBL_IF_ROUND_ELSE(
    GRect(window_bounds.origin.x, window_bounds.origin.y + 6, window_bounds.size.w, window_bounds.size.h),
    window_bounds
  ));
  s_time_layer = text_layer_get_layer(s_time_text_layer);

#if defined(CLIPPING) && CLIPPING == 1
  if (fonts[font].clip_right != 0) {
    Layer *s_simple_bg_layer = layer_create(window_bounds);
    layer_set_update_proc(s_simple_bg_layer, bg_update_proc);
    layer_add_child(window_layer, s_simple_bg_layer);

    layer_set_frame(s_time_layer, GRect(window_bounds.origin.x, window_bounds.origin.y,
      window_bounds.size.w - fonts[font].clip_right, window_bounds.size.h));
//      layer_set_bounds(s_time_layer, GRect(window_bounds.origin.x, window_bounds.origin.y,
//        window_bounds.size.w - fonts[font_index].clip_right, window_bounds.size.h));
    layer_set_clips(s_time_layer, false);
  }
#endif
  text_layer_set_text_alignment(s_time_text_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(s_time_text_layer, GTEXT_OVERFLOW_MODE);
  text_layer_set_background_color(s_time_text_layer, GColorBlack);
  text_layer_set_text_color(s_time_text_layer, GColorWhite);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, s_time_layer);
  
  load_fonts();
  update_time();
}

static void main_window_unload(Window *window) {
  unload_fonts();
  // Destroy TextLayer
  text_layer_destroy(s_time_text_layer);
}

static void inbox_received_handler(DictionaryIterator *iterator, void *context) {
  // Read tuples for data
  Tuple* font_tup = dict_find(iterator, MESSAGE_KEY_FONT);
  if (font_tup) {
    char *font_str = font_tup->value->cstring;
    enum font new_font = str2font(font_str);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "inbox_received_handler: font_str:%s, new_font:%d, font:%d", font_str, new_font, font);
    if (font != new_font) {
      font = new_font;
      persist_write_int(PERSIST_KEY_FONT, font);
      APP_LOG(APP_LOG_LEVEL_DEBUG, "inbox_received_handler: persist_write_int(%d, %d) font:%s", PERSIST_KEY_FONT, font, FONT_ID[font]);
      load_fonts();
      update_time();
    }
  }

  rtltr_inbox_received_handler(iterator, context);
}

static void init_rtltr(void) {
  rtltr_strings_are_visual_encoded();
  rtltr_ensure_registered_string_arrays_capacity(3);
  rtltr_register_string_array(MINUTES, MAX_MINUTES);
  rtltr_register_string_array(HOURS, MAX_HOURS);
  rtltr_register_string_array(HOURS_WITH_TO, MAX_HOURS_WITH_TO);
  rtltr_register_callback_after_reverse_registered_strings(update_time);
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(128, 128);
}

static void init() {
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  if (persist_exists(PERSIST_KEY_FONT)) {
    font = persist_read_int(PERSIST_KEY_FONT);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "init: persist_read_int: font:%d(%s)", font, FONT_ID[font]);
  }

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  init_rtltr();
  rtltr_load_settings();
  rtltr_init();
}

static void deinit() {
  rtltr_free();
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
