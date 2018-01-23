#include "counter.h"

void
_config_load(void *data)
{
   int found = 0;
	Eina_List *l;
	Config_Item *list_data;

   printf("LIST COUNT %i\n", eina_list_count(configlist));

   EINA_LIST_FOREACH(configlist, l, list_data)
   {
	   if(list_data->id == id_num)
		{
			ci_name = eina_stringshare_add(list_data->name);
			ci_unit = eina_stringshare_add(list_data->unit);
			ci_value = list_data->value;
			ci_factor = list_data->factor;
			ci_r = list_data->r;
			ci_g = list_data->g;
			ci_b = list_data->b;
			ci_a = list_data->a;
			found = 1;
		}
   }

   if(found == 0)
	{
		ci_name = eina_stringshare_add("NAME");
		ci_unit = eina_stringshare_add("UNIT");
		ci_value = 0;
		ci_factor= 1;
		ci_r = 11;
		ci_g = 54;
		ci_b = 71;
		ci_a = 255;
	}
	
	printf("LOAD FOUND: %i\n", found);
}


void
_config_save(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
	Eina_List *l;
	Config_Item *list_data;
	Config_Item *list_data1;
	list_data1 = calloc(1, sizeof(Config_Item));
	int found = 0;
	
	printf("SAVE\n");
	
	Evas_Object *mainbox = data;
   Evas_Object *ly = evas_object_data_get(mainbox, "ly");
	
	
	if(data != NULL)
	{
   Evas_Object *en_name = evas_object_data_get(mainbox, "en_name");
   Evas_Object *en_unit = evas_object_data_get(mainbox, "en_unit");
   Evas_Object *en_value = evas_object_data_get(mainbox, "en_value");
   Evas_Object *en_factor = evas_object_data_get(mainbox, "en_factor");
	ci_name = elm_object_text_get(en_name);
	ci_unit = elm_object_text_get(en_unit);
	ci_value = atof(elm_object_text_get(en_value));
	ci_factor = atof(elm_object_text_get(en_factor));
	}
	
   EINA_LIST_FOREACH(configlist, l, list_data)
   {
	   if(list_data->id == id_num)
		{
			list_data->name = ci_name;
			list_data->unit = ci_unit;
// 			list_data->value = atof(elm_object_text_get(en_value));
			list_data->value = ci_value;
			list_data->factor = ci_factor;
			list_data->r = ci_r;
			list_data->g = ci_g;
			list_data->b = ci_b;
			list_data->a = ci_a;
			found = 1;
			}
   }
		  
	if(found == 0)
	{
		list_data1->id = id_num;
		list_data1->name = ci_name;
		list_data1->unit = ci_unit;
		list_data1->value = ci_value;
		list_data1->factor = ci_factor;
		list_data1->r = ci_r;
		list_data1->g = ci_g;
		list_data1->b = ci_b;
		list_data1->a = ci_a;

		configlist = eina_list_append(configlist, list_data1);
	}

//    ci_name = elm_object_text_get(hoversel);

	printf("SAVE FOUND: %i\n", found);
_save_eet();
_set_content(ly, NULL, NULL, NULL);
}


unsigned int createHEX(int r, int g, int b, int a)
{
	return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + (a & 0xff);
}


