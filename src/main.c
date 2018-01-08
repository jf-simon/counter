#include "counter.h"

Evas_Object *win = NULL;
// static    Evas_Object *popup = NULL;
Evas_Object *ly = NULL;

static E_Gadget_Site_Orient gorient;
static E_Gadget_Site_Anchor ganchor;

//////////////////////////////// EET /////////////////////////////////

typedef struct {
        Eina_List   *configlist_eet;
} Counter_List_Eet;


typedef struct {
        int         id;
        const char *name;
        const char *unit;
        double      value;
        double      factor;
		  int         r;
	     int         g;
        int         b;
        int         a;
} My_Conf_Type;
   

static const char MY_CONF_FILE_ENTRY[] = "config";

static Eet_Data_Descriptor *_my_conf_descriptor;
static Eet_Data_Descriptor *_my_conf_sub_descriptor;

static void
_my_conf_descriptor_init(void)
{
    Eet_Data_Descriptor_Class eddc;
    EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, Counter_List_Eet);
    _my_conf_descriptor = eet_data_descriptor_stream_new(&eddc);
   
    EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, My_Conf_Type);
    _my_conf_sub_descriptor = eet_data_descriptor_stream_new(&eddc);
   
    #define MY_CONF_ADD_BASIC(member, eet_type) \
        EET_DATA_DESCRIPTOR_ADD_BASIC             \
        (_my_conf_descriptor, Counter_List_Eet, # member, member, eet_type)
		  
    #define MY_CONF_SUB_ADD_BASIC(member, eet_type) \
        EET_DATA_DESCRIPTOR_ADD_BASIC                 \
        (_my_conf_sub_descriptor, My_Conf_Type, # member, member, eet_type)
    
//     MY_CONF_SUB_ADD_BASIC(note_name, EET_T_STRING);
    MY_CONF_SUB_ADD_BASIC(id, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(name, EET_T_STRING);
    MY_CONF_SUB_ADD_BASIC(unit, EET_T_STRING);
	 MY_CONF_SUB_ADD_BASIC(value, EET_T_DOUBLE);
	 MY_CONF_SUB_ADD_BASIC(factor, EET_T_DOUBLE);
	 MY_CONF_SUB_ADD_BASIC(r, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(g, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(b, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(a, EET_T_INT);

    // And add the sub descriptor as a linked list at 'subs' in the main struct
    EET_DATA_DESCRIPTOR_ADD_LIST
     (_my_conf_descriptor, Counter_List_Eet, "configlist_eet", configlist_eet, _my_conf_sub_descriptor);

     
    #undef MY_CONF_ADD_BASIC
    #undef MY_CONF_SUB_ADD_BASIC
}

//////////////////////////////// EET /////////////////////////////////




void
_read_eet()
{
    Eet_File *ef;
    
    Counter_List_Eet *my_conf;
        
    eet_init();

   char buf[4096], buf2[4096];

   snprintf(buf2, sizeof(buf2), "%s/counter", efreet_config_home_get());
   ecore_file_mkpath(buf2);
   snprintf(buf, sizeof(buf), "%s/counter_config.cfg", buf2);
	
    ef = eet_open(buf, EET_FILE_MODE_READ);
    if (!ef)
    {
//         printf("ERROR: could not open '%s' for read\n", home_file);
        return;
    }
    
    my_conf = eet_data_read(ef, _my_conf_descriptor, MY_CONF_FILE_ENTRY);
        
    configlist =  my_conf->configlist_eet;
  
    eet_close(ef);
    eet_shutdown();
}


void
_save_eet()
{
    Eet_File *ef;
    eet_init();
	 
   char buf[4096];
	 
   snprintf(buf, sizeof(buf), "%s/counter/counter_config.cfg", efreet_config_home_get());
   
	ef = eet_open(buf, EET_FILE_MODE_WRITE);
       
    if(!ef)
    {
        printf("ERROR\n");
//         fprintf(stderr, "ERROR: could not open '%s' for write\n", home_file);
//            return EINA_FALSE;   //TODO got elm_main -> END
    }else
    {

        Counter_List_Eet *my_conf = calloc(1, sizeof(Counter_List_Eet));

        if (!my_conf)
        {
            fprintf(stderr, "ERROR: could not calloc My_Conf_Type\n");
//                 return NULL;   //TODO got elm_main -> END
        }

		  my_conf->configlist_eet = configlist;
 
        eet_data_write(ef, _my_conf_descriptor, MY_CONF_FILE_ENTRY, my_conf, EINA_TRUE);
    }
    
    eet_close(ef);
    eet_shutdown();
	 
}
/*
static void
_my_conf_descriptor_shutdown(void)
{
    eet_data_descriptor_free(_my_conf_sub_descriptor);
    eet_data_descriptor_free(_my_conf_descriptor);
}*/


static void
_delete_id(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
	Eina_List *l;
	Config_Item *list_data;

   EINA_LIST_FOREACH(configlist, l, list_data)
   {
	   if(list_data->id == id_num)
		{
         configlist = eina_list_remove(configlist, list_data);
		}
   }
    _save_eet();
}


static void
orient_change(void *data, Evas_Object *obj, void *event_info)
{
// 	Evas_Object *ly = data;
   gorient = (uintptr_t)event_info;
//    update_anchor_orient(data, gorient, ganchor, obj);
}

static void
anchor_change(void *data, Evas_Object *obj, void *event_info)
{
// 	Evas_Object *ly = data;
   ganchor = (uintptr_t)event_info;
//    update_anchor_orient(data, gorient, ganchor, obj);
}
/*

static void
_mouse_in_online(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{ 
		edje_object_signal_emit(ly, "mouse_in_online_go", "e");
}

static void
_mouse_out_online(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
	   edje_object_signal_emit(ly, "mouse_out_online_go", "e");
}
*/


/*
static void
delete_popup_evas(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   if(popup)
     {
        evas_object_del(popup);
        popup = NULL;
        return;
     }
}*/


/*
static void
delete_popup_edje(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
   if(popup)
     {
        evas_object_del(popup);
        popup = NULL;
        return;
     }
}*/

/*
static void
show_popup(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
   Evas_Object *win = data;
	Evas_Object *box, *lbl;
   char buffer[PATH_MAX];
   char buffer1[PATH_MAX];
	
	if(ci_popup == 1)
		return;
	
   snprintf(buffer, sizeof(buffer), "%s<br>%s", losungstext, losungsvers);
   snprintf(buffer1, sizeof(buffer1), "%s<br>%s", lehrtext, lehrtextvers);
	
   popup = elm_win_add(win, "Popup",  ELM_WIN_POPUP_MENU);
   elm_win_alpha_set(popup, 1);
	
   box = elm_box_add(popup);
   elm_box_horizontal_set(box, EINA_FALSE);
   elm_win_resize_object_add(popup, box);
   evas_object_show(box);
 
   lbl = elm_label_add(box);
	elm_label_line_wrap_set(lbl, ELM_WRAP_WORD);
   elm_label_wrap_width_set(lbl, ELM_SCALE_SIZE(400));
   elm_object_text_set(lbl, buffer);
   elm_box_pack_end(box, lbl);
   evas_object_show(lbl);
	
	lbl = elm_label_add(box);
	elm_label_line_wrap_set(lbl, ELM_WRAP_WORD);
   elm_label_wrap_width_set(lbl, ELM_SCALE_SIZE(400));
   elm_object_text_set(lbl, buffer1);
   elm_box_pack_end(box, lbl);
   evas_object_show(lbl);

   evas_object_size_hint_align_set(box, 0.5, 0.5);
	
   evas_object_show(popup);
}*/

void
_set_content(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
	Evas_Object *ly = data;
   char buf[4096];
	
   edje_object_part_text_set(ly, "name", ci_name); 
   edje_object_part_text_set(ly, "unit", ci_unit);
	
	double new = floor(ci_value);
	
	if(fmod(ci_value, new) == 0)
		snprintf(buf, sizeof(buf), "%.0f", ci_value);
	else
		snprintf(buf, sizeof(buf), "%.2f", ci_value);
	
   
	edje_object_part_text_set(ly, "value", buf);
		
	printf("set content\n");
}



void
set_color(Evas_Object *ly)
{
	edje_object_color_class_set(ly, "colorclass", /* class name   */
                               ci_r, ci_g, ci_b, ci_a,  /* Object color */
                               255, 255, 255, 0,   /* Text outline */
                               39, 90, 187, 255);  /* Text shadow  */
	printf("SET COLOR: %i %i %i %i,\n", ci_r,ci_g,ci_b,ci_a);
}


void
_plus(void *data, Evas_Object *obj, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
// 	Evas_Object *win = data;
	Evas_Object *ly = obj;
	
   char buf[4096];
	ci_value = ci_value + ci_factor;	
	
	double new = floor(ci_value);
	
	if(fmod(ci_value, new) == 0)
		snprintf(buf, sizeof(buf), "%.0f", ci_value);
	else
		snprintf(buf, sizeof(buf), "%.2f", ci_value);
	
	edje_object_part_text_set(ly, "value", buf);
	
	_config_save(NULL, NULL, NULL, NULL);
}


void
_minus(void *data, Evas_Object *obj, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
// 	Evas_Object *win = data;
	Evas_Object *ly = obj;
   char buf[4096];
	ci_value = ci_value - ci_factor;
	
	double new = floor(ci_value);
	
	if(fmod(ci_value, new) == 0)
		snprintf(buf, sizeof(buf), "%.0f", ci_value);
	else
		snprintf(buf, sizeof(buf), "%.2f", ci_value);
	
	edje_object_part_text_set(ly, "value", buf);
	
	_config_save(NULL, NULL, NULL, NULL);
}


int elm_main(int argc, char *argv[])
{
   char buf[PATH_MAX];
//    char buf2[PATH_MAX];
//    int gadget = 0;
   char buf1[16];
	
	Config_Item *config;
	config = calloc(1, sizeof(Config_Item));
		
		
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
 
   if (getenv("E_GADGET_ID"))
     {
//         gadget = 1;
        snprintf(buf1, sizeof(buf1), "%s", getenv("E_GADGET_ID"));
        id_num = atoi(buf1);
     }

    _my_conf_descriptor_init();
   _read_eet();
   //new window
   win = elm_win_add(NULL, "counter", ELM_WIN_BASIC);
   elm_win_title_set(win, "Counter");
   elm_win_autodel_set(win, EINA_TRUE);
   elm_win_alpha_set(win, EINA_TRUE);	

	evas_object_size_hint_aspect_set(win, EVAS_ASPECT_CONTROL_BOTH, 1, 1);
	
   // LAYOUT CREATE START// 
   ly = elm_layout_add(win);		  
   snprintf(buf, sizeof(buf), "%s/themes/counter.edj", PACKAGE_DATA_DIR);
	evas_object_size_hint_weight_set(ly, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	

   elm_win_resize_object_add(win, ly);
   evas_object_show(ly);
    // LAYOUT CREATE END// 

   evas_object_data_set(win, "config", config);
	evas_object_resize(win, 10, 10);
   evas_object_show(win);
	elm_layout_file_set(ly, buf, "counter");
//    edje_object_signal_callback_add(ly, "online", "online", open_bibelserver, win);
   edje_object_signal_callback_add(ly, "settings", "settings", _settings_2, win);
   edje_object_signal_callback_add(ly, "minus", "minus", _minus, win);
   edje_object_signal_callback_add(ly, "plus", "plus", _plus, win);
//    edje_object_signal_callback_add(ly, "show_popup", "show_popup", show_popup, win);
//    edje_object_signal_callback_add(ly, "delete_popup", "delete_popup", delete_popup_edje, win);
// 	edje_object_signal_callback_add(ly, "mouse_in_online_go", "mouse_in_online_go", _mouse_in_online, NULL);
//    edje_object_signal_callback_add(ly, "mouse_out_online_go", "mouse_out_online_go", _mouse_out_online, NULL);
//    edje_object_signal_callback_add(ly, "switch_wheel", "switch_wheel", _set_text_wheel, ly);
	
	Evas_Object *edje_obj = elm_layout_edje_get(ly);
	
	evas_object_smart_callback_add(win, "gadget_site_orient", orient_change, ly);
   evas_object_smart_callback_add(win, "gadget_site_anchor", anchor_change, ly);
   evas_object_smart_callback_add(win, "gadget_configure", _settings_1, edje_obj);
   evas_object_smart_callback_add(win, "gadget_removed", _delete_id, NULL);
		
	
	
	_config_load(ly);							// load config data from eet to tmp vars
	
// 				Evas_Object *edje_obj = elm_layout_edje_get(ly);
	set_color(edje_obj);
	
	_set_content(edje_obj, NULL, NULL, NULL);

  //run app RUN!
  elm_run();

  //shutdown!
//         _my_conf_descriptor_shutdown();
  return 0;
}

ELM_MAIN();
