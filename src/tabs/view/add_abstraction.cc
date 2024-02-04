#include "add_abstraction.h"
#include "common.h"
#include <gtkmm/dialog.h>
#include <libappanvil/tree/AbstractionRule.hh>
#include <utility>

AddAbstraction::AddAbstraction()
  : builder{ Gtk::Builder::create_from_resource("/modal/rule/abstraction.glade") },
    dialog{ Common::get_widget_shared<Gtk::Dialog>("dialog", builder) },
    headerbar{ Common::get_widget<Gtk::HeaderBar>("headerbar", builder) },
    stack_1{ Common::get_widget<Gtk::Stack>("stack_1", builder)},
    stack_2{ Common::get_widget<Gtk::Stack>("stack_2", builder)},
    button_cancel{ Common::get_widget<Gtk::Button>("button_cancel", builder)},
    button_next{ Common::get_widget<Gtk::Button>("button_next", builder)},
    button_back{ Common::get_widget<Gtk::Button>("button_back", builder)},
    button_accept{ Common::get_widget<Gtk::Button>("button_accept", builder)},
    ab_combobox{ Common::get_widget<Gtk::ComboBox>("ab_combobox", builder) },
    ab_entry{ Common::get_widget<Gtk::Entry>("ab_entry", builder) },
    ab_preview{ Common::get_widget<Gtk::TextView>("ab_preview", builder) },
    ab_text{ Common::get_widget<Gtk::TextView>("ab_text", builder) }
{
    // Connect the buttons
    auto handle_button_cancel = sigc::mem_fun(*this, &AddAbstraction::handle_button_cancel);
    auto handle_button_next   = sigc::mem_fun(*this, &AddAbstraction::handle_button_next);
    auto handle_button_back   = sigc::mem_fun(*this, &AddAbstraction::handle_button_back);
    auto handle_button_accept = sigc::mem_fun(*this, &AddAbstraction::handle_button_accept);
    button_cancel->signal_clicked().connect(handle_button_cancel);
    button_next->signal_clicked().connect(handle_button_next);
    button_back->signal_clicked().connect(handle_button_back);
    button_accept->signal_clicked().connect(handle_button_accept);

    // Connect 'ab_entry' widget
    auto handle_entry = sigc::mem_fun(*this, &AddAbstraction::handle_entry_changed);
    ab_entry->signal_changed().connect(handle_entry);

    handle_entry_changed();

    // Show the dialog
    dialog->show_all();
}

void AddAbstraction::handle_button_cancel()
{
    dialog->response(Gtk::RESPONSE_CANCEL);
}

void AddAbstraction::handle_button_next()
{
    stack_1->set_visible_child("page1");
    stack_2->set_visible_child("page1");
}

void AddAbstraction::handle_button_back()
{
    stack_1->set_visible_child("page0");
    stack_2->set_visible_child("page0");
}

void AddAbstraction::handle_button_accept()
{
    dialog->response(Gtk::RESPONSE_ACCEPT);
}

void AddAbstraction::handle_entry_changed()
{
    auto text = ab_entry->get_text();

    if(text.empty())
    {
        button_next->set_sensitive(false);
    } else {
        button_next->set_sensitive(true);
        auto preview_text = "include <" + text + ">";
        ab_preview->get_buffer()->set_text(preview_text);
    }
}

AddAbstraction::show_dialog_response AddAbstraction::show_dialog()
{
    AddAbstraction add_abstraction;
    int response = add_abstraction.dialog->run();

    if(response != Gtk::ResponseType::RESPONSE_ACCEPT)
    {
        AppArmor::Tree::AbstractionRule default_rule;
        return show_dialog_response(response, default_rule);
    }
    
    AppArmor::Tree::AbstractionRule new_rule(0, 0, add_abstraction.ab_entry->get_text());
    return show_dialog_response(response, new_rule);
}
