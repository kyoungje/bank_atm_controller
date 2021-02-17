#include <stdarg.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdio.h>

#include "Python.h"

// enum constants for ATM controller status
enum AtmCtrlStatus {
    ATMCTRL_STATUS_IDLE = 0,
    ATMCTRL_STATUS_INSERTED,
    ATMCTRL_STATUS_AUTHENTICATED,
};

// enum constants for return values
enum AtmCtrlReturn {
    ATMCTRL_RET_SUCCESS = 0,
    ATMCTRL_RET_FAIL_ARGUMENT = -1,
    ATMCTRL_RET_FAIL_ACCOUNT = -2,
    ATMCTRL_RET_FAIL_INTERNAL = -3,
};

typedef struct _AtmCtrlAccount { 
    unsigned long id; 
    unsigned long balance;
    char name[256];
} AtmCtrlAccount;

#define MAX_ACCOUNT_NUM             5

static PyObject *ErrorObject;

static enum AtmCtrlStatus CurrentStatus = ATMCTRL_STATUS_IDLE;
static AtmCtrlAccount AccountData[MAX_ACCOUNT_NUM];
 
static void write_log(const char *fmt, ...) 
{
    FILE *fp;
	char date[20];
	struct timeval tv;
    va_list args;

    fp = fopen("./atmctrl_log.txt", "at+");

    // write timestamp first
	gettimeofday(&tv, NULL);
	strftime(date, sizeof(date) / sizeof(*date), "%Y-%m-%d %H:%M:%S", gmtime(&tv.tv_sec));
	fprintf(fp, "[%s.%03d] ", date, tv.tv_usec / 1000);

    // and write actual log here
    va_start (args, fmt);
    vfprintf(fp, fmt, args);
    va_end (args);

    fclose(fp);
}

static PyObject*  return_error(unsigned int error_num) 
{
    write_log("Failed!! error id [%d]", error_num);

    return Py_BuildValue("i", error_num);
}

// ATM controller API functions
static PyObject* atmctrl_getStatus(PyObject *self, PyObject *args) 
{   
    write_log("getStatus: current status [%d]\n", CurrentStatus);

    return Py_BuildValue("i", CurrentStatus);   
}

static PyObject* atmctrl_insertCard(PyObject *self, PyObject *args) 
{   
    write_log("insertCard event!\n");

    CurrentStatus = ATMCTRL_STATUS_INSERTED;

    return Py_BuildValue("i", ATMCTRL_RET_SUCCESS);   
}

static PyObject* atmctrl_validatePIN(PyObject *self, PyObject *args) 
{   
    long pin;
   
    if (!PyArg_ParseTuple(args, "l", &pin))
        return NULL;

    write_log("validatePIN: input PIN [%d]\n", pin);

    // Currently PIN is alway validated and return success
    CurrentStatus = ATMCTRL_STATUS_AUTHENTICATED;

    return Py_BuildValue("i", ATMCTRL_RET_SUCCESS);   
}

static PyObject* atmctrl_getAccounts(PyObject *self, PyObject *args) 
{   
    write_log("getAccounts: Entry\n");

    unsigned int tuple_len = 3;   // id, balance, name    
    PyObject *account_list = PyList_New(0);
    if (account_list == NULL)
        return return_error(ATMCTRL_RET_FAIL_INTERNAL);

    // Create the account list
    for (int i = 0; i < MAX_ACCOUNT_NUM; i++) {
        PyObject * the_tuple = PyTuple_New(tuple_len);
        if (the_tuple == NULL)
            return return_error(ATMCTRL_RET_FAIL_INTERNAL);

        PyObject *id_obj = PyLong_FromLong(AccountData[i].id);
        if(id_obj == NULL) 
            return return_error(ATMCTRL_RET_FAIL_INTERNAL);    

        PyTuple_SET_ITEM(the_tuple, 0, id_obj);

        PyObject *bal_obj = PyLong_FromLong(AccountData[i].balance);
        if (bal_obj == NULL)
            return return_error(ATMCTRL_RET_FAIL_INTERNAL);

        PyTuple_SET_ITEM(the_tuple, 1, bal_obj);

        PyObject *name_obj = PyUnicode_FromString(AccountData[i].name);
        if (name_obj == NULL)
            return return_error(ATMCTRL_RET_FAIL_INTERNAL);
    
        PyTuple_SET_ITEM(the_tuple, 2, name_obj);
        
        if (PyList_Append(account_list, the_tuple) == -1) {
            return return_error(ATMCTRL_RET_FAIL_INTERNAL);
        }
    }
    write_log("getAccounts: End\n");

    return account_list;   
}

