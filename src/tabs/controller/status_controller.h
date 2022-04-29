#ifndef TABS_STATUS_H
#define TABS_STATUS_H

#include "../column_header.h"
#include "jsoncpp/json/json.h"

#include <gtkmm/checkbutton.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/searchentry.h>
#include <memory>

template<class Tab>
class StatusController
{
public:
  StatusController();

  std::shared_ptr<Tab> get_tab();

  // Virtual destructor to avoid a warning
  virtual ~StatusController() = default;

  // Default copy-constructor, move constructor, move assignment operator, and copy assignment operator
  StatusController(const StatusController<Tab> &other) noexcept = default;
  StatusController(StatusController<Tab> &&other)      noexcept = default;
  StatusController &operator=(const StatusController<Tab> &other) noexcept = default;
  StatusController &operator=(StatusController<Tab> &&other)      noexcept = default;

protected:
  bool filter(const Gtk::TreeModel::iterator &node);
  bool filter_children(const Gtk::TreeModel::iterator &node);

  /**
   * @brief Decide whether a string should be added to the table
   *
   * @details
   * Takes a string as input and decides whether it should be added the table.
   * Uses the text in the searchbar and the state of the checkboxes to determine
   * whether the string can be added.
   *
   * If `use_regex` is true, then the `match_case` will be ignored.
   *
   * @returns `true` if the string should be added, `false` if it should not
   */
  static bool filter(const std::string &str, const std::string &rule, const bool &use_regex, const bool &match_case,
                     const bool &whole_word);

  // Should probably add a description!
  static Json::Value parse_JSON(const std::string &raw_json);

private:
  // The corresponding tab is /tabs/view
  std::shared_ptr<Tab> tab;

  // clang-tidy complains about the `COLUMN_TYPE_STRING` macro, so we assign it here and tell clang-tidy not to look at it
  static constexpr unsigned int COLUMN_TYPE_STRING = G_TYPE_STRING; // NOLINT
};

#endif // TABS_STATUS_H
