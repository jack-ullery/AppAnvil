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
  // For production
  StatusController();
  // For unit testing
  explicit StatusController(std::shared_ptr<Tab> tab);

  // Returns the Tab that this controller communicates with
  // This tab will be some GTKmm widget from /src/tabs/view
  std::shared_ptr<Tab> get_tab();

  // Virtual destructor to avoid a warning
  virtual ~StatusController() = default;

  // Default copy-constructor, move constructor, move assignment operator, and copy assignment operator
  StatusController(const StatusController<Tab> &other) noexcept            = default;
  StatusController(StatusController<Tab> &&other) noexcept                 = default;
  StatusController &operator=(const StatusController<Tab> &other) noexcept = default;
  StatusController &operator=(StatusController<Tab> &&other) noexcept      = default;

protected:
  /**
   * @brief Decide whether a TreeRow should be visible
   *
   * @details
   * Takes a pointer to a TreeRow as input and decides whether it should be visible.
   * Uses the text in the searchbar and the state of the checkboxes to determine
   * whether the TreeRow can be shown.
   *
   * If `use_regex` is true, then the `match_case` will be ignored.
   *
   * @returns `true` if the TreeRow should be shown, `false` if it should not
   */
  bool filter(const Gtk::TreeModel::const_iterator &node);

  /**
   * @brief Decide whether a string should be visible
   *
   * @details
   * Takes a string from the column of a row as input and decides whether it should be visible.
   * Uses the text in the searchbar and the state of the checkboxes to determine
   * whether the string can be shown.
   *
   * If `use_regex` is true, then the `match_case` will be ignored.
   *
   * @returns `true` if the string should be shown, `false` if it should not
   */
  static bool should_filter(const std::string &str,
                            const std::string &rule,
                            const bool &use_regex,
                            const bool &match_case,
                            const bool &whole_word);

  // Should probably add a description!
  static Json::Value parse_JSON(const std::string &raw_json);

private:
  // The corresponding tab is /tabs/view
  std::shared_ptr<Tab> tab;

  // clang-tidy complains about the `COLUMN_TYPE_STRING` macro, so we assign it here and tell clang-tidy not to look at it
  static constexpr unsigned int COLUMN_TYPE_STRING = G_TYPE_STRING; // NOLINT
  static constexpr unsigned int COLUMN_TYPE_UINT   = G_TYPE_UINT;   // NOLINT
};

#endif // TABS_STATUS_H
