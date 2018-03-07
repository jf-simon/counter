#include "news.h"

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
			ci_url = eina_stringshare_add(list_data->url);
// 			ci_unit = eina_stringshare_add(list_data->unit);
// 			ci_value = list_data->value;
// 			ci_factor = list_data->factor;
			ci_r = list_data->r;
			ci_g = list_data->g;
			ci_b = list_data->b;
			ci_a = list_data->a;
			found = 1;
		}
   }

   if(found == 0)
	{
		ci_url = eina_stringshare_add("http://www.tagesschau.de/xml/rss2");
// 		ci_unit = eina_stringshare_add("UNIT");
// 		ci_value = 0;
// 		ci_factor= 1;
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
   Evas_Object *en_url = evas_object_data_get(mainbox, "en_url");
//    Evas_Object *en_unit = evas_object_data_get(mainbox, "en_unit");
//    Evas_Object *en_value = evas_object_data_get(mainbox, "en_value");
//    Evas_Object *en_factor = evas_object_data_get(mainbox, "en_factor");
	ci_url = elm_object_text_get(en_url);
// 	ci_unit = elm_object_text_get(en_unit);
// 	ci_value = atof(elm_object_text_get(en_value));
// 	ci_factor = atof(elm_object_text_get(en_factor));
	}
	
   EINA_LIST_FOREACH(configlist, l, list_data)
   {
	   if(list_data->id == id_num)
		{
			list_data->url = ci_url;
// 			list_data->unit = ci_unit;
// // 			list_data->value = atof(elm_object_text_get(en_value));
// 			list_data->value = ci_value;
// 			list_data->factor = ci_factor;
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
		list_data1->url = ci_url;
// 		list_data1->unit = ci_unit;
// 		list_data1->value = ci_value;
// 		list_data1->factor = ci_factor;
		list_data1->r = ci_r;
		list_data1->g = ci_g;
		list_data1->b = ci_b;
		list_data1->a = ci_a;

		configlist = eina_list_append(configlist, list_data1);
	}

//    ci_name = elm_object_text_get(hoversel);

	printf("SAVE FOUND: %i\n", found);
_save_eet();
// _get_data();
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


static void
_popup_del(void *data EINA_UNUSED, Evas *e EINA_UNUSED, Evas_Object *obj, void *event_data EINA_UNUSED)
{
   evas_object_del(obj);
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
_config_save1(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{
	_config_save(data, NULL, NULL, NULL);	
}


void
_settings(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{	
	Evas_Object *en_url, *en_unit, *en_value, *en_factor, *popup, *fr, *cs;
   Evas_Object *o, *mainbox, *box_settings, *box_url, *box_value, *box_unit, *box_factor, *lbl;
	
	Evas_Object *ly = obj;
	Evas_Object *win = data;
		  
   popup = elm_win_add(win, "win", ELM_WIN_BASIC);
   elm_win_alpha_set(popup, 1);
	
   mainbox = elm_box_add(popup);
	
	
   elm_box_horizontal_set(mainbox, EINA_FALSE);
   E_EXPAND(mainbox);
   E_ALIGN(mainbox, 0.0, 0.0);
   evas_object_show(mainbox);
	
	
   fr = elm_frame_add(mainbox);
   elm_object_text_set(fr, "News Settings");
   E_EXPAND(fr);
   E_ALIGN(fr, 0.5, 0.5);
   E_FILL(fr);
   evas_object_show(fr);

   box_settings = elm_box_add(fr);
   elm_box_horizontal_set(box_settings, EINA_FALSE);
   E_EXPAND(box_settings);
   evas_object_show(box_settings);

	
		   box_url = elm_box_add(box_settings);
			elm_box_horizontal_set(box_url, EINA_TRUE);
			E_EXPAND(box_url);
			E_ALIGN(box_url, 0.0, 0.5);
			evas_object_show(box_url);
			
			lbl = elm_label_add(box_url);
			elm_object_text_set(lbl, "Feed URL: ");
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, EVAS_HINT_FILL, EVAS_HINT_FILL);
			elm_box_pack_end(box_url, lbl);
			evas_object_show(lbl);
			
			en_url = elm_entry_add(box_url);
			elm_config_context_menu_disabled_set(EINA_FALSE);
			elm_object_text_set(en_url, ci_url);
			elm_entry_editable_set(en_url, EINA_TRUE);
			elm_entry_single_line_set(en_url, EINA_TRUE);
			evas_object_size_hint_weight_set(en_url, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(en_url, EVAS_HINT_FILL, EVAS_HINT_FILL);
			elm_box_pack_end(box_url, en_url);
			evas_object_show(en_url);
			evas_object_data_set(mainbox, "en_url", en_url);
			
			elm_box_pack_end(box_settings, box_url);
	


   o = elm_separator_add(box_settings);
   elm_separator_horizontal_set(o, EINA_TRUE);
   elm_box_pack_end(box_settings, o);
   evas_object_show(o);
	
	
	cs = elm_colorselector_add(box_settings);
   evas_object_size_hint_weight_set(cs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(cs, EVAS_HINT_FILL, EVAS_HINT_FILL);
    
	elm_colorselector_mode_set(cs, ELM_COLORSELECTOR_BOTH);
	elm_colorselector_palette_name_set(cs, "news");
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


   elm_object_content_set(fr, box_settings);
   elm_box_pack_end(mainbox, fr);

   evas_object_smart_callback_add(en_url, "changed", _config_save1, mainbox);
	evas_object_smart_callback_add(cs, "changed", _colorselector_changed_cb, mainbox); 
	

   evas_object_data_set(mainbox, "win", win);
   evas_object_data_set(mainbox, "ly", ly);
	
   elm_object_content_set(popup, mainbox);
	
	evas_object_event_callback_add(popup, EVAS_CALLBACK_MOUSE_OUT, _config_save, mainbox);
	evas_object_event_callback_add(popup, EVAS_CALLBACK_HIDE, _popup_del, NULL);
   evas_object_show(popup);
}

