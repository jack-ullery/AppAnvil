#include "status_column_record.h"
#include <iostream>
#include <vector>

/*
    Public Methods
*/

std::shared_ptr<StatusColumnRecord> StatusColumnRecord::create(const std::shared_ptr<Gtk::TreeView>& view, std::vector<std::string> names){

    std::shared_ptr<StatusColumnRecord> record{new StatusColumnRecord(names)};

    auto store = Gtk::TreeStore::create(*record);
    record->store = store;

    view->set_model(store);

    for(uint i = 0; i < names.size(); i++){
        // Add a visible column, and title it using the string from 'names' 
        view->append_column(names[i], record->column[i]);
        // Set some default settings for the columns
        // Note this a Gtk::TreeViewColumn which is different then the Gtk::TreeModelColumn we have as a field in StatusColumnRecord
        auto *column = view->get_column(i);
        column->set_reorderable();
        column->set_resizable();
        column->set_min_width(MIN_COL_WIDTH);
        column->set_sort_column(record->column[i]);
    }

    return record;
}

Gtk::TreeRow StatusColumnRecord::new_column(){
    return *(store->append());
}

Gtk::TreeRow StatusColumnRecord::new_child_column(const Gtk::TreeRow& parent){
    return *(store->append(parent.children()));
}

void StatusColumnRecord::clear(){
    store->clear();
}

/*
    Protected Methods
*/
StatusColumnRecord::StatusColumnRecord(const std::vector<std::string>& names)
{
    this->column = std::vector<Gtk::TreeModelColumn<std::string>>(names.size());
    for(uint i = 0; i < column.size(); i++){
        column[i] = Gtk::TreeModelColumn<std::string>();
        add(column[i]);
    }
}
