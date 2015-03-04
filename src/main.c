#include <pebble.h>
  
static const char * const MINUTES[] = {
    "הקדו",
    "יתשו\nתוקד",
    "שולשו\nתוקד",
    "עבראו\nתוקד",
    "השימחו",
    "ששו\nתוקד",
    "עבשו\nתוקד",
    "הנומשו\nתוקד",
    "עשתו\nתוקד",
    "הרשעו",
    "תחאו\nהרשע\nתוקד",
    "םייתשו\nהרשע\nתוקד",
    "שולשו\nהרשע\nתוקד",
    "עבראו\nהרשע\nתוקד",
    "עברו",
    "ששו\nהרשע\nתוקד",
    "עבשו\nהרשע\nתוקד",
    "הנומשו\nהרשע\nתוקד",
    "עשתו\nהרשע\nתוקד",
    "םירשעו",
    "םירשע\nתחאו",
    "םירשע\nםייתשו",
    "םירשע\nשולשו",
    "םירשע\nעבראו",
    "םירשע\nשמחו",
    "םירשע\nששו",
    "םירשע\nעבשו",
    "םירשע\nהנומשו",
    "םירשע\nעשתו",
    "יצחו",
    "םישולש\nתחאו",
    "םישולש\nםייתשו",
    "םישולש\nשולשו",
    "םישולש\nעבראו",
    "םישולש\nשמחו",
    "םישולש\nששו",
    "םישולש\nעבשו",
    "םישולש\nהנומשו",
    "םישולש\nעשתו",
    "םירשע",
    "םיעברא\nתחאו",
    "םיעברא\nםייתשו",
    "םיעברא\nשולשו",
    "םיעברא\nעבראו",
    "עבר",
    "םיעברא\nששו",
    "םיעברא\nעבשו",
    "םיעברא\nהנומשו",
    "םיעברא\nעשתו",
    "הרשע",
    "םישימח\nתחאו",
    "םישימח\nםייתשו",
    "םישימח\nשולשו",
    "םישימח\nעבראו",
    "השימח",
    "עברא\nתוקד",
    "שולש\nתוקד",
    "יתש\nתוקד",
    "הקד"
};

static const char * const HOURS[] = {
    "םייתש\nהרשע",
    "תחא",
    "םייתש",
    "שולש",
    "עברא",
    "שמח",
    "שש",
    "עבש",
    "הנומש",
    "עשת",
    "רשע",
    "תחא\nהרשע"
};

static const char * const HOURS_WITH_TO[] = {
    "םייתשל\nהרשע",
    "תחאל",
    "םייתשל",
    "שולשל",
    "עבראל",
    "שמחל",
    "ששל",
    "עבשל",
    "הנומשל",
    "עשתל",
    "רשעל",
    "תחאל\nהרשע"
};  

static Window *s_main_window;
static TextLayer *s_time_layer;

static GFont s_time_font42;
static GFont s_time_font36;
static GFont s_time_font28;


void get_heb_desc_from_time(struct tm *t, char* timeText){

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
    
    return;

}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  //static char buffer[] = "00:00";
  static char buffer[100] = {0};
  strcpy(buffer, "");
  get_heb_desc_from_time(tick_time,buffer);
  
  int textLen = strlen(buffer);
    
  if (textLen < 20)
    text_layer_set_font(s_time_layer, s_time_font42);
  else if (textLen < 40)
    text_layer_set_font(s_time_layer, s_time_font36);
  else
    text_layer_set_font(s_time_layer, s_time_font28);


  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 0, 144, 168));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  // Make sure the time is displayed from the start   
  
  
  // Create GFont
  s_time_font42 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_KEY_ASHKENAZ_42));
  s_time_font36 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_KEY_ASHKENAZ_36));
  s_time_font28 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_KEY_ASHKENAZ_28));

  // Apply to TextLayer
  //text_layer_set_font(s_time_layer, s_time_font28);

  // Improve the layout to be more like a watchface
  //text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  update_time();
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  // Unload GFont
  fonts_unload_custom_font(s_time_font42);
  fonts_unload_custom_font(s_time_font36);
  fonts_unload_custom_font(s_time_font28);
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);

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

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

