# Games Distribution Service: Front End


**Phase 3: Front End Requirements Testing**


A console program in C++ that acts as the point-of-purchase terminal for a games distribution service. It reads commands from standard input, asks follow-up questions, and writes a Daily Transaction File for the Back End. A test suite of 46 automated tests checks it against the requirements.

---

## 1. Files

All files are in the `app/` folder. Run every command from inside it.

| File | Purpose |
|---|---|
| `main.cpp` | Starts the program and reads the four file names |
| `FrontEnd.h/.cpp` | Command loop, login rules, one handler per command |
| `FileManager.h/.cpp` | Reads the input files, writes the Daily Transaction File |
| `SessionManager.h`, `UserStore.h`, `GameStore.h` | Who is logged in; the accounts; the games |
| `User.h`, `Game.h`, `TextUtils.h` | Data classes and helper functions |
| `data/` | The three input files used by the tests |
| `make_*_tests.sh`, `mk_test.sh` | Create the tests |
| `run_tests.sh`, `check_outputs.sh` | Run the tests and check the results |
| `tests/` | `inputs/`, `expected/` and `outputs/` (created by the scripts) |

---

## 2. How to compile

You need a C++ compiler. The project was built and tested with `g++`.

Check that it is installed:

```
g++ --version
```

Compile all three `.cpp` files together:

```
g++ -o distribution-system main.cpp FileManager.cpp FrontEnd.cpp
```

Or compile every `.cpp` file in the folder at once:

```
g++ -o distribution-system *.cpp
```

This creates the program `FrontEnd` (`FrontEnd.exe` on Windows).

---

## 3. How to Run

```
./distribution-system currentaccounts.txt availablegames.txt gamescollection.txt transout.atf < test.txt
```

Commands come from standard input (a file, or typed). The four files are: accounts (read), games (read), game collection (read), daily transaction file (written). Without file names, those same four names are used from the current folder.

---

## 4. Run the tests

The scripts need bash (Git Bash on Windows). From `app/`:

```
chmod +x run_tests.sh check_outputs.sh
./run_tests.sh latest
./check_outputs.sh latest
```

1. **Makes** the scripts executable.
1. **Runs** every test. Results go to `tests/outputs/latest/` (`NAME.out` terminal output, `NAME.atf` transaction file).
2. **Check** the results. It prints `PASS` or `FAIL` per test and a summary, and saves every difference in `tests/outputs/latest/report.txt` (`<` = expected, `>` = actual).
3. **Note**: If the input and expected folders are not included, then run `for f in make_*_tests.sh; do bash "$f"; done` in the bash terminal to create the tests for the input and expeted folders. Then run the `./run_tests.sh latest` and `./check_outputs.sh latest` scripts.

Use a different name in place of `latest` to keep several runs, for example `./run_tests.sh run2`.

---

## 5. Data files

Every file ends with a line containing only `END`.

**`currentaccounts.txt`**: 28 characters per line: username (15), type (2), credit (9). Types: `AA` admin, `FS` full-standard, `BS` buy-standard, `SS` sell-standard.
```
admin01         AA 000500.00
fulluser        FS 000100.00
buyer01         BS 000100.00
seller01        SS 000050.00
pooruser        BS 000010.00
deleteme        FS 000020.00
sysadmin        AA 000900.00
END
```

**`availablegames.txt`**: 49 characters per line: game name (26), seller (15), price (6).
```
Spiderman                  seller01        015.00
Hades                      seller01        024.99
Celeste                    admin01         019.99
Minecraft                  fulluser        020.00
GTA V                      deleteme        060.00
END
```

**`gamescollection.txt`**: game name (26), owner (15).
```
Celeste                    fulluser       
END
```

---

## 6. Tests Files

Each test is three files with the same name:

| File | Contents |
|---|---|
| `tests/inputs/NAME.in` | the commands typed |
| `tests/expected/NAME.out` | the expected terminal output |
| `tests/expected/NAME.etf` | the expected Daily Transaction File |

All 46 tests use the same data files in `data/` (7 accounts, 5 games, 1 collection entry). They cover login, logout, addcredit, create, delete, sell, buy, refund, the list commands and search, and include both valid and invalid actions. Expected results come from the requirements.