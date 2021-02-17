# Simple bank_atm_controller Project
### Simple bank atm controller API project

## Overview

This project is intended to implement functions of the controller of bank ATM(Automatic Teller Machine). 

Please note that this project is created to show how ATM controller functions can be designed as Python API extension functions, implemented in C and test the extension module with the Python test program.

The APIs are defined in very simplified way in order to demonstrate the core transactions of ATM at the moment. :)

Please email kyoungje@gmail.com if you find any questions.

---
## Prerequistite and Build the project
1. Download GCC compiler and Python3 to your build environment. 
    Note that I've implemented this code with Python3 on my Ubuntu machine.)

2. Clone this project using the following command
   ```
   git clone https://github.com/kyoungje/bank_atm_controller.git
   ```
3. Run the following command to compile and install the module
   ```
   sudo python3 setup.py install
   ```
4. To see if the module has been compiled and installed, run the help of the module after importing the atmctrl module. (ie. `help(atmctrl)`)
   
   The function list and information should be displayed like "API function list" shows as below.

---
## Python build troubleshoot
You need to check the version of Python on your system and run commands like the following examples.

1. fatal error: Python.h: No such file or directory
    - sudo apt-get install python3-dev 

2. ModuleNotFoundError: No module named 'distutils.core'
    - sudo apt install python3.8-distutils
   
---
## Requriements
1. The following scenarios are supported.
   ```
   Insert card -> PIN number -> Select Account -> do Balance/Deposite/Withdraw
   ```
2. At minimum, 1 dollar bill can be transacted.

3. Bank always admits the PIN number validation.
   
---
## API function list 

    Help on module atmctrl:

    NAME
        atmctrl

    FUNCTIONS
        deposit(...)
            Desposit money to the account id that is passed as an argument
        
        getAccounts(...)
            Get the list of account data that is comprised of id, balance and name
        
        getBalance(...)
            Get the current balance by the account id that is passed as an argument
        
        getStatus(...)
            Get status of the ATM controller
        
        insertCard(...)
            To give the card insertion event to the ATM controller
        
        validatePIN(...)
            Validate PIN code from application and return the result (You can see the result by atmctrl_getStatus() as well
        
        withdraw(...)
            Withdraw money from the account id that is passed as an argument

    DATA
        ATMCTRL_RET_FAIL_ACCOUNT = -2
        ATMCTRL_RET_FAIL_ARGUMENT = -1
        ATMCTRL_RET_SUCCESS = 0
        ATMCTRL_STATUS_AUTHENTICATED = 2
        ATMCTRL_STATUS_IDLE = 0
        ATMCTRL_STATUS_INSERTED = 1

    FILE
        /usr/local/lib/python3.8/dist-packages/atmctrl.cpython-38-x86_64-linux-gnu.so


---
## Test
Run **test.py** and then you can actually use the functions of the atmctrl module by choosing menus.

 `Python3 test.py`

    [1] Show and Select Account
    [2] Show Balance 
    [3] Withdraw 
    [4] Deposit 
    [5] Exit 

    Enter your selection: 

If you want to look at the flow of APIs, see the log file in the same directory.

`atmctrl_log.txt`
