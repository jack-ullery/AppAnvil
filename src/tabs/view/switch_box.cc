#include "switch_box.h"
#include "switch_box.h"
#include "switch_row.h"

SwitchBox::SwitchBox()
{
  auto ptr = std::shared_ptr<SwitchRow>(new SwitchRow("Title", "Subtitle"));
  switch_list.push_back(ptr);
  this->add(*ptr);
}
