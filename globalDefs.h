#ifndef GLOBALDEFS_H
#define GLOBALDEFS_H

#define DELETEPTR(ptr) if(NULL != ptr) \
{ \
    delete ptr; \
    ptr = NULL; \
}

#endif // GLOBALDEFS_H
