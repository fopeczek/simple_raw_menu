#include "menu.h"
#include "delete_new.h"

void FlatRawMenu::init_screen(uint8_t visible_row_count, uint8_t item_count){
	if (m_active_item >= item_count)
		m_active_item = 0;
	if (m_item_count != item_count){
		if (m_item_count > 0) {
//			delete[] m_draw_callbacks;
//			delete[] m_callbacks;
		}
		m_draw_callbacks = new Draw_Callback[item_count];
		m_callbacks = new Callback[item_count];

	}
	m_item_count = item_count;
	m_visible_row_count = visible_row_count;
}

//int freeMemory();

void FlatRawMenu::add_item(Draw_Callback draw, Callback callback, uint8_t item_position){
//	Serial<<"setup H4.4 A "<<freeMemory()<<". \n";Serial.flush();
	if (item_position >= m_item_count){
		Serial<<"ERROR:Wrong menu size";
		return;
	}
//	Serial<<"setup H4.4 B "<<freeMemory()<<". \n";Serial.flush();
	m_draw_callbacks[item_position] = draw;
//	Serial<<"setup H4.4 C "<<freeMemory()<<". \n";Serial.flush();
	m_callbacks[item_position] = callback;
//	Serial<<"setup H4.4 D "<<freeMemory()<<". \n";Serial.flush();
	return;
}

void FlatRawMenu::draw() {
	if (m_item_count ==0)
		return;
	for (uint8_t i = m_first_visible_row; i<min(m_first_visible_row + m_visible_row_count, m_item_count) ; i++) {
		draw_by_item_position(i);
	}
}

void FlatRawMenu::draw_by_line_nr(uint8_t line_nr) {
	if (line_nr < m_visible_row_count)
		draw_by_item_position(m_first_visible_row + line_nr);
}

void FlatRawMenu::draw_by_item_position(uint8_t item_position){
	m_draw_callbacks[item_position](item_position - m_first_visible_row, item_position == m_active_item);
}

void FlatRawMenu::scroll_down(){
	if (m_first_visible_row + m_visible_row_count < m_item_count) {
		m_first_visible_row++;
		draw();
	}
}


void FlatRawMenu::scroll_up(){
	if (m_first_visible_row > 0) {
		m_first_visible_row--;
		draw();
	}
}

uint8_t FlatRawMenu::move_selection_down(){
	Serial.println("A"); Serial.flush();
	uint8_t active_item = m_active_item;
	m_active_item = (m_active_item + 1) % m_item_count;
	if (m_active_item == 0) {
		m_first_visible_row = 0;
		this->draw();
		return m_active_item;
	}
	if (m_first_visible_row + m_visible_row_count > m_active_item) {
		draw_by_item_position(active_item);
		draw_by_item_position(m_active_item);
	} else{
		scroll_down();
	}
	return m_active_item;
}

uint8_t FlatRawMenu::move_selection_up(){
	uint8_t active_item = m_active_item;
	if (active_item == 0) {
		m_active_item = m_item_count - 1;
		if (m_active_item >= m_visible_row_count) {
			m_first_visible_row = m_active_item - m_visible_row_count + 1;
			this->draw();
			return m_active_item;
		}
		draw_by_item_position(active_item);
		draw_by_item_position(m_active_item);
		return m_active_item;
	}
	m_active_item = m_active_item - 1;

	if (m_first_visible_row < m_active_item) {
		draw_by_item_position(active_item);
		draw_by_item_position(m_active_item);
	} else {
		scroll_up();
	}
	Serial.print(m_active_item);
	return m_active_item;
}


void FlatRawMenu::select(){
	m_callbacks[m_active_item]();
}

void FlatRawMenu::reset_selection(){
	m_active_item = 0;
	m_first_visible_row = 0;
	draw();
}
