# This script is used to create test files for the search commands. 
# It uses the mk function defined in mk_test.sh to generate the input and expected output files for each test case.
source ./mk_test.sh

mk search_success_games_and_users <<'EOF'
login
fulluser
search
de
logout
--- terminal
Enter transaction: Enter username: Logged in as fulluser.
Enter transaction: Enter search text: Games matching "de":
  Spiderman                  seller01        15.00
  Hades                      seller01        24.99
2 game(s).
Users matching "de":
  deleteme
1 user(s).
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 fulluser        FS 000100.00
EOF

mk search_success_admin_sees_all_admins <<'EOF'
login
admin01
search
admin
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter search text: Games matching "admin":
0 game(s).
Users matching "admin":
  admin01
  sysadmin
2 user(s).
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF

mk search_nonadmin_hides_idle_admin <<'EOF'
login
fulluser
search
admin
logout
--- terminal
Enter transaction: Enter username: Logged in as fulluser.
Enter transaction: Enter search text: Games matching "admin":
0 game(s).
Users matching "admin":
  admin01
1 user(s).
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 fulluser        FS 000100.00
EOF

mk search_case_insensitive <<'EOF'
login
buyer01
search
DE
logout
--- terminal
Enter transaction: Enter username: Logged in as buyer01.
Enter transaction: Enter search text: Games matching "DE":
  Spiderman                  seller01        15.00
  Hades                      seller01        24.99
2 game(s).
Users matching "DE":
  deleteme
1 user(s).
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 buyer01         BS 000100.00
EOF

mk search_no_matches <<'EOF'
login
seller01
search
zzz
logout
--- terminal
Enter transaction: Enter username: Logged in as seller01.
Enter transaction: Enter search text: Games matching "zzz":
0 game(s).
Users matching "zzz":
0 user(s).
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 seller01        SS 000050.00
EOF

mk search_fail_1_empty_query <<'EOF'
login
admin01
search

logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter search text: Error: Please enter some text to search for.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF

mk search_fail_2_not_logged_in <<'EOF'
search
--- terminal
Enter transaction: Error: You must log in first.
Enter transaction: 
EOF