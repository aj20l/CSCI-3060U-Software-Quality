import Codes as Code

# help class for accounts information
class Account:
    def __init__(self, username, privilege, credits):
        self.username = username
        self.privilege = privilege
        self.credits = credits

    def __str__(self):
        return f"{self.username.ljust(Code.USERNAME_WIDTH, " ")}{self.privilege.ljust(Code.TYPE_WIDTH, " ")}{self.credits.ljust(Code.CREDIT_WIDTH, " ")}"

    def get_username(self):
        return self.username

    def get_credits(self):
        return self.credits
    
    def set_credits(self, credits):
        self.credits = credits

# manager class for user related data processing
class UserAccountsManager:
    def __init__(self):
        self.current_accounts = []

    # splices a transactions input data to obtain needed data to load an account
    def load_account(self, data):
        username = data[:16]
        privilege = data[16:19]
        credits = data [19:29]
        self.current_accounts.append(Account(username, privilege, credits))

    # returns an account bases of if it matches the user being searched
    def get_account(self, username):
        for account in self.current_accounts:
            if account.get_username() == username:
                return account

    # returns the list of current accounts
    def get_accounts(self):
        return self.current_accounts

    # removes an account from the matched account
    def remove_account(self, r_account):
        for index, account in enumerate(self.current_accounts):
            if account == r_account:
                self.current_accounts.pop(index)

    def list_accounts(self):
        for account in self.current_accounts:
            print(account)





