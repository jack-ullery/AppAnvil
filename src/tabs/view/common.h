#ifndef TABS_VIEW_COMMON_FUNCTIONS_H
#define TABS_VIEW_COMMON_FUNCTIONS_H

#include <gtkmm/bin.h>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/label.h>
#include <memory>

class Common
{
public:
  // Gets a shared pointer to a widget defined by a Gtk::Builder
  template<typename T_Widget>
  static std::shared_ptr<T_Widget> get_widget_shared(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder);

  // Gets a unique pointer to a widget defined by a Gtk::Builder
  template<typename T_Widget>
  static std::unique_ptr<T_Widget> get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder);
};

// Included definition for functions in header file to avoid annoying linker issues with templates

template<typename T_Widget>
std::unique_ptr<T_Widget> Common::get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder)
{
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

template<typename T_Widget>
std::shared_ptr<T_Widget> Common::get_widget_shared(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder)
{
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::shared_ptr<T_Widget>(raw_addr);
}

#endif // TABS_VIEW_COMMON_FUNCTIONS_H