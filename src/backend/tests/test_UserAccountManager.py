import pytest

from UserAccountManager import UserAccountsManager, Account
import Codes as Code

class TestAccount:
    def test__init__(self):
        username = "admin01        "
        privilege = "AA"
        credits = "000500.00"

        account = Account(username, privilege, credits)

        assert account.username == username
        assert account.privilege == privilege
        assert account.credits == credits

    def test__str__(self):
        username = "admin01        "
        privilege = "AA"
        credits = "000500.00"
        expected = username + " " + privilege + " " + credits

        account = Account(username, privilege, credits)

        assert str(account) == expected

    def test_get_username(self):
        username = "admin01        "
        privilege = "AA"
        credits = "000500.00"

        account = Account(username, privilege, credits)

        assert account.get_username() == username

    def test_get_credits(self):
        username = "admin01        "
        privilege = "AA"
        credits = "000500.00"

        account = Account(username, privilege, credits)

        assert account.get_credits() == credits

    def test_set_credits(self):
        username = "admin01        "
        privilege = "AA"
        credits = "000500.00"
        new_credits = "000400.00"

        account = Account(username, privilege, credits)
        account.set_credits(new_credits)

        assert account.get_credits() == new_credits


class TestUserAccountManager:
    def test__init__(self):
        manager = UserAccountsManager()
        assert manager.current_accounts == []

    def test_load_account(self):
        username = "admin01        "
        privilege = "AA"
        credits = "000500.00"
        data = username + " " + privilege + " " + credits

        manager = UserAccountsManager()
        manager.load_account(data)

        result = manager.current_accounts

        assert len(result) == 1
        assert result[0].username == username
        assert result[0].privilege == privilege
        assert result[0].credits == credits

    def test_get_account(self):
        username = "admin01        "
        privilege = "AA"
        credits = "000500.00"

        manager = UserAccountsManager()
        account = Account(username, privilege, credits)
        manager.current_accounts.append(account)

        assert manager.get_account(username) == account
        

    def test_get_accounts(self):
        username_one = "admin01        "
        privilege_one = "AA"
        credits_one = "000500.00"
        username_two = "fulluser       "
        privilege_two = "FS"
        credits_two = "000100.00"

        manager = UserAccountsManager()

        account_one = Account(username_one, privilege_one, credits_one)
        account_two = Account(username_two, privilege_two, credits_two)

        manager.current_accounts.append(account_one)
        manager.current_accounts.append(account_two)

        assert manager.get_accounts() == [account_one, account_two]

    def test_remove_account(self):
        username_one = "admin01        "
        privilege_one = "AA"
        credits_one = "000500.00"
        username_two = "fulluser       "
        privilege_two = "FS"
        credits_two = "000100.00"

        account_one = Account(username_one, privilege_one, credits_one)
        account_two = Account(username_two, privilege_two, credits_two)

        manager = UserAccountsManager()
        manager.current_accounts.append(account_one)
        manager.current_accounts.append(account_two)

        manager.remove_account(account_one)

        assert manager.current_accounts == [account_two]
