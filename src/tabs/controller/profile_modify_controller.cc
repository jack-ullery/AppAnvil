#include "profile_modify_controller.h"

#include <apparmor_parser.hh>
#include <gtkmm/enums.h>
#include <gtkmm/messagedialog.h>
#include <iostream>
#include <memory>
#include <tree/AbstractionRule.hh>

std::shared_ptr<ProfileModify> ProfileModifyController::get_profile_modify()
{
  return modify;
}

void ProfileModifyController::intialize_abstractions()
{
  abstraction_record->clear();
  auto abstractions = this->profile->getAbstractions();

  for (auto &abstraction : abstractions) {
    // Find the last slash, and use that information to extract the filename from the abstraction
    std::string abstraction_str = abstraction.getPath();
    auto pos                    = abstraction_str.find_last_of('/');
    if (pos == std::string::npos) {
      pos = 0;
    } else {
      pos++;
    }

    // Add the data to the row
    auto shared_rule    = std::make_shared<AppArmor::Tree::AbstractionRule>(abstraction);
    std::string trimmed = abstraction_str.substr(pos);

    auto row = abstraction_record->new_row();
    row->set_value(0, shared_rule);
    row->set_value(1, trimmed);
  }
}

void ProfileModifyController::intialize_file_rules()
{
  file_rule_record->clear();
  auto rules = this->profile->getFileRules();

  for (AppArmor::Tree::FileRule &rule : rules) {
    auto shared_rule           = std::make_shared<AppArmor::Tree::FileRule>(rule);
    const std::string filename = rule.getFilename();
    const auto filemode        = rule.getFilemode();
    const auto prefix          = rule.getPrefix();

    auto row = file_rule_record->new_row();
    row->set_value(FILE_RULE_POS::Data, shared_rule);
    row->set_value(FILE_RULE_POS::Path, filename);
    row->set_value(FILE_RULE_POS::Read, filemode.getRead());
    row->set_value(FILE_RULE_POS::Write, filemode.getWrite());
    row->set_value(FILE_RULE_POS::Link, filemode.getLink());
    row->set_value(FILE_RULE_POS::Lock, filemode.getLock());
    row->set_value(FILE_RULE_POS::Exec, filemode.getExecuteMode());
    row->set_value(FILE_RULE_POS::Advanced, prefix.operator std::string());

    auto change_fun = sigc::mem_fun(*this, &ProfileModifyController::handle_file_rule_changed);
    file_rule_record->set_change_func(change_fun);
  }
}

void ProfileModifyController::update_all_tables()
{
  intialize_abstractions();
  intialize_file_rules();
}

void ProfileModifyController::handle_profile_changed()
{
  // Find the new parsed profile from the list of profiles
  for (const auto &new_profile : parser->getProfileList()) {
    if (new_profile.name() == profile->name()) {
      profile = std::make_shared<AppArmor::Profile>(new_profile);
    }
  }

  // Update all the rows and tables
  update_all_tables();
  modify->update_profile_text();

  // Decide whether the cancel/save buttons should be visible
  modify->handle_apply_visible();
}

void ProfileModifyController::handle_file_rule_changed(const std::string &path)
{
  auto row = file_rule_record->get_iter(path);

  // Get the FileRule stored in the row
  std::shared_ptr<AppArmor::Tree::FileRule> rule;
  row->get_value(FILE_RULE_POS::Data, rule);

  if (rule != nullptr) {
    const std::string filename = rule->getFilename();
    const auto filemode        = rule->getFilemode();

    bool read       = filemode.getRead();
    bool write      = filemode.getWrite();
    bool append     = filemode.getAppend();
    bool memory_map = filemode.getMemoryMap();
    bool link       = filemode.getLink();
    bool lock       = filemode.getLock();
    std::string exec_mode;

    row->get_value(FILE_RULE_POS::Read, read);
    row->get_value(FILE_RULE_POS::Write, write);
    row->get_value(FILE_RULE_POS::Link, link);
    row->get_value(FILE_RULE_POS::Lock, lock);
    row->get_value(FILE_RULE_POS::Exec, exec_mode);

    try {
      // Attempt to create a FileMode object representing the permissions of the updated rule
      AppArmor::Tree::FileMode new_filemode(read, write, append, memory_map, link, lock, exec_mode);

      if (new_filemode.empty()) {
        // If the rule no longer contains permissions, delete it
        handle_remove_rule(*rule);
      } else {
        // If the rule still contains permissions, modify it
        AppArmor::Tree::FileRule new_rule(0, -1, rule->getFilename(), new_filemode, rule->getExecTarget());
        handle_edit_rule(*rule, new_rule);
      }
    } catch (const std::exception &ex) {
      std::cerr << "Error when specifying Execute Mode:" << std::endl << ex.what() << std::endl;

      // Reset the exec_mode to its previous value
      row->set_value(FILE_RULE_POS::Exec, filemode.getExecuteMode());
    }
  } else {
    std::cerr << "Error: Could not locate AppArmor::FileRule for selected row" << std::endl;
  }
}

