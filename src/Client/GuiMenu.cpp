#include <menu.h>
#include <ncurses.h>
#include <string>
#include "GuiMenu.h"
#include "Gui.h"
#include "ClientSocket.h"

GuiMenu::GuiMenu(Window *parent, int rows, int cols) : Window(parent), rows_(rows), cols_(cols){
	window(derwin(parent->window(), rows_-2, cols_-3, 1, 1));
	item_.push_back(NULL);
}

void GuiMenu::addItem(const char* name, const char* description){
	item_[item_.size() - 1] = new_item(name, description);
	item_.push_back(NULL);
}

GuiMenu::~GuiMenu(){
	for(unsigned i=0; i<item_.size()-1; i++){
		free_item(item_[i]);
	}
	free_menu(menu_);
}

void GuiMenu::createMenu(){
	menu_ = new_menu(item_.data());
	set_menu_win(menu_, window());
	set_menu_sub(menu_, derwin(window(), 0, 0, 1, 1));
	attron(COLOR_PAIR(Gui::GREEN));
	box(window(), 0, 0);
	attroff(COLOR_PAIR(Gui::GREEN));
	post_menu(menu_);
	wrefresh(window());
}

void GuiMenu::up() const{
	menu_driver(menu_, REQ_UP_ITEM);
}

void GuiMenu::down() const{
	menu_driver(menu_, REQ_DOWN_ITEM);
}

GuiMenu& GuiMenu::operator--(int){
	ITEM *item = current_item(menu_);
	std::string pseudo(item_name(item));
	ClientSocket socket;
	socket << "decUserBalanceByOne(" + pseudo + ")";
	return (*this);
}

GuiMenu& GuiMenu::operator++(int){
	ITEM *item = current_item(menu_);
	std::string pseudo(item_name(item));
	ClientSocket socket;
	socket << "incUserBalanceByOne(" + pseudo + ")";
	return (*this);
}

