
typedef enum DriverAttriTypedef_
{
    Private = 0,
    Public = 1
}DriverAttriTypedef;

typedef struct DriverModuleTypedef_
{
    void * PrivateData;
    DriverAttriTypedef DriverAttri;
}