static PyObject* atmctrl_getBalance(PyObject *self, PyObject *args) 
{   
    unsigned long id;
   
    if (!PyArg_ParseTuple(args, "l", &id))
        return return_error(ATMCTRL_RET_FAIL_ARGUMENT);

    if (id >= MAX_ACCOUNT_NUM)
        return return_error(ATMCTRL_RET_FAIL_ACCOUNT);

    write_log("getBalance: id [%d], balance [%d]\n", id);

    return Py_BuildValue("i", AccountData[id].balance);   
}

static PyObject* atmctrl_withdraw(PyObject *self, PyObject *args) 
{   
    unsigned long id, amount;
   
    if (!PyArg_ParseTuple(args, "ll", &id, &amount))
        return return_error(ATMCTRL_RET_FAIL_ARGUMENT);

    if (id >= MAX_ACCOUNT_NUM)
        return return_error(ATMCTRL_RET_FAIL_ACCOUNT);

    if (amount > AccountData[id].balance)
        return return_error(ATMCTRL_RET_FAIL_ACCOUNT);

    AccountData[id].balance -= amount;

    write_log("withdraw: id [%d], amount [%d]\n", id, amount);

    return Py_BuildValue("i", ATMCTRL_RET_SUCCESS);   
}

static PyObject* atmctrl_deposit(PyObject *self, PyObject *args) 
{   
    unsigned long id, amount;
   
    if (!PyArg_ParseTuple(args, "ll", &id, &amount))
        return return_error(ATMCTRL_RET_FAIL_ARGUMENT);

    if (id >= MAX_ACCOUNT_NUM)
        return return_error(ATMCTRL_RET_FAIL_ACCOUNT);

    AccountData[id].balance += amount;

    write_log("deposit: id [%d], amount [%d]\n", id, amount);

    return Py_BuildValue("i", ATMCTRL_RET_SUCCESS);   
}

static struct PyMethodDef methods[] =
{
    {"getStatus", atmctrl_getStatus, METH_VARARGS, "Get status of the ATM controller"},
    {"insertCard", atmctrl_insertCard, METH_VARARGS, "To give the card insertion event to the ATM controller"},
    {"validatePIN", atmctrl_validatePIN, METH_VARARGS, "Validate PIN code from application and return the result (You can see the result by atmctrl_getStatus() as well"},
    {"getAccounts", atmctrl_getAccounts, METH_VARARGS, "Get the list of account data that is comprised of id, balance and name"},
    {"getBalance", atmctrl_getBalance, METH_VARARGS, "Get the current balance by the account id that is passed as an argument"},
    {"withdraw", atmctrl_withdraw, METH_VARARGS, "Withdraw money from the account id that is passed as an argument"},
    {"deposit", atmctrl_deposit, METH_VARARGS, "Desposit money to the account id that is passed as an argument"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef atmctrlDef =
{
    PyModuleDef_HEAD_INIT,
    "atmctrl", /* name of module */
    "",          /* module documentation, may be NULL */
    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    methods
};

PyMODINIT_FUNC PyInit_atmctrl(void)
{
    PyObject *module;
    module = PyModule_Create(&atmctrlDef);
    if (module == NULL) return NULL;
    
    // Add macros to module
    PyModule_AddIntMacro(module, ATMCTRL_STATUS_IDLE);
    PyModule_AddIntMacro(module, ATMCTRL_STATUS_INSERTED);
    PyModule_AddIntMacro(module, ATMCTRL_STATUS_AUTHENTICATED);

    PyModule_AddIntMacro(module, ATMCTRL_RET_SUCCESS);
    PyModule_AddIntMacro(module, ATMCTRL_RET_FAIL_ARGUMENT);
    PyModule_AddIntMacro(module, ATMCTRL_RET_FAIL_ACCOUNT);

    // Initialize accounts
    for (int i=0; i < MAX_ACCOUNT_NUM; i++) {
        AccountData[i].id = i;
        AccountData[i].balance = 0;
        sprintf(AccountData[i].name, "Account-%d", i);
    }

    return module;
}
