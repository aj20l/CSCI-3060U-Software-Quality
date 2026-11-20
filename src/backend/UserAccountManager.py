import backend.Codes as Code

class Account:
    def __init__(self, username, privilege, credits):
        self.username = username
        self.privilege = privilege
        self.credits = credits

    def __str__(self):
        return f"{self.username.ljust(Code.USERNAME_WIDTH, " ")} {self.privilege.ljust(Code.TYPE_WIDTH, " ")} {self.credits.ljust(Code.CREDIT_WIDTH, " ")}"

    def get_username(self):
        return self.username

    def set_credits(self, credits):
        self.credits = credits

    def get_credits(self):
        return self.credits

class UserAccountsManager:
    def __init__(self):
        self.current_accounts = []

    def load_account(self, data):
        username = data[:16]
        privilege = data[16:19]
        credits = data [19:29]
        self.current_accounts.append(Account(username, privilege, credits))

    def get_account(self, username):
        for account in self.current_accounts:
            if account.get_username() == username:
                return account

    def list_accounts(self):
        for account in self.current_accounts:
            print(account)





