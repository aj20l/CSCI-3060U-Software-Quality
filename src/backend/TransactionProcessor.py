from FileManager import FileManager
from UserAccountManager import UserAccountsManager
from GameCollectionManager import GameCollectionManager
from AvailableGamesManager import AvailableGamesManager
import Codes as Code

class Transaction:
    # helper class structure for daily transaction data
    def __init__(self, transaction):
        self.transaction = transaction[3:]
        self.code = transaction[:2]

    def __str__(self):
        return f"{self.transaction}"

    def get_code(self):
        return f"{self.code}"

class TransactionProcessor:
    def __init__(self):
        self.file_manager = FileManager(Code.DATA_PATH)
        self.user_account_manager = UserAccountsManager()
        self.game_collection_manager = GameCollectionManager()
        self.available_games_manager = AvailableGamesManager()

    # Loads the data from the transaction, old accounts, games, and games collection file
    # before processing the daily transactions
    def load_files(self):
        for line in self.file_manager.read_file(Code.CURRENT_ACCOUNT_FILE):
            self.user_account_manager.load_account(line)

        for line in self.file_manager.read_file(Code.GAMES_COLLECTION_FILE):
            self.game_collection_manager.load_collection(line)

        for line in self.file_manager.read_file(Code.AVAILABLE_GAMES_FILE):
            self.available_games_manager.load_game(line)

        # self.user_account_manager.list_accounts()
        # self.game_collection_manager.list_collection()
        # self.available_games_manager.list_games()

        for line in self.file_manager.read_file(Code.MERGED_DAILY_TRANSACTION_FILE):
            self.process_daily_transactions(Transaction(line))

        # writes to respected files after daily transactions have been processed
        self.file_manager.write_file(Code.CURRENT_ACCOUNT_FILE, 
                                     self.user_account_manager.get_accounts())
        self.file_manager.write_file(Code.GAMES_COLLECTION_FILE, 
                                     self.game_collection_manager.get_collestions())
        self.file_manager.write_file(Code.AVAILABLE_GAMES_FILE, 
                                     self.available_games_manager.get_games())

        

    # from a given line from the daily transaction, processes
    # the data in the line based on the code it has
    def process_daily_transactions(self, transaction):
        if transaction.get_code() == Code.CODE_END_OF_SESSION:
            pass
        elif transaction.get_code() == Code.CODE_CREATE:
            # processes transactions with the create code
            self.user_account_manager.load_account(str(transaction))

        elif transaction.get_code() == Code.CODE_DELETE:
            # processes transactions with delete code by username
            transaction = str(transaction)
            username = transaction[:16]
            account = self.user_account_manager.get_account(username)
            self.user_account_manager.remove_account(account)

        elif transaction.get_code() == Code.CODE_SELL:  
            # processes transactions with sell code by
            # loading a new game with the inputted information
            self.available_games_manager.load_game(str(transaction))

        elif transaction.get_code() == Code.CODE_BUY:
            # processes transactions with buy code by 
            # adding game to users collection
            self.game_collection_manager.load_collection(str(transaction))

        elif transaction.get_code() == Code.CODE_REFUND:
            # processess transaction with refund code by obtaining
            # the buyer, seller, and refund amount in the logged transaction
            transaction = str(transaction)
            buyer = transaction[:16]
            seller = transaction[16:32]
            refund_amount = float(transaction[32:42])
            
            buyer_account = self.user_account_manager.get_account(buyer)
            buyer_credit = float(buyer_account.get_credits()) + refund_amount
            formatted = f"{buyer_credit:.2f}".rjust(9,"0")
            buyer_account.set_credits(formatted)

            seller_account = self.user_account_manager.get_account(seller)
            seller_credit = float(seller_account.get_credits()) - refund_amount
            formatted = f"{seller_credit:.2f}".rjust(9,"0")
            seller_account.set_credits(formatted)

        elif transaction.get_code() == Code.CODE_ADD_CREDIT:
            # processes transactions with add credit code
            transaction = str(transaction)
            username = transaction[:16]
            added_credits = float(transaction[19:29])

            account = self.user_account_manager.get_account(username)
            new_credit_amount = float(account.get_credits()) + added_credits
            formatted = f"{new_credit_amount:.2f}".rjust(9,"0")
            account.set_credits(formatted)

        else:
            # error should show up for untracked transaction code
            pass


class FailedTransactionLogger:
    def __init__(self):
        self.failed_transactions = []

    def log_failure(self, transaction, reason):
        error_message = f"{Code.ERROR_PREFIX}Transaction '{transaction}' failed - Reason: {reason}"
        self.failed_transactions.append(error_message)
        print(error_message)
