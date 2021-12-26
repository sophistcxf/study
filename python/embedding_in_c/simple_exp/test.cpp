#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>

#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid = getpid();
    std::cout << "pid " << pid << std::endl;

    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }
    Py_SetProgramName(program);  /* optional but recommended */
    Py_Initialize();
    PyRun_SimpleString("from time import time,ctime\n"
            "print('Today is', ctime(time()))\n");


    // 从文件执行 python
    FILE* fp = fopen("test.py", "rb");
    PyRun_SimpleFile(fp, "test.py");

    // 多次执行时，python解释器的状态是存储的
    for (int i = 0; i < 10; ++i) {
        fseek(fp, 0, SEEK_SET);
        PyRun_SimpleFile(fp, "test.py");
    }

    fclose(fp);

    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    PyMem_RawFree(program);
    return 0;
}
