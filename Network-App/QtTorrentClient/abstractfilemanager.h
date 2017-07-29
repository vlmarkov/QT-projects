#ifndef _ABSTRACT_FILE_MANAGER_H_
#define _ABSTRACT_FILE_MANAGER_H_

/*
 * Declare an abstract base class for all file i/o procedures
 */
class AbstractFileManager {
    public:
        virtual void f1() = 0;
        virtual void f2() = 0;
        virtual void f3() = 0;
};

#endif // _ABSTRACT_FILE_MANAGER_H_

