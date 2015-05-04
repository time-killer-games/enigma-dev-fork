/** Copyright (C) 2014-2015 Harijs Grinbergs
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <algorithm> //For std::remove
#include <unordered_map>
#include <string>
#include <deque>
using std::string;
using std::unordered_map;
using std::deque;

#include "Universal_System/var4.h"
#include "Universal_System/CallbackArrays.h" //For mouse_check_button
#include "Universal_System/resource_data.h" //For script_execute
//#include "Universal_System/spritestruct.h"
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GSfont.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GSd3d.h" //For stencil stuff

#include "elements.h"
#include "styles.h"
#include "skins.h"
#include "windows.h"
//#include "include.h"
#include "common.h"

//Children
#include "buttons.h"
#include "toggles.h"
#include "sliders.h"
#include "scrollbars.h"
#include "labels.h"

namespace gui
{
  extern unsigned int gui_elements_maxid;
  extern unordered_map<unsigned int, Element> gui_elements;

  bool windowStopPropagation = false; //Stop event propagation in windows and between
	deque<unsigned int> gui_window_order; //This allows changing rendering order (like depth)
  
	extern unsigned int gui_style_window;

	extern int gui_bound_skin;

	//Implements button class
	Window::Window(){
    style_id = gui_style_window; //Default style
    enigma_user::gui_style_set_font_halign(style_id, enigma_user::gui_state_all, enigma_user::fa_center);
    enigma_user::gui_style_set_font_valign(style_id, enigma_user::gui_state_all, enigma_user::fa_top);
    callback.fill(-1); //Default callbacks don't exist (so it doesn't call any script)
	}

  void Window::callback_execute(int event){
    if (callback[event] != -1){
      enigma_user::script_execute(callback[event], id, true, state, event);
    }
	}

	//Update all possible button states (hover, click, toggle etc.)
	void Window::update(gs_scalar tx, gs_scalar ty){
    if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left)){ //Press
      if (windowStopPropagation == false && box.point_inside(tx,ty)){
        state = enigma_user::gui_state_on;
        callback_execute(enigma_user::gui_event_pressed);
        if (draggable == true){
          drag = true;
          drag_xoffset = tx-box.x;
          drag_yoffset = ty-box.y;
        }
      }else{
        state = enigma_user::gui_state_default;
      }
    }

    if (windowStopPropagation == false && box.point_inside(tx,ty)){ //Hover or resize
      if (resizable == true){
        rect box_left(box.x,box.y,20,box.h);
        rect box_top(box.x,box.y,box.w,20);
        rect box_right(box.w+box.x-20,box.y,20,box.h);
        rect box_bottom(box.x,box.y+box.h-20,box.w,20);

        //Resize
        if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left) && (box_left.point_inside(tx,ty) || box_top.point_inside(tx,ty) || box_right.point_inside(tx,ty) || box_bottom.point_inside(tx,ty))){ //Press
          if (box_left.point_inside(tx,ty)) { if (!box_top.point_inside(tx,ty)) { resize_side = 0; } else { resize_side = 1; } }
          else if (box_top.point_inside(tx,ty)) { if (!box_right.point_inside(tx,ty)) { resize_side = 2; } else { resize_side = 3; } }
          else if (box_right.point_inside(tx,ty)) { if (!box_bottom.point_inside(tx,ty)) { resize_side = 4; } else { resize_side = 5; } }
          else if (box_bottom.point_inside(tx,ty)) { if (!box_left.point_inside(tx,ty)) { resize_side = 6; } else { resize_side = 7; } }
          drag_xoffset = tx-box.x;
          drag_yoffset = ty-box.y;
          resize_xoffset = tx;
          resize_yoffset = ty;
          resize_width = box.w;
          resize_height = box.h;
          resize = true;
          drag = false;
        }
      }else{
        callback_execute(enigma_user::gui_event_hover);
        windowStopPropagation = true;
      }
    }

    if (resize == true){
      windowStopPropagation = true;
      switch (resize_side){
        case 0: { //Resizing left side
          box.w = fmax(resize_width - (tx - resize_xoffset), min_box.w);
          box.x = tx-drag_xoffset;
        } break;
        case 1: { //Resizing top-left
          box.w = fmax(resize_width - (tx - resize_xoffset), min_box.w);
          box.h = fmax(resize_height - (ty - resize_yoffset), min_box.h);
          box.x = tx-drag_xoffset;
          box.y = ty-drag_yoffset;
        } break;
        case 2: { //Resizing top
          box.h = fmax(resize_height - (ty - resize_yoffset), min_box.h);
          box.y = ty-drag_yoffset;
        } break;
        case 3: { //Resizing top-right
          box.w = fmax(resize_width + (tx - resize_xoffset), min_box.w);
          box.h = fmax(resize_height - (ty - resize_yoffset), min_box.h);
          box.y = ty-drag_yoffset;
        } break;
        case 4: { //Resizing right side
          box.w = fmax(resize_width + (tx - resize_xoffset), min_box.w);
        } break;
        case 5: { //Resizing bottom-right
          box.w = fmax(resize_width + (tx - resize_xoffset), min_box.w);
          box.h = fmax(resize_height + (ty - resize_yoffset), min_box.h);
        } break;
        case 6: { //Resizing top
          box.h = fmax(resize_height + (ty - resize_yoffset), min_box.h);
        } break;
        case 7: { //Resizing top-left
          box.w = fmax(resize_width - (tx - resize_xoffset), min_box.w);
          box.h = fmax(resize_height + (ty - resize_yoffset), min_box.h);
          box.x = tx-drag_xoffset;
        } break;
      }
      callback_execute(enigma_user::gui_event_resize);
      if (enigma_user::mouse_check_button_released(enigma_user::mb_left)){
				resize = false;
				callback_execute(enigma_user::gui_event_released);
			}
    }else if (drag == true){
      windowStopPropagation = true;
			box.x = tx-drag_xoffset;
			box.y = ty-drag_yoffset;
			callback_execute(enigma_user::gui_event_drag);
			if (enigma_user::mouse_check_button_released(enigma_user::mb_left)){
				drag = false;
				callback_execute(enigma_user::gui_event_released);
			}
		}
	}

	void Window::draw(){
	  //Draw window
    get_element(sty,gui::Style,gui::GUI_TYPE::STYLE,style_id);
    if (sty.sprites[state] != -1){
      enigma_user::draw_sprite_padded(sty.sprites[state],-1,
                                      sty.border.left,
                                      sty.border.top,
                                      sty.border.right,
                                      sty.border.bottom,
                                      box.x,
                                      box.y,
                                      box.x+box.w,
                                      box.y+box.h,
                                      sty.sprite_styles[state].color,
                                      sty.sprite_styles[state].alpha);
		}

		//Draw text
		sty.font_styles[state].use();

		gs_scalar textx = 0.0, texty = 0.0;
    switch (sty.font_styles[state].halign){
      case enigma_user::fa_left: textx = box.x+sty.padding.left; break;
      case enigma_user::fa_center: textx = box.x+box.w/2.0; break;
      case enigma_user::fa_right: textx = box.x+box.w-sty.padding.right; break;
    }

    switch (sty.font_styles[state].valign){
      case enigma_user::fa_top: texty = box.y+sty.padding.top; break;
      case enigma_user::fa_middle: texty = box.y+box.h/2.0; break;
      case enigma_user::fa_bottom: texty = box.y+box.h-sty.padding.bottom; break;
    }

		enigma_user::draw_text(textx,texty,text);
	}

	void Window::update_text_pos(int state){
	}
}

namespace enigma_user
{
	int gui_window_create(){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_elements.emplace(gui::gui_elements_maxid, gui::Window());
		}else{
      get_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,gui::gui_bound_skin,-1);
			gui::gui_elements.emplace(gui::gui_elements_maxid, gui::gui_elements[ski.window_style]);
		}
    gui::Window &win = gui::gui_elements[gui::gui_elements_maxid];
		win.visible = true;
		win.id = gui::gui_elements_maxid;
		gui::gui_window_order.push_back(gui::gui_elements_maxid);
		return gui::gui_elements_maxid++;
	}

	int gui_window_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_elements.emplace(gui::gui_elements_maxid, gui::Window());
		}else{
      get_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,gui::gui_bound_skin,-1);
			gui::gui_elements.emplace(gui::gui_elements_maxid, gui::gui_elements[ski.window_style]);
      printf("Creating window with size %i\n", sizeof(gui::gui_elements[gui::gui_elements_maxid]));
		}
    gui::Window &win = gui::gui_elements[gui::gui_elements_maxid];
		win.visible = true;
		win.id = gui::gui_elements_maxid;
		win.box.set(x, y, w, h);
		win.text = text;
		win.update_text_pos();
		gui::gui_window_order.push_back(gui::gui_elements_maxid);
		return gui::gui_elements_maxid++;
	}

	void gui_window_destroy(int id){
    check_element(gui::GUI_TYPE::WINDOW,id);
		gui::gui_elements.erase(gui::gui_elements.find(id));
		//This is the fancy remove/erase idiom, which is the fastest way I know how to delete an element by value from vector
		gui::gui_window_order.erase(std::remove(gui::gui_window_order.begin(), gui::gui_window_order.end(), id), gui::gui_window_order.end());
	}

	void gui_window_set_state(int id, int state){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.state = state;
	}

  ///Setters
	void gui_window_set_text(int id, string text){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
		win.text = text;
	}

	void gui_window_set_position(int id, gs_scalar x, gs_scalar y){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
		win.box.x = x;
		win.box.y = y;
	}

	void gui_window_set_size(int id, gs_scalar w, gs_scalar h){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
		win.box.w = w;
		win.box.h = h;
		win.update_text_pos();
	}

	void gui_window_set_callback(int id, int event, int script_id){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    if (event == enigma_user::gui_event_all){
      win.callback.fill(script_id);
	  }else{
      win.callback[event] = script_id;
	  }
  }

  void gui_window_set_style(int id, int style_id){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.style_id = (style_id != -1? style_id : gui::gui_style_window);
  }

	void gui_window_set_visible(int id, bool visible){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
		win.visible = visible;
	}

  void gui_window_set_draggable(int id, bool draggable){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
		win.draggable = draggable;
	}

  void gui_window_set_resizable(int id, bool resizable){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
		win.resizable = resizable;
	}

  void gui_window_set_stencil_mask(int id, bool stencil){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
		win.stencil_mask = stencil;
	}

	///Getters
  int gui_window_get_style(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
    return win.style_id;
  }

	int gui_window_get_state(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
		return win.state;
	}

	int gui_window_get_callback(int id, int event){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
		return win.callback[event];
	}

  bool gui_window_get_draggable(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,false);
    return win.draggable;
  }

  bool gui_window_get_resizable(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,false);
    return win.resizable;
  }

	bool gui_window_get_visible(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,false);
	  return win.visible;
	}

  gs_scalar gui_window_get_width(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
    return win.box.w;
  }

	gs_scalar gui_window_get_height(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
    return win.box.h;
	}

	gs_scalar gui_window_get_x(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
    return win.box.x;
	}

	gs_scalar gui_window_get_y(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
	  return win.box.y;
	}

	string gui_window_get_text(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,"");
    return win.text;
	}

  bool gui_window_get_stencil_mask(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,false);
		return win.stencil_mask;
	}

	///Depth changers
	void gui_window_push_to_front(int id){
    check_element(gui::GUI_TYPE::WINDOW,id);
    auto it = find(gui::gui_window_order.begin(), gui::gui_window_order.end(), id);
    if (it != gui::gui_window_order.end()){
      gui::gui_window_order.erase(it);
      gui::gui_window_order.push_back(id);
    }
	}

  void gui_window_push_to_back(int id){
    check_element(gui::GUI_TYPE::WINDOW,id);
    auto it = find(gui::gui_window_order.begin(), gui::gui_window_order.end(), id);
    if (it != gui::gui_window_order.end()){
      gui::gui_window_order.erase(it);
      gui::gui_window_order.push_front(id);
    }
	}

  ///Drawers
	void gui_window_draw(int id){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
    win.update();
		win.draw();
		//Draw children
		///TODO(harijs) - This needs to implemented!
    /*if (gui::gui_windows[id].child_buttons.empty() == false){
      for (const auto &b : gui::gui_windows[id].child_buttons){
        gui::gui_buttons[b].update();
        gui::gui_buttons[b].draw();
      }
    }*/
		enigma_user::draw_set_halign(phalign);
		enigma_user::draw_set_valign(pvalign);
		enigma_user::draw_set_color(pcolor);
		enigma_user::draw_set_alpha(palpha);
	}

	void gui_windows_draw(){
	  if (gui::gui_window_order.size() == 0) return;

		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
		gui::windowStopPropagation = false;

    //Update loop in reverse direction
    bool window_click = false; //Something clicked in the window (or the window itself)?
    int window_swap_id = -1;
    for (int ind = gui::gui_window_order.size()-1; ind >= 0; --ind){
      int i = gui::gui_window_order[ind];
      get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,i);
      if (win.visible == true){
        //Update children
        if (win.child_buttons.empty() == false){
          for (int b=win.child_buttons.size()-1; b>=0; --b){
            get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,win.child_buttons[b]);
            if (but.visible == false) continue; //Skip invisible objects
            but.update(win.box.x,win.box.y);
          }
        }
        if (win.child_toggles.empty() == false){
          for (int b=win.child_toggles.size()-1; b>=0; --b){
            get_element(tog,gui::Toggle,gui::GUI_TYPE::TOGGLE,win.child_toggles[b]);
            if (tog.visible == false) continue; //Skip invisible objects
            tog.update(win.box.x,win.box.y);
          }
        }
        if (win.child_sliders.empty() == false){
          for (int b=win.child_sliders.size()-1; b>=0; --b){
            get_element(sli,gui::Slider,gui::GUI_TYPE::SLIDER,win.child_sliders[b]);
            if (sli.visible == false) continue;
            sli.update(win.box.x,win.box.y);
          }
        }
        if (win.child_scrollbars.empty() == false){
          for (int b=win.child_scrollbars.size()-1; b>=0; --b){
            get_element(scr,gui::Scrollbar,gui::GUI_TYPE::SCROLLBAR,win.child_scrollbars[b]);
            if (scr.visible == false) continue;
            scr.update(win.box.x,win.box.y);
          }
        }
        //This checks if any of the inside elements are pressed
        if (gui::windowStopPropagation == true && window_click == false) { window_click = true; window_swap_id = ind; }
        win.update();
        //This checks for the click on the window itself. I cannot just check window propagation, because hover also stops propagation, but should not change the draw order
        if (gui::windowStopPropagation == true && window_click == false) { 
          window_click = true; if (win.state == enigma_user::gui_state_on) { window_swap_id = ind; }
        }
      }
		}
		//printf("Window selected = %i and click = %i and size = %i && mouse_check_button_pressed %i\n", window_swap_id, window_click, gui::gui_window_order.size(), enigma_user::mouse_check_button_pressed(enigma_user::mb_left));
		//I REALLY HATE THE MOUSE CHECK HERE :( - Harijs
		if (window_click == true && enigma_user::mouse_check_button_pressed(enigma_user::mb_left)) { //Push to front
        int t = gui::gui_window_order[window_swap_id]; //Get the id of the clicked window
        gui::gui_window_order.erase(gui::gui_window_order.begin()+window_swap_id); //Delete the id from its current place
        gui::gui_window_order.push_back(t); //put on top
    }

    //Draw loop
    for (auto &wi : gui::gui_window_order){
      get_element(w,gui::Window,gui::GUI_TYPE::WINDOW,wi);
      if (w.visible == true){
        //Stencil test
        w.draw();
        if (w.stencil_mask == true){
          enigma_user::d3d_stencil_start_mask();
          w.draw();
          enigma_user::d3d_stencil_use_mask();
        }

        //Draw children
        if (w.child_buttons.empty() == false){
          for (const auto &b : w.child_buttons){
            get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,b);
            if (but.visible == true) but.draw(w.box.x,w.box.y);
          }
        }
        if (w.child_toggles.empty() == false){
          for (const auto &t : w.child_toggles){
            get_element(tog,gui::Toggle,gui::GUI_TYPE::TOGGLE,t);
            if (tog.visible == true) tog.draw(w.box.x,w.box.y);
          }
        }
        if (w.child_sliders.empty() == false){
          for (const auto &s : w.child_sliders){
            get_element(sli,gui::Slider,gui::GUI_TYPE::SLIDER,s);
            if (sli.visible == true) sli.draw(w.box.x,w.box.y);
          }
        }
        if (w.child_scrollbars.empty() == false){
          for (const auto &s : w.child_scrollbars){
            get_element(scr,gui::Scrollbar,gui::GUI_TYPE::SCROLLBAR,s);
            if (scr.visible == true) scr.draw(w.box.x,w.box.y);
          }
        }
        if (w.child_labels.empty() == false){
          for (const auto &l : w.child_labels){
            get_element(lab,gui::Label,gui::GUI_TYPE::LABEL,l);
            if (lab.visible == true) lab.draw(w.box.x,w.box.y);          
          }
        }

        if (w.stencil_mask == true){
          enigma_user::d3d_stencil_end_mask();
        }
			}
		}
		enigma_user::draw_set_halign(phalign);
		enigma_user::draw_set_valign(pvalign);
		enigma_user::draw_set_color(pcolor);
		enigma_user::draw_set_alpha(palpha);
	}

  void gui_window_add_button(int id, int bid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,bid);
    win.child_buttons.push_back(bid);
    but.parent_id = id;
  }

  void gui_window_add_toggle(int id, int tid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    get_element(tog,gui::Toggle,gui::GUI_TYPE::TOGGLE,tid);
    win.child_toggles.push_back(tid);
    tog.parent_id = id;
  }

  void gui_window_add_slider(int id, int sid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    get_element(sli,gui::Slider,gui::GUI_TYPE::SLIDER,sid);
    win.child_sliders.push_back(sid);
    sli.parent_id = id;
  }

  void gui_window_add_scrollbar(int id, int sid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    get_element(scr,gui::Scrollbar,gui::GUI_TYPE::SCROLLBAR,sid);
    win.child_scrollbars.push_back(sid);
    scr.parent_id = id;
  }

  void gui_window_add_label(int id, int lid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    get_element(lab,gui::Label,gui::GUI_TYPE::LABEL,lid);
    win.child_labels.push_back(lid);
    lab.parent_id = id;
  }

  void gui_window_remove_button(int id, int bid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    auto it = find(win.child_buttons.begin(), win.child_buttons.end(), bid);
    if (it != win.child_buttons.end()){
      get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,bid);
      win.child_buttons.erase(it);
      but.parent_id = -1;
    }
  }

  void gui_window_remove_toggle(int id, int tid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    auto it = find(win.child_toggles.begin(), win.child_toggles.end(), tid);
    if (it != win.child_toggles.end()){
      get_element(tog,gui::Toggle,gui::GUI_TYPE::TOGGLE,tid);
      win.child_toggles.erase(it);
      tog.parent_id = -1;
    }
  }

  void gui_window_remove_slider(int id, int sid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    auto it = find(win.child_sliders.begin(), win.child_sliders.end(), sid);
    if (it != win.child_sliders.end()){
      get_element(sli,gui::Slider,gui::GUI_TYPE::SLIDER,sid);
      win.child_sliders.erase(it);
      sli.parent_id = -1;
    }
  }

  void gui_window_remove_scrollbar(int id, int sid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    auto it = find(win.child_scrollbars.begin(), win.child_scrollbars.end(), sid);
    if (it != win.child_scrollbars.end()){
      get_element(scr,gui::Scrollbar,gui::GUI_TYPE::SCROLLBAR,sid);
      win.child_scrollbars.erase(it);
      scr.parent_id = -1;
    }
  }

  void gui_window_remove_label(int id, int lid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    auto it = find(win.child_labels.begin(), win.child_labels.end(), lid);
    if (it != win.child_labels.end()){
      get_element(lab,gui::Label,gui::GUI_TYPE::LABEL,lid);
      win.child_labels.erase(it);
      lab.parent_id = -1;
    }
  }

  int gui_window_get_button_count(int id){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    return win.child_buttons.size();
  }

  int gui_window_get_toggle_count(int id){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    return win.child_toggles.size();
  }

  int gui_window_get_slider_count(int id){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    return win.child_sliders.size();
  }

  int gui_window_get_scrollbar_count(int id){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    return win.child_scrollbars.size();
  }

  int gui_window_get_label_count(int id){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    return win.child_labels.size();
  }

  int gui_window_get_button(int id, int but){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    return win.child_buttons[but];
  }

  int gui_window_get_toggle(int id, int tog){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    return win.child_toggles[tog];
  }

  int gui_window_get_slider(int id, int sli){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    return win.child_sliders[sli];
  }

  int gui_window_get_scrollbar(int id, int scr){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    return win.child_scrollbars[scr];
  }

  int gui_window_get_label(int id, int lab){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    return win.child_labels[lab];
  }
}
