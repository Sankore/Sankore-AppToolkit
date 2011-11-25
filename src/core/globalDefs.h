#ifndef GLOBALDEFS_H
#define GLOBALDEFS_H

#define APP_NAME        "Sankore Application Toolkit"
#define APP_ORG         "Open-Sankore.org"
#define APP_VERSION     1.1
#define APP_DOMAIN      "http://open-sankore.org/"

#define DELETEPTR(ptr) if(NULL != ptr) \
{ \
    delete ptr; \
    ptr = NULL; \
}

#endif // GLOBALDEFS_H
