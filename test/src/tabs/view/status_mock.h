#include "../../../../src/tabs/search_info.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeview.h>

// Mock object of the Status class in ./src/tab/view/status.h
class StatusMock
{
    public:
        MOCK_METHOD(std::shared_ptr<Gtk::TreeView>, get_view, ());
        MOCK_METHOD(std::shared_ptr<Gtk::ScrolledWindow>, get_window, ());
        MOCK_METHOD(void, set_apply_label_text, (const std::string &str));
        MOCK_METHOD(void, set_status_label_text, (const std::string &str));
        MOCK_METHOD(void, set_refresh_signal_handler, (const Glib::SignalProxyProperty::SlotType &func));
        MOCK_METHOD(void, set_apply_signal_handler, (const Glib::SignalProxyProperty::SlotType &func));
        MOCK_METHOD(Glib::ustring, get_selection_text, ());
        MOCK_METHOD(SearchInfo, get_search_info, ());
};
