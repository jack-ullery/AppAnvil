#ifndef TEST_SRC_TABS_MODEL_PROCESS_ADAPTER_TEST_H
#define TEST_SRC_TABS_MODEL_PROCESS_ADAPTER_TEST_H

#include "../../../../src/tabs/controller/processes_controller.cc"
#include "../../../../src/tabs/model/database.h"
#include "../../../../src/tabs/model/process_adapter.cc"
#include "../../../../src/tabs/view/processes.h"
#include "database_mock.h"
#include "status_column_record_mock.h"

// template class ProcessesController<Processes, Database, ProcessAdapterChild>;
template class ProcessesController<Processes, DatabaseMock, ProcessAdapter<DatabaseMock, StatusColumnRecord>>;

#endif // TEST_SRC_TABS_MODEL_PROCESS_ADAPTER_TEST_H