void show_error_message(const std::string &type_continuous_tense, const std::string &rule)
{
  std::stringstream msg;
  msg << "<span size=\"large\">Our apologies!</span>" << std::endl << std::endl;
  msg << "AppAnvil encountered an error when " << type_continuous_tense << " the following rule: " << std::endl
      << "<span font_family=\"mono\" weight=\"light\">  " << rule << "</span>" << std::endl << std::endl;
  msg << "This is probably a software bug." << std::endl << std::endl
      << "As a workaround, you might be able to make the change manually after clicking <span weight=\"heavy\">Profile Text</span>." << std::endl << std::endl;
  Gtk::MessageDialog dialog(msg.str(), true, Gtk::MessageType::MESSAGE_ERROR);
  dialog.run();
}

void ProfileModifyController::handle_edit_rule(AppArmor::Tree::FileRule &old_rule, const AppArmor::Tree::FileRule &new_rule) noexcept
{
  std::cout << "Editing Rule: " << new_rule.operator std::string() << std::endl;
  try {
    parser->editRule(*profile, old_rule, new_rule, profile_stream);
  } catch (const std::exception &ex) {
    std::cerr << "Error Editing Rule: " << ex.what() << std::endl << std::endl;
    show_error_message("editing", old_rule.operator std::string());
  }

  handle_profile_changed();
}

template<AppArmor::RuleDerived RuleType>
void ProfileModifyController::handle_remove_rule(RuleType &old_rule) noexcept
{
  std::cout << "Removing Rule: " << old_rule.operator std::string() << std::endl;
  try {
    parser->removeRule(*profile, old_rule, profile_stream);
  } catch (const std::exception &ex) {
    std::cerr << "Error Removing Rule: " << ex.what() << std::endl << std::endl;
    show_error_message("removing", old_rule.operator std::string());
  }

  handle_profile_changed();
}

template<AppArmor::RuleDerived RuleType>
void ProfileModifyController::handle_add_rule(const RuleType &new_rule) noexcept
{
  std::cout << "Add Rule: " << new_rule.operator std::string() << std::endl;
  try {
    parser->addRule(*profile, new_rule, profile_stream);
  } catch (const std::exception &ex) {
    std::cerr << "Error Adding Rule: " << ex.what() << std::endl << std::endl;
    show_error_message("adding", new_rule.operator std::string());
  }

  handle_profile_changed();
}

void ProfileModifyController::handle_cancel_called()
{
  parser->cancelChanges();
  handle_profile_changed();
}

void ProfileModifyController::handle_apply_called()
{
  int re = parser->saveChanges();
  if (re == 0) {
    handle_profile_changed();
  }
}

// Assumes column 0 holds the rule, will segfault otherwise
template<AppArmor::RuleDerived RuleType>
inline void ProfileModifyController::handle_remove_selected_rule(const std::shared_ptr<Gtk::TreeView> &view)
{
  auto selection   = view->get_selection();
  const auto nrows = selection->count_selected_rows();
  if (nrows == 1) {
    // Get the rule from the selected row
    std::shared_ptr<RuleType> rule;
    auto row = *selection->get_selected();
    row->get_value(0, rule);

    // Delete the rule
    handle_remove_rule<RuleType>(*rule);
  } else {
    std::cout << "Not removing rule because " << nrows << " are selected." << std::endl;
  }
}

void ProfileModifyController::handle_remove_abstraction_button()
{
  auto view = modify->get_abstraction_view();
  handle_remove_selected_rule<AppArmor::Tree::AbstractionRule>(view);
}

void ProfileModifyController::handle_remove_file_rule_button()
{
  auto view = modify->get_file_rule_view();
  handle_remove_selected_rule<AppArmor::Tree::FileRule>(view);
}

ProfileModifyController::ProfileModifyController(const std::shared_ptr<AppArmor::Parser> &parser,
                                                 const std::shared_ptr<AppArmor::Profile> &profile)
  : parser{ parser },
    profile{ profile },
    modify{ std::make_shared<ProfileModify>(parser, profile) },
    abstraction_record{ StatusColumnRecord::create(modify->get_abstraction_view(), abstraction_col_names) },
    file_rule_record{ StatusColumnRecord::create(modify->get_file_rule_view(), file_rule_col_names) }
{
  auto change_fun = sigc::mem_fun(*this, &ProfileModifyController::handle_file_rule_changed);
  file_rule_record->set_change_func(change_fun);

  auto handle_cancel_fun = sigc::mem_fun(*this, &ProfileModifyController::handle_cancel_called);
  auto handle_apply_fun  = sigc::mem_fun(*this, &ProfileModifyController::handle_apply_called);
  modify->connect_apply_buttons(handle_cancel_fun, handle_apply_fun);

  auto handle_prof_fun = sigc::mem_fun(*this, &ProfileModifyController::handle_profile_changed);
  modify->connect_handle_profile_changed(handle_prof_fun);

  // Connect the buttons that are used to remove things
  auto handle_remove_abstr = sigc::mem_fun(*this, &ProfileModifyController::handle_remove_abstraction_button);
  auto handle_remove_frule = sigc::mem_fun(*this, &ProfileModifyController::handle_remove_file_rule_button);
  modify->connect_handle_remove_rule(handle_remove_abstr, handle_remove_frule);

  // Connect the buttons that are used to add things
  auto handle_add_abstr = sigc::mem_fun(*this, &ProfileModifyController::handle_add_rule<AppArmor::AbstractionRule>);
  auto handle_add_frule = sigc::mem_fun(*this, &ProfileModifyController::handle_add_rule<AppArmor::FileRule>);
  modify->connect_handle_add_rule(handle_add_abstr, handle_add_frule);

  update_all_tables();
}
