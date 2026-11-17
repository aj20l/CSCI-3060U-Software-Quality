# Games Distribution Service: Front End

**Phase 2: Front End Rapid Prototype**


The Front End is a console program written in C++. It acts as the point-of-purchase terminal for the games distribution service. It reads transaction codes as lines of text, asks follow-up questions for each one, and writes a Daily Transaction file for the Back End.

This is a **rapid first version**. Only part of the Front End is built.

---

## 1. Files in this project

| File | Purpose |
|---|---|
| `main.cpp` | Starts the program and reads the optional file names from the command line |
| `FrontEnd.h` / `FrontEnd.cpp` | Main loop: reads commands, enforces login rules, runs each transaction |
| `SessionManager.h` | Remembers who is logged in and whether they are an admin |
| `UserStore.h` | Keeps all current accounts in memory and finds them by username |
| `GameStore.h` | Keeps all games for sale in memory and finds them |
| `FileManager.h` / `FileManager.cpp` | The only class that reads or writes files |
| `User.h` | One account (username, account type, credit) |
| `Game.h` | One game (name, seller, price) |

Headers with no matching `.cpp` file (`User.h`, `Game.h`, `UserStore.h`, `GameStore.h`, `SessionManager.h`) are header-only: all their code is inside the `.h` file.

---

## 2. How to compile

You need a C++ compiler. The project was built and tested with `g++`.

Check that it is installed:

```
g++ --version
```

Compile all three `.cpp` files together:

```
g++ -o FrontEnd main.cpp FileManager.cpp FrontEnd.cpp
```

Or compile every `.cpp` file in the folder at once:

```
g++ -o FrontEnd *.cpp
```

This creates the program `FrontEnd` (`FrontEnd.exe` on Windows).


---

## 3. How to run

The program reads its **commands from standard input**, one per line. It also needs two data files (see section 4).

### Run with a file of commands (how the tests run)

```
./FrontEnd < test.txt
```

On Windows use Command Prompt (cmd) or Git Bash, and run it as `FrontEnd < test.txt`. PowerShell does not support `<`.

### Run by typing commands

```
./FrontEnd
```

Type one command per line and press Enter. To end the input, press **Ctrl+D** (Mac/Linux) or **Ctrl+C** (Windows).

### Choose the file names

```
./FrontEnd [accountsFile] [gamesFile] [dailyTransactionFile] < test.txt
```

If you leave them out, these defaults are used:

| Argument | Default | What it is |
|---|---|---|
| 1 | `current_accounts.txt` | Current User Accounts file (read) |
| 2 | `available_games.txt` | Available Games file (read) |
| 3 | `daily_transaction.txt` | Daily Transaction file (written) |

Example: `./FrontEnd accounts_test1.txt games_test1.txt out1.txt < test.txt`

The data files must be in the folder you run the program from (or give their full paths).

---

## 4. Input files

### `current_accounts.txt`: Current User Accounts

One account per line, exactly 28 characters: username padded to 15 characters, a space, the 2-letter type, a space, and the 9-character credit. The file ends with a line containing only `END`.

```
admin01         AA 000500.00
buyer01         BS 000050.00
seller01        SS 000000.00
END
```

Account types: `AA` = admin, `FS` = full-standard, `BS` = buy-standard, `SS` = sell-standard.

### `available_games.txt`: Available Games

One game per line: game name padded to 26 characters, a space, the seller's username padded to 15 characters, a space, and the 6-character price. Each line is 49 characters. The file ends with a line containing only `END`.

```
Minecraft                  seller01        015.00
GTA VI                     seller01        024.99
Wolverine                  admin01         019.99
END
```

If either file is missing, `login` shows an error message instead of starting a session.

---

## 5. Output: Daily Transaction file

When the user logs out, the Front End writes the Daily Transaction file (default `daily_transaction.txt`). In this version it contains only the end-of-session record:

```
00                    000000.00
```

That is `00`, a space, 15 spaces for the (empty) username, a space, 2 spaces for the (empty) account type, a space, and `000000.00`. The file is rewritten at every logout.

---

## 6. Commands

| Command | Who can use it | Status |
|---|---|---|
| `login` | Anyone, when not logged in | **Done** |
| `logout` | Anyone, when logged in | **Done** |
| `listaccounts` | Admin only | **Done** |
| `listgames` | Anyone, when logged in | **Done** |
| `create` | Admin only | Not built yet |
| `delete` | Admin only | Not built yet |
| `sell` | All except buy-standard | Not built yet |
| `buy` | All except sell-standard | Not built yet |
| `refund` | Admin only | Not built yet |
| `addcredit` | Everyone | Not built yet |

Commands marked "Not built yet" are recognised, and the program answers `Error: That transaction is not available yet.`
Anything else answers `Error: Unknown transaction code.`

### `login`

1. Reads the accounts file.
2. Asks `Enter username:`.
3. If the username exists, reads the games file and starts the session.
4. Rejected if already logged in, or if the username is unknown.

### `logout`

Ends the session and writes the Daily Transaction file. Rejected if not logged in. After logout, only `login` is accepted.

### `listaccounts` (admin-only)

Prints every active account with its type and credit. Only an admin may use it. Other account types get `Error: Your account type is not permitted to do that.`

```
Active accounts:
Username         Type           Credit
admin01          admin          500.00
buyer01          buy-standard   50.00
seller01         sell-standard  0.00
3 account(s).
```

This transaction does not write anything to the Daily Transaction file, because the requirements give it no transaction code.

---

## 7. What is done

- Reads the **Current User Accounts** file and the **Available Games** file.
- `login`, `logout` and the login rules:
  - no transaction other than login before a login
  - no second login until logout
  - the accounts file is read before login is processed
  - the games file is read once the username is accepted
- `listaccounts`: privileged transaction (admin only).
- Permission handling for admin accounts.
- Daily Transaction file written at logout (end-of-session record only).
- Optional file names on the command line, so the test scripts can point at different files.
- Games are loaded into memory (`Game`, `GameStore`) ready for `buy` and `sell`.

---

## 8. What is left for Phase 3

**Build the remaining transactions**

- `create`: admin only; username at most 15 characters and unique; asks for the account type; record `01`
- `delete`: admin only; user must exist and not be the current user; cancels that user's games; record `02`
- `sell`: not for buy-standard; price at most 999.99; game name at most 25 characters and unique; no further transactions on a new game this session; record `03`
- `buy`: not for sell-standard; game must exist; buyer must have enough credit and not already own the game; moves the credit to the seller; record `04`
- `refund`: admin only; buyer and seller must both be current users; moves credit from seller to buyer; record `05`
- `addcredit`: admin names the account, a standard user adds to their own; at most $1000.00 per account per session; record `06`


