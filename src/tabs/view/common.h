#ifndef TABS_VIEW_COMMON_FUNCTIONS_H
#define TABS_VIEW_COMMON_FUNCTIONS_H

#include <gtkmm/builder.h>
#include <memory>

namespace Common
{
  // Gets a shared pointer to a widget defined by a Gtk::Builder
  template<typename T_Widget>
  std::unique_ptr<T_Widget> get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder)
  {
    T_Widget *raw_addr = builder->get_widget<T_Widget>(name);
    return std::unique_ptr<T_Widget>(raw_addr);
  }

  // Gets a unique pointer to a widget defined by a Gtk::Builder
  template<typename T_Widget>
  std::shared_ptr<T_Widget> get_widget_shared(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder)
  {
    T_Widget *raw_addr = builder->get_widget<T_Widget>(name);
    return std::shared_ptr<T_Widget>(raw_addr);
  }
}

#endif // TABS_VIEW_COMMON_FUNCTIONS_H