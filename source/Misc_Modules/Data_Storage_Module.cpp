#include <Misc_Modules/Data_Storage_Module.h>

using namespace LMD;


Data_Storage_Module::Data_Storage_Module()
{

}

Data_Storage_Module::~Data_Storage_Module()
{
    delete m_data;
}





Data_Storage_Module_Stub::~Data_Storage_Module_Stub()
{
    delete data_stub;
}
