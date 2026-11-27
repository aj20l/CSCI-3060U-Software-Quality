from TransactionProcessor import TransactionProcessor, Transaction
from FileManager import FileManager
from UserAccountManager import UserAccountsManager, Account
from GameCollectionManager import GameCollectionManager, GameCollection
from AvailableGamesManager import AvailableGamesManager, Game
import Codes as Code

class TestTransaction:
    def test__init__(self):
        code = "01"
        transaction_in = "newplayer       FS 000000.00"
        data = code + " " + transaction_in

        transaction = Transaction(data)

        assert transaction.code == code
        assert transaction.transaction == transaction_in

    def test__str__(self):
        code = "01"
        transaction_in = "newplayer       FS 000000.00"
        data = code + " " + transaction_in

        transaction = Transaction(data)
        assert str(transaction) == transaction_in

    def test_get_code(self):
        code = "01"
        transaction_in = "newplayer       FS 000000.00"
        data = code + " " + transaction_in

        transaction = Transaction(data)
        assert transaction.get_code() == code

class TestTransactionProcessor:
    def test__init__(self):
        transaction_processor = TransactionProcessor(Code.TEST_DATA_PATH)

        assert transaction_processor.file_manager.filePath == Code.TEST_DATA_PATH
        assert transaction_processor.user_account_manager.current_accounts == []
        assert transaction_processor.game_collection_manager.game_collection == []
        assert transaction_processor.available_games_manager.available_games == []

    # https://chatgpt.com/s/t_6b084c7a8620819196a6c090fb9c33ee
    def test_load_files(self):
        
        transaction_processor = TransactionProcessor(Code.TEST_DATA_PATH)
        transaction_processor.load_files()

        assert transaction_processor.user_account_manager.current_accounts is not None
        assert transaction_processor.game_collection_manager.game_collection is not None
        assert transaction_processor.available_games_manager.available_games is not None

    def test_write_files(self):
        transaction_processor = TransactionProcessor(Code.TEST_DATA_PATH)
        transaction_processor.load_files()
        result = transaction_processor.write_files()

        assert result is None

    def test_process_daily_transaction_file(self):
        transaction_processor = TransactionProcessor(Code.TEST_DATA_PATH)
        transaction_processor.load_files()
        result = transaction_processor.process_daily_transaction_file()

        assert result is None

    def test_process_transaction_create(self):
        transaction_code = "01"
        username = "newplayer        "
        privilege = "FS"
        credit_amount = "000000.00"

        transaction_data = transaction_code + " " + username + " " + privilege + " " + credit_amount

        transaction_processor = TransactionProcessor(Code.TEST_DATA_PATH)
        transaction = Transaction(transaction_data)
        transaction_processor.process_transactions(transaction)

        assert transaction_processor.user_account_manager.current_accounts is not None

    def test_process_transaction_delete(self):
        transaction_code = "02"
        username = "seller01       "
        privilege = "SS"
        credit_amount = "000050.00"

        transaction_data = transaction_code + " " + username + " " + privilege + " " + credit_amount

        account = Account(username, privilege, credit_amount)
        transaction = Transaction(transaction_data)

        transaction_processor = TransactionProcessor(Code.TEST_DATA_PATH)
        transaction_processor.user_account_manager.current_accounts.append(account)
        transaction_processor.process_transactions(transaction)

        assert transaction_processor.user_account_manager.current_accounts == []

    def test_process_transaction_sell(self):
        transaction_code = "03"
        game_name = "Portal                    "
        seller = "admin01        "
        price = "025.00"
        transaction_data = transaction_code + " " + game_name + " " + seller + " " + price

        transaction = Transaction(transaction_data)
        transaction_processor = TransactionProcessor(Code.TEST_DATA_PATH)
        transaction_processor.process_transactions(transaction)

        assert transaction_processor.available_games_manager.available_games is not None

    def test_process_transaction_buy(self):
        transaction_code = "04"
        game_name = "GTA VI                    "
        seller = "seller01       "
        buyer = "admin01        "
        price = "024.99"
        transaction_data = transaction_code + " " + game_name + " " + seller + " " + buyer + " " + price

        transaction = Transaction(transaction_data)
        transaction_processor = TransactionProcessor(Code.TEST_DATA_PATH)
        transaction_processor.process_transactions(transaction)

        assert transaction_processor.game_collection_manager.game_collection is not None

    def test_process_transaction_refund(self):
        target_username = "admin01        "
        target_privilege = "AA"
        target_current_credit = "000500.00"

        recipient_username = "seller01       "
        recipient_privilege = "SS"
        recipient_current_credit = "000050.00"
        
        transaction_code = "05"
        refund_amount = "000024.99"
        transaction_data = transaction_code + " " + target_username + " " + recipient_username + " " + refund_amount
        target_new_credit = float(target_current_credit) + float(refund_amount)
        target_new_credit = f"{target_new_credit:.2f}".rjust(Code.CREDIT_WIDTH,"0")

        recipient_new_credit = float(recipient_current_credit) - float(refund_amount)
        recipient_new_credit = f"{recipient_new_credit:.2f}".rjust(Code.CREDIT_WIDTH,"0")

        target_account = Account(target_username, target_privilege, target_current_credit)
        recipient_account = Account(recipient_username, recipient_privilege, recipient_current_credit)

        transaction = Transaction(transaction_data)
        transaction_processor = TransactionProcessor(Code.TEST_DATA_PATH)
        transaction_processor.user_account_manager.current_accounts.append(target_account)
        transaction_processor.user_account_manager.current_accounts.append(recipient_account)
        transaction_processor.process_transactions(transaction)

        assert transaction_processor.user_account_manager.get_account(target_username).get_credits() == target_new_credit
        assert transaction_processor.user_account_manager.get_account(recipient_username).get_credits() == recipient_new_credit

    def test_process_transaction_add_credit(self):
        username = "admin01        "
        privilege = "AA"
        current_amount = "000500.00"

        transaction_code = "06"
        add_amount = "000100.00"
        transaction_data = transaction_code + " " + username + " " + privilege + " " + add_amount
        expected_new_credit_amount = float(current_amount) + float(add_amount)
        expected_new_credit_amount = f"{expected_new_credit_amount:.2f}".rjust(Code.CREDIT_WIDTH,"0")

        account = Account(username, privilege, current_amount)
        transaction = Transaction(transaction_data)
        transaction_processor = TransactionProcessor(Code.TEST_DATA_PATH)

        transaction_processor.user_account_manager.current_accounts.append(account)
        transaction_processor.process_transactions(transaction)

        assert transaction_processor.user_account_manager.get_account(username).get_credits() == expected_new_credit_amount
        


        