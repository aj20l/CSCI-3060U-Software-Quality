from TransactionProcessor import TransactionProcessor

if __name__ == "__main__":
    transactionProcessor = TransactionProcessor()
    transactionProcessor.load_files()
    transactionProcessor.user_account_manager.list_accounts()
    # transactionProcessor.write_data()
