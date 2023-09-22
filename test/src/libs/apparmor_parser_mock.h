#ifndef TEST_SRC_LIBS_APPARMOR_PARSER_MOCK_H
#define TEST_SRC_LIBS_APPARMOR_PARSER_MOCK_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <libappanvil/tree/AbstractionRule.hh>
#include <libappanvil/tree/FileRule.hh>
#include <libappanvil/tree/LinkRule.hh>
#include <libappanvil/tree/ProfileRule.hh>
#include <libappanvil/tree/RuleList.hh>
#include <libappanvil/tree/RuleNode.hh>

// Mock object of the AppArmor::Parser class from libappanvil
class AppArmorParserMock
{
public:
  AppArmorParserMock() {
      EXPECT_CALL(*this, hasChanges()).Times(1).WillOnce(testing::Return(false));
  }

  MOCK_METHOD(void, getProfileList, ());
  MOCK_METHOD(void, removeRule, (AppArmor::Tree::ProfileRule & profile, AppArmor::Tree::RuleNode &rule));
  MOCK_METHOD(void, removeRule, (AppArmor::Tree::ProfileRule & profile, AppArmor::Tree::RuleNode &rule, std::ostream &output));
  MOCK_METHOD(void, addRule, (AppArmor::Tree::ProfileRule & profile, const std::string &fileglob, const std::string &filemode));
  MOCK_METHOD(void,
              addRule,
              (AppArmor::Tree::ProfileRule & profile, const std::string &fileglob, const std::string &filemode, std::ostream &output));
  MOCK_METHOD(
    void,
    editRule,
    (AppArmor::Tree::ProfileRule & profile, AppArmor::Tree::FileRule &oldRule, const std::string &fileglob, const std::string &filemode));
  MOCK_METHOD(void,
              editRule,
              (AppArmor::Tree::ProfileRule & profile,
               AppArmor::Tree::FileRule &oldRule,
               const std::string &fileglob,
               const std::string &filemode,
               std::ostream &output));
  MOCK_METHOD(bool, hasChanges, ());
  MOCK_METHOD(void, saveChanges, ());
  MOCK_METHOD(void, cancelChanges, ());
  MOCK_METHOD(std::string, getPath, ());

  operator std::string() {
    return "this is a string";
  }
};

#endif