static void
_colorselector_changed_cb(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{	
	Evas_Object *mainbox = data;
   Evas_Object *ic = evas_object_data_get(mainbox, "ic");
   Evas_Object *ly = evas_object_data_get(mainbox, "ly");

   elm_colorselector_color_get(obj, &ci_r, &ci_g, &ci_b, &ci_a);
	
	evas_object_color_set(ic, ci_r, ci_g, ci_b, ci_a);
	
	set_color(ly);
}


void
_settings_1(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{
	Evas_Object *win = obj;
	Evas_Object *ly = data;
	printf("SETTINGS1\n");
	_settings(win, ly, NULL);
}


void
_settings_2(void *data, Evas_Object *obj, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
	Evas_Object *win = data;
	Evas_Object *ly = obj;
	
	printf("SETTINGS2\n");
	_settings(win, ly, NULL);
}



void
_settings(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{	
	Evas_Object *en_name, *en_unit, *en_value, *en_factor, *popup, *fr, *cs;
   Evas_Object *o, *mainbox, *box_settings;
   Evas_Object *check_popup;
	
	Evas_Object *ly = obj;
	Evas_Object *win = data;
   char buf[PATH_MAX];

		  
   popup = elm_win_add(win, "win", ELM_WIN_BASIC);
   elm_win_alpha_set(popup, 1);
	
   mainbox = elm_box_add(popup);
	
	
   elm_box_horizontal_set(mainbox, EINA_FALSE);
   E_EXPAND(mainbox);
   E_ALIGN(mainbox, 0.0, 0.0);
   evas_object_show(mainbox);
	
	
   fr = elm_frame_add(mainbox);
   elm_object_text_set(fr, "Counter Settings");
   E_EXPAND(fr);
   E_ALIGN(fr, 0.5, 0.5);
   E_FILL(fr);
   evas_object_show(fr);

   box_settings = elm_box_add(fr);
   elm_box_horizontal_set(box_settings, EINA_FALSE);
   E_EXPAND(box_settings);
   evas_object_show(box_settings);
/*	
	
   ic = elm_icon_add(box_settings);
	snprintf(buf, sizeof(buf), "%s/images/module_icon1.png", PACKAGE_DATA_DIR);
	elm_image_file_set(ic, buf, NULL);
   evas_object_size_hint_weight_set(ic, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(ic, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_min_set(ic, 56, 56);
	evas_object_color_set(ic, ci_r, ci_g, ci_b, ci_a);
	elm_box_pack_end(box_settings, ic);

   evas_object_data_set(mainbox, "ic", ic);
   evas_object_show(ic);*/
/*
   tg_theme = elm_check_add(box_settings);
   elm_object_style_set(tg_theme, "toggle");
   elm_object_text_set(tg_theme, gettext("Theme: "));
	elm_check_state_set(tg_theme, ci_theme);
   elm_object_part_text_set(tg_theme, "on", gettext("Black"));
   elm_object_part_text_set(tg_theme, "off", gettext("White"));
   elm_box_pack_end(box_settings, tg_theme);
   evas_object_show(tg_theme);
	evas_object_data_set(mainbox, "tg_theme", tg_theme);
	
	
	o = elm_separator_add(box_settings);
   elm_separator_horizontal_set(o, EINA_TRUE);
   elm_box_pack_end(box_settings, o);
   evas_object_show(o);
	
	*/
	
	cs = elm_colorselector_add(box_settings);
   evas_object_size_hint_weight_set(cs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(cs, EVAS_HINT_FILL, EVAS_HINT_FILL);
    
	elm_colorselector_mode_set(cs, ELM_COLORSELECTOR_BOTH);
	elm_colorselector_palette_name_set(cs, "counter");
	elm_colorselector_palette_color_add(cs, 253, 232, 82, 255);
   elm_colorselector_palette_color_add(cs, 255, 186, 0, 255);
   elm_colorselector_palette_color_add(cs, 223, 147, 37, 255);
   elm_colorselector_palette_color_add(cs, 238, 119, 99, 255);
   elm_colorselector_palette_color_add(cs, 164, 182, 166, 255);
   elm_colorselector_palette_color_add(cs, 146, 175, 29, 255);
   elm_colorselector_palette_color_add(cs, 41, 68, 59, 255);
   elm_colorselector_palette_color_add(cs, 0, 109, 128, 255);
   elm_colorselector_palette_color_add(cs, 11, 54, 71, 255);
   elm_colorselector_palette_color_add(cs, 41, 46, 68, 255);
   elm_colorselector_palette_color_add(cs, 116, 13, 14, 255);
   elm_colorselector_palette_color_add(cs, 101, 33, 44, 255);
   elm_colorselector_palette_color_add(cs, 132, 50, 118, 255);
   elm_colorselector_palette_color_add(cs, 68, 40, 55, 255);
   elm_colorselector_palette_color_add(cs, 63, 37, 60, 255);
   elm_colorselector_palette_color_add(cs, 115, 87, 63, 255);
   elm_colorselector_palette_color_add(cs, 66, 70, 73, 255);
   elm_colorselector_palette_color_add(cs, 255, 255, 255, 255);
   elm_colorselector_palette_color_add(cs, 0, 0, 0, 255);
   elm_box_pack_end(box_settings, cs);
   evas_object_show(cs);
	evas_object_data_set(mainbox, "cs", cs);
	
	
	o = elm_separator_add(box_settings);
   elm_separator_horizontal_set(o, EINA_TRUE);
   elm_box_pack_end(box_settings, o);
   evas_object_show(o);
	
   en_name = elm_entry_add(box_settings);
//    elm_config_context_menu_disabled_set(EINA_TRUE);
   elm_object_text_set(en_name, ci_name);
   elm_entry_editable_set(en_name, EINA_TRUE);
   elm_entry_single_line_set(en_name, EINA_TRUE);
   evas_object_size_hint_weight_set(en_name, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_name, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box_settings, en_name);
	evas_object_show(en_name);
   evas_object_data_set(mainbox, "en_name", en_name);

	o = elm_separator_add(box_settings);
   elm_separator_horizontal_set(o, EINA_TRUE);
   elm_box_pack_end(box_settings, o);
   evas_object_show(o);
	
   en_unit = elm_entry_add(box_settings);
//    elm_config_context_menu_disabled_set(EINA_TRUE);
   elm_object_text_set(en_unit, ci_unit);
   elm_entry_editable_set(en_unit, EINA_TRUE);
   elm_entry_single_line_set(en_unit, EINA_TRUE);
   evas_object_size_hint_weight_set(en_unit, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_unit, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box_settings, en_unit);
	evas_object_show(en_unit);
   evas_object_data_set(mainbox, "en_unit", en_unit);
	
	o = elm_separator_add(box_settings);
//    elm_separator_horizontal_set(o, EINA_TRUE);
   elm_box_pack_end(box_settings, o);
   evas_object_show(o);
	
   en_value = elm_entry_add(box_settings);
//    elm_config_context_menu_disabled_set(EINA_TRUE);
	
	char buf1[4096];
   snprintf(buf1, sizeof(buf1), "%.2f", ci_value);
	
   elm_object_text_set(en_value, buf1);
   elm_entry_editable_set(en_value, EINA_TRUE);
   elm_entry_single_line_set(en_value, EINA_TRUE);
   evas_object_size_hint_weight_set(en_value, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_value, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box_settings, en_value);
	evas_object_show(en_value);
   evas_object_data_set(mainbox, "en_value", en_value);	
				
	o = elm_separator_add(box_settings);
   elm_separator_horizontal_set(o, EINA_TRUE);
   elm_box_pack_end(box_settings, o);
   evas_object_show(o);
	
   en_factor = elm_entry_add(box_settings);
//    elm_config_context_menu_disabled_set(EINA_TRUE);
	
   snprintf(buf1, sizeof(buf1), "%.2f", ci_factor);
	
   elm_object_text_set(en_factor, buf1);
   elm_entry_editable_set(en_factor, EINA_TRUE);
   elm_entry_single_line_set(en_factor, EINA_TRUE);
   evas_object_size_hint_weight_set(en_factor, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_factor, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box_settings, en_factor);
	evas_object_show(en_factor);
   evas_object_data_set(mainbox, "en_factor", en_factor);

   o = elm_separator_add(box_settings);
   elm_separator_horizontal_set(o, EINA_TRUE);
   elm_box_pack_end(box_settings, o);
   evas_object_show(o);


   elm_object_content_set(fr, box_settings);
   elm_box_pack_end(mainbox, fr);


	
//    evas_object_smart_callback_add(check_online, "changed", _check_bibelserver_changed, hoversel);
//    evas_object_smart_callback_add(check_switch, "changed", _check_switch_changed, sl_switch_time);
//    evas_object_smart_callback_add(check_layout, "changed", _check_layout_changed, mainbox);
//    evas_object_smart_callback_add(sl_font, "changed", _sl_font_changed, mainbox);
//    evas_object_smart_callback_add(sl_font, "changed", _sl_font_changed_value, ly);
//    evas_object_smart_callback_add(tg_theme, "changed", _tg_changed_cb, ic);
//    evas_object_smart_callback_add(tg_theme, "changed", _config_save, mainbox);
	
	Evas_Object *edje_obj = elm_layout_edje_get(ly);
   evas_object_smart_callback_add(en_name, "changed", _config_save, mainbox);
   evas_object_smart_callback_add(en_unit, "changed", _config_save, mainbox);
   evas_object_smart_callback_add(en_value, "changed", _config_save, mainbox);
   evas_object_smart_callback_add(en_factor, "changed", _config_save, mainbox);
	evas_object_smart_callback_add(cs, "changed", _colorselector_changed_cb, mainbox); 
	

   evas_object_data_set(mainbox, "win", win);
   evas_object_data_set(mainbox, "ly", ly);
	
   elm_object_content_set(popup, mainbox);
	
	evas_object_event_callback_add(popup, EVAS_CALLBACK_MOUSE_OUT, _config_save, mainbox);
   evas_object_show(popup);
}

