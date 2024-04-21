#include <fstream>
#include <gtkmm/dialog.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodelsort.h>
#include <libappanvil/tree/FileRule.hh>
#include <utility>

#include "add_file_rule.h"
#include "../../threads/command_caller.h"
#include "common.h"

AddFileRule::AddFileRule()
  : builder{ Gtk::Builder::create_from_resource("/modal/rule/file_rule.glade") },
    dialog{ Common::get_widget_shared<Gtk::Dialog>("dialog", builder) },
    headerbar{ Common::get_widget<Gtk::HeaderBar>("headerbar", builder) },
    stack_1{ Common::get_widget<Gtk::Stack>("stack_1", builder)},
    stack_2{ Common::get_widget<Gtk::Stack>("stack_2", builder)},
    button_cancel{ Common::get_widget<Gtk::Button>("button_cancel", builder)},
    button_next{ Common::get_widget<Gtk::Button>("button_next", builder)},
    button_back{ Common::get_widget<Gtk::Button>("button_back", builder)},
    button_accept{ Common::get_widget<Gtk::Button>("button_accept", builder)},
    fr_entry{ Common::get_widget<Gtk::Entry>("fr_entry", builder) },
    fr_preview{ Common::get_widget<Gtk::TextView>("fr_preview", builder) },
    fr_text{ Common::get_widget<Gtk::TextView>("fr_text", builder) }
{
    // Connect the buttons
    auto handle_button_cancel = sigc::mem_fun(*this, &AddFileRule::handle_button_cancel);
    auto handle_button_next   = sigc::mem_fun(*this, &AddFileRule::handle_button_next);
    auto handle_button_back   = sigc::mem_fun(*this, &AddFileRule::handle_button_back);
    auto handle_button_accept = sigc::mem_fun(*this, &AddFileRule::handle_button_accept);
    button_cancel->signal_clicked().connect(handle_button_cancel);
    button_next->signal_clicked().connect(handle_button_next);
    button_back->signal_clicked().connect(handle_button_back);
    button_accept->signal_clicked().connect(handle_button_accept);

    // Connect 'fr_entry' widget
    auto handle_entry = sigc::mem_fun(*this, &AddFileRule::handle_entry_changed);
    fr_entry->signal_changed().connect(handle_entry);

    handle_entry_changed();

    // Show the dialog
    dialog->show_all();
}

void AddFileRule::handle_button_cancel()
{
    dialog->response(Gtk::RESPONSE_CANCEL);
}

void AddFileRule::handle_button_next()
{
    stack_1->set_visible_child("page1");
    stack_2->set_visible_child("page1");
}

void AddFileRule::handle_button_back()
{
    stack_1->set_visible_child("page0");
    stack_2->set_visible_child("page0");
}

void AddFileRule::handle_button_accept()
{
    dialog->response(Gtk::RESPONSE_ACCEPT);
}

void AddFileRule::handle_entry_changed()
{
    // auto text = fr_entry->get_text();
    // prospective_rule = AppArmor::Tree::FileRule(text);

    // if(text.empty())
    // {
    //     button_next->set_sensitive(false);
    // } else {
    //     button_next->set_sensitive(true);

    //     // Populate the "FileRule Preview" TextView
    //     auto preview_text = prospective_rule.operator std::string();
    //     fr_preview->get_buffer()->set_text(preview_text);

    //     // Populate the "FileRule Text" TextView
    //     auto file_rule_path = "/etc/apparmor.d/file_rules/" + text;
    //     std::ifstream file_rule_file(file_rule_path);
    //     std::stringstream file_rule_stream;
    //     file_rule_stream << file_rule_file.rdbuf();
    //     fr_text->get_buffer()->set_text(file_rule_stream.str());
    // }
}

AddFileRule::show_dialog_response AddFileRule::show_dialog()
{
    AddFileRule add_file_rule;
    int response = add_file_rule.dialog->run();
    return show_dialog_response(response, add_file_rule.prospective_rule);
}
