from FileManager import FileManager
from UserAccountManager import UserAccountsManager
from GameCollectionManager import GameCollectionManager
from AvailableGamesManager import AvailableGamesManager

CODE_END_OF_SESSION = "00"
CODE_CREATE = "01"
CODE_DELETE = "02"
CODE_SELL = "03"
CODE_BUY = "04"
CODE_REFUND = "05"
CODE_ADD_CREDIT = "06"
END_MARKER = "END"

DATA_PATH = "../data"
CURRENT_ACCOUNT_FILE = "currentaccounts.txt"
GAMES_COLLECTION_FILE = "gamescollection.txt"
AVAILABLE_GAMES_FILE = "availablegames.txt"
MERGED_DAILY_TRANSACTION_FILE = "transout.atf"

ERROR_PREFIX = "ERROR: "

class Transaction:
    def __init__(self, transaction):
        self.transaction = transaction[3:]
        self.code = transaction[:2]

    def __str__(self):
        return f"{self.transaction}"

    def get_code(self):
        return f"{self.code}"

class TransactionProcessor:
    def __init__(self):
        self.file_manager = FileManager(DATA_PATH)
        self.user_account_manager = UserAccountsManager()
        self.game_collection_manager = GameCollectionManager()
        self.available_games = AvailableGamesManager()

    # Loads the data from the transaction, old accounts, games, and games collection file
    def load_files(self):
        for line in self.file_manager.read_file(CURRENT_ACCOUNT_FILE):
            self.user_account_manager.load_account(line)

        for line in self.file_manager.read_file(GAMES_COLLECTION_FILE):
            self.game_collection_manager.load_collection(line)

        for line in self.file_manager.read_file(AVAILABLE_GAMES_FILE):
            self.available_games.load_game(line)

        self.user_account_manager.list_accounts()
        self.game_collection_manager.list_collection()
        self.available_games.list_games()

        for line in self.file_manager.read_file(MERGED_DAILY_TRANSACTION_FILE):
            self.process_daily_transactions(Transaction(line))

    # Loads data from the daily transaction file to be processed
    def process_daily_transactions(self, transaction):
        if transaction.get_code() == CODE_END_OF_SESSION:
            pass
        elif transaction.get_code() == CODE_CREATE:
            pass
            # should edit new user account file (edits currentaccount.txt)
        elif transaction.get_code() == CODE_DELETE:
            pass
            # should edit new user account file (edits currentaccount.txt)
        elif transaction.get_code() == CODE_SELL:
            self.available_games.load_game(str(transaction))
            # should edit available games file
        elif transaction.get_code() == CODE_BUY:
            self.game_collection_manager.load_collection(str(transaction))
            # should edit game collection file
        elif transaction.get_code() == CODE_REFUND:
            transaction = str(transaction)
            buyer = transaction[:16]
            seller = transaction[16:32]
            refund_amount = float(transaction[32:42])
            
            buyer_account = self.user_account_manager.get_account(buyer)
            buyer_credit = float(buyer_account.get_credits()) + refund_amount
            buyer_account.set_credits(str(buyer_credit).rjust(9,"0"))

            seller_account = self.user_account_manager.get_account(seller)
            seller_credit = float(seller_account.get_credits()) - refund_amount
            seller_account.set_credits(str(seller_credit).rjust(9,"0"))
            # should edit current account credit
        elif transaction.get_code() == CODE_ADD_CREDIT:
            pass
            # should edit new user account file (edits currentaccount.txt)
        else:
            pass


class FailedTransactionLogger:
    def __init__(self):
        self.failed_transactions = []

    def log_failure(self, transaction, reason):
        error_message = f"{ERROR_PREFIX}Transaction '{transaction}' failed - Reason: {reason}"
        self.failed_transactions.append(error_message)
        print(error_message)