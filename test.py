# test program for the ATM controller
import atmctrl

def showAccounts():
    print("\n===================================\n")
    print("\n[ List of Accounts ]\n")

    account_list = atmctrl.getAccounts()
    for acc in account_list:
        print("ID: ", acc[0], ", Balance: $", acc[1], ",\t Name: ", acc[2])

    print("\n===================================")
    

if __name__ == '__main__':    

    acc_id = 0

    # ATM Controller menu loop
    while True:
        # Wake up from idle status
        input("\nInsert card and press any key...")

        # Reading PIN from user
        id = int(input("\nEnter account pin: "))

        # Validate the PIN
        if atmctrl.ATMCTRL_RET_SUCCESS != atmctrl.validatePIN(id):
            print("\nYou're not allowed to proceed. start again.")
            continue

        # Get status of the ATM controller
        if atmctrl.ATMCTRL_STATUS_AUTHENTICATED != atmctrl.getStatus():
            print("\nYou're not allowed to proceed. start again.")
            continue

        showAccounts()
            
        # Iterating over account session
        while True:
            # Printing menu
            print("\n [1] Show and Select Account\n [2] Show Balance \n [3] Withdraw \n [4] Deposit \n [5] Exit ")

            # Reading selection of menu
            selection = int(input("\nEnter your selection: "))

            # Select accounts
            if selection == 1:
                showAccounts()

                # Select account
                acc_id = int(input("\nEnter account id: "))
                print("\nCurrent account id: ", acc_id)
                continue

            # Show Balance
            if selection == 2:
                # Printing balance
                print("\nCurrent account id", acc_id, ", balance is $", atmctrl.getBalance(acc_id))

            # Withdraw
            elif selection == 3:
                # Reading amount
                amt = int(input("\nEnter the amount to withdraw: "))

                if amt < atmctrl.getBalance(acc_id):
                    # Calling withdraw method
                    if atmctrl.ATMCTRL_RET_SUCCESS == atmctrl.withdraw(acc_id, amt):                 
                        # Printing updated balance
                        print("\nOK.. Current balance is $" + str(atmctrl.getBalance(acc_id)))
                    else:
                        print("\Withdraw job failed..")
                else:
                    print("\nYour balance is not enough: current balance is $" + str(atmctrl.getBalance(acc_id)))

            # Deposit
            elif selection == 4:
                # Reading amount
                amt = int(input("\nEnter amount to deposit: "))

                # Calling deposit method
                if atmctrl.ATMCTRL_RET_SUCCESS == atmctrl.deposit(acc_id, amt):                 
                    # Printing updated balance
                    print("\nOK.. Current balance is $" + str(atmctrl.getBalance(acc_id)) )
                else:
                    print("\nDeposit job failed..")                

            elif selection == 5:
                print("\nBye!")
                break

            # Any other choice
            else:
                print("\nInvalid choice. try again!")
