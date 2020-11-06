/*
 * menu.h
 *
 *  Created on: Jul 28, 2020
 *      Author: silent
 */

#ifndef MENU_H_
#define MENU_H_
#include "function_objects.h"
#include "Arduino.h"
#include "Streaming.h"

class FlatRawMenu {
public:
	FlatRawMenu():m_active_item(0),m_first_visible_row(0), m_item_count(0), m_draw_callbacks(0), m_callbacks(0) {
		//Serial<<"Init FlatRawMenu()\n";
	}

	using Draw_Callback = FunctionObject<void(uint8_t row, bool selected)>;
	using Callback = FunctionObject<void(void)>;

	void init_screen(uint8_t visible_row_count, uint8_t item_count); //(re)allocates memory
	void add_item(Draw_Callback draw, Callback callback, uint8_t item_id);
	void draw_by_item_position(uint8_t item_position);
	void draw_by_line_nr(uint8_t visible_line_nr);
	void draw();
	void scroll_down();
	void scroll_up();
	void reset_selection();
	uint8_t move_selection_down(); //returns a new selected item
	uint8_t move_selection_up(); //returns a new selected item
	uint8_t get_selected_item_id() {return m_active_item;}
	void select();
	uint8_t m_active_item; //item position, not line nr
private:
	uint8_t m_first_visible_row;
	uint8_t m_visible_row_count;
	uint8_t m_item_count;
public:
	Draw_Callback* m_draw_callbacks;
	Callback* m_callbacks;
};


#endif /* MENU_H_ */
