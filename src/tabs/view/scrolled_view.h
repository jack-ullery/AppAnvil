#ifndef TABS_SCROLLED_VIEW_H
#define TABS_SCROLLED_VIEW_H

#include <gtkmm/bin.h>
#include <gtkmm/builder.h>
#include <gtkmm/frame.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeview.h>
#include <memory>

class ScrolledView : public Gtk::Bin
{
public:
  explicit ScrolledView(const std::string &title);
  std::shared_ptr<Gtk::TreeView> get_view();

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Widgets
  std::shared_ptr<Gtk::Frame> frame;
  std::shared_ptr<Gtk::Label> frame_title;
  std::shared_ptr<Gtk::ScrolledWindow> scrolled_window;
  std::shared_ptr<Gtk::TreeView> tree_view;
};

#endif // TABS_SCROLLED_VIEW_H