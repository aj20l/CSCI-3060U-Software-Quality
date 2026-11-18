import AvailableGamesManager
import UserAccountManager
from backend.GameCollectionManager import GameCollectionManager

class TransactionProcessor:
    userAccounts=UserAccountManager()
    gameCollections=GameCollectionManager()
    gamesList=AvailableGamesManager()



    def createUserTransaction(self,line):
        pass

    def deleteUserTransaction(self,line):
        pass

    def sellTransaction(self,line):
        pass

    def buyTransaction(self,line):
        pass

    def refundTransaction(self,line):
        pass

    def addCreditTransaction(self,line):
        pass

    def sessionEnd(self,line):
        pass

    def transactionLogger(self):
        pass

    def failedTransactionlogger(self):
        pass