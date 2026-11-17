import TransactionProcessor







transactionList=[]
def readDailyTransactions():
    file = open("dailyTransaction.txt", "r")
    for line in file:
        transactionList.append(line)
    file.close()
    return transactionList


#maybe move this into transaction processor
def processTransaction():
    for transaction in transactionList:
        line=transaction.split("_")
        if line[0]=="01":
            TransactionProcessor.createUserTransaction()
        if line[0]=="02":
            TransactionProcessor.deleteUserTransaction()
        if line[0]=="03":
            TransactionProcessor.sellTransaction()
        if line[0]=="04":
            TransactionProcessor.buyTransaction()
        if line[0]=="05":
            TransactionProcessor.refundTransaction()
        if line[0]=="06":
            TransactionProcessor.addCreditTransaction()
        if line[0]=="00":
            TransactionProcessor.sessionEnd()












