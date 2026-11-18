from TransactionProcessor import TransactionProcessor

if __name__ == "__main__":
    transactionProcessor = TransactionProcessor()
    transactionProcessor.load_files()
    transactionProcessor.process_daily_transactions()
