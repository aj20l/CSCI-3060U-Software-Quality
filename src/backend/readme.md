# Game Store Backend Processor

## Project Structure

* **`backend.py`**: The main entry point of the application. It initializes and runs the `TransactionProcessor`.
* **`Codes.py`**: A centralized configuration file containing transaction codes, file paths, file names, and strict column-width formatting constants.
* **`TransactionProcessor.py`**: The core engine. It coordinates loading data, processing the merged daily transaction file (`transout.atf`), and writing the updated records back to the database files.
* **`FileManager.py`**: Handles standard read/write operations for the text-based database files, ensuring the `END` marker is respected and properly appended.
* **`UserAccountManager.py`**: Manages user accounts, privileges, and credit balances.
* **`AvailableGamesManager.py`**: Manages the marketplace catalog of games currently listed for sale.
* **`GameCollectionManager.py`**: Manages the libraries/collections of games owned by specific users.

## Data Files

The system expects a `data` directory located one level up from the execution directory (`../data/`). The following files are utilized by the system:

* **Input/Output Files:**
  * `currentaccounts.txt`: Contains active user accounts, their privilege levels, and credit balances.
  * `availablegames.txt`: Contains the games currently available for purchase in the store.
  * `gamescollection.txt`: Maps purchased games to their respective owners.
* **Transaction Input:**
  * `transout.atf`: The merged daily transaction file. This contains a sequenced list of transaction codes and payloads to be processed by the backend.

*(Note: All data files are expected to be terminated with an `END` marker on the final line).*

## Transaction Codes

The `TransactionProcessor` reads lines from `transout.atf` and parses them based on the following 2-digit prefixes:

| Code | Action | Description |
| :--- | :--- | :--- |
| `00` | End of Session | Marks the end of a transaction block. |
| `01` | Create | Creates a new user account. |
| `02` | Delete | Removes an existing user account. |
| `03` | Sell | Lists a new game on the marketplace. |
| `04` | Buy | Adds a game to a user's collection. |
| `05` | Refund | Transfers credits from a seller back to a buyer. |
| `06` | Add Credit | Adds a specified amount of credits to a user's balance. |

## How to Run

1. Ensure you have installed all the requirments in the requirements.txt file in your virtual python environment
2. Navigate to `./backend` directory
3. Verify there exits `currentaccounts.txt`, `availablegames.txt` `gamescollection.txt`, and `transout.atf` files exists in the data file
3. Execute the backend script from your terminal:

```bash
python backend.py
```

# How to Run Tests

1. Navigate to the `./backend` directory
2. Ensure required packages have been installed in your virtual python environment
2. Execute script in terminal
```bash
pytest ./tests/
```
To run specific test for a single test file
```bash
pytest ./tests/test_file_name
```
