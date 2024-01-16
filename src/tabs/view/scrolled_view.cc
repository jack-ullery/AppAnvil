#include "common.h"
#include "scrolled_view.h"

ScrolledView::ScrolledView(const std::string &title)
    :   builder{ Gtk::Builder::create_from_resource("/resources/scrolled_view.glade") },
        frame{Common::get_widget<Gtk::Frame>("frame", builder)},
        frame_title{Common::get_widget<Gtk::Label>("frame_title", builder)},
        scrolled_window{Common::get_widget<Gtk::ScrolledWindow>("scrolled_window", builder)},
        tree_view{Common::get_widget<Gtk::TreeView>("tree_view", builder)}
{
    frame_title->set_text(title);
    this->add(*frame);
}

std::shared_ptr<Gtk::TreeView> ScrolledView::get_view()
{
    return tree_view;
